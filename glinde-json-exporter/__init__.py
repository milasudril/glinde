# ##### BEGIN GPL LICENSE BLOCK #####
#
#  This program is free software; you can redistribute it and/or
#  modify it under the terms of the GNU General Public License
#  as published by the Free Software Foundation; either version 2
#  of the License, or (at your option) any later version.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with this program; if not, write to the Free Software Foundation,
#  Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
#
# ##### END GPL LICENSE BLOCK #####

bl_info={
	'name': 'Glinde JSON mesh exporter',
	'author': 'Torbjörn Rathsman',
	'version': (1, 0, 0),
	'blender': (2, 77, 0),
	'location': 'File > Import-Export',
	'description': 'Exports mesh objects as a Glinde JSON mesh',
	'category': 'Import-Export'
    }


if 'bpy' in locals():
	import importlib
	if 'exporter' in locals():
		importlib.reload(exporter)

import bpy
from bpy_extras.io_utils import ExportHelper
from collections import defaultdict
from collections import OrderedDict
from array import array


def vertexString(index,normal,uv):
	return '%d %.8g %.8g %.8g %.8g %.8g' \
			%(index,normal.x,normal.y,normal.z,uv.x,uv.y)

class Vertex:
	def __init__(self,index,position,normal,uv):
		self.index=index
		self.position=position
		self.normal=normal
		self.uv=uv

	def indexWrite(self,file_out,first):
		if first:
			file_out.write('%d'%(self.index))
		else:
			file_out.write(',%d'%(self.index))

	def positionWrite(self,file_out,first):
		if first:
			file_out.write('%.8e,%.8e,%.8e'\
				%(self.position.x,self.position.y,self.position.z))
		else:
			file_out.write(',%.8e,%.8e,%.8e'%\
				(self.position.x,self.position.y,self.position.z))

	def uvWrite(self,file_out,first):
		if first:
			file_out.write('%.8e,%.8e'%(self.uv.x,self.uv.y))
		else:
			file_out.write(',%.8e,%.8e'%(self.uv.x,self.uv.y))

	def normalWrite(self,file_out,first):
		if first:
			file_out.write('%.8e,%.8e,%.8e'\
				%(self.normal.x,self.normal.y,self.normal.z))
		else:
			file_out.write(',%.8e,%.8e,%.8e'\
				%(self.normal.x,self.normal.y,self.normal.z))

class MeshTemp:
	def __init__(self,vertex_index,face_array):
		self.vertex_index=vertex_index
		self.face_array=face_array
		self.textures=[]



def verticesIndex(mesh):
	uv_layer=mesh.uv_layers.active.data
	meshes=[]

	meshes.append(MeshTemp(OrderedDict(),[]));
	vertex_id=0
	material_prev=0
	for face in sorted(mesh.polygons,key=lambda face: face.material_index):
		if not(face.material_index==material_prev):
			 vertex_id=0
			 material_prev=face.material_index
			 meshes.append(MeshTemp(OrderedDict(),[]))

		for i in face.loop_indices:
			vi=mesh.loops[i].vertex_index
			uv=uv_layer[i].uv
			vert=mesh.vertices[vi]
			if face.use_smooth:
				normal=vert.normal
			else:
				normal=face.normal
			vertex_value=vertexString(vi,normal,uv)
			if vertex_value in meshes[material_prev].vertex_index:
				vertex_id_current=meshes[material_prev].vertex_index[vertex_value].index
			else:
				meshes[material_prev].vertex_index[vertex_value]=Vertex(vertex_id,vert.co,normal,uv)
				meshes[material_prev].textures=mesh.materials[face.material_index]['textures']
				vertex_id_current=vertex_id
				vertex_id=vertex_id+1
			meshes[material_prev].face_array.append(vertex_id_current)
	return meshes

def verticesWrite(vertex_index,file_out):
	first=1
	for key in vertex_index:
		vertex_index[key].positionWrite(file_out,first)
		first=0

def normalsWrite(vertex_index,file_out):
	first=1
	for key in vertex_index:
		vertex_index[key].normalWrite(file_out,first)
		first=0

def uvsWrite(vertex_index,file_out):
	first=1
	for key in vertex_index:
		vertex_index[key].uvWrite(file_out,first)
		first=0

def facesWrite(face_array,file_out):
	first=1
	for vertex_index in face_array:
		if first:
			file_out.write('%d'%(vertex_index))
			first=0
		else:
			file_out.write(',%d'%(vertex_index))

def texturesWrite(textures,file_out):
	data_has=0
	for texture_name in textures:
		if data_has:
			file_out.write(',')
		data_has=1
		file_out.write('"%s"'%(texture_name))

def meshWrite(item,file_out):
	mesh=item.to_mesh(bpy.context.scene,True,'PREVIEW')
	mesh.transform(item.matrix_world)
	meshes=verticesIndex(mesh)

	k=0
	for mesh in meshes:
		if not(k==0):
			file_out.write(',')
		file_out.write('\t\t\t{\n')
		file_out.write('\t\t\t "vertices":[')
		verticesWrite(mesh.vertex_index,file_out)
		file_out.write(']\n')

		file_out.write('\t\t\t,"normals":[')
		normalsWrite(mesh.vertex_index,file_out)
		file_out.write(']\n')

		file_out.write('\t\t\t,"uvs":[')
		uvsWrite(mesh.vertex_index,file_out)
		file_out.write(']\n')

		file_out.write('\t\t\t,"faces":[')
		facesWrite(mesh.face_array,file_out)
		file_out.write(']\n')

		file_out.write('\t\t\t,"textures":[')
		texturesWrite(mesh.textures,file_out)
		file_out.write(']\n')
		file_out.write('\t\t\t}\n')
		k=k+1

def meshesWrite(file_out):
	objects=bpy.context.scene.objects
	data_has=0
	for item in objects:
		if item.type=='MESH':
			if data_has:
				file_out.write(',')
			data_has=1
			meshWrite(item,file_out)


def framesWrite(file_out):
	frame_start=bpy.context.scene.frame_start
	frame_end=bpy.context.scene.frame_end
	for i in range(frame_start,frame_end):
		bpy.context.scene.frame_set(i)
		file_out.write('\t\t[\n')
		meshesWrite(file_out)
		file_out.write('\t\t]\n')
		if i+1!=frame_end:
			file_out.write('\t\t\t\t,')

def frameTagsWrite(file_out):
	data_has=0
	for k,v in bpy.context.scene.timeline_markers.items():
		if data_has:
			file_out.write(',')
		else:
			file_out.write(' ')
		data_has=1
		file_out.write('"%s": %d' %(v.name,v.frame))

class GlindeMeshExporter(bpy.types.Operator,ExportHelper):
	bl_idname='export_glindamesh.fmt'
	bl_label='Export'
	bl_options={'PRESET'}
	filename_ext='.json'

	def execute(self,context):
		file_out=open(self.filepath,'w');
		file_out.write('{\n')
		file_out.write(' "frames":\n\t[\n')
		framesWrite(file_out)
		file_out.write('\n\t]\n')
		file_out.write(',"frame_tags":\n\t{\n')
		frameTagsWrite(file_out);
		file_out.write('\t}\n')
		file_out.write('}\n')
		return {'FINISHED'}

def menu_func(self,context):
	self.layout.operator(GlindeMeshExporter.bl_idname, text='Glinde JSON mesh exporter');

def register():
	bpy.utils.register_module(__name__);
	bpy.types.INFO_MT_file_export.append(menu_func);

def unregister():
	bpy.utils.unregister_module(__name__);
	bpy.types.INFO_MT_file_export.remove(menu_func);

if __name__ == '__main__':
	register();

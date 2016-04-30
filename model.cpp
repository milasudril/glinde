#ifdef __WAND__
target[name[model.o] type[object]]
#endif

#include "model.h"
#include "resourceobject.h"
#include "datasource.h"
#include "stringkey.h"
#include "narrow_cast.h"
#include "logwriter.h"

using namespace Glinda;

static ArraySimple<Mesh>
meshesLoad(TextureManager& textures,const ResourceObject& meshes
	,const char* source_name)
	{
	auto n_meshes=meshes.objectCountGet();
	GLINDA_DEBUG_PRINT("Got %zu individual meshes",n_meshes);

	return std::move(ArraySimple<Mesh>(n_meshes,[&meshes,&textures,source_name](size_t l)
		{
		auto mesh=meshes.objectGet(l);
		return Mesh(textures,mesh,source_name);
		}));
	}

static ArraySimple< Model::Frame >
framesLoad(TextureManager& textures,const ResourceObject& obj
	,const char* source_name)
	{
	auto frames=obj.objectGet("frames");
	auto n_frames=frames.objectCountGet();
	GLINDA_DEBUG_PRINT("Got %zu frames",n_frames);

	return std::move(ArraySimple<Model::Frame>(n_frames,[&frames,&textures,source_name](size_t k)
		{
		auto meshes=frames.objectGet(k);

		auto ret=meshesLoad(textures,meshes,source_name);
		auto bb=boundingBoxGet(ret);
		return Model::Frame{std::move(ret),bb};
		}));
	}

static std::map<Stringkey,size_t>
frameTagsLoad(const ResourceObject& obj, const char* source_name
	,size_t frame_count)
	{
	std::map<Stringkey,size_t> ret;
	auto frame_tags=obj.objectGet("frame_tags");
	auto i=frame_tags.objectIteratorGet();
	while(!i.endAt())
		{
		auto tmp=i.get();

		auto frame=narrow_cast<size_t>(static_cast<long long int>(tmp.second));
		if(frame>=frame_count)
			{
			throw ErrorMessage("%s: Frame tag %s refers to an invalid frame number. "
				"Valid range is 0 to %zu"
				,source_name,frame_count-1);
			}

		auto ip=ret.emplace(Stringkey(tmp.first),frame);

		if(!ip.second)
			{
			throw ErrorMessage("%s: The tag %s (or one with identical hash), "
				"has already been used for another frame",source_name,tmp.first);
			}

		i.next();
		}
	return std::move(ret);
	}

Model::Model(TextureManager& textures,DataSource& source)
	{
	logWrite(LogMessageType::INFORMATION,"Loading model \"%s\""
		,source.nameGet());
	ResourceObject data(source);
	m_frames=framesLoad(textures,data,source.nameGet());
	m_frame_tags=frameTagsLoad(data,source.nameGet(),m_frames.length());
	}

Model::~Model()
	{
	}

size_t Model::frameIndexGet(const Stringkey& key) const
	{
	auto i=m_frame_tags.find(Stringkey(key));
	if(i==m_frame_tags.end())
		{
		throw ErrorMessage("Invalid key %s",key);
		}
	return i->second;
	}

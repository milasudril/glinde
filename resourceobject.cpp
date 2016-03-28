#ifdef __WAND__
target[name[resourceobject.o] type[object] dependency[jansson;external]]
#endif

#include "resourceobject.h"
#include "errormessage.h"
#include "datasource.h"
#include "logwriter.h"

#include <jansson.h>
#include <cstring>

using namespace Glinda;

static size_t loadCallback(void* buffer, size_t length, void* eventhandler)
	{
	return reinterpret_cast<DataSource*>(eventhandler)->read(buffer,length);
	}

ResourceObject::Iterator::Iterator(ResourceObject& object):r_object(object)
	{
	m_handle=json_object_iter(static_cast<json_t*>(object.m_handle));
	}

std::pair<const char*,ResourceObject> ResourceObject::Iterator::get() noexcept
	{
	auto key=json_object_iter_key(m_handle);
	ResourceObject value{json_object_iter_value(m_handle),key};

	return {key,std::move(value)};
	}

void ResourceObject::Iterator::next() noexcept
	{
	m_handle=json_object_iter_next(static_cast<json_t*>(r_object.m_handle),m_handle);
	}

bool ResourceObject::Iterator::atEnd() noexcept
	{
	return m_handle==nullptr;
	}


ResourceObject::ResourceObject(DataSource&& readhandler)
	{
	json_error_t status;
	m_handle=json_load_callback(loadCallback,&readhandler,0,&status);
	if(m_handle==nullptr)
		{
		throw ErrorMessage("Could not load JSON data. %s:%d: %s."
			,readhandler.nameGet(),status.line,status.text);
		}
	m_name=ArraySimple<char>(strlen(readhandler.nameGet()) + 1 );
	memcpy(m_name.begin(),readhandler.nameGet(),m_name.length()*sizeof(char));
	}

ResourceObject::ResourceObject(void* handle,const char* name)
	{
	m_handle=handle;
	json_incref(static_cast<json_t*>(m_handle));

	m_name=ArraySimple<char>(strlen(name) + 1 );
	memcpy(m_name.begin(),name,m_name.length()*sizeof(char));
	}

ResourceObject::~ResourceObject()
	{
	json_decref(static_cast<json_t*>(m_handle));
	}

ResourceObject::Type ResourceObject::typeGet() const noexcept
	{
	switch(json_typeof(static_cast<const json_t*>(m_handle)))
		{
		case JSON_OBJECT:
			return Type::OBJECT;

		case JSON_ARRAY:
			return Type::ARRAY;

		case JSON_STRING:
			return Type::STRING;

		case JSON_INTEGER:
			return Type::INTEGER;

		case JSON_REAL:
			return Type::FLOAT;

		default:
			return Type::OBJECT;
		}
	}

ResourceObject ResourceObject::objectGet(const char* name)
	{
	auto result=json_object_get(static_cast<const json_t*>(m_handle),name);
	if(result==NULL)
		{
		throw ErrorMessage("%s: Could not get child object \"%s\"."
			,m_name.begin()
			,name);
		}

	return ResourceObject(result,name);
	}

size_t ResourceObject::objectCountGet() const noexcept
	{return json_array_size(static_cast<const json_t*>(m_handle));}

ResourceObject ResourceObject::objectGet(size_t index)
	{
	auto result=json_array_get(static_cast<const json_t*>(m_handle),index);
	if(result==NULL)
		{
		throw ErrorMessage("%s: Could not get child object number %zu."
			,m_name.begin(),index + 1);
		}
	return ResourceObject(result,m_name.begin());
	}

const char* ResourceObject::stringGet() const noexcept
	{
	auto ret=json_string_value(static_cast<const json_t*>(m_handle));
	if(ret==NULL)
		{return nullptr;}
	return ret;
	}

long long int ResourceObject::integerGet() const noexcept
	{return json_integer_value(static_cast<const json_t*>(m_handle));}

double ResourceObject::floatGet() const noexcept
	{
	if(typeGet()==Type::INTEGER)
		{
		logWrite(LogMessageType::WARNING
			,"%s: Reading floating point value encoded as integer."
			,m_name.begin());
		return static_cast<double>(integerGet());
		}
	return json_real_value(static_cast<const json_t*>(m_handle));
	}

ResourceObject::operator const char*() const
	{
	auto ret=stringGet();
	if(ret==nullptr)
		{
		throw ErrorMessage("%s: Current resource object is not a string."
			,m_name.begin());
		}
	return ret;
	}

ResourceObject::operator long long int() const
	{
	if(typeGet()!=Type::INTEGER)
		{
		throw ErrorMessage("%s: Current resource object is not an integer."
			,m_name.begin());
		}
	return integerGet();
	}

ResourceObject::operator double() const
	{
	switch(typeGet())
		{
		case Type::INTEGER:
		case Type::FLOAT:
			return floatGet();

		default:
			throw ErrorMessage("%s: Current resource object is not an integer."
				,m_name.begin());
		}
	}

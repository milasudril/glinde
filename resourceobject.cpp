#ifdef __WAND__
target[name[resourceobject.o] type[object] dependency[jansson;external]]
#endif

#include "resourceobject.h"
#include "errormessage.h"
#include "datasource.h"

#include <jansson.h>

using namespace Glinda;

static size_t loadCallback(void* buffer, size_t length, void* eventhandler)
	{
	return reinterpret_cast<DataSource*>(eventhandler)->read(buffer,length);
	}

ResourceObject::ResourceObject(DataSource&& readhandler)
	{
	json_error_t status;
	m_handle=json_load_callback(loadCallback,&readhandler,0,&status);
	if(m_handle==nullptr)
		{
		throw ErrorMessage("Could not load JSON data. %s:%d: %s."
			,status.source,status.line,status.text);
		}
	}

ResourceObject::ResourceObject(void* handle)
	{
	m_handle=handle;
	json_incref(static_cast<json_t*>(m_handle));
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
		throw ErrorMessage("Could not get object \"%s\" from the resource object"
			,name);
		}

	return ResourceObject(result);
	}

size_t ResourceObject::objectCountGet() const noexcept
	{return json_array_size(static_cast<const json_t*>(m_handle));}

ResourceObject ResourceObject::objectGet(size_t index)
	{
	auto result=json_array_get(static_cast<const json_t*>(m_handle),index);
	if(result==NULL)
		{
		throw ErrorMessage("Could not get object number %zu from the resource object"
			,index);
		}
	return ResourceObject(result);
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
		{return integerGet();}
	return json_real_value(static_cast<const json_t*>(m_handle));
	}

ResourceObject::operator const char*() const
	{
	auto ret=stringGet();
	if(ret==nullptr)
		{
		throw ErrorMessage("Current resource object is not a string.");
		}
	return ret;
	}

ResourceObject::operator long long int() const
	{
	if(typeGet()!=Type::INTEGER)
		{
		throw ErrorMessage("Current resource object is not an integer.");
		}
	return integerGet();
	}

ResourceObject::operator double() const
	{
	switch(typeGet())
		{
		case Type::INTEGER:
			return integerGet();

		case Type::FLOAT:
			return floatGet();

		default:
			throw ErrorMessage("Current resource object is not an integer.");
		}
	}

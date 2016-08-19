//@	{"targets":[{"name":"messagetest","type":"application"}]}

#include "messageshort.h"
#include <cstdio>

class Type
	{
	public:
		Type():x(1)
			{printf("Construct %p\n",this);}

		~Type()
			{
			printf("Destroy %p%s\n",this,(x==0)?" [zombie]":"");}

		Type(const Type& obj):x(obj.x)
			{printf("Copy %p to %p\n",&obj,this);}

		Type(Type&& obj):x(obj.x)
			{
			printf("Move %p to %p\n",&obj,this);
			obj.x=0;
			}

		Type& operator=(const Type& obj)
			{
			printf("Copy-assign %p to %p\n",&obj,this);
			x=obj.x;
			return *this;
			}

		Type& operator=(Type&& obj)
			{
			printf("Move-assign %p to %p\n",&obj,this);
			std::swap(x,obj.x);
			return *this;
			}

	private:
		int x;
	};

class Processor:public Glinde::MessageShort::Processor
	{
	public:
		void operator()(Glinde::MessageShort&& msg)
			{
			printf("Processor\n");
			auto data=msg.dataRelease<Type>();
			printf("--\n");
			}
	};

int main()
	{
	using namespace Glinde;
	Type x;
	Processor proc;
	MessageShort foo(MessageShort::Time(0,0),proc,std::move(x));
	foo.process();
	return 0;
	}

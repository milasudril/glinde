//@	{"targets":[{"name":"messagetest","type":"application"}]}

#include "messageshort.h"
#include <cstdio>
#include <queue>

class Type
	{
	public:
		Type():x(1)
			{printf("Construct %p\n",this);}

		~Type()
			{
			printf("Destroy %p%s\n",this,(x==0)?" [zombie]":"");
			}

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

		int xGet() const noexcept
			{return x;}

	private:
		int x;
	};

class Callback
	{
	public:
		void operator()(Glinde::MessageShort::Time t,Type&& data)
			{
			printf("Processor %p %d\n",&data,data.xGet());
			printf("--\n");
			}
	};

int main()
	{
	using namespace Glinde;
	Type x;
	std::priority_queue<MessageShort,std::vector<MessageShort>
		,std::greater<MessageShort>> q;

	MessageShort::Processor<Type,Callback> proc{Callback()};

	q.push(MessageShort(MessageShort::Time(0,0),proc,Type()));
	auto msg=std::move(const_cast<MessageShort&>(q.top()));
	msg.process();

	return 0;
	}

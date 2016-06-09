#include <unistd.h>
#include <sys/wait.h>
#include <pthread.h>
#include <errno.h>
#include <dlfcn.h>
#include <stdlib.h>
#include <cassert>
#include <cstdio>

static constexpr int READ_FD=0;
static constexpr int WRITE_FD=1;

void* write_thread(void* param)
	{
	auto f=reinterpret_cast<FILE*>(param);

	const char* header="#include <cstdio>\n"
		"#include <cmath>\n"
		"extern \"C\""
		"	{\n"
		"	int testfunc();\n"
		"	}\n"
		"int testfunc()\n"
		"	{\n";

	fprintf(f,"%s",header);

	int ch_in;
	while( (ch_in=getchar())!=EOF)
		{
		putc(ch_in,f);
		}

	fprintf(f,"}\n");
	fclose(f);
	return nullptr;
	}

int main()
	{
	int parent_to_child[2];
	pipe(parent_to_child);

	int child_to_parent[2];
	pipe(child_to_parent);

	char name[]="/tmp/test-XXXXXXXX.so";
	int object=mkstemps(name,3);
	printf("Writing to %s\n",name);

	auto pid_new=fork();
	switch(pid_new)
		{
		case 0:
			{
			close(parent_to_child[WRITE_FD]);
			dup2(parent_to_child[READ_FD],STDIN_FILENO);

			close(child_to_parent[READ_FD]);
			dup2(child_to_parent[WRITE_FD],STDERR_FILENO);

			const char* args[]={"g++","-fpic","-shared","-o",name,"-x","c++","-std=c++11","-",nullptr};
			execvp("g++",const_cast<char* const*>(args));
			}
			return errno;

		case -1:
			close(parent_to_child[0]);
			close(parent_to_child[1]);
			unlink(name);
			close(object);
			return -1;
		}
	close(parent_to_child[READ_FD]);
	close(child_to_parent[WRITE_FD]);

	auto f=fdopen(parent_to_child[WRITE_FD],"wb");
	assert(f!=NULL);
	pthread_t writer;
	pthread_create(&writer,nullptr,write_thread,f);

	auto file_in=fdopen(child_to_parent[READ_FD],"rb");
	assert(file_in!=NULL);
	int ch_in;
	while((ch_in=getc(file_in))!=EOF)
		{
		putchar(ch_in);
		}
	fclose(file_in);
	int status;
	waitpid(pid_new,&status,0);
	pthread_join(writer,nullptr);
	printf("Done: Process exit status %d\n",status);

	if(status!=0)
		{
		unlink(name);
		close(object);
		return -1;
		}


	auto lib=dlopen(name,RTLD_NOW);
	if(lib==NULL)
		{
		printf("Error loading library: %s",dlerror());
		return -1;
		}

	auto function=reinterpret_cast<int (*)()>(dlsym(lib,"testfunc"));
	auto x=function();
	printf("Return status:%d\n",x);
	dlclose(lib);
	unlink(name);
	close(object);

	return status==0;
	}

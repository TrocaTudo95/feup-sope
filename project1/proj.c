#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <wait.h>
#include <errno.h>
#include <dirent.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <signal.h>
#include <assert.h>

void sigint_handler(int sig) {

								static int in= 0;

								if(sig == 2 && in == 0 ) // Received control+c signal askign user to leave
								{
									in = 1;
									char c;
									printf("Are you sure you want to terminate(Y/N) ");
									c = getchar();
									if (c == 'y' || c == 'Y')
										exit(2);
								}
								else
								in = 0;
}


typedef struct {
								char * name;
								int print;
								int delete;
								int exec;
								int perm;
								int hasType;
								int hasName;
								int hasPerm;
								char type;
}Options;

void initOptions(Options *options) {
	options->name = malloc(sizeof(char *));
	options->print = 0;
	options->delete = 0;
	options->exec = 0;
	options->hasName = 0;
	options->hasType = 0;
	options->hasType = 0;

}

int main(int argc, char const *argv[])
{
								Options options;
								initOptions(& options);
								if(argc  < 5) {
																printf("Invalid amount of arguments \n");
																return 1;
								}

								if(strcmp(argv[2],"-name")==0){
									options.hasName = 1;
									options.name = argv[3];
									printf("ola %s\n", options.name);
								}

								if (strcmp(argv[2], "-type") == 0) {
									options.hasType = 1;
									options.type = argv[3];
									printf("ola\n");
								}


								struct
								sigaction action;
// prepare the 'sigaction struct'
								action.sa_handler = sigint_handler;
								sigemptyset(&action.sa_mask);
								action.sa_flags = 0;
// install the handler
								sigaction(SIGINT,&action,NULL);


								DIR *current_dir;
								struct dirent *directory_info;
								char path[256];
								strcpy(path, argv[1]);
								current_dir = opendir(path);
								//	dp = opendir ("./test")

								char ** temp_argv;
								size_t nr_bites;
								nr_bites=sizeof(temp_argv)*argc;
								//	temp_argv=(char**)malloc(nr_bites);
								//	memcpy(temp_argv,argv,nr_bites);

								//rewinddir(current_dir);  nao sei se é preciso aqui se não
								while( (directory_info = readdir (current_dir))!=NULL) {
																if(strcmp(directory_info->d_name, ".")!=0 && strcmp(directory_info->d_name, "..") !=0 &&(directory_info->d_type==DT_DIR))
																{  //Se fores uma pasta entao da fork() e continua no loop senao imprime o ficheiro (que depois vamos filtrar)
																								int pid;
																								if((pid=fork())==0) {
																																char slash[2] = "/";
																																char * new_str;
																																if((new_str = malloc(strlen(path)+1+1)) != NULL) {
																																								new_str[0] = '\0'; // ensures the memory is an empty string
																																								strcat(new_str,path);
																																								strcat(new_str,slash);
																																}

																																char * new_str2;
																																if((new_str2 = malloc(strlen(new_str)+strlen(directory_info->d_name)+1)) != NULL) {
																																								new_str2[0] = '\0';
																																								strcat(new_str2,new_str);
																																								strcat(new_str2,directory_info->d_name);
																																}

																																strcpy(path, new_str2);
																																current_dir = opendir(path);

																								}
																}
																else if(strcmp(directory_info->d_name, ".")!=0 && strcmp(directory_info->d_name, "..") !=0 && directory_info->d_type!=DT_DIR)
																								printf("File: %s   In path: %s \n", directory_info->d_name, path);

								}
								(void) closedir (current_dir);




								return 0;
}

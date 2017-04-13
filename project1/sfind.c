#include <math.h>
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

	static int in = 0;

	if (sig == 2 && in == 0) // Received control+c signal askign user to leave
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
								int hasMode;
								char * type;
								int mode;
}Options;

void initOptions(Options *options) {
	options->name = malloc(sizeof(char *));
	options->print = 0;
	options->delete = 0;
	options->exec = 0;
	options->hasName = 0;
	options->hasType = 0;
	options->hasMode = 0;
	options->type = malloc(sizeof(char *));

}

int main(int argc, char const *argv[])
{
								
								if(argc  < 5) {
																printf("Invalid amount of arguments \n");
																return 1;
								}
								char slash[2] = "/";
								Options options;
								initOptions(&options);

								if(strcmp(argv[2],"-name")==0){
									options.hasName = 1;
									options.name = argv[3];
								}

								else if (strcmp(argv[2], "-type") == 0) {
									options.hasType = 1;
									options.type = argv[3];
								}
								
								else if (strcmp(argv[2], "-mode") == 0) {
									options.hasMode = 1;
									options.mode = strtol(argv[3], NULL, 10);
								}

								if (strcmp(argv[4], "-delete") == 0) {
									options.delete = 1;
								}
								else if (strcmp(argv[4], "-print")==0) {
									options.print = 1;
								}


								struct sigaction action;
								struct stat sb;
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
								while ((directory_info = readdir(current_dir)) != NULL) {
									if (strcmp(directory_info->d_name, ".") != 0 && strcmp(directory_info->d_name, "..") != 0 && (directory_info->d_type == DT_DIR))
									{  //Se fores uma pasta entao da fork() e continua no loop senao imprime o ficheiro (que depois vamos filtrar)
										int pid;
										if ((pid = fork()) == 0) {
											char * new_str;
											if ((new_str = malloc(strlen(path) + 1 + 1)) != NULL) {
												new_str[0] = '\0'; // ensures the memory is an empty string
												strcat(new_str, path);
												strcat(new_str, slash);
											}

											char * new_str2;
											if ((new_str2 = malloc(strlen(new_str) + strlen(directory_info->d_name) + 1)) != NULL) {
												new_str2[0] = '\0';
												strcat(new_str2, new_str);
												strcat(new_str2, directory_info->d_name);
											}

											strcpy(path, new_str2);
											current_dir = opendir(path);

											///imprimir/apagar caso seja diretorio
											if (options.hasType == 1) {
												if (strcmp(options.type, "d") == 0) {
													if (directory_info->d_type == DT_DIR) {
														if (options.print == 1)
														printf("Directory: %s   In path: %s \n", directory_info->d_name, path);
														else if(options.delete==1)
															execlp("rmdir", path, NULL);
													}
												}
											}

										}
									}
									else if (strcmp(directory_info->d_name, ".") != 0 && strcmp(directory_info->d_name, "..") != 0 && directory_info->d_type != DT_DIR) {
										///////////////////search by name//////////////////////////////
										if (options.hasName == 1) {
											if (strcmp(directory_info->d_name, options.name) == 0) {
												printf("xau\n");
												if (options.print == 1)
													printf("File: %s   In path: %s \n", directory_info->d_name, path);
												else if (options.delete == 1) {
													char temp[256];
													strcpy(temp, path);
													strcat(temp, slash);
													strcat(temp, directory_info->d_name);
													execlp("rm", "-i", temp, NULL);
												}
											}
										}

										/////////////////////////////////////////////search by type//////////////
										else if (options.hasType == 1) {
											if (strcmp(options.type, "f") == 0) {
												if (directory_info->d_type == DT_REG) {
													if (options.print == 1)
													printf("Regular file: %s   In path: %s \n", directory_info->d_name, path);
													else if (options.delete == 1) {
														char temp[256];
														strcpy(temp, path);
														strcat(temp, slash);
														strcat(temp, directory_info->d_name);
														execlp("rm", "-i", temp, NULL);
													}
												}
											}
											if (strcmp(options.type, "l") == 0) {
												if (directory_info->d_type == DT_LNK) {
													if (options.print == 1)
													printf("Linked file: %s   In path: %s \n", directory_info->d_name, path);
													else if (options.delete == 1) {
														char temp[256];
														strcpy(temp, path);
														strcat(temp, slash);
														strcat(temp, directory_info->d_name);
														execlp("rm", "-i", temp, NULL);
													}
												}
											}
										}


										///////////////////////////////////////////////search by permissions///////////////////////////
										else if (options.hasMode == 1) {
											char temp[256];
											strcpy(temp, path);
											strcat(temp, slash);
											strcat(temp, directory_info->d_name);
											stat(temp, &sb);
											int mask = 0x01ff;
											int mode = mask & sb.st_mode;
											if (mode == options.mode) {
												if (options.print == 1)
													printf("file with permissions %d: %s   In path: %s \n", mode,directory_info->d_name, path);
												else if (options.delete == 1)
													execlp("rm", "-i", temp, NULL);
											}
											

										}


									}
								}
								(void) closedir (current_dir);




								return 0;
}

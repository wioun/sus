#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>

#define DEF_USER "root"

int main(int argc, char *argv[]) {

	short usr = 0;
	
    // Parsing args
    if (argc < 2){
        printf("Usage: sus [-u user] [command]\n");
        return 1;
    }
	if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0) {
		printf("Execute the command as another user using su.\nUsage: sus [-u user] [command]\n-u --user\tspecify the user.\n-h --help\tshow help message.\n"); 
		return 0;
	}
	if (strcmp(argv[1], "-u") == 0 || strcmp(argv[1], "--user") == 0) {
		usr = 1;
		if (argc < 4) {
		    printf("Syntax error.\nUsage: sus [-u user] [command]\n");
		    return 1;
		}
	}
		
    // Command
    char cmd[1024] = "";
    
    short usarg; // count of arguments to specify a user ('-u user' (+2) or nothing (+0))
    if (usr == 0) usarg = 1;
    else usarg = 3; 
    for (int i = usarg; i < argc; i++) {
        if (i > usarg) {
            strcat(cmd, " "); 
        }
        strcat(cmd, argv[i]); 
    }
    
    char fcmd[1050];
    if (usr == 0) sprintf(fcmd, "su %s -c '%s'", DEF_USER, cmd);
    else sprintf(fcmd, "su %s -c '%s'", argv[2], cmd);
    //printf("'%s' %hd\n", cmd, usr); // for debug
    	
	// Execute
    int result = system(fcmd);
    if (result == -1) {
        perror("system");
        exit(EXIT_FAILURE);
    }
    exit(EXIT_SUCCESS);
    return 0;
}

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>

#define DEFAULT_USER "root"

int main(int argc, char *argv[]) {
        
    /*| Parsing args |*/ 
    short usr = 0;
    if (argc < 2){

        printf("Usage: sus [-u user] [command]\n");
        return 1;
    }
    if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0) {

        printf("Execute the command as another user using su.\nUsage: sus [-u user] [command]\n-u --user\tspecify the user\n-h --help\tdisplay help message\n-v --version\tdisplay version information\n"); 
        return 0;
    }
    if (strcmp(argv[1], "-v") == 0 || strcmp(argv[1], "--version") == 0) {

        printf("SuS 1.1\nCopyright (C) Wioun 2024, GNU GPL v3.\n"); 
        return 0;
    }
    if (strcmp(argv[1], "-u") == 0 || strcmp(argv[1], "--user") == 0) {

        if (argc < 4) {
            
            printf("Usage: sus [-u user] [command]\n");
            return 1;
        }
		usr = 1;
    }

    /*| Command |*/
	short usrarg; // count of arguments to specify a user ('-u user' (+2) or nothing (+0)).
	if (usr == 0) usrarg = 1; else usrarg = 3; 

	size_t cmdlen; // memory allocation for command.
    for (int i = usrarg; i < argc; i++) cmdlen += strlen(argv[i]) + 1; // +1 for space.
	char *cmd = malloc(cmdlen + 1); // +1 for '\0'.
	if (cmd == NULL) {

        perror("malloc"); 
        exit(EXIT_FAILURE);
    }

	size_t fcmdlen; // memory allocation for full command.
	if (usr == 0) fcmdlen = cmdlen + strlen("su  -c ''") + strlen(DEFAULT_USER); // two spaces, yes.
	else fcmdlen = cmdlen + strlen("su  -c ''") + strlen(argv[2]);
    char *fcmd = malloc(fcmdlen);
	if (fcmd == NULL) {

        perror("malloc"); 
        exit(EXIT_FAILURE);
    }

	cmd[0] = '\0'; // command itself.
	for (int i = usrarg; i < argc; i++) {

		if (i > usrarg) {

			strcat(cmd, " ");
		}
		strcat(cmd, argv[i]);
	}
    
    if (usr == 0) sprintf(fcmd, "su %s -c '%s'", DEFAULT_USER, cmd); // full command.
	else sprintf(fcmd, "su %s -c '%s'", argv[2], cmd);

    /*| Execute |*/ 
	pid_t pid = fork();
    if (pid == -1) {

        perror("fork");
        return 1;
    } 
	else if (pid == 0) {

        // printf("fcmd: '%s'\ncmd: '%s'\nusr: %hd\n", fcmd, cmd, usr); // for debug.
        execl("/bin/sh", "sh", "-c", fcmd, (char *)NULL);
		perror("execl");
		free(cmd);
        exit(EXIT_FAILURE);
    } 
	else {

        wait(NULL);
    }
    return 0;
}

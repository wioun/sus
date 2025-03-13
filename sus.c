#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>
#include <getopt.h>

#define DEFAULT_USER "root"

int main(int argc, char *argv[]) {
   
    /*| Parsing args |*/
    
    static struct option long_options[] = {
        {"help", no_argument, 0, 'h'},
        {"version", no_argument, 0, 'v'},
        {"user", required_argument, 0, 'u'},
        {0, 0, 0, 0}
    };

    char usr = 0;
    char *user;
    int option_index = 0;
    int opt = getopt_long(argc, argv, "hvu:", long_options, &option_index);

    switch (opt) {
        case 'h':
            printf("Usage: sus [-u user] [command]\nExecute the command as another user using su.\n-u --user\tspecify the user\n-h --help\tdisplay help message\n-v --version\tdisplay version information\n"); 
            exit(0);

        case 'v':
            printf("SuS 2.0\nCopyright (C) Wioun 2024–2025, GNU GPL v3.\n"); 
            exit(0);

        case 'u':
            usr = 1;
            user = optarg;
            break;

        default:
            break;
    }

    if (optind == argc) {
        fprintf(stderr, "Usage: %s [-u user] [command]\n", argv[0]);
        exit(1);
    }

    /*| Command |*/
    
    char cmdarg = optind;
    size_t cmdlen; // memory allocation for command.
    for (int i = cmdarg; i < argc; i++) 
        cmdlen += strlen(argv[i]) + 1; // +1 for space.
    char *cmd = malloc(cmdlen + 1); // +1 for '\0'.
    if (cmd == NULL) {
        perror("malloc"); 
        exit(EXIT_FAILURE);
    }

    size_t fcmdlen; // memory allocation for full command.
    if (usr == 0) 
        fcmdlen = cmdlen + strlen("su  -c 'set -f; '") + strlen(DEFAULT_USER); // two spaces, yes.
    else
        fcmdlen = cmdlen + strlen("su  -c 'set -f; '") + strlen(user);
    char *fcmd = malloc(fcmdlen);
    if (fcmd == NULL) {
        perror("malloc"); 
        exit(EXIT_FAILURE);
    }

    cmd[0] = '\0'; // command itself.
    for (int i = cmdarg; i < argc; i++) {
        if (i > cmdarg) {
            strcat(cmd, " ");
        }
        strcat(cmd, argv[i]);
    }

    if (usr == 0) sprintf(fcmd, "su %s -c 'set -f; %s'", DEFAULT_USER, cmd); // full command.
    else sprintf(fcmd, "su %s -c 'set -f; %s'", user, cmd);

    /*| Execute |*/ 
    
    const char *shell = getenv("SHELL");
    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        return 1;
    }
    else if (pid == 0) {
        // printf("fcmd: '%s'\ncmd: '%s'\nusr: %hd\n", fcmd, cmd, usr); // for debug.
        execl(shell, shell, "-c", fcmd, (char *)NULL);
        perror("execl");
        free(cmd);
        exit(EXIT_FAILURE);
    } 
    else {
        wait(NULL);
    }
    
    return 0;
}

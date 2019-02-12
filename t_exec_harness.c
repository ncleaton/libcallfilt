#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#ifdef HAVE_EXECVEAT
#  include <sys/types.h>
#  include <dirent.h>
#endif
#ifdef HAVE_POSIX_SPAWN
#  include <spawn.h>
#endif

extern char **environ;

int
do_exec_via_execvp(char **argv)
{
    return execvp(argv[0], argv);
}

int
do_exec_via_execve(char **argv)
{
    return execve(argv[0], argv, environ);
}

#ifdef HAVE_EXECVEAT
int
do_exec_via_execveat(char **argv)
{
    return execveat(dirfd(opendir(".")), argv[0], argv, environ, 0);
}
#endif

int
do_exec_via_execv(char **argv)
{
    return execv(argv[0], argv);
}

int
do_exec_via_execvpe(char **argv)
{
    return execvpe(argv[0], argv, environ);
}

int
do_exec_via_execl(char **argv)
{
    return execl(argv[0], argv[0], argv[1], argv[2], NULL);
}

int
do_exec_via_execlp(char **argv)
{
    return execlp(argv[0], argv[0], argv[1], argv[2], NULL);
}

int
do_exec_via_execle(char **argv)
{
    return execle(argv[0], argv[0], argv[1], argv[2], NULL, environ);
}

int
do_exec_via_fexecve(char **argv)
{
    return fexecve(fileno(fopen(argv[0],"r")), argv, environ);
}

int
do_exec_via_popen(char **argv)
{
    FILE *f;

    f = popen(argv[2], "w");
    if (f)
        exit(pclose(f) >> 8);
    return -1;
}

int
do_exec_via_system(char **argv)
{
    int ret;

    ret = system(argv[2]);
    if (ret != -1)
        exit(ret >> 8);
    return ret;
}

#ifdef HAVE_POSIX_SPAWN
int
do_exec_via_posix_spawn(char **argc)
{
    pid_t pid;
    int ret;

    ret = posix_spawn(&pid, argv[0], NULL, NULL, argv, environ);
    if (ret == 0) {
        wait(&ret);
        exit(ret >> 8);
    }
    errno = ret
    return -1;
}

int
do_exec_via_posix_spawnp(char **argc)
{
    pid_t pid;
    int ret;

    ret = posix_spawnp(&pid, argv[0], NULL, NULL, argv, environ);
    if (ret == 0) {
        wait(&ret);
        exit(ret >> 8);
    }
    errno = ret
    return -1;
}
#endif

struct exec_method {
    char *name;
    int (*call)(char**);
};

struct exec_method methods[] = {
    {"execvp",       &do_exec_via_execvp},
    {"execve",       &do_exec_via_execve},
#ifdef HAVE_EXECVEAT
    {"execveat",     &do_exec_via_execveat},
#endif
    {"execv",        &do_exec_via_execv},
    {"execvpe",      &do_exec_via_execvpe},
    {"execl",        &do_exec_via_execl},
    {"execlp",       &do_exec_via_execlp},
    {"execle",       &do_exec_via_execle},
    {"fexecve",      &do_exec_via_fexecve},
    {"popen",        &do_exec_via_popen},
    {"system",       &do_exec_via_system},
#ifdef HAVE_POSIX_SPAWN
    {"posix_spawn",  &do_exec_via_posix_spawn},
    {"posix_spawnp", &do_exec_via_posix_spawnp},
#endif
    {NULL, NULL},
};

void
list_call_names(void) {
    int i;

    for ( i=0 ; methods[i].name ; i++ )
        printf("%s\n", methods[i].name);
}

int
main(int argc, char** argv) {
    int i, ret;
    char *shell_cmd, *exec_call, *exec_argv[3];

    if (argc == 2 && strcmp(argv[1], "list") == 0) {
        list_call_names();
        return 0;
    }

    if (argc != 3) {
        fprintf(stderr, "Usage: exec_testharness (list | EXEC_CALL SHELL_CMD)\n");
        return 1;
    }

    exec_call = argv[1];
    shell_cmd = argv[2];
    exec_argv[0] = "/bin/sh";
    exec_argv[1] = "-c";
    exec_argv[2] = shell_cmd;
    exec_argv[3] = NULL;
    
    for ( i=0 ; methods[i].name ; i++ ) {
        if (strcmp(methods[i].name, exec_call) == 0) {
            ret = (*methods[i].call)(exec_argv);
    
            if (ret != -1) {
                fprintf(stderr, "%s call returned %d not -1\n", methods[i].name, ret);
                return 1;
            }

            if (errno != EACCES) {
                fprintf(stderr, "%s call set errno to %s not EACCES\n", methods[i].name, strerror(errno));
                return 1;
            }

            return 0;
        }
    }
        
    fprintf(stderr, "unknown exec method [%s]\n", exec_call);
    return 0;
}

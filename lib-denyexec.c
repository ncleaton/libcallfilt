#define _GNU_SOURCE
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

static int
_exec_nope(const char *libcall, const char *prog)
{
    fprintf(stderr, "libcallfilt blocked %s(%s)\n", libcall, prog);
    errno = EACCES;
    return -1;
}

int execvp(const char *file, char *const argv[])
{
    return _exec_nope("execvp", file);
}

int execve(const char *filename, char *const argv[], char *const envp[])
{
    return _exec_nope("execve", filename);
}

int execv(const char *path, char *const argv[])
{
    return _exec_nope("execv", path);
}

int execvpe(const char *file, char *const argv[], char *const envp[])
{
    return _exec_nope("execvpe", file);
}

int execl(const char *path, const char *arg, ...)
{
    return _exec_nope("execl", path);
}

int execlp(const char *file, const char *arg, ...)
{
    return _exec_nope("execlp", file);
}

int execle(const char *path, const char *arg, ...)
{
    return _exec_nope("execle", path);
}

int fexecve(int fd, char *const argv[], char *const envp[])
{
    fprintf(stderr, "libcallfilt blocked fexecve()\n");
    errno = EACCES;
    return -1;
}

FILE *popen(const char *command, const char *type)
{
    fprintf(stderr, "libcallfilt blocked popen(%s)\n", command);
    errno = EACCES;
    return NULL;
}

int system(const char *command)
{
    fprintf(stderr, "libcallfilt blocked system(%s)\n", command);
    errno = EACCES;
    return -1;
}

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define MAX_LD_PRELOAD 2048

void
usage(char *progname) {
    fprintf(stderr, "usage: %s denyexec COMMAND ARG ...\n", progname);
}

char *
heap_sprintf_ss(char *format, char *s1, char *s2) {
    char *buf;
    int size, len;

    size = strlen(format) + strlen(s1) + strlen(s2);
    size -= 4; // 2 instances of "%s" won't be in the output
    size += 1; // terminating NULL
    buf = malloc(size);
    if (!buf) {
        fprintf(stderr, "out of memory\n");
        exit(1);
    }
    len = snprintf(format, size, s1, s2);
    if (len < 0) {
        perror("snprintf");
        exit(1);
    }
    if (len != size-1) {
        fprintf(stderr, "ERROR: unexpected snprintf results %d %d\n", len, size);
        exit(1);
    }
    return buf;
}

void
add_ld_preload(char *libname)
{
    char *new_ldp, *old_ldp, *tmp_ldp;

    old_ldp = getenv("LD_PRELOAD");
    new_ldp = heap_sprintf_ss("%s/lib%s.so", INST_PKGLIBDIR, libname);
    if (old_ldp) {
        tmp_ldp = heap_sprintf_ss("%s %s", new_ldp, old_ldp);
        free(new_ldp);
        new_ldp = tmp_ldp;
    }

    if (setenv("LD_PRELOAD", new_ldp, 1) < 0) {
        perror("setenv");
        exit(1);
    }

    free(new_ldp);
}
        
int main(int argc, char** argv) {
    if (argc < 3) {
        usage(argv[0]);
        return 1;
    }

    if ( strcmp(argv[1], "denyexec") == 0 ) {
        add_ld_preload("denyexec");
        execvp(argv[2], argv+2);
        perror("execvp");
        return 1;
    }

    usage(argv[0]);
    return 1;
}

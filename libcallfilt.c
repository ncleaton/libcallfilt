#include <stdio.h>
#include <stdlib.h>

#define MAX_LD_PRELOAD 2048

void
usage(char *progname) {
    fprintf(stderr, "usage: %s denyexec COMMAND ARG ...\n", progname);
}

void
add_ld_preload(char *libname)
{
    char *buf, *ldp;

    buf = malloc(MAX_LD_PRELOAD);
    if (!buf) {
        fprintf(stderr, "out of memory\n");
        exit(1);
    }

    ldp = getenv("LD_PRELOAD");
    if (ldp) {
        snprintf(buf, MAX_LD_PRELOAD, "%s/lib%s.so %s", INST_PKGLIBDIR, libname, ldp);
    } else {
        snprintf(buf, MAX_LD_PRELOAD, "%s/lib%s.so", INST_PKGLIBDIR, libname);
    }

    setenv("LD_PRELOAD", buf, 1);
    free(buf);
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

#define _POSIX_C_SOURCE 200809L
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>
#include <errno.h>

#define MAX_DATAFILES 64 // placeholder undefined

static void usage(void) {
    puts("Usage: ./pa5_lookup <log_file> [ <datafile> ... ]");
}

static double elapsed_sec(struct timeval a, struct timeval b) {
    return (b.tv_sec - a.tv_sec) + (b.tv_usec - a.tv_usec) / 1e6;
}

static int resolve_line(const char *host, FILE *logf) {
    struct addrinfo hints = { .ai_family = AF_UNSPEC };
    struct addrinfo *res, *p;

    if (getaddrinfo(host, NULL, &hints, &res) != 0) {
        fprintf(logf, "%s, NOT_RESOLVED\n", host);
        return -1;
    }

    // outputs only IPv4 else NOT_RESOLVED

    for (p = res; p && p->ai_family != AF_INET; p = p->ai_next);

    if (!p) {
        fprintf(logf, "%s, NOT_RESOLVED\n", host);
        freeaddrinfo(res);
        return -1;
    }

    char ipstr[INET_ADDRSTRLEN];
    struct sockaddr_in *ipv4 = (struct sockaddr_in *)p->ai_addr;
    inet_ntop(AF_INET, &ipv4->sin_addr, ipstr, sizeof ipstr);

    fprintf(logf, "%s, %s\n", host, ipstr);

    freeaddrinfo(res);
    return 0;
}

int main(int argc, char **argv) {

    if (argc < 3) { usage(); return EXIT_FAILURE; }
    if (argc - 2 > MAX_DATAFILES) {
        fprintf(stderr, "Too many input files (max %d)\n", MAX_DATAFILES);
        return EXIT_FAILURE;
    }

    FILE *logf = fopen(argv[1], "w");
    if (!logf) {
        perror("fopen log_file");
        return EXIT_FAILURE;
    }

    // timer
    struct timeval start, end;
    gettimeofday(&start, NULL);

    char *line = NULL;
    size_t cap = 0;
    for (int i = 2; i < argc; ++i) {
        FILE *df = fopen(argv[i], "r");
        if (!df) {
            fprintf(stderr, "invalid file %s\n", argv[i]);
            continue;
        }

        while (getline(&line, &cap, df) != -1) {
            char *nl = strpbrk(line, "\r\n");
            if (nl) *nl = '\0';
            if (*line == '\0') continue;

            resolve_line(line, logf);
        }
        fclose(df);
    }
    free(line);
    fclose(logf);

    // end timer
    gettimeofday(&end, NULL);
    printf("./pa5_lookup: total time is %.6f seconds\n",
        elapsed_sec(start, end));

    return EXIT_SUCCESS;
}

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char *argv[])
{
    time_t t;
    char str[16];
    struct tm *lt;

    if (argc == 1)
        t = time(NULL);
    else {
        t = strtol(argv[1], NULL, 10);
        if (t <= 0)
            t = time(NULL);
    }

    lt = localtime(&t);
    sprintf(str, "%04d-%02d-%02d", lt->tm_year + 1900, lt->tm_mon + 1, lt->tm_mday);
    puts(str);

    exit(EXIT_SUCCESS);
}


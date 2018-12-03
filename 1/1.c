#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    long output = 0;
    FILE *inputs = NULL;
    char *line = NULL;
    size_t len = 0;

    if (argc != 2)
    {
        fprintf(stderr, "missing argument: filename\n");
        return 1;
    }

    inputs = fopen(argv[1], "r");
    if (inputs == NULL)
    {
        fprintf(stderr, "cannot open %s\n", argv[1]);
        return 1;
    }

    while (getline(&line, &len, inputs) != -1)
    {
        char *end = line;
        long val = 0;

        errno = 0;
        val = strtol(line, &end, 0);
        if (line == end || errno != 0)
        {
            fprintf(stderr, "cannot parse %s\n", line);
            return 1;
        }

        output += val;
    }

    free(line);
    fclose(inputs);

    printf("%ld\n", output);
    return 0;
}
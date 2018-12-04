#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool test(const char *input, int target)
{
    int i, j;

    for (i = 0; i < strlen(input); i++)
    {
        char val = input[i];
        int count = 0;
        for (j = 0; j < strlen(input); j++)
        {
            if (input[j] == val)
            {
                count++;
            }
        }
        if (count == target)
        {
            return true;
        }
    }

    return false;
}

int main(int argc, char *argv[])
{
    long count_a = 0;
    long count_b = 0;
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
        if (test(line, 2))
        {
            count_a++;
        }
        if (test(line, 3))
        {
            count_b++;
        }
    }

    free(line);
    fclose(inputs);

    printf("%ld\n", count_a * count_b);
    return 0;
}
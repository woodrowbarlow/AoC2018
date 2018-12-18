#include<ctype.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

typedef unsigned char byte;

size_t input_size = 0;
byte *input = NULL;
bool *eliminations = NULL;

int get_first()
{
    int i;
    for (i = 0; i < input_size; i++)
    {
        if (!eliminations[i])
        {
            return i;
        }
    }
    return -1;
}

int get_next(int current)
{
    int i;
    for (i = current + 1; i < input_size; i++)
    {
        if (!eliminations[i])
        {
            return i;
        }
    }
    return -1;
}

int find_eliminations(void)
{
    int current = get_first();
    if (current == -1)
    {
        return 0;
    }

    while (true)
    {
        int next = get_next(current);
        if (next == -1)
        {
            return 0;
        }
        if (input[current] + 0x20 == input[next] ||
            input[current] - 0x20 == input[next])
        {
            eliminations[current] = true;
            eliminations[next] = true;
            if (tolower(eliminations[current]) != tolower(eliminations[next]))
                printf("%c%c\n", eliminations[current], eliminations[next]);
            return 2;
        }
        current = next;
    }

    return 0;
}

void print_final_count(void)
{
    int sum = 0;
    int i;
    for (i = 0; i < input_size; i++)
    {
        if (!eliminations[i])
        {
            sum++;
        }
    }
    printf("%d\n", sum);
}

int main(int argc, char *argv[])
{
    int ret = 0;
    int fd = -1;
    struct stat sb;

    if (argc != 2)
    {
        fprintf(stderr, "missing argument: filename\n");
        return 1;
    }

    fd = open(argv[1], O_RDONLY);
    if (fd == -1)
    {
        fprintf(stderr, "failed to open %s\n", argv[1]);
        return 1;
    }

    if (fstat(fd, &sb) == -1)
    {
        fprintf(stderr, "fstat failed\n");
        ret = 1;
        goto exit;
    }

    // minus one because we don't need the EOF byte at the end
    input_size = sb.st_size - 1;
    input = mmap(NULL, input_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (input == MAP_FAILED)
    {
        fprintf(stderr, "mmap failed\n");
        ret = 1;
        goto exit;
    }

    eliminations = malloc(input_size * sizeof(bool));
    if (!eliminations)
    {
        fprintf(stderr, "malloc failed\n");
        ret = 1;
        goto exit;
    }

    do
    {
        ret = find_eliminations();
    } while (ret != 0);

    print_final_count();

exit:
    if (eliminations != NULL)
    {
        free(eliminations);
    }
    if (input != NULL)
    {
        munmap(input, sb.st_size);
    }
    if (fd != -1)
    {
        close(fd);
    }

    return ret;
}
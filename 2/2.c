#include <fcntl.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

// i'm pretty proud of this one

#define HASH_LENGTH 26

int diff(const char *a, const char *b)
{
    int diff = 0;
    for (int i = 0; i < HASH_LENGTH; i++)
    {
        if (a[i] != b[i])
        {
            diff++;
        }
    }
    return diff;
}

void print_output(const char *a, const char *b)
{
    for (int i = 0; i < HASH_LENGTH; i++)
    {
        if (a[i] == b[i])
        {
            putchar(a[i]);
        }
    }
    putchar('\n');
}

int main(int argc, char *argv[])
{
    int ret = 0;
    char (*input)[HASH_LENGTH + 1] = NULL;
    int fd = -1;
    struct stat sb;
    off_t lines = 0;

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

    input = mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (input == MAP_FAILED)
    {
        fprintf(stderr, "mmap failed\n");
        ret = 1;
        goto exit;
    }

    lines = sb.st_size / (HASH_LENGTH + 1);

    for (int i = 0; i < lines; i++)
    {
        for (int j = i; j < lines; j++)
        {
            if (diff(input[i], input[j]) == 1)
            {
                print_output(input[i], input[j]);
                goto exit;
            }
        }
    }

exit:
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
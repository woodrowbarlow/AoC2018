#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

struct node {
    long val;
    struct node *left;
    struct node *right;
};

struct node *root;

bool contains(struct node *root, long val)
{
    if (root == NULL)
    {
        return false;
    }
    else if (val == root->val)
    {
        return true;
    }
    else if (val < root->val)
    {
        return contains(root->left, val);
    }
    else
    {
        return contains(root->right, val);
    }
}

void insert(struct node **root, long val)
{
    if (*root == NULL)
    {
        *root = malloc(sizeof(struct node));
        (*root)->val = val;
        (*root)->left = NULL;
        (*root)->right = NULL;
        return;
    }

    if (val < (*root)->val)
    {
        return insert(&(*root)->left, val);
    }
    else
    {
        return insert(&(*root)->right, val);
    }
}

void clear(struct node **root)
{
    if ((*root)->left != NULL)
    {
        return clear(&(*root)->left);
    }
    if ((*root)->right != NULL)
    {
        return clear(&(*root)->right);
    }

    free(*root);
    *root = NULL;
}

int main(int argc, char *argv[])
{
    long output = 0;
    FILE *inputs = NULL;
    char *line = NULL;
    size_t len = 0;
    int ret = 1;

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

    while (ret != 0)
    {
        while (getline(&line, &len, inputs) != -1)
        {
            char *end = line;
            long val = 0;

            insert(&root, output);

            errno = 0;
            val = strtol(line, &end, 0);
            if (line == end || errno != 0)
            {
                fprintf(stderr, "cannot parse %s\n", line);
                return 1;
            }

            output += val;
            if (contains(root, output))
            {
                printf("%ld\n", output);
                ret = 0;
                break;
            }
        }

        fseek(inputs, 0, SEEK_SET);
    }

    free(line);
    fclose(inputs);
    clear(&root);

    return ret;
}
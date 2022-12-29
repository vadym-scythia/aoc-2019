#include <stdio.h>

typedef struct space_object_
{
    char* name;
    int orbits_count;
    struct space_object_ *parent;
} space_object;

void init_space_object(space_object *current, space_object *parent)
{
    if (parent == NULL)
    {
        current->orbits_count = 0;
    }
    else
    {
        current->orbits_count = parent->orbits_count + 1;
        current->parent = parent;
    }
}

void is_object_exists(space_object[] arr, char *name)
{

}

int main(int argc, char **argv[])
{
    // R45)497

    // todo:
    // create cycle, where
    // getting one pair of orbits per time from file (!)
    // creating parent form the pair, if it didn't exist
    // creating child, if it didn't exist
    // add child's orbits count to total count;
    int total_orbits = 0;
    space_object a, b, c;
    
    init_space_object(&a, NULL);
    total_orbits += a.orbits_count;

    init_space_object(&b, &a);
    init_space_object(&c, &b);

    printf("%d", total_orbits);

    return 0;
}
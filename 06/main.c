#include <stdio.h>

typedef struct space_object_
{
    char name[3];
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

//void is_object_exists(space_object[] arr, char *name)
//{

//}

int get_position_if_exists(space_object arr[], char name[])
{
    for (int i = 0; i < 100; i++)
    {
        if (arr[i].name[0] == name[0] && arr[i].name[1] == name[1] && arr[i].name[2] == name[2])
        {
            return i;
        }
    }

    return -1;
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
    char *inputString = "R45)497,TYR)159,BZG)R45\0";
    char *pointerToInputString = inputString;
    int total_orbits = 0, i = 0;
    space_object space_objects[100];

    while (*pointerToInputString != '\0')
    {
        if (*pointerToInputString == ',')
        {
            pointerToInputString++;
            continue;
        }

        if (*(pointerToInputString + 3) == ')')
        {
            space_object parent, child;
            char parent_name[3], child_name[3];
            
            parent_name[0] = *pointerToInputString;
            parent_name[1] = *(pointerToInputString + 1);
            parent_name[2] = *(pointerToInputString + 2);
            child_name[0] = *(pointerToInputString + 4);
            child_name[1] = *(pointerToInputString + 5);
            child_name[2] = *(pointerToInputString + 6);
            
            int parent_position = get_position_if_exists(space_objects, parent_name);

            if (parent_position == -1)
            {
                space_objects[i].name[0] = parent_name[0];
                space_objects[i].name[1] = parent_name[1];
                space_objects[i].name[2] = parent_name[2];
                space_objects[i].orbits_count = 0;
                i++;
            }

            int child_position = get_position_if_exists(space_objects, child_name);

            // todo: if child exists, check whether is it a parent
            // if yes, add one more orbit to all its children
            if (child_position == -1)
            {
                space_objects[i].name[0] = child_name[0];
                space_objects[i].name[1] = child_name[1];
                space_objects[i].name[2] = child_name[2];
                space_objects[i].orbits_count = 1;
                i++;
            }
            else
            {
                space_objects[child_position].orbits_count += 1;
            }

            pointerToInputString += 6;
        }

        pointerToInputString++;
    }

    for (int j = 0; j < i; j++)
    {
        total_orbits += space_objects[j].orbits_count;
    }

    printf("%d", total_orbits);

    return 0;
}
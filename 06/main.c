#include <stdio.h>
#include <malloc.h>

#define MAX_LINE_LENGTH 9

typedef struct {
    char *array;
    size_t used;
    size_t size;
} String;

void init_string(String *ptr, size_t initSize) {
    ptr->array = malloc(initSize * sizeof(char));
    ptr->used = 0;
    ptr->size = initSize;
}

void insert_string(String *ptr, char element[]) {
    if (ptr->size <= ptr->used + MAX_LINE_LENGTH) {
        ptr->size = ptr->used * 2;
        char *tmp = realloc(ptr->array, ptr->size * sizeof(char));

        if (tmp != NULL) {
            ptr->array = tmp;
        }
    }

    strcpy(ptr->array + ptr->used, element);
    ptr->used += sizeof(element);
}

char *read_file(char *path) {
    String result;
    init_string(&result, 1000);
    char line[MAX_LINE_LENGTH] = {0};

    FILE *file = fopen(path, "r");

    if (!file) {
        perror(path);
    }

    while (fgets(line, MAX_LINE_LENGTH, file)) {
        line[7] = ',';
        line[8] = '\0';
        insert_string(&result, line);
    }

    if (fclose(file)) {
        perror(path);
    }

    result.array[result.used - 1] = '\0';

    return result.array;
}

typedef struct space_object_ {
    char name[3];
    int orbits_count;
    struct space_object_ *ptr_parent;
    struct space_object_ **ptr_children;
    size_t num_children;
    size_t max_children;
} space_object;

space_object *init_space_object()
{
    space_object *ptr_result, *tmp = malloc(sizeof(space_object));

    if (tmp != NULL) {
        ptr_result = tmp;
        ptr_result->orbits_count = 0;
        ptr_result->num_children = 0;
        ptr_result->max_children = 10;
        ptr_result->ptr_children = NULL;
        ptr_result->ptr_parent = NULL;
    }
}

space_object *create_pair(space_object *ptr_parent, space_object *ptr_child) {
    if (ptr_parent == NULL || ptr_child == NULL) {
        printf("ptr_parent or ptr_child was NULL in create_pair");
        return NULL;
    }

    if (ptr_parent->num_children == ptr_parent->max_children) {
        ptr_parent->max_children *= 2;
        space_object **tmp = realloc(ptr_parent->ptr_children, ptr_parent->max_children * sizeof(struct space_object *));
        if (tmp != NULL) {
            ptr_parent->ptr_children = tmp;
            for (int i = ptr_parent->num_children; i < ptr_parent->max_children; i++) {
                ptr_parent->ptr_children[i] = malloc(sizeof(struct space_object_));
            }
        } else
            return NULL;
    }

    if (ptr_parent->ptr_children == NULL) {
        space_object **tmp = malloc(ptr_parent->max_children * sizeof(struct space_object *));
        if (tmp != NULL) {
            ptr_parent->ptr_children = tmp;
            for (int i = 0; i < ptr_parent->max_children; i++) {
                ptr_parent->ptr_children[i] = malloc(sizeof(struct space_object_));
            }
        } else
            return NULL;
    }

    ptr_child->ptr_parent = ptr_parent;
    ptr_parent->ptr_children[ptr_parent->num_children] = ptr_child;
    ptr_parent->num_children++;

    return ptr_parent;
}

void add_parent(space_object *ptr_parent)
{
    if (ptr_parent == NULL)
        printf("invalid ptr_parent in add_parent");

    int num_children = ptr_parent->num_children;
    if (num_children < 0)
        printf("invalid num_children in add_parent");

    for (int i = 0; i < num_children; i++) {
        space_object *ptr_current_child = ptr_parent->ptr_children[i];
        if (ptr_current_child == NULL)
            printf("invalid ptr_current_child in add_parent");

        ptr_current_child->orbits_count++;
        add_parent(ptr_current_child);
    }
}

void add_child(space_object *ptr_child)
{
    space_object *ptr_child_cpy = ptr_child;
    space_object *ptr_parent = ptr_child->ptr_parent;
    if (ptr_parent == NULL)
        printf("ptr_parent is in NULL in add_child");

    int parent_orbits = ptr_parent->orbits_count;
    if (parent_orbits < 0)
        printf("invalid parent_orbits value in add child");

    ptr_child_cpy->orbits_count = parent_orbits + 1;
    int num_children = ptr_child_cpy->num_children;
    if (num_children < 0)
        printf("invalid num_children in add_child");
    else if (num_children == 0)
        return;
    else {
        for (int i = 0; i < ptr_child_cpy->num_children; i++) {
            add_child(ptr_child_cpy->ptr_children[i]);
        }
    }
}

typedef struct store_ {
    space_object **ptr_space_objects;
    size_t num_store;
    size_t max_store;
} store;

store *init_store() {
    store *ptr_result, *tmp = malloc(sizeof(store));

    if (tmp != NULL) {
        ptr_result = tmp;
        ptr_result->num_store = 0;
        ptr_result->max_store = 10;
        ptr_result->ptr_space_objects = NULL;
    }
}

store* add_to_store(store *ptr_store, space_object *ptr_parent, space_object *ptr_child) {
    if (ptr_store == NULL && ptr_parent == NULL && ptr_child == NULL) {
        printf("ptr_store or ptr_parent or ptr_child was NULL in create_pair");
        return NULL;
    }

    if (ptr_store->num_store == ptr_store->max_store) {
        ptr_store->max_store *= 2;
        space_object **tmp = realloc(ptr_store->ptr_space_objects, ptr_store->max_store * sizeof(struct space_object *));
        if (tmp != NULL) {
            ptr_store->ptr_space_objects = tmp;
            for (int i = ptr_store->num_store; i < ptr_store->max_store; i++) {
                ptr_store->ptr_space_objects[i] = malloc(sizeof(struct space_object_));
            }
        } else
            return NULL;
    }

    if (ptr_store->ptr_space_objects == NULL) {
        space_object **tmp = malloc(ptr_store->max_store * sizeof(struct space_object *));
        if (tmp != NULL) {
            ptr_store->ptr_space_objects = tmp;
            for (int i = 0; i < ptr_store->max_store; i++) {
                ptr_store->ptr_space_objects[i] = malloc(sizeof(struct space_object_));
            }
        } else
            return NULL;
    }

    if (ptr_parent != NULL) {
        ptr_store->ptr_space_objects[ptr_store->num_store] = ptr_parent;
        ptr_store->num_store++;
    }

    if (ptr_child != NULL) {
        ptr_store->ptr_space_objects[ptr_store->num_store] = ptr_child;
        ptr_store->num_store++;
    }

    return ptr_store;
}

space_object *is_in_store(store *ptr_store, space_object *ptr_candidate) {
    store *ptr_store_cpy = ptr_store;

    for (int i = 0; i < ptr_store_cpy->num_store; i++) {
        space_object *ptr_space_object = ptr_store_cpy->ptr_space_objects[i];
        if (ptr_space_object->name[0] == ptr_candidate->name[0]
        && ptr_space_object->name[1] == ptr_candidate->name[1]
        && ptr_space_object->name[2] == ptr_candidate->name[2])
            return ptr_space_object;
    }

    return NULL;
}

int count_total_orbits_in_store(store *ptr_store) {
    int result = 0;
    store *ptr_store_cpy = ptr_store;

    for (int i = 0; i < ptr_store_cpy->num_store; i++) {
        result += ptr_store_cpy->ptr_space_objects[i]->orbits_count;
    }

    return result;
}

int main(int argc, char **argv) {
    char *ptr_input_string = read_file("input.txt");
    int total_orbits = 0;
    store *ptr_store = init_store();

    while (*ptr_input_string != '\0') {
        space_object *ptr_parent = init_space_object(), *ptr_child = init_space_object(),
                *ptr_check_parent = NULL, *ptr_check_child = NULL;

        if (*(ptr_input_string + 3) == ')') {
            ptr_parent->name[0] = *ptr_input_string;
            ptr_parent->name[1] = *(ptr_input_string + 1);
            ptr_parent->name[2] = *(ptr_input_string + 2);
            ptr_child->name[0] = *(ptr_input_string + 4);
            ptr_child->name[1] = *(ptr_input_string + 5);
            ptr_child->name[2] = *(ptr_input_string + 6);

            ptr_check_parent = is_in_store(ptr_store, ptr_parent);
            if (ptr_check_parent != NULL)
                ptr_parent = ptr_check_parent;

            ptr_check_child = is_in_store(ptr_store, ptr_child);
            if (ptr_check_child != NULL)
                ptr_child = ptr_check_child;

            create_pair(ptr_parent, ptr_child);

            if (ptr_check_parent != NULL && ptr_check_child != NULL) {
                add_child(ptr_check_child);
            } else if (ptr_check_parent != NULL && ptr_check_child == NULL) {
                add_child(ptr_child);
                ptr_store = add_to_store(ptr_store, NULL, ptr_child);
            } else if (ptr_check_parent == NULL && ptr_check_child == NULL) {
                add_parent(ptr_parent);
                ptr_store = add_to_store(ptr_store, ptr_parent, ptr_child);
            } else {
                add_parent(ptr_parent);
                ptr_store = add_to_store(ptr_store, ptr_parent, NULL);
            }

            ptr_input_string += 8;
        }
    }

    total_orbits = count_total_orbits_in_store(ptr_store);
    printf("%d", total_orbits);

    return 0;
}
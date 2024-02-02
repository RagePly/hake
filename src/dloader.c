#include <hake/dloader.h>
#include <hake/util/id.h>
#include <hake/util/paths.h>
#include <dlfcn.h>
#include <string.h>
#include <stdio.h>

HAKE_TEMPLATE_DYNAMIC_ARRAY(hake_GoalClass, hake_GoalClassArray)

hake_GoalClass hake_GoalClass_new(char *name, hake_DLibrary library) {
    hake_GoalClass goal_class = malloc(sizeof(struct hake_GoalClass_s));
    char *name_copy = malloc(strlen(name) + 1);
    strcpy(name_copy, name);
    hake_DLibrary library_copy = hake_DLibrary_clone(library);

    goal_class->name = name_copy;
    goal_class->refcount = 1;
    goal_class->library = library_copy;
    goal_class->goal_status = hake_DLibrary_load(library, name, "goal_status");
    goal_class->goal_strategy = hake_DLibrary_load(library, name, "goal_strategy");
    goal_class->state_free = hake_DLibrary_load(library, name, "state_free");

    return goal_class;
}

hake_Goal hake_GoalClass_new_goal(hake_GoalClass goal_class) {
    return NULL; 
}

hake_GoalClass hake_GoalClass_clone(hake_GoalClass goal_class) {
    goal_class->refcount++;
    return goal_class;
}

void hake_GoalClass_free(hake_GoalClass goal_class) {
    if ((--goal_class->refcount) == 0) {
        free(goal_class->name);
        hake_DLibrary_free(goal_class->library);
        free(goal_class);
    }
}


hake_DLibrary hake_DLibrary_new(void *handle) {
    hake_DLibrary library = malloc(sizeof(struct hake_DLibrary_s));
    hake_Id_gen(library->id);
    library->handle = handle;
    library->refcount = 1;
    return library;
}

hake_DLibrary hake_DLibrary_clone(hake_DLibrary library) {
    library->refcount++;
    return library;
}

void * hake_DLibrary_load(hake_DLibrary library, char *namespace, char *name) {
    char *hake = "_hake_";
    int hake_l = strlen(hake);
    int namespace_l = strlen(namespace);
    int name_l = strlen(name);

    char *fname = malloc(namespace_l + hake_l + name_l + 1);
    strcpy(fname, namespace);
    strcat(fname, hake);
    strcat(fname, name);

    dlerror();
    void *sym = dlsym(library->handle, fname);
    
    char* error = dlerror();
    if (error != NULL)
        fprintf(stderr, "%s:%d dlerror -> %s\n", __FILE__, __LINE__, error);

    free(fname);
    return sym;
}

void hake_DLibrary_free(hake_DLibrary library) {
    if (--(library->refcount) == 0) {
        dlclose(library->handle);     
        free(library);
    }
}

hake_GoalLoader hake_GoalLoader_init() {
    hake_GoalClassArray classes = hake_GoalClassArray_init();
    hake_GoalLoader loader = malloc(sizeof(struct hake_GoalLoader_s));
    loader->classes = classes;
    return loader;
}

void hake_GoalLoader_free(hake_GoalLoader loader) {
    for (int i = 0; i < loader->classes->size; i++)
        hake_GoalClass_free(loader->classes->data[i]);
    hake_GoalClassArray_free(loader->classes);
    free(loader);
}

int hake_GoalLoader_load(hake_GoalLoader loader, char *filename) {
    void *handle = dlopen(filename, RTLD_LAZY);
    if (!handle) {
        fprintf(stderr, "%s\n", dlerror());
        return HAKE_GOAL_LOAD_CANT_LOAD;
    }

    char *leaf = hake_path_get_leaf(filename); 
    char *name = hake_path_new_without_extension(leaf);
    hake_DLibrary library = hake_DLibrary_new(handle); 
    hake_GoalClass goal_class = hake_GoalClass_new(name, library);
    hake_GoalClassArray_append(loader->classes, goal_class);

    return HAKE_GOAL_LOAD_OK;
}

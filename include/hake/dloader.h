#ifndef _HAKE_DLOADER_H_
#define _HAKE_DLOADER_H_

#include <hake/defines.h>
#include <hake/goal.h>
#include <hake/util/da.h>

typedef struct hake_DLibrary_s {
    hake_Id id;
    int refcount; /* number of classes in use referencing this library */
    void *handle; /* handle to the loaded library */
} *hake_DLibrary;

// TODO: add function to load entry and entry_count
hake_DLibrary hake_DLibrary_new(void *handle);
hake_DLibrary hake_DLibrary_clone(hake_DLibrary library);
void * hake_DLibrary_load(hake_DLibrary library, char *namespace, char *name);
void hake_DLibrary_free(hake_DLibrary library);

typedef struct hake_GoalClass_s {
    char *name;
    int refcount;
    hake_DLibrary library;
    hake_GoalStatus goal_status;
    hake_GoalStrategy goal_strategy;
    hake_UserStateFree state_free;
} *hake_GoalClass;

hake_GoalClass hake_GoalClass_new(char *name, hake_DLibrary library);
hake_GoalClass hake_GoalClass_clone(hake_GoalClass goal_class);
hake_Goal hake_GoalClass_new_goal(hake_GoalClass goal_class);
void hake_GoalClass_free(hake_GoalClass goal_class);

HAKE_TEMPLATE_DYNAMIC_ARRAY_DEFINE(hake_GoalClass, hake_GoalClassArray)

typedef struct hake_GoalLoader_s {
    hake_GoalClassArray classes; 
} *hake_GoalLoader;

hake_GoalLoader hake_GoalLoader_init();
void hake_GoalLoader_free(hake_GoalLoader loader);

enum hake_GoalLoaderResult {
    HAKE_GOAL_LOAD_OK = 0,
    HAKE_GOAL_LOAD_CANT_LOAD,
};

// Respect the entry to generate many classes (cloned DLibraries)
int hake_GoalLoader_load(hake_GoalLoader loader, char *filename);


#endif /* _HAKE_DLOADER_H_ */

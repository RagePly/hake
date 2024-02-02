#include <hake/goal.h>
#include <hake/util/id.h>
#include <string.h>
#include <stdlib.h>

HAKE_TEMPLATE_DYNAMIC_ARRAY(hake_Goal, hake_GoalArray)

hake_Goal hake_Goal_init(char *name) {
    int name_length = strlen(name);
    char *owned_name = malloc(name_length + 1);
    strcpy(owned_name, name); 
    
    hake_Goal goal = malloc(sizeof(struct hake_Goal_s));
    goal->name = owned_name;
    goal->depends = hake_GoalArray_init();
    
    hake_Id_gen(goal->id);

    return goal;
}

void hake_Goal_free(hake_Goal goal) {
    hake_GoalArray_free(goal->depends);
    if (goal->state_free != NULL) goal->state_free(goal);
    free(goal->name);
    free(goal);
}

void hake_Goal_add_dependancy(hake_Goal goal, hake_Goal dependancy) {
    hake_GoalArray_append(goal->depends, dependancy);
}

int hake_Goal_check_dependencies(hake_Goal goal) {
    for (int i = 0; i < goal->depends->size; i++) {
        int status = hake_Goal_check_status(goal->depends->data[i]);
        if (status) return status;
    }
    return HAKE_STATUS_DONE;
}
int hake_Goal_check_status(hake_Goal goal) {
    if (hake_Goal_check_dependencies(goal)) return HAKE_STATUS_MISSING_DEPENDENCY;
    return goal->goal_status(goal);
}

hake_Goal hake_create_goal(
    char *name, 
    hake_UserState state, 
    hake_UserStateFree state_free, 
    hake_GoalStatus goal_status
) {
    hake_Goal goal = hake_Goal_init(name);
    goal->state = state;
    goal->state_free = state_free;
    goal->goal_status = goal_status;
    return goal;
}

int hake_goal_status_done(hake_Goal) {
    return HAKE_STATUS_DONE;
}

int hake_goal_status_not_done(hake_Goal) {
    return HAKE_STATUS_NOT_DONE;
}

int hake_goal_status_toggle(hake_Goal goal) {
    return *(hake_ToggleState*) goal->state ? HAKE_STATUS_DONE : HAKE_STATUS_NOT_DONE;
}

int hake_goal_strategy_toggle(hake_Goal goal) {
    *(hake_ToggleState*) goal->state = ! *(hake_ToggleState*) goal->state;
    return HAKE_STRATEGY_COMPLETE;
}

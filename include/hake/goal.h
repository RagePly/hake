#ifndef _HAKE_GOAL_H_
#define _HAKE_GOAL_H_

#include <hake/defines.h>
#include <hake/util/da.h>
#include <stdlib.h>

enum hake_StatusFlag {
    HAKE_STATUS_DONE = 0,
    HAKE_STATUS_NOT_DONE,
    HAKE_STATUS_MISSING_DEPENDENCY,
};

enum hake_StrategyFlag {
    HAKE_STRATEGY_COMPLETE = 0,
    HAKE_STRATEGY_FAILED,
};

typedef struct hake_Goal_s *    hake_Goal;
typedef void *                  hake_UserState;

/* Function definitions */
typedef int     (*hake_GoalStatus)        (hake_Goal);
typedef int     (*hake_GoalStrategy)      (hake_Goal);
typedef void    (*hake_UserStateFree)     (hake_Goal);

HAKE_TEMPLATE_DYNAMIC_ARRAY_DEFINE(hake_Goal, hake_GoalArray)

struct hake_Goal_s {
    /* Identification */
    hake_Id id;
    char *name;
    
    hake_GoalArray depends;

    /* State */
    hake_UserState state;
    hake_UserStateFree state_free;

    /* State Behaviour */
    hake_GoalStatus goal_status;    
};

/* Goal methods */
hake_Goal hake_Goal_init(char *name);
void hake_Goal_free(hake_Goal goal);
void hake_Goal_add_dependancy(hake_Goal goal, hake_Goal dependancy);
int hake_Goal_check_dependencies(hake_Goal goal);
int hake_Goal_check_status(hake_Goal goal);

/* Utility functions */

hake_Goal hake_create_goal( char *name, hake_UserState state, hake_UserStateFree state_free, hake_GoalStatus goal_status);

/* Basic goal functions */
int hake_goal_status_done(hake_Goal goal);
int hake_goal_status_not_done(hake_Goal goal);

/* Basic goal toggle */
typedef int hake_ToggleState;
int hake_goal_status_toggle(hake_Goal goal);
int hake_goal_strategy_toggle(hake_Goal goal);

#endif /* _HAKE_GOAL_H_ */

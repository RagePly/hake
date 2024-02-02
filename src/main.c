#include <hake/defines.h>
#include <hake/goal.h>
#include <hake/dloader.h>
#include <stdio.h>

void hake_print_usagef(FILE* file) { 
    fputs("usage: hake\n", file);
}

void hake_print_usage_err() { hake_print_usagef(stderr); }
void hake_print_usage_std() { hake_print_usagef(stdout); }

int main(int argc, char **argv) {
    if (argc > 1) {
        hake_print_usage_err();
        return HAKE_EXIT_ERR;
    }

    hake_Goal goal_ok0 = hake_create_goal("DummyDone0", NULL, NULL, hake_goal_status_done);
    hake_Goal goal_ok1 = hake_create_goal("DummyDone1", NULL, NULL, hake_goal_status_done);
    hake_Goal goal_ok2 = hake_create_goal("DummyDone2", NULL, NULL, hake_goal_status_done);
    hake_Goal goal_no0 = hake_create_goal("DummyNotDone0", NULL, NULL, hake_goal_status_not_done);
    hake_Goal goal_no1 = hake_create_goal("DummyNotDone1", NULL, NULL, hake_goal_status_not_done);
    hake_ToggleState toggle_state = 0;
    hake_Goal goal_toggle = hake_create_goal("DummyToggle0", &toggle_state, NULL, hake_goal_status_toggle);
    
    hake_Goal_add_dependancy(goal_ok0, goal_ok2);
    hake_Goal_add_dependancy(goal_ok1, goal_no0);
    hake_Goal_add_dependancy(goal_no0, goal_ok2);
    hake_Goal_add_dependancy(goal_no1, goal_no0);

    printf("ok0 [%s] %s -> %d\n", goal_ok0->id, goal_ok0->name, hake_Goal_check_status(goal_ok0));
    printf("ok1 [%s] %s -> %d\n", goal_ok1->id, goal_ok1->name, hake_Goal_check_status(goal_ok1));
    printf("ok2 [%s] %s -> %d\n", goal_ok2->id, goal_ok2->name, hake_Goal_check_status(goal_ok2));
    printf("no0 [%s] %s -> %d\n", goal_no0->id, goal_no0->name, hake_Goal_check_status(goal_no0));
    printf("no1 [%s] %s -> %d\n", goal_no1->id, goal_no1->name, hake_Goal_check_status(goal_no1));
    printf("toggle0 [%s] %s -> %d\n", goal_toggle->id, goal_toggle->name, hake_Goal_check_status(goal_toggle));
    printf("toggle0 [%s] %s :: %d\n", goal_toggle->id, goal_toggle->name, hake_goal_strategy_toggle(goal_toggle));
    printf("toggle0 [%s] %s -> %d\n", goal_toggle->id, goal_toggle->name, hake_Goal_check_status(goal_toggle));

    hake_Goal_free(goal_ok0);
    hake_Goal_free(goal_ok1);
    hake_Goal_free(goal_ok2);
    hake_Goal_free(goal_no0);
    hake_Goal_free(goal_no1);
    hake_Goal_free(goal_toggle);

    hake_GoalLoader loader = hake_GoalLoader_init();
    hake_GoalLoader_load(loader, "build/goals/filesystem.so");
    hake_GoalLoader_free(loader);

    return HAKE_EXIT_OK;
}

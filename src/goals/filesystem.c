#include <hake/goal.h>
#include <hake/goals/filesystem.h>
#include <stdio.h>
#include <errno.h>

static const int    filesystem_hake_export_count = 1;
static const char * filesystem_hake_export[] = { "touch", };

int filesystem_touch_goal_status(hake_Goal goal) {
    filesystem_State state = (filesystem_State) goal->state;
    char *filename = state->filename;
    FILE *fp = fopen(filename, "r"); 

    if (fp == NULL) {
        int err = errno;
        switch (err) {
            default:
                fprintf(stderr, "%s:%d unknown errno %d\n", __FILE__, __LINE__, err);
        }
        return HAKE_STATUS_NOT_DONE;
    }

    fclose(fp);
    return HAKE_STATUS_DONE;
}

int filesystem_touch_goal_strategy(hake_Goal goal) {
    filesystem_State state = (filesystem_State) goal->state;
    char *filename = state->filename;
    FILE *fp = fopen(filename, "w"); 

    if (fp == NULL) {
        int err = errno;
        switch (err) {
            default:
                fprintf(stderr, "%s:%d unknown errno %d\n", __FILE__, __LINE__, err);
        }
        return HAKE_STRATEGY_FAILED;
    }

    fclose(fp);
    return HAKE_STRATEGY_COMPLETE;
}

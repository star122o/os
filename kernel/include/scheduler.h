#ifndef SCHEDULER_H
#define SCHEDULER_H
#include <stdint.h>

#define MAX_TASKS 16

struct task {
    uint32_t id;
    uint32_t esp;
    uint32_t eip;
    int active;
};

void scheduler_init(void);
int scheduler_add_task(uint32_t eip);
void scheduler_switch(void);
struct task* scheduler_current(void);

#endif

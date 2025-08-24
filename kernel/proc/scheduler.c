#include <scheduler.h>
#include <stdint.h>

static struct task tasks[MAX_TASKS];
static int current = 0;
static int num_tasks = 0;

void scheduler_init(void) {
    for (int i = 0; i < MAX_TASKS; i++) {
        tasks[i].active = 0;
        tasks[i].id = i;
    }
    current = 0;
    num_tasks = 0;
}

int scheduler_add_task(uint32_t eip) {
    if (num_tasks >= MAX_TASKS) return -1;
    tasks[num_tasks].eip = eip;
    tasks[num_tasks].esp = 0;
    tasks[num_tasks].active = 1;
    num_tasks++;
    return num_tasks - 1;
}

void scheduler_switch(void) {
    int next = (current + 1) % num_tasks;
    while (!tasks[next].active) {
        next = (next + 1) % num_tasks;
        if (next == current) return;
    }
    current = next;
}

struct task* scheduler_current(void) {
    return &tasks[current];
}

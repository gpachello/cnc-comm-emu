#include "cnc_state.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdio.h>

void cnc_state_init(cnc_state_t *s)
{
    if (!s) return;
    s->part_count = 100;
    strncpy(s->program, "O0001", sizeof(s->program)-1);
    s->program[sizeof(s->program)-1] = '\0';
    strncpy(s->status, "IDLE", sizeof(s->status)-1);
    s->status[sizeof(s->status)-1] = '\0';
    s->simulate_random = 1;
    /* seed random if needed */
    srand((unsigned)time(NULL) ^ (uintptr_t)s);
}

void cnc_state_tick(cnc_state_t *s)
{
    if (!s) return;
    if (s->simulate_random) {
        /* small random drift */
        int delta = (rand() % 5); /* 0..4 */
        if ((rand() & 1) == 0) s->part_count += delta;
        else s->part_count += 0; /* sometimes stable */

        /* random program changes rarely */
        if ((rand() % 100) < 5) {
            int n = (rand() % 8000) + 1;
            snprintf(s->program, sizeof(s->program), "O%04d", n);
        }

        /* random status */
        int r = rand() % 100;
        if (r < 75) strncpy(s->status, "RUN", sizeof(s->status));
        else if (r < 95) strncpy(s->status, "IDLE", sizeof(s->status));
        else strncpy(s->status, "ALARM", sizeof(s->status));
    }
}

void cnc_state_set_random(cnc_state_t *s, int on)
{
    if (!s) return;
    s->simulate_random = on ? 1 : 0;
}

int32_t cnc_state_get_part_count(const cnc_state_t *s)
{
    if (!s) return 0;
    return s->part_count;
}

const char* cnc_state_get_program(const cnc_state_t *s)
{
    if (!s) return "";
    return s->program;
}

const char* cnc_state_get_status(const cnc_state_t *s)
{
    if (!s) return "";
    return s->status;
}

void cnc_state_set_program(cnc_state_t *s, const char *prog)
{
    if (!s || !prog) return;
    strncpy(s->program, prog, sizeof(s->program)-1);
    s->program[sizeof(s->program)-1] = '\0';
}

void cnc_state_set_part_count(cnc_state_t *s, int32_t v)
{
    if (!s) return;
    s->part_count = v;
}

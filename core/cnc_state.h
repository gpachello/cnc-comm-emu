#ifndef CNC_STATE_H
#define CNC_STATE_H

#include <stdint.h>

typedef struct {
    int32_t part_count;
    char program[32];
    char status[16];   /* "IDLE", "RUN", "ALARM" */
    int simulate_random; /* 0 = fixed, 1 = randomize responses */
} cnc_state_t;

/* initialize with defaults */
void cnc_state_init(cnc_state_t *s);

/* tick (advance) state - can change values if randomize on */
void cnc_state_tick(cnc_state_t *s);

/* set deterministic mode */
void cnc_state_set_random(cnc_state_t *s, int on);

/* helpers */
int32_t cnc_state_get_part_count(const cnc_state_t *s);
const char* cnc_state_get_program(const cnc_state_t *s);
const char* cnc_state_get_status(const cnc_state_t *s);
void cnc_state_set_program(cnc_state_t *s, const char *prog);
void cnc_state_set_part_count(cnc_state_t *s, int32_t v);

#endif  /* CNC_STATE_H */

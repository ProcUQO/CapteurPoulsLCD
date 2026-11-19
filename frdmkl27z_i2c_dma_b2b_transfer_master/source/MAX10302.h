#ifndef MAX10302_H
#define MAX10302_H

#include <stdint.h>

#define MAX30102_FINGER_MIN 15000

void max_init(void);
int max_read_sample(uint32_t *rouge, uint32_t *infrarouge);
int bpm_update(uint32_t ir_raw);

#endif

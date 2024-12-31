#ifndef MMA8452Q_DRIVE_H
#define MMA8452Q_DRIVE_H

void mma8452q_init(void);
void mma8452q_read_accel(int *x, int *y, int *z);
int detect_step(int x, int y, int z);

#endif

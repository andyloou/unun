#include "mma8452q_drive.h"
#include "i2c_drive.h"
#include <stdlib.h>

#define MMA8452Q_ADDRESS 0x1D // Ð?a ch? I2C c?a c?m bi?n

// Thanh ghi MMA8452Q
#define REG_CTRL_REG1  0x2A
#define REG_OUT_X_MSB  0x01

void mma8452q_init(void) {
    // Ðua MMA8452Q vào ch? d? ch?
    i2c_write(1, MMA8452Q_ADDRESS, (char[]){REG_CTRL_REG1, 0x00});

    // C?u hình ch? d? ho?t d?ng
    i2c_write(1, MMA8452Q_ADDRESS, (char[]){REG_CTRL_REG1, 0x01});
}

void mma8452q_read_accel(int *x, int *y, int *z) {
    char data[6];
    char addr = REG_OUT_X_MSB;

    i2c_start(1);
    i2c_add(1, MMA8452Q_ADDRESS, 0); // Ch?n thanh ghi b?t d?u
    i2c_data(1, addr);
    i2c_stop(1);

    i2c_start(1);
    i2c_add(1, MMA8452Q_ADDRESS, 1); // Ð?c d? li?u
    for (int i = 0; i < 6; i++) {
        data[i] = i2c_rx(1, (i == 5) ? NACK : ACK);
    }
    i2c_stop(1);

    *x = (data[0] << 8 | data[1]) >> 4;
    *y = (data[2] << 8 | data[3]) >> 4;
    *z = (data[4] << 8 | data[5]) >> 4;
}

int detect_step(int x, int y, int z) {
    static int prev_x = 0, prev_y = 0, prev_z = 0;
    static int step_count = 0;

    int delta_x = abs(x - prev_x);
    int delta_y = abs(y - prev_y);
    int delta_z = abs(z - prev_z);

    if (delta_x > 100 || delta_y > 100 || delta_z > 100) {
        step_count++;
    }

    prev_x = x;
    prev_y = y;
    prev_z = z;

    return step_count;
}

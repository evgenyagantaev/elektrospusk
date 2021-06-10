#ifndef MMA845_DRIVER_H
#define MMA845_DRIVER_H

#define MMA845_ADDRESS 0x1c
#define MMA845_SHIFTED_ADDRESS MMA845_ADDRESS<<1
#define WRITE_COMMAND MMA845_SHIFTED_ADDRESS
#define READ_COMMAND MMA845_SHIFTED_ADDRESS + 1

#define CONTROL_REG1 0x2a
#define CONTROL_REG1_CONF 0x03 // fast read, active mod

uint8_t read_register(uint8_t register_address);
void write_register(uint8_t register_address, uint8_t data);



#endif


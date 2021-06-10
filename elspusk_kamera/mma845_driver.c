#include "main.h"
#include "mma845_driver.h"

uint8_t read_register(uint8_t register_address)
{
	uint8_t buf;

	buf = WRITE_COMMAND;
	bcm2835_i2c_write((const char *)(&buf), 1);
	buf = register_address;
	bcm2835_i2c_write((const char *)(&buf), 1);
	buf = READ_COMMAND;
	bcm2835_i2c_write((const char *)(&buf), 1);
	bcm2835_i2c_read((char *)(&buf), 1);

	return buf;
}


void write_register(uint8_t register_address, uint8_t data)
{
	uint8_t buf;

	buf = WRITE_COMMAND;
	bcm2835_i2c_write((const char *)(&buf), 1);
	buf = register_address;
	bcm2835_i2c_write((const char *)(&buf), 1);
	buf = data;
	bcm2835_i2c_write((const char *)(&buf), 1);
	
}

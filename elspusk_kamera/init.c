#include "main.h"
#include "mma845_driver.h"
#include "step_driver.h"




void init(void)
{
	struct timespec sleep_interval;
	sleep_interval.tv_sec = 2;
	sleep_interval.tv_nsec = 0;	// 


	// Inicializiruyem GPIO
	bcm2835_gpio_fsel(STEP_PIN, BCM2835_GPIO_FSEL_OUTP);        	
	bcm2835_gpio_write(STEP_PIN, LOW);                          	
	bcm2835_gpio_fsel(DIR_PIN, BCM2835_GPIO_FSEL_OUTP);         	
	bcm2835_gpio_write(DIR_PIN, LOW);                           	
	bcm2835_gpio_fsel(ENB_PIN, BCM2835_GPIO_FSEL_OUTP);         	
	bcm2835_gpio_write(ENB_PIN, LOW);                           	
                                                                
	bcm2835_gpio_fsel(PARK_BUTTON, BCM2835_GPIO_FSEL_INPT);   	
	bcm2835_gpio_set_pud(PARK_BUTTON, BCM2835_GPIO_PUD_UP);                     	
	                                                            	
	bcm2835_gpio_write(ENB_PIN, 0);                             	  	     


	bcm2835_i2c_begin();                //Start I2C operations.
    bcm2835_i2c_setSlaveAddress(0x1c);  //I2C address of accelerometer
    bcm2835_i2c_set_baudrate(125000);      // baudrate	


	// configure accelerometer ****************************
	
	// konfigurirovanie data ready na int1

	// konfigurirovanie freefall na int2

	// konfigurirovanie poroga freefall

	// perevod v standby rezhim
	write_register(CONTROL_REG1, 0x00);
	printf("acc standby\r\n");
	nanosleep(&sleep_interval, NULL);
	// 8g range 
	write_register(CONFIG_REG1, 0x02);
	printf("acc 8g range set\r\n");
	nanosleep(&sleep_interval, NULL);
	// perevod v aktivnyi rezhim
	write_register(CONTROL_REG1, CONTROL_REG1_CONF);
	printf("acc configure finished\r\n");



}

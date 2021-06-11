#include "main.h"
#include "mma845_driver.h"
#include "step_driver.h"

void init(void)
{
	// Inicializiruyem GPIO
	bcm2835_gpio_fsel(STEP_PIN, BCM2835_GPIO_FSEL_OUTP);        	
	bcm2835_gpio_write(STEP_PIN, LOW);                          	
	bcm2835_gpio_fsel(DIR_PIN, BCM2835_GPIO_FSEL_OUTP);         	
	bcm2835_gpio_write(DIR_PIN, LOW);                           	
	bcm2835_gpio_fsel(ENB_PIN, BCM2835_GPIO_FSEL_OUTP);         	
	bcm2835_gpio_write(ENB_PIN, LOW);                           	
                                                                
	bcm2835_gpio_fsel(YELLOW_BUTTON, BCM2835_GPIO_FSEL_INPT);   	
	bcm2835_gpio_set_pud(YELLOW_BUTTON, 2);                     	
	bcm2835_gpio_fsel(GREEN_BUTTON, BCM2835_GPIO_FSEL_INPT);    	
	bcm2835_gpio_set_pud(GREEN_BUTTON, 2);                      	
	bcm2835_gpio_fsel(BLUE_BUTTON, BCM2835_GPIO_FSEL_INPT);     	
	bcm2835_gpio_set_pud(BLUE_BUTTON, 2);                       	
	                                                            	
	bcm2835_gpio_write(ENB_PIN, 0);                             	  	     


	bcm2835_i2c_begin();                //Start I2C operations.
    bcm2835_i2c_setSlaveAddress(0x1c);  //I2C address of accelerometer
    bcm2835_i2c_set_baudrate(125000);      // baudrate	


	// configure accelerometer ****************************
	
	// konfigurirovanie data ready na int1

	// konfigurirovanie freefall na int2

	// konfigurirovanie poroga freefall

	// perevod v aktivnyi rezhim
	write_register(CONTROL_REG1, CONTROL_REG1_CONF);



}

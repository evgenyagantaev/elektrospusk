#include "main.h"
#include "step_driver.h"


void step(void)
{
	struct timespec sleep_interval;
	sleep_interval.tv_sec = 0;
	sleep_interval.tv_nsec = STEP_DELAY;	// 

	bcm2835_gpio_write(STEP_PIN, 1);
	nanosleep(&sleep_interval, NULL);
	bcm2835_gpio_write(STEP_PIN, 0);
	nanosleep(&sleep_interval, NULL);
}

void step_clockwise(void)
{
	bcm2835_gpio_write(DIR_PIN, 1);
	step();
}

void step_counterclockwise(void)
{
	bcm2835_gpio_write(DIR_PIN, 0);
	step();
}


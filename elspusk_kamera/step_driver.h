#ifndef STEP_DRIVER_H
#define STEP_DRIVER_H

#define STEP_PIN RPI_BPLUS_GPIO_J8_31
#define DIR_PIN RPI_BPLUS_GPIO_J8_33
#define ENB_PIN RPI_BPLUS_GPIO_J8_35

#define STEP_DELAY 200000
//#define STEP_DELAY 17000000


void step(void);
void step_clockwise(void);
void step_counterclockwise(void);



#endif

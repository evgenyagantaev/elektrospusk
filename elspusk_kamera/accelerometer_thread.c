#include "main.h"
#include "mma845_driver.h"

#define ACC_THRESHOLD 113

extern int gun_shot_flag;

void *accelerometer_thread(void *param)
{
	int8_t X, Y, Z;

	struct timespec sleep_interval_short;
	sleep_interval_short.tv_sec = 0;
	sleep_interval_short.tv_nsec = 10000000;	// 10 ms 
	
	while(1)
	{                                                                           	
	                                                                            	
		read_xyz(&X, &Y, &Z);                                                   	
		//printf("%+05d   %+05d   %+05d\r\n", X, Y, Z);                           	

		int accel_summ = X + Y + Z;

		if(accel_summ > ACC_THRESHOLD)
		{
			gun_shot_flag = 1;
			printf("SHOT\r\n");                           	
			nanosleep(&sleep_interval_short, NULL);
		}
	}
}

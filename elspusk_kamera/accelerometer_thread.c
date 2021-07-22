#include "main.h"
#include "math.h"
#include "mma845_driver.h"

#define ACC_THRESHOLD 350 
#define NO_SENSE_DELAY 51    //  kalashnikov period is 64 accel samples (1 sample = 1.65 mSec)

extern pthread_mutex_t mutex_udp;
extern pthread_mutex_t mutex_uart;
//pthread_mutex_lock(&mutex_udp);
//pthread_mutex_unlock(&mutex_udp);

extern int gun_shot_flag;
extern int shot_counter;

extern int panel_socket;
extern struct sockaddr_in panel_addr;

static int threshold;

void accel_set_threshold(int value)
{
	threshold = value;
}

void *accelerometer_thread(void *param)
{
	uint8_t message[64];

	int8_t X, Y, Z;

	static int summ0, summ1, summ2;

	struct timespec no_sense_delay;
	no_sense_delay.tv_sec = 0;
	no_sense_delay.tv_nsec = 47000000;	// 47 ms 

	accel_set_threshold(ACC_THRESHOLD);   // ak74
	
	int no_sense_counter = 0;

	while(1)
	{                                                                           	
	                                                                            	
		read_xyz(&X, &Y, &Z);                                                   	
		//int accel_summ = abs(Y);
		int accel_summ = abs(X+17) + abs(Y) + abs(Z);
		// shift
		summ2 = summ1;
		summ1 = summ0;
		summ0 = accel_summ;
		accel_summ = summ0 + summ1 + summ2;
		pthread_mutex_lock(&mutex_uart);
		printf("%+05d   %+05d   %+05d  %+05d\r\n", X, Y, Z, accel_summ);                           	
		pthread_mutex_unlock(&mutex_uart);

		if(no_sense_counter > 0)  // just registered a shot
		{
			no_sense_counter--;
		}
		else
		{
			if(accel_summ > ACC_THRESHOLD)
			{
				gun_shot_flag = 1;
				shot_counter++;
				no_sense_counter = NO_SENSE_DELAY;
				pthread_mutex_lock(&mutex_uart);
				printf("<<<SHOT>>>\r\n");  
				pthread_mutex_unlock(&mutex_uart);
				// send data via udp socket ***************************************************
				sprintf(message, "<<<SHOT>>>");
				pthread_mutex_lock(&mutex_udp);
				sendto(panel_socket, message, strlen(message), 0, (struct sockaddr *)&panel_addr, sizeof(panel_addr));
				pthread_mutex_unlock(&mutex_udp);
				//*****************************************************************************                         	
				//nanosleep(&no_sense_delay, NULL);
			}
		}
	}
}

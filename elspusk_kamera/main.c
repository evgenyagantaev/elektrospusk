#include "main.h"
#include "step_driver.h"
#include "init.h"
//#include "mma845_driver.h"


// global variables *****************************************************

int udp_pipe_thread_parameter = 0;
int accelerometer_thread_parameter = 0;

int gun_shot_flag = 0;

// end global variables *************************************************

// functions ************************************************************

void *udp_pipe_thread(void *param);
void *accelerometer_thread(void *param);

void park(void)
{

	int i;

	int step_counter = 0;
	
	struct timespec sleep_interval_short;
	sleep_interval_short.tv_sec = 0;
	sleep_interval_short.tv_nsec = 10000000;	// 
	
	printf("park started\r\n");
	
	while((step_counter < 400) && (bcm2835_gpio_lev(PARK_BUTTON) == 1))
	{
		step_clockwise();
		step_counter++;
		nanosleep(&sleep_interval_short, NULL);
	}
	for(i=0; i<37; i++)
	{
		step_counterclockwise();
		nanosleep(&sleep_interval_short, NULL);
	}
	
	printf("park finished *****\r\n");
}

void one_shot(void)
{
	int i;

	int step_counter = 0;
	
	struct timespec sleep_interval_short;
	sleep_interval_short.tv_sec = 0;
	sleep_interval_short.tv_nsec = 10000000;	// 
	
	printf("one shot started\r\n");
	gun_shot_flag = 0;
	while((step_counter < 150) && (gun_shot_flag == 0))
	{
		step_counterclockwise();
		step_counter++;
		nanosleep(&sleep_interval_short, NULL);
	}

	if(gun_shot_flag == 1)
		gun_shot_flag = 0;

	for(i=0; i<step_counter; i++)
		step_clockwise();
	
	printf("one shot finished *****\r\n");
}


int bunch_shot(int shots_number)
{
	int i;
	int real_number_of_shots = 0;
	int step_counter = 0;
	
	struct timespec sleep_interval_short;
	sleep_interval_short.tv_sec = 0;
	sleep_interval_short.tv_nsec = 5000000;	// 
	
	printf("bunch shot %d started\r\n", shots_number);
	gun_shot_flag = 0;
	while((step_counter < 150) && (gun_shot_flag == 0))
	{
		step_counterclockwise();
		step_counter++;
		nanosleep(&sleep_interval_short, NULL);
	}

	if(gun_shot_flag == 1)
	{
		gun_shot_flag = 0;
		real_number_of_shots++;
		for(i=0; i<(shots_number-1); i++)
		{
			// wait for next shot
			int counter = 0;
			while((counter < 200) && (gun_shot_flag == 0))
			{
				counter++;
				nanosleep(&sleep_interval_short, NULL);
			}
			if(gun_shot_flag == 1)
			{
				gun_shot_flag = 0;
				real_number_of_shots++;
			}
		}

	}

	for(i=0; i<step_counter; i++)
		step_clockwise();

	printf("real shots - %d\r\n", real_number_of_shots);
	printf("bunch shot finished *****\r\n");
}

// end functions ********************************************************





//*****************************
//*****************************
//*****************************
//*****************************
//*****************************

int main(int argc, char *argv)
{


	struct timespec sleep_interval;
	sleep_interval.tv_sec = 7;
	sleep_interval.tv_nsec = 0;	// 


	struct timespec sleep_interval_short;
	sleep_interval_short.tv_sec = 0;
	sleep_interval_short.tv_nsec = 500000;	// 


	pthread_t keyboard_polling_thread;
	int keyboard_thread_id;
	pthread_t udp_command_pipe_thread;
	int udp_thread_id;
	pthread_t accelerometer_registration_thread;
	int accelerometer_thread_id;


// local variables
//****************************
	int i, j, k;

// end of local variables block
//************************************

	// Inicializiruem periferiyu
	if(!bcm2835_init())
	{
		printf("\nbcm2835 initialization failed\n");
		return 1;
	}
	else // Everything OK with periphery
	{
		printf("\n");
		printf("Test of raspberry pi stepper reverse control\n");

		init();

		// start the udp pipe thread
		//pthread_create(&udp_command_pipe_thread, NULL, udp_pipe_thread, (void *)(&udp_pipe_thread_parameter));
		// start the accelerometer thread
		pthread_create(&accelerometer_registration_thread, NULL, accelerometer_thread, (void *)(&accelerometer_thread_parameter));

		//printf("getchar in");
		//getchar();
		//printf("getchar out");


		int counter = 0;
		int direction = 0;

		uint8_t buf[2];
		buf[0] = 0x14;

		while(1)
		{
		
			park();
			nanosleep(&sleep_interval, NULL);

		}


		// Deinicializiruem periferiyu
		bcm2835_gpio_write(ENB_PIN, 1);
		bcm2835_close();

	
		return 0;
	}//end if(!bcm2835_init())
}// end main

//***************************************************
//***************************************************
//***************************************************
//***************************************************
//***************************************************
//***************************************************

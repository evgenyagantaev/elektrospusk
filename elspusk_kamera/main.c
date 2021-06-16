#include "main.h"
#include "step_driver.h"
#include "init.h"
//#include "mma845_driver.h"


int udp_pipe_thread_parameter = 0;
int accelerometer_thread_parameter = 0;

// functions ************************************************************

void *udp_pipe_thread(void *param);
void *accelerometer_thread(void *param);

// end functions ********************************************************


// global variables *****************************************************

int gun_shot_flag = 0;

// end global variables *************************************************



//*****************************
//*****************************
//*****************************
//*****************************
//*****************************

int main(int argc, char *argv)
{


	struct timespec sleep_interval;
	sleep_interval.tv_sec = 1;
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
		
			//read_xyz(&X, &Y, &Z);
			//write_register(CONTROL_REG1, CONTROL_REG1_CONF);
			//bcm2835_i2c_write((const char*)(&buf[0]), 1);
			//bcm2835_i2c_read((char*)(&buf[1]), 1);
			//bcm2835_i2c_read_register_rs((char*)(&buf[0]), (char*)(&buf[1]), 1);
			//buf[1] = read_register(buf[0]);
			//printf("%x\r\n", buf[1]);
			//printf("%+05d   %+05d   %+05d\r\n", X, Y, Z);

			nanosleep(&sleep_interval, NULL);

			/*
			if(direction == 0) //clockwise
			{
				step_clockwise();
				counter++;
				if(counter >= 400)
				{
					//nanosleep(&sleep_interval, NULL);
					counter = 0;
					direction = 1;
				}
			}
			else  // counterclockwise
			{
				step_counterclockwise();
				counter++;
				if(counter >= 400)
				{
					nanosleep(&sleep_interval, NULL);
					counter = 0;
					direction = 0;
				}
			}
			//*/

			//step_counterclockwise();

			/*
			if(!bcm2835_gpio_lev(GREEN_BUTTON))
			{
				step_clockwise();
				nanosleep(&sleep_interval, NULL);
			}
			else if(!bcm2835_gpio_lev(BLUE_BUTTON))
			{
				step_counterclockwise();
				nanosleep(&sleep_interval, NULL);
			}
			else if(!bcm2835_gpio_lev(YELLOW_BUTTON))
			{
				phase_counter = 0;

				for(i=0; i<100; i++)
				{
					step_counterclockwise();
					nanosleep(&sleep_interval, NULL);
				}
				while(!bcm2835_gpio_lev(YELLOW_BUTTON))
				{
					nanosleep(&sleep_interval_short, NULL);
				}
				for(i=0; i<100; i++)
				{
					step_clockwise();
					nanosleep(&sleep_interval, NULL);
				}

			}
			else 
			{
				holding();
				nanosleep(&sleep_interval_short, NULL);
			}
			//*/

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

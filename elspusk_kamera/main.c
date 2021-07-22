#include "main.h"
#include "step_driver.h"
#include "init.h"
//#include "mma845_driver.h"



// global variables *****************************************************

int udp_pipe_thread_parameter = 0;
uint8_t global_command_buffer[128];
int new_command_received_flag = 0;

int accelerometer_thread_parameter = 0;
int command_thread_parameter = 0;

int gun_shot_flag = 0;
int shot_counter = 0;

// 1 - stechkin   2 - yarygin   3 - ak74   4 - akm   5 - svd
int type_of_weapon = 3;   // ak74 (default; power on reset)

pthread_mutex_t mutex_udp;
pthread_mutex_t mutex_uart;
//pthread_mutex_lock(&mutex_udp);
//pthread_mutex_unlock(&mutex_udp);

// end global variables *************************************************

// functions ************************************************************

void *udp_pipe_thread(void *param);
void *accelerometer_thread(void *param);
void *command_interpreter_thread(void *param);

void park(void);
void one_shot(void);

// end functions ********************************************************





//*****************************
//*****************************
//*****************************
//*****************************
//*****************************

int main(int argc, char *argv)
{

	if (pthread_mutex_init(&mutex_udp, NULL) != 0)
    {
        printf("\n mutex udp init failed\n");
        return 1;
    }
	if (pthread_mutex_init(&mutex_uart, NULL) != 0)
    {
        printf("\n mutex uart init failed\n");
        return 1;
    }

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
	pthread_t command_thread;
	int command_thread_id;


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
		pthread_create(&udp_command_pipe_thread, NULL, udp_pipe_thread, (void *)(&udp_pipe_thread_parameter));
		// start the accelerometer thread
		pthread_create(&accelerometer_registration_thread, NULL, accelerometer_thread, (void *)(&accelerometer_thread_parameter));
		// start the command interpreter thread
		pthread_create(&command_thread, NULL, command_interpreter_thread, (void *)(&command_thread_parameter));

		//printf("getchar in");
		//getchar();
		//printf("getchar out");


		int counter = 0;
		int direction = 0;

		uint8_t buf[2];
		buf[0] = 0x14;

		while(1)
		{
		
			//one_shot();
			//park();
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

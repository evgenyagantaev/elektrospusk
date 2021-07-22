#include "main.h"
#include "step_driver.h"

extern pthread_mutex_t mutex_udp;
extern pthread_mutex_t mutex_uart;
//pthread_mutex_lock(&mutex_udp);
//pthread_mutex_unlock(&mutex_udp);

extern int type_of_weapon;

extern uint8_t global_command_buffer[];
extern int new_command_received_flag;
extern int gun_shot_flag;
extern int shot_counter;
extern int panel_socket;
extern struct sockaddr_in panel_addr;

int max_turn = 150;

//****************************************************

void accel_set_threshold(int value);


//****************************************************

void turn(int dir, int steps)
{

	int i;

	struct timespec sleep_interval_short;
	sleep_interval_short.tv_sec = 0;
	sleep_interval_short.tv_nsec = 3000000;	// 
	
	pthread_mutex_lock(&mutex_uart);
	printf("turn started\r\n");
	pthread_mutex_unlock(&mutex_uart);
	
	for(i=0; i<steps; i++)
	{
		if(dir)
			step_counterclockwise();
		else
			step_clockwise();
		nanosleep(&sleep_interval_short, NULL);
	}
	
	pthread_mutex_lock(&mutex_uart);
	printf("turn finished *****\r\n");
	pthread_mutex_unlock(&mutex_uart);
}


void park(void)
{

	int i;

	int step_counter = 0;
	
	struct timespec sleep_interval_short;
	sleep_interval_short.tv_sec = 0;
	sleep_interval_short.tv_nsec = 10000000;	// 
	
	pthread_mutex_lock(&mutex_uart);
	printf("park started\r\n");
	pthread_mutex_unlock(&mutex_uart);
	
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
	
	pthread_mutex_lock(&mutex_uart);
	printf("park finished *****\r\n");
	pthread_mutex_unlock(&mutex_uart);
}



void fast_park(void)
{

	int i;

	int step_counter = 0;
	
	struct timespec sleep_interval_short;
	sleep_interval_short.tv_sec = 0;
	sleep_interval_short.tv_nsec = 300000;	// 
	
	pthread_mutex_lock(&mutex_uart);
	printf("fast park started\r\n");
	pthread_mutex_unlock(&mutex_uart);
	
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
	
	pthread_mutex_lock(&mutex_uart);
	printf("fast park finished *****\r\n");
	pthread_mutex_unlock(&mutex_uart);
}


void one_shot(void)
{
	int i;

	int step_counter = 0;
	
	struct timespec sleep_interval_short;
	sleep_interval_short.tv_sec = 0;
	sleep_interval_short.tv_nsec = 3000000;	// 
	
	pthread_mutex_lock(&mutex_uart);
	printf("one shot started\r\n");
	pthread_mutex_unlock(&mutex_uart);
	gun_shot_flag = 0;
	while((step_counter < max_turn) && (gun_shot_flag == 0))
	{
		step_counterclockwise();
		step_counter++;
		nanosleep(&sleep_interval_short, NULL);
	}

	if(gun_shot_flag == 1)
		gun_shot_flag = 0;

	while((step_counter >= 0) && (bcm2835_gpio_lev(PARK_BUTTON) == 1))
	{
		step_clockwise();
		step_counter--;
	}
	if(step_counter >= 0)  // park button pressed
	{
		for(i=0; i<37; i++)   // relax turn
		{
			step_counterclockwise();
			nanosleep(&sleep_interval_short, NULL);
		}
	}
	
	pthread_mutex_lock(&mutex_uart);
	printf("one shot finished *****\r\n");
	pthread_mutex_unlock(&mutex_uart);
}


int bunch_shot(int shots_number)
{
	int i;
	int real_number_of_shots = 0;
	int step_counter = 0;
	
	struct timespec sleep_interval_short;
	sleep_interval_short.tv_sec = 0;
	sleep_interval_short.tv_nsec = 3000000;	// 
	
	pthread_mutex_lock(&mutex_uart);
	printf("bunch shot %d started\r\n", shots_number);
	pthread_mutex_unlock(&mutex_uart);
	gun_shot_flag = 0;
	while((step_counter < max_turn) && (gun_shot_flag == 0))
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

	while((step_counter >= 0) && (bcm2835_gpio_lev(PARK_BUTTON) == 1))
	{
		step_clockwise();
		step_counter--;
	}
	if(step_counter >= 0)  // park button pressed
	{
		for(i=0; i<37; i++)   // relax turn
		{
			step_counterclockwise();
			nanosleep(&sleep_interval_short, NULL);
		}
	}

	pthread_mutex_lock(&mutex_uart);
	printf("real shots - %d\r\n", real_number_of_shots);
	printf("bunch shot finished *****\r\n");
	pthread_mutex_unlock(&mutex_uart);
}


int random_bunch_shot_start()
{
	int i;
	int step_counter = 0;
	
	struct timespec sleep_interval_short;
	sleep_interval_short.tv_sec = 0;
	sleep_interval_short.tv_nsec = 5000000;	// 
	
	pthread_mutex_lock(&mutex_uart);
	printf("random bunch shot started\r\n");
	pthread_mutex_unlock(&mutex_uart);
	gun_shot_flag = 0;
	while((step_counter < max_turn) && (gun_shot_flag == 0))
	{
		step_counterclockwise();
		step_counter++;
		nanosleep(&sleep_interval_short, NULL);
	}

	if(gun_shot_flag == 1)
	{
		gun_shot_flag = 0;
	}

}




void *command_interpreter_thread(void *param)
{

	int i;

	uint8_t message[64];
	
	struct timespec sleep_interval;
	sleep_interval.tv_sec = 0;
	sleep_interval.tv_nsec = 0;	//

	int mode;
	int number;
	int length;
	int interval;

	int dojob = 1;

	while(dojob)
	{
		if(new_command_received_flag)
		{
			// reset flag
			new_command_received_flag = 0;

			//    
			if((strlen(global_command_buffer) == 13) && (global_command_buffer[0] == 'c') && (global_command_buffer[12] == 'G'))
			{

				//printf(">>>%s\r\n", global_command_buffer);

				int sscanf_result = sscanf(global_command_buffer, "c%1d%03d%02d%05dG", &mode, &number, &length, &interval);
				if(sscanf_result > 0)
				{
					pthread_mutex_lock(&mutex_uart);
					printf("mode=%1d number=%03d length=%02d interval=%05d\r\n", mode, number, length, interval);
					pthread_mutex_unlock(&mutex_uart);

					sleep_interval.tv_sec = interval/1000; // interval in sec

					if(mode == 0)
					{
						// this is check connect and set weapon message
						if(length == 1)  // stechkin
						{
							accel_set_threshold(165);
							type_of_weapon = 1;
						}
						else if(length == 2)  // yarygin
						{
							accel_set_threshold(195);
							type_of_weapon = 2;
						}
						else if(length == 3)  // ak74
						{
							accel_set_threshold(350);
							type_of_weapon = 3;
						}
						else if(length == 4)  // akm
						{
							accel_set_threshold(475);
							type_of_weapon = 4;
						}
						else if(length == 5)  // svd
						{
							accel_set_threshold(650);
							type_of_weapon = 5;
						}

						// send data via udp socket ***************************************************
						sprintf(message, "c%1d%03d%02d%05dG", 0, 0, 0, type_of_weapon);
						pthread_mutex_lock(&mutex_udp);
						sendto(panel_socket, message, strlen(global_command_buffer), 0, (struct sockaddr *)&panel_addr, sizeof(panel_addr));
						pthread_mutex_unlock(&mutex_udp);
						//*****************************************************************************

					}
					else if(mode == 1)
					{
						for(i=0; i<number; i++)
						{
							one_shot();
							fast_park();
							nanosleep(&sleep_interval, NULL);
						}
						park();
						// send data via udp socket ***************************************************
						sprintf(message, "c%1d%03d%02d%05dG", mode, number, length, shot_counter);
						shot_counter = 0;
						pthread_mutex_lock(&mutex_udp);
						sendto(panel_socket, message, strlen(global_command_buffer), 0, (struct sockaddr *)&panel_addr, sizeof(panel_addr));
						pthread_mutex_unlock(&mutex_udp);
						//*****************************************************************************
					}
					else if(mode == 2)
					{
						for(i=0; i<number; i++)
						{
							bunch_shot(length);
							fast_park();
							nanosleep(&sleep_interval, NULL);
						}
						park();
						// send data via udp socket ***************************************************
						sprintf(message, "c%1d%03d%02d%05dG", mode, number, length, shot_counter);
						shot_counter = 0;
						pthread_mutex_lock(&mutex_udp);
						sendto(panel_socket, message, strlen(global_command_buffer), 0, (struct sockaddr *)&panel_addr, sizeof(panel_addr));
						pthread_mutex_unlock(&mutex_udp);
						//*****************************************************************************
					}
					else if(mode == 3 && interval == 1)
					{
						
						random_bunch_shot_start();
					}
					else if(mode == 3 && interval == 0)
					{
						
						fast_park();
						
						// send data via udp socket ***************************************************
						sprintf(message, "c%1d%03d%02d%05dG", mode, number, length, shot_counter);
						shot_counter = 0;
						pthread_mutex_lock(&mutex_udp);
						sendto(panel_socket, message, strlen(global_command_buffer), 0, (struct sockaddr *)&panel_addr, sizeof(panel_addr));
						pthread_mutex_unlock(&mutex_udp);
						//*****************************************************************************
					}
					else if(mode == 4)
					{
						park();
					}
					else if(mode == 5)
					{
						turn(interval, number);
					}
					else if(mode == 6)
					{
						if(interval == 1)
						{
							turn(1, number);
							sleep_interval.tv_sec = 3;
							nanosleep(&sleep_interval, NULL);
							turn(0, number);
						}
						else
						{
							max_turn = number;
							pthread_mutex_lock(&mutex_uart);
							printf("max turn - %d remembered\r\n", max_turn);
							pthread_mutex_unlock(&mutex_uart);
						}
					}
				}
			}
		}// end if(new_command_received_flag)

	}// end while(dojob)
}



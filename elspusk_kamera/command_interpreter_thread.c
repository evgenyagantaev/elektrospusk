#include "main.h"
#include "step_driver.h"


extern uint8_t global_command_buffer[];
extern int new_command_received_flag;
extern int gun_shot_flag;
extern int shot_counter;
extern int panel_socket;
extern struct sockaddr_in panel_addr;

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

void *command_interpreter_thread(void *param)
{

	int i;

	struct timespec sleep_interval;
	sleep_interval.tv_sec = 0;
	//sleep_interval.tv_nsec = 0;	//

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
				int sscanf_result = sscanf(global_command_buffer, "c%1d%03d%02d%05dG", &mode, &number, &length, &interval);

				if(sscanf_result > 0)
				{
					sleep_interval.tv_nsec = interval * 1000000; // interval in msec

					if(mode == 0)
					{

					}
					else if(mode == 1)
					{
						for(i=0; i<number; i++)
						{
							one_shot();
							nanosleep(&sleep_interval, NULL);
						}
						park();
						// send data via udp socket ***************************************************
						sprintf(global_command_buffer, "c%1d%03d%02d%05dG", mode, number, length, shot_counter);
						shot_counter = 0;
						sendto(panel_socket, global_command_buffer, strlen(global_command_buffer), 0, (struct sockaddr *)&panel_addr, sizeof(panel_addr));
						//*****************************************************************************
					}
					else if(mode == 2)
					{
						for(i=0; i<number; i++)
						{
							bunch_shot(length);
							nanosleep(&sleep_interval, NULL);
						}
						park();
						// send data via udp socket ***************************************************
						sprintf(global_command_buffer, "c%1d%03d%02d%05dG", mode, number, length, shot_counter);
						shot_counter = 0;
						sendto(panel_socket, global_command_buffer, strlen(global_command_buffer), 0, (struct sockaddr *)&panel_addr, sizeof(panel_addr));
						//*****************************************************************************
					}
				}
			}
		}// end if(new_command_received_flag)

	}// end while(dojob)
}



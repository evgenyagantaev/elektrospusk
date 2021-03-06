#include "main.h"

#define BUFFER_LENGTH 1024
#define PORT 61312

extern pthread_mutex_t mutex_udp;
extern pthread_mutex_t mutex_uart;
//pthread_mutex_lock(&mutex_udp);
//pthread_mutex_unlock(&mutex_udp);

extern uint8_t global_command_buffer[];
extern int new_command_received_flag;

int panel_socket;
struct sockaddr_in panel_addr;

void *udp_pipe_thread(void *param)
{
	int doJob = 1;
	int i,j,k;
	long counter_long;

	struct timespec sleep_interval;

	sleep_interval.tv_sec = 1;
	//sleep_interval.tv_nsec = 15000000;	// 15 miliseconds
	sleep_interval.tv_nsec = 0;	// 

	uint8_t buffer[BUFFER_LENGTH] = "c10010000000G";
	//buffer[0] = 'i'; buffer[1] = 'n'; buffer[2] = 'i';  buffer[3] = 't';  buffer[4] = 0; 
	int datagramm_length;
	int address_length;

	//int panel_socket;
	int so_broadcast = 1;
	//struct sockaddr_in panel_addr;
	struct sockaddr_in server_addr;

	i = 1;
	panel_socket = socket(AF_INET, SOCK_DGRAM, 0);
	if(panel_socket < 0)
	{
		pthread_mutex_lock(&mutex_uart);
		printf("\r\npanel_socket failure ******************\r\n");
		pthread_mutex_unlock(&mutex_uart);
		*((int *)param) = 1; 
		return (void *)param;
	}

	//setsockopt(panel_socket, SOL_SOCKET, SO_BROADCAST, &so_broadcast, sizeof so_broadcast);

	panel_addr.sin_family = AF_INET;
	panel_addr.sin_port = htons(PORT);
	//panel_addr.sin_port = PORT;
	panel_addr.sin_addr.s_addr = inet_addr("192.168.2.127");
	//*****
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(PORT);
	//server_addr.sin_addr.s_addr = inet_addr("192.168.1.121");
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	pthread_mutex_lock(&mutex_uart);
	printf("\r\npanel address %s port: %d\r\n", inet_ntoa(panel_addr.sin_addr), PORT);
	pthread_mutex_unlock(&mutex_uart);

	bind(panel_socket, (struct sockaddr *)&server_addr, sizeof(panel_addr));
	address_length = sizeof(panel_addr);

	i = 0;
	counter_long = 0;

	datagramm_length = strlen(buffer);
	
	while(doJob)
	{

		// receive data from udp socket ***************************************************
		datagramm_length = recvfrom(panel_socket, buffer, 1024, 0, (struct sockaddr *)&panel_addr, &address_length);
		pthread_mutex_lock(&mutex_udp);
		sendto(panel_socket, buffer, datagramm_length, 0, (struct sockaddr *)&panel_addr, sizeof(panel_addr));
		pthread_mutex_unlock(&mutex_udp);
		//printf("received %d ", datagramm_length);
		//printf("from address %s ", inet_ntoa(panel_addr.sin_addr));
		//printf("from port %d\r\n", panel_addr.sin_port);
		if(datagramm_length < BUFFER_LENGTH)
		{
			buffer[datagramm_length] = 0;
			// copy datagramm into global buffer
			strncpy(global_command_buffer, buffer, 64);
			new_command_received_flag = 1;
		}
		else
		{
			buffer[BUFFER_LENGTH - 1] = 0;
		}
		pthread_mutex_lock(&mutex_uart);
		printf("%s\r\n", global_command_buffer);
		pthread_mutex_unlock(&mutex_uart);

		//*****************************************************************************



		// send data via udp socket ***************************************************
		//sendto(panel_socket, buffer, datagramm_length, 0, (struct sockaddr *)&panel_addr, sizeof(panel_addr));
		//*****************************************************************************

		//printf("udp thread ********\r\n ");
		//nanosleep(&sleep_interval, NULL);
		
	}
}

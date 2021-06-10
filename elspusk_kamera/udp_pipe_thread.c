#include "main.h"

#define BUFFER_LENGTH 1024

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

	int panel_socket;
	int so_broadcast = 1;
	struct sockaddr_in panel_addr;

	i = 1;
	panel_socket = socket(AF_INET, SOCK_DGRAM, 0);
	if(panel_socket < 0)
	{
		printf("\r\npanel_socket failure ******************\r\n");
		*((int *)param) = 1; 
		return (void *)param;
	}

	//setsockopt(panel_socket, SOL_SOCKET, SO_BROADCAST, &so_broadcast, sizeof so_broadcast);

	panel_addr.sin_family = AF_INET;
	//panel_addr.sin_port = htons(61312);
	panel_addr.sin_port = 61312;
	panel_addr.sin_addr.s_addr = inet_addr("192.168.2.127");

	printf("\r\npanel address %s\r\n", inet_ntoa(panel_addr.sin_addr));

	bind(panel_socket, (struct sockaddr *)&panel_addr, sizeof(panel_addr));
	address_length = sizeof(panel_addr);

	i = 0;
	counter_long = 0;

	datagramm_length = strlen(buffer);
	
	while(doJob)
	{

		// receive data from udp socket ***************************************************
		//datagramm_length = recvfrom(panel_socket, buffer, 1024, 0, (struct sockaddr *)&panel_addr, &address_length);
		//sendto(panel_socket, buffer, datagramm_length, 0, (struct sockaddr *)&panel_addr, sizeof(panel_addr));
		printf("received %d ", datagramm_length);
		printf("from address %s ", inet_ntoa(panel_addr.sin_addr));
		printf("from port %d\r\n", panel_addr.sin_port);
		if(datagramm_length < BUFFER_LENGTH)
		{
			buffer[datagramm_length] = 0;
		}
		else
		{
			buffer[BUFFER_LENGTH - 1] = 0;
		}
		printf("%s\r\n", buffer);

		//*****************************************************************************



		// send data via udp socket ***************************************************
		sendto(panel_socket, buffer, datagramm_length, 0, (struct sockaddr *)&panel_addr, sizeof(panel_addr));
		//*****************************************************************************

		printf("udp thread\r\n ");
		nanosleep(&sleep_interval, NULL);
		
	}
}

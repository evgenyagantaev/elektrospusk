void *metab_udp_thread(void *param)
{
	int doJob = 1;
	int i,j,k;

	uint8_t buffer[1024];
	char start_string[] = {'L', 'I', 'N', 'K', 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
	int datagramm_length;
	int address_length;

	struct timespec sleep_interval;

	sleep_interval.tv_sec = 0;
	sleep_interval.tv_nsec = 15000000;	// 15 miliseconds


	int unit_socket, base_socket;
	int so_broadcast = 1;

	struct sockaddr_in unit_addr;
	struct sockaddr_in server_addr;
	struct sockaddr_in base_addr;

	//debug
	printf("metabolism thread started\r\n");


	i = 1;
	unit_socket = socket(AF_INET, SOCK_DGRAM, 0);
	if(unit_socket < 0)
	{
		printf("\r\nunit_socket failure ******************\r\n");
		*((int *)param) = 1; 
		return (void *)param;
	}

	setsockopt(unit_socket, SOL_SOCKET, SO_BROADCAST, &so_broadcast, sizeof so_broadcast);

	base_socket = socket(AF_INET, SOCK_DGRAM, 0);
	if(base_socket < 0)
	{
		printf("\r\nbase_socket failure ******************\r\n");
		*((int *)param) = 1; 
		return (void *)param;
	}

	
	base_addr.sin_family = AF_INET;
	base_addr.sin_port = htons(115);
	base_addr.sin_addr.s_addr = inet_addr("192.168.173.2");

	printf("\r\nbase address %s\r\n", inet_ntoa(base_addr.sin_addr.s_addr));

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(63001);
	//server_addr.sin_addr.s_addr = inet_addr("192.168.1.121");
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	printf("\r\nserver address %s\r\n", inet_ntoa(server_addr.sin_addr.s_addr));
	
	
	/*
	while(1)
	{
		unit_addr.sin_addr.s_addr = inet_addr("255.255.255.255");
		//unit_addr.sin_addr.s_addr = inet_addr("192.168.1.255");
		//unit_addr.sin_addr.s_addr = inet_addr("192.168.1.1");
		unit_addr.sin_port = htons(63002);
		sendto(unit_socket, buffer, 14, 0, (struct sockaddr *)&unit_addr, sizeof(unit_addr));
		printf("sent 14\r\n");

		sleep(1);
	}
	//*/

	//debug
	//server_addr.sin_port = htons(63002);
	//debug


	bind(unit_socket, (struct sockaddr *)&server_addr, sizeof(server_addr));
	address_length = sizeof(unit_addr);

	//debug**************************************
	/*
	// send broadcast datagramm
	unit_addr.sin_family = AF_INET;
	unit_addr.sin_port = htons(63001);
	unit_addr.sin_addr.s_addr = htonl(INADDR_BROADCAST);
	sendto(unit_socket, start_string, 10, 0, (struct sockaddr *)&unit_addr, sizeof(unit_addr));
	printf("sent \r\n", datagramm_length + 4);
	

	while(1)
	{
		datagramm_length = recvfrom(unit_socket, buffer, 1024, 0, (struct sockaddr *)&unit_addr, &address_length);
		printf("received %d ", datagramm_length);
		printf("from address %s ", inet_ntoa(unit_addr.sin_addr.s_addr));
		printf("from port %d\r\n", unit_addr.sin_port);
	}
	//*/
	//debug**************************************

	bind(unit_socket, (struct sockaddr *)&server_addr, sizeof(server_addr));
	// wait for receiving start datagramm from device
	datagramm_length = 11;
	datagramm_length = recvfrom(unit_socket, buffer, 1024, 0, (struct sockaddr *)&unit_addr, &address_length);
	printf("received %d ", datagramm_length);
	printf("from address %s ", inet_ntoa(unit_addr.sin_addr.s_addr));
	printf("from port %d\r\n", unit_addr.sin_port);
	for(i=0; i<14; i++)
		printf("%x", buffer[i]);
	printf("\r\n");

	// write an ip address of device in datagramm buffer
	buffer[datagramm_length] = 0xc0; //192
	buffer[datagramm_length + 1] = 0xa8; //168
	buffer[datagramm_length + 2] = 0x00; //0
	buffer[datagramm_length + 3] = 0x7a; //123
	for(i=0; i<14; i++)
		printf("%x", buffer[i]);
	printf("\r\n");

	// send broadcast datagramm
	unit_addr.sin_family = AF_INET;
	unit_addr.sin_port = htons(63002);
	unit_addr.sin_addr.s_addr = htonl(INADDR_BROADCAST);
	sendto(unit_socket, buffer, datagramm_length + 4, 0, (struct sockaddr *)&unit_addr, sizeof(unit_addr));
	printf("sent \r\n", datagramm_length + 4);
	
	// write a device command in datagramm buffer
	buffer[0] = 0x5a;
	buffer[1] = 0x30;

	// send datagramm with command
	unit_addr.sin_port = htons(63002);
	unit_addr.sin_addr.s_addr = htonl(INADDR_BROADCAST);
	sendto(unit_socket, buffer, 2, 0, (struct sockaddr *)&unit_addr, sizeof(unit_addr));
	printf("sent 2\r\n");
	
	while(1)
	{
		datagramm_length = recvfrom(unit_socket, buffer, 1024, 0, (struct sockaddr *)&unit_addr, &address_length);
		sendto(base_socket, buffer, datagramm_length, 0, (struct sockaddr *)&base_addr, sizeof(base_addr));
		printf("received %d ", datagramm_length);
		printf("from address %s ", inet_ntoa(unit_addr.sin_addr.s_addr));
		printf("from port %d\r\n", unit_addr.sin_port);

		//for(i=0; i<datagramm_length; i++)
			//printf("%x ", buffer[i]);
		//printf("\r\n");

	}

	/*
	i = 0;
	while(doJob)
	{
		//printf("%d\r\n", i);
		i++;
		if(i == 256)
			i = 0;

		// send data via udp socket ***************************************************
		sendto(sock, (char *)pointer, 9* number_of_sensors*2, 0, (struct sockaddr *)&addr, sizeof(addr));
		//*****************************************************************************

		nanosleep(&sleep_interval, NULL);
	}
	//*/
}

/* Compute Networks Project,2019
   Group : 13
   Team: Aditya Shah, Jeel Pujara, Dhruvil Shah, Himanshu Bhadania, Varun Patel 
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <time.h>
#include<pthread.h>

#define MC_PORT 5432
#define BUFFER_SIZE 1024
#define MAX_PENDING 10

int send_file(int socket)
{    
     FILE *fp;  
     int size, readpacket_size, stat, readpacket_index;
     char send_buffer[BUFFER_SIZE], read_buffer[BUFFER_SIZE];

     fp = fopen("menu.txt", "r");

     if(!fp)
     {
          printf("File not opened");
     }    

     fseek(fp, 0, SEEK_END);
	size = ftell(fp);
     fseek(fp, 0, SEEK_SET);
     write(socket, (void *)&size, sizeof(int));

     while(!feof(fp))
     {
          readpacket_size = fread(send_buffer, 1, sizeof(send_buffer)-1, fp);

          do{
               stat = write(socket, send_buffer, readpacket_size);  
          }while (stat < 0);

          readpacket_index++;  

   
          bzero(send_buffer, sizeof(send_buffer));
     }
     fclose(fp);
}

void delay(int milliseconds)
{
	long pause;
	clock_t now,then;

	pause = milliseconds*(CLOCKS_PER_SEC/1000);
	now = then = clock();
	while( (now-then) < pause )
	   now = clock();
}

//Thread function for 1st station 
void *myThreadFun1(void *vargp)
{ 
	int s1; 
	struct sockaddr_in sin; 
	char buf[BUFFER_SIZE];
	int len;
	char *mcast_addr; 

	mcast_addr = "239.192.13.1";

	if ((s1 = socket(PF_INET, SOCK_DGRAM, 0)) < 0) {
		perror("\nServer Error");
		exit(1);
	}

	memset((char *)&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = inet_addr(mcast_addr);
	sin.sin_port = htons(MC_PORT);

	printf("\nWrite messages below to multicast!\n\n");

	FILE *fp;
	int size, readpacket_size, stat=0, readpacket_index;
	char send_buffer[BUFFER_SIZE], read_buffer[BUFFER_SIZE];
	readpacket_index = 1;

	fp = fopen("1.mp4", "r");
	printf("Getting Picture Size\n");   

	if(fp == NULL) {
		printf("Error Opening Image File"); } 

	fseek(fp, 0, SEEK_END);
	size = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	printf("Total Picture size: %i\n",size);

	printf("Sending Picture Size\n");
	sendto(s1, (void *)&size, sizeof(int), 0,(struct sockaddr *)&sin, sizeof(sin)); 

	printf("Sending Picture as Byte Array\n");

	int n=0, count=0;
	while(!feof(fp))
	{
		n = fread(send_buffer, 1, BUFFER_SIZE, fp);
		count += n;
		printf("Packet Size: = %d\n", n);

		if (sendto(s1, send_buffer, sizeof(send_buffer), 0,(struct sockaddr *)&sin, sizeof(sin)) == -1) {
			perror("\nSender Error");
			exit(1);
		}
		printf("Packet Number: %i\n",readpacket_index);
		readpacket_index++;
		delay(10);
	}
	printf("%d bytes sent. \n", count);
	while (1);

	close(s1);  
	return 0;
}

//Thread function for station 2 
void *myThreadFun2(void *vargp)
{ 
	int s2; 
	struct sockaddr_in sin; 
	char buf[BUFFER_SIZE];
	int len;

	char *mcast_addr; 

	mcast_addr = "239.192.13.2";

	if ((s2 = socket(PF_INET, SOCK_DGRAM, 0)) < 0) {
		perror("\nServer Error");
		exit(1);
	}

	memset((char *)&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = inet_addr(mcast_addr);
	sin.sin_port = htons(MC_PORT);

	printf("\nWrite messages below to multicast!\n\n");

	FILE *fp;
	int size, readpacket_size, stat=0, readpacket_index;
	char send_buffer[BUFFER_SIZE], read_buffer[BUFFER_SIZE];
	readpacket_index = 1;


	fp = fopen("2.mp4", "r");
	printf("Getting Picture Size\n");   

	if(fp == NULL) {
		printf("Error Opening Image File"); } 

	fseek(fp, 0, SEEK_END);
	size = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	printf("Total Picture size: %i\n",size);

	printf("Sending Picture Size\n");
	sendto(s2, (void *)&size, sizeof(int), 0,(struct sockaddr *)&sin, sizeof(sin)); 

	printf("Sending Picture as Byte Array\n");

	int n=0, count=0;
	while(!feof(fp))
	{
		n = fread(send_buffer, 1, BUFFER_SIZE, fp);
		count += n;
		printf("Packet Size: = %d\n", n);


		if (sendto(s2, send_buffer, sizeof(send_buffer), 0,(struct sockaddr *)&sin, sizeof(sin)) == -1) {
		perror("\nSender Error");
		exit(1);
		}
		printf("Packet Number: %i\n",readpacket_index);
		readpacket_index++;
		delay(10);
	}
	printf("%d bytes sent. \n", count);
	while (1);

	close(s2);  
	return 0;
}

//Thread function for station 3
void *myThreadFun3(void *vargp)
{
	int s3; 
	struct sockaddr_in sin; 
	char buf[BUFFER_SIZE];
	int len;

	char *mcast_addr; 

	mcast_addr = "239.192.13.3";

	if ((s3 = socket(PF_INET, SOCK_DGRAM, 0)) < 0) {
		perror("server: socket");
		exit(1);
	}

	memset((char *)&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = inet_addr(mcast_addr);
	sin.sin_port = htons(MC_PORT);

	FILE *fp;
	int size, readpacket_size, stat=0, readpacket_index;
	char send_buffer[BUFFER_SIZE], read_buffer[BUFFER_SIZE];
	readpacket_index = 1;


	fp = fopen("3.mp4", "r");
	printf("Getting Picture Size\n");   

	if(fp == NULL) {
		printf("Error Opening Image File"); } 

	fseek(fp, 0, SEEK_END);
	size = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	printf("Total Picture size: %i\n",size);

	printf("Sending Picture Size\n");
	sendto(s3, (void *)&size, sizeof(int), 0,(struct sockaddr *)&sin, sizeof(sin)); 

	printf("Sending Picture as Byte Array\n");

	int n=0, count=0;
	while(!feof(fp))
	{
		n = fread(send_buffer, 1, BUFFER_SIZE, fp);
		count += n;
		printf("Packet Size: = %d\n", n);


		if (sendto(s3, send_buffer, sizeof(send_buffer), 0,(struct sockaddr *)&sin, sizeof(sin)) == -1) {
			perror("\nSender Error");
			exit(1);
		}
		printf("Packet Number: %i\n",readpacket_index);
		readpacket_index++;
		delay(10);
	}
	printf("%d bytes sent. \n", count);
	while (1);

	close(s3);  
	return 0;
}

//Thread function for station 4
void *myThreadFun4(void *vargp)
{
	int s4; 
	struct sockaddr_in sin; 
	char buf[BUFFER_SIZE];
	int len;
	char *mcast_addr; 

	mcast_addr = "239.192.13.4";

	if ((s4 = socket(PF_INET, SOCK_DGRAM, 0)) < 0) {
		perror("\nServer Error");
		exit(1);
	}

	memset((char *)&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = inet_addr(mcast_addr);
	sin.sin_port = htons(MC_PORT);

	FILE *fp;
	int size, readpacket_size, stat=0, readpacket_index;
	char send_buffer[BUFFER_SIZE], read_buffer[BUFFER_SIZE];
	readpacket_index = 1;


	fp = fopen("4.mp4", "r");
	printf("Getting Picture Size\n");   

	if(fp == NULL) {
		printf("Error Opening Image File"); } 

	fseek(fp, 0, SEEK_END);
	size = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	printf("Total Picture size: %i\n",size);

	printf("Sending Picture Size\n");
	sendto(s4, (void *)&size, sizeof(int), 0,(struct sockaddr *)&sin, sizeof(sin)); 

	printf("Sending Picture as Byte Array\n");

	int n=0, count=0;
	while(!feof(fp))
	{
		n = fread(send_buffer, 1, BUFFER_SIZE, fp);
		count += n;
		printf("Packet Size: = %d\n", n);


		if (sendto(s4, send_buffer, sizeof(send_buffer), 0,(struct sockaddr *)&sin, sizeof(sin)) == -1) {
			perror("\nSender Error");
			exit(1);
		}
		printf("Packet Number: %i\n",readpacket_index);
		readpacket_index++;
		delay(10);
	}
	printf("%d bytes sent. \n", count);
	while (1);

	close(s4);  
	return 0;
}

void *tcp_thread(void *vargp)
{
	struct sockaddr_in sin;
	char buf[BUFFER_SIZE];
	int len=1;
	int s, new_s;
	char str[INET_ADDRSTRLEN];

	bzero((char *)&sin, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = inet_addr("0.0.0.0");
	sin.sin_port = htons(MC_PORT);
	printf("1");

	if ((s = socket(PF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("\nSocket Error");
		exit(1);
	}

	inet_ntop(AF_INET, &(sin.sin_addr), str, INET_ADDRSTRLEN);

	if ((bind(s, (struct sockaddr *)&sin, sizeof(sin))) < 0)
	{
		perror("\nBind Error");
		exit(1);
	}

	listen(s, MAX_PENDING);

	while (1)
	{
		while((new_s = accept(s, (struct sockaddr *)&sin, &len)) < 0)
		{
			perror("\nAccept Error");
			exit(1);
		}
		send_file(new_s);
	}
	close(new_s);
}

int main(int argc, char *argv[])
{
	pthread_t tcp, thread_id, thread_id1, thread_id2, thread_id3;
	
	/*pthread_create() function starts a new thread for tcp.*/
	pthread_create(&tcp, NULL, tcp_thread,  NULL); 
	delay(1);
	/*pthread_create() function starts a new thread for station1.*/
	pthread_create(&thread_id, NULL, myThreadFun1,  NULL); 
	delay(1);
	/*pthread_create() function starts a new thread for station2.*/
	pthread_create(&thread_id1, NULL, myThreadFun2,  NULL); 
	delay(1);
	/*pthread_create() function starts a new thread for station3.*/
	pthread_create(&thread_id2, NULL, myThreadFun3,  NULL); 
	delay(1);
	/*pthread_create() function starts a new thread for station4.*/
	pthread_create(&thread_id3, NULL, myThreadFun4,  NULL); 
	delay(1);
	
	/*pthread_join() function waits for the thread specified by thread
       to terminate.  If that thread has already terminated, then
       pthread_join() returns immediately*/
	pthread_join(tcp,NULL);
	pthread_join(thread_id,NULL);
	pthread_join(thread_id1,NULL);
	pthread_join(thread_id2,NULL);
	pthread_join(thread_id3,NULL);

	while(1);
}


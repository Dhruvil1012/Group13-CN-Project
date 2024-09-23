/* Compute Networks Project,2019
   Group : 13
   Team: Aditya Shah, Jeel Pujara, Dhruvil Shah, Himanshu Bhadania, Varun Patel 
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include <net/if.h>
#include <gtk/gtk.h>

#define SERVER_PORT 5432
#define MAX_LINE 1024
#define BUF_SIZE MAX_LINE
#define IF_NAME "enp2s0"
#define MC_PORT SERVER_PORT

char *ip;
char *first = "239.192.13.1";
char *sec = "239.192.13.2";
char *thr = "239.192.13.3";	
char *four = "239.192.13.4";	
	
int should_terminate =0;
int is_running=0;
int current_playing=0;

void close_vlc()
{
	system("bash vlc_close.sh");
	current_playing=0;
}

int play_vlc1()
{
	system("kill -CONT $(pgrep vlc)");
	return 0;
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

void *vlcfun(void *vargp)
{
	char com[100];
	sprintf(com, "cat output.mp4 | vlc-wrapper -");
	system(com);
	printf("Successfully recieved!!");
}

void *udp(void *arg)
{
	play_vlc1();
	close_vlc();
	close_vlc();
	int s, len;
	struct sockaddr_in sin;
	struct ifreq ifr;
	struct ip_mreq mcast_req;
	struct sockaddr_in mcast_saddr;
	socklen_t mcast_saddr_len;
	char *if_name;
	char buf[BUF_SIZE];
	char *mcast_addr;
	
	/* Add code to take port number from user */
	mcast_addr = ip;
	if_name = IF_NAME;
	
	/* create socket */
	if((s = socket(PF_INET, SOCK_DGRAM, 0)) < 0)
	{
		perror("\nSocket-UDP Error");
		exit(1);
	}
	perror("\nSocket c");
	/* build address data structure */
	memset((char *)&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = htonl(INADDR_ANY);
	sin.sin_port = htons(MC_PORT);
	
	/*Use the interface specified */ 
	memset(&ifr, 0, sizeof(ifr));
	strncpy(ifr.ifr_name, if_name, sizeof(if_name)-1);

	int temp=1;
	if (setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &temp, sizeof(int)) == -1)
	{
		perror("\nSetsockopt Error");
		exit(1);
	}
	if ((setsockopt(s, SOL_SOCKET, SO_BINDTODEVICE, (void *)&ifr, sizeof(ifr))) < 0)
	{
		perror("\nReceiver: setsockopt() error");
		close(s);
		exit(1);
	}

	/* bind the socket */
	if ((bind(s, (struct sockaddr *) &sin, sizeof(sin))) < 0)
	{
		perror("\nBind-UDP Error");
		exit(1);
	}

	/* Multicast specific code follows */
	/* build IGMP join message structure */
	mcast_req.imr_multiaddr.s_addr = inet_addr(mcast_addr);
	mcast_req.imr_interface.s_addr = htonl(INADDR_ANY);
	
	/* send multicast join message */
	if ((setsockopt(s, IPPROTO_IP, IP_ADD_MEMBERSHIP, (void*) &mcast_req, sizeof(mcast_req))) < 0)
	{
		perror("\nJoin-UDP Erro");
		exit(1);
	}
	
	/* receive multicast messages */  
	printf("\nReady to listen!\n\n");

	FILE *fp = fopen("output.mp4","w"); 
	fclose(fp);
	while(1)
	{
		FILE *fp = fopen("output.mp4","a+b"); 
		//system("clear");
		int  flag = 1;
		int packet = 0;
		while(flag == 1)
		{
			if(should_terminate==1)
			{
				fclose(fp);
				close(s);
				should_terminate =0;
				return 0;
				
			}
			/* reset sender struct */
			memset(&mcast_saddr, 0, sizeof(mcast_saddr));
			mcast_saddr_len = sizeof(mcast_saddr);

			/* clear buffer and receive */
			memset(buf, 0, sizeof(buf));
			if ((len = recvfrom(s, buf, BUF_SIZE, 0, (struct sockaddr*)&mcast_saddr, &mcast_saddr_len)) < 0)
			{
				perror("\nRecv Error");
				exit(1);
			}
			fwrite(buf,1,len,fp); 
			delay(1);

			packet += 1;
			if(packet==100)
			{
				pthread_t thread_id;
				printf("Before Thread\n");
				pthread_create(&thread_id, NULL, vlcfun, NULL);
			}
			//system("clear");
			if(len < BUF_SIZE)
			{	flag = 0;}
		}
		fclose(fp);
	}
	close(s);
}

int receive_file(int socket)
{
	int recv_size = 0, size = 0, read_size, write_size;
	int packet_index =1, stat;
	char filearray[1024];
	FILE *file;
	fd_set fds;
	
	do{
		stat = read(socket, &size, sizeof(int));
	}while(stat<0);
	
	char buffer[] = "Got it";
	
	do{
		stat = write(socket, &buffer, sizeof(buffer));
	}while(stat<0);

	file = fopen("menu.txt", "w");

	if( file == NULL)
	{
		printf("\nFile could not be opened");
		return -1;
	}

	while(recv_size < size)
	{
		FD_ZERO(&fds);
		FD_SET(socket,&fds);

		do{
			read_size = read(socket, filearray, 1024);
		}while(read_size < 0);
		
		write_size = fwrite(filearray,1,read_size, file);

		if(read_size !=write_size)
			printf("\nError in read write");
		
		recv_size += read_size;
	}
	fclose(file);
	return 1;
}

void Station1(GtkWidget* widget, gpointer data)
{
	if(current_playing==1)
		return;
	current_playing=1;

	pthread_t thread_id_1; 

	ip = first;
	
	if(is_running==1)
	{
		should_terminate=1;
		while(should_terminate==1);
	}	
	is_running = 1;     
	
	pthread_create(&thread_id_1, NULL, udp,  NULL); 
	//system("clear");
}

void Station2(GtkWidget* widget, gpointer data)
{
	if(current_playing==2)
		return;
	current_playing=2;

	pthread_t thread_id_2;
	
	ip=sec;
	
	if(is_running==1)
	{
		should_terminate=1;
		while(should_terminate==1);
	}
	is_running = 1;	
	
	pthread_create(&thread_id_2, NULL, udp,  NULL);
	//system("clear");
}

void Station3(GtkWidget* widget, gpointer data)
{
	if(current_playing==3)
		return;
	current_playing=3;

	pthread_t thread_id_3; 

	ip = thr;
	
	if(is_running==1)
	{
		should_terminate=1;
		while(should_terminate==1);
	}	
	is_running = 1;
	
	pthread_create(&thread_id_3, NULL, udp,  NULL); 
	//system("clear");
}

void Station4(GtkWidget* widget, gpointer data)
{
	if(current_playing==4)
		return;
	current_playing=4;

	pthread_t thread_id_4; 

	ip = four;
	
	if(is_running==1)
	{
		should_terminate=1;
		while(should_terminate==1);
	}	
	is_running = 1;
	
	pthread_create(&thread_id_4, NULL, udp,  NULL); 
	//system("clear");
}

void exit_vlc(GtkWidget* widget, gpointer data)
{
	 close_vlc();
	 close_vlc();
	//system("clear");
}

void pause_vlc(GtkWidget* widget, gpointer data)
{
	system("kill -STOP $(pgrep vlc)");
	//system("clear");
}

void play_vlc(GtkWidget* widget, gpointer data)
{
	system("kill -CONT $(pgrep vlc)");
	//system("clear");
}

gint delete_event_handler(GtkWidget* widget, GdkEvent* event, gpointer data)
{  
       g_print("GUI closed\n");  
       return FALSE;
}

void destroy(GtkWidget* widget, gpointer data)
{
       gtk_main_quit();
}

int main(int argc, char *argv[])
{
	FILE *fp;
	struct hostent *hp;
	struct sockaddr_in sin;
	char *host;
	char buf[MAX_LINE];
	int s, len;
	
	if(argc==2)
	{
		host = argv[1];
	}
	else
	{
		perror("\nCommand line: ./client 10.20.x.y");
	}
	
	//host = "10.20.24.81";
	/* translate host name into peer's IP address */
	hp = gethostbyname(host);
	if (!hp) {
		perror("\nHost Error");
		exit(1);
	}

	/* build address data structure */
	bzero((char *)&sin, sizeof(sin));
	sin.sin_family = AF_INET;
	bcopy(hp->h_addr, (char *)&sin.sin_addr, hp->h_length);
	sin.sin_port = htons(SERVER_PORT);

	/* active open */
	if ((s = socket(PF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("\nSocket Error");
		exit(1);
	}

	if (connect(s, (struct sockaddr *)&sin, sizeof(sin)) < 0)
	{
		perror("\nConnect Error");
		close(s);
		exit(1);
	}

	receive_file(s);
	
	/* GtkWidget is the storage type for widgets */
	GtkWidget *window;
	GtkWidget *button1, *button2, *button3, *button4;
	GtkWidget *exitbutton5, *pausebutton6, *playbutton7;
	GtkWidget *box, *fixed;
	
	/* this is called in all GTK applications.  arguments are parsed from
	* the command line and are returned to the application. */
	gtk_init(&argc, &argv);  
	
	/* create a new window */
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL); 
	gtk_window_set_title (GTK_WINDOW (window), "Internet TV");
	
	fixed = gtk_fixed_new();
  	gtk_container_add(GTK_CONTAINER(window), fixed);
  
	/* when the window is given the "destroy" signal (this can be given
    * by the application, or the window manager, the function destroy
    * will be called as defined above.  The data passed to the callback
    * function is NULL and is ignored in the callback. */ 
	g_signal_connect(G_OBJECT(window), "delete_event", G_CALLBACK(delete_event_handler), NULL);  
	g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(destroy), NULL);  
	
	/* sets the border width of the window. */
	gtk_window_set_default_size (GTK_WINDOW (window), 500, 700); 
	//gtk_container_set_border_width(GTK_CONTAINER(window), 600);  
  	
  	box = gtk_button_box_new (GTK_ORIENTATION_HORIZONTAL);
  	gtk_container_add (GTK_CONTAINER (window), box);
  
	/* creates a new button with the label "Hello World". */
	button1 = gtk_button_new_with_label("Station 1, Nick, Mr. Bean, 239.192.13.1");
	button2 = gtk_button_new_with_label("Station 2, CN, Ninja Hathori, 239.192.13.2");
	button3 = gtk_button_new_with_label("Station 3, Pogo, Doreman, 239.192.13.3");
	button4 = gtk_button_new_with_label("Station 4, CN, Shin chan, 239.192.13.4");
	exitbutton5 = gtk_button_new_with_label("Exit VLC");
	pausebutton6 = gtk_button_new_with_label("Pause");
	playbutton7 = gtk_button_new_with_label("Play");
	
	gtk_fixed_put(GTK_FIXED(fixed), button1, 15, 70);
	gtk_widget_set_size_request(button1, 400, 50);
	
	gtk_fixed_put(GTK_FIXED(fixed), button2, 15, 135);
	gtk_widget_set_size_request(button2, 400, 50);
	
	gtk_fixed_put(GTK_FIXED(fixed), button3, 15, 195);
	gtk_widget_set_size_request(button3, 400, 50);
	
	gtk_fixed_put(GTK_FIXED(fixed), button4, 15, 255);
	gtk_widget_set_size_request(button4, 400, 50);
	
	gtk_fixed_put(GTK_FIXED(fixed), exitbutton5, 15, 315);
	gtk_widget_set_size_request(exitbutton5, 400, 50);
	
	gtk_fixed_put(GTK_FIXED(fixed), pausebutton6, 15, 375);
	gtk_widget_set_size_request(pausebutton6, 400, 50);
	
	gtk_fixed_put(GTK_FIXED(fixed), playbutton7, 15, 435);
	gtk_widget_set_size_request(playbutton7, 400, 50);
	/* When the button receives the "clicked" signal, it will call the
     * function hello() passing it NULL as it's argument.  The hello() function is
     * defined above. */
     
     g_signal_connect(G_OBJECT(button1), "clicked", G_CALLBACK(Station1), NULL);  
	g_signal_connect(G_OBJECT(button2), "clicked", G_CALLBACK(Station2), NULL);
	g_signal_connect(G_OBJECT(button3), "clicked", G_CALLBACK(Station3), NULL);  
	g_signal_connect(G_OBJECT(button4), "clicked", G_CALLBACK(Station4), NULL);
	g_signal_connect(G_OBJECT(exitbutton5), "clicked", G_CALLBACK(exit_vlc), NULL);
	g_signal_connect(G_OBJECT(pausebutton6), "clicked", G_CALLBACK(pause_vlc), NULL);
	g_signal_connect(G_OBJECT(playbutton7), "clicked", G_CALLBACK(play_vlc), NULL);
	
	/* This will cause the window to be destroyed by calling
     * gtk_widget_destroy(window) when "clicked.  Again, the destroy
     * signal could come from here, or the window manager. */
     gtk_container_add(GTK_CONTAINER(box), button1);
	gtk_container_add(GTK_CONTAINER(box), button2);
	gtk_container_add(GTK_CONTAINER(box), button3);
	gtk_container_add(GTK_CONTAINER(box), button4);
	gtk_container_add(GTK_CONTAINER(box), exitbutton5);
	gtk_container_add(GTK_CONTAINER(box), pausebutton6);
	gtk_container_add(GTK_CONTAINER(box), playbutton7);
	
	/* the final step is to display this newly created window... */  
	gtk_widget_show_all(window);  
	
	/* all GTK applications must have a gtk_main().     Control ends here
     * and waits for an event to occur (like a key press or mouse event). */
	gtk_main();
	
	return 0;  
}

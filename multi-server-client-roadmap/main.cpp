
using namespace std;
#include <iostream>
#include <cstdlib>
#include <string>
#include <string.h>
#include <strings.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/errno.h>
#include <sys/time.h>
#include <math.h>
#include <signal.h>
#include "vertex.h"
#include "edge.h"
#include "roadmap.h"

#define SOCKET_BUFFER_SIZE 2048

class vertex;
class edge;
class road;
class roadmap;

// function prototypes
void sighandler();
void * mgmt_client_thread_func();
void * user_client_thread_func();
void * producer_thread_func();
void * consumer_thread_func();
void * performance_stats_thread_func();
// int get_msg_size();
// useconds_t get_producer_delay();
// useconds_t get_consumer_delay();
// double normal_distribution();
int get_msg_size (char *s);

int B_BUFFER_SIZE = SOCKET_BUFFER_SIZE;
int P_COUNT;
int C_COUNT;
int MGMT_CLIENT;
int USER_CLIENT;

char socket_buffer[SOCKET_BUFFER_SIZE];
struct sockaddr_in serv_addr, cli_addr;

pthread_mutex_t lock;
sig_atomic_t quit = 0;

int buffer_index_write = 0;
int buffer_index_read = 0;
int buffer_msg_count = 0;
char* buffer;
int* buffer_msg_sizes;

unsigned long produced_request = 0;
unsigned long processed_request = 0;
unsigned long producer_block = 0;
unsigned long consumer_idle = 0;

struct timeval execution_start_time;
struct timeval execution_end_time;
struct timeval total_execution_time;

double total_execution_t=0.0;
double total_block_t=0.0;
double average_block_t=0.0;
double average_percentage_block_t=0.0;
double percentage_req_processed=0.0;
double percentage_req_blocked=0.0;



int main (int argc,char *argv[]) {
    cerr << "Program started" << endl;
    
    if (argc != 6){
        cerr << "INCORRECT EXECUTION COMMAND " << endl;
        cerr << "USAGE: ./<object_name> <BUFFER_SIZE> <ProducerCount> <ConsumerCount> <MGMT_CLIENT> <USER_CLIENT>" << endl;
        exit (1);
    }
    
    class roadmap x;
    
//    if (signal(SIGALRM,sighandler)==SIG_ERR)
//       cerr <<  "err" << endl;
//    if (signal(SIGINT,sighandler)==SIG_ERR)
//        cerr <<  "err" << endl;
    
    // for testing 10 sec runtime
    alarm(10);
    
    // capturing the command line arguments
    P_COUNT         = atoi(argv[1]);
    C_COUNT         = atoi(argv[2]);
    MGMT_CLIENT     = atoi(argv[3]);
    USER_CLIENT     = atoi(argv[4]);
    
    pthread_t *thread_mgmt_client = (pthread_t *)malloc(sizeof(pthread_t)*(MGMT_CLIENT));
    pthread_t *thread_user_client = (pthread_t *)malloc(sizeof(pthread_t)*(USER_CLIENT));
    pthread_t *thread = (pthread_t *)malloc(sizeof(pthread_t)*(P_COUNT+C_COUNT));
    buffer            =      (char *)malloc(sizeof(char)*B_BUFFER_SIZE);
    buffer_msg_sizes  =       (int *)malloc(sizeof(int)*B_BUFFER_SIZE);
    // pthread_t performance_stats_thread;
    pthread_mutex_init(&lock, NULL);
    
    // buffer initialization
    for (int i=0; i<B_BUFFER_SIZE; i++){
        buffer[i] = '\0';
        buffer_msg_sizes[i] = 0;
    }
    
    // cerr << "Trace: main: var initialized " << endl;
    
    // pthread_create(&performance_stats_thread,NULL,&performance_stats_thread_func,NULL);
    
    // create management clients
    for (int i=0; i<MGMT_CLIENT; i++)
        pthread_create(&thread_mgmt_client[i],NULL,&mgmt_client_thread_func,NULL);
    // create user clients
    for (int i=0; i<USER_CLIENT; i++)
        pthread_create(&thread_user_client[i],NULL,&user_client_thread_func,NULL);
    // create Producer Threads
    for (int i=0; i<P_COUNT; i++)
        pthread_create(&thread[i],NULL,&producer_thread_func,NULL);
    // create Consumer Threads
    for (int i=0; i<C_COUNT; i++)
        pthread_create(&thread[P_COUNT+i],NULL,&consumer_thread_func,NULL);
    
    // cerr << "Trace: main: all threads created " << endl;
    // gettimeofday(&execution_start_time,NULL);
    
    // cerr << "Program working.. Please Wait.. " << endl;
    
    // to force main to wait till all the threads are done
    for (int i=0; i<MGMT_CLIENT; i++)
        pthread_join(thread_mgmt_client[i],NULL);
    for (int i=0; i<USER_CLIENT; i++)
        pthread_join(thread_user_client[i],NULL);
    for (int i=0; i<P_COUNT; i++)
        pthread_join(thread[i],NULL);
    for (int i=0; i<C_COUNT; i++)
        pthread_join(thread[P_COUNT+i],NULL);
            
         
    // memory cleanup 
    pthread_mutex_destroy(&lock);
    free(thread_mgmt_client);
    free(thread_user_client);
    free(thread);
    free(buffer);
    free(buffer_msg_sizes);
    
    cerr << "Program ended" << endl;
    return 0;
}


void * mgmt_client_thread_func(void){
    int sockfd, portnum, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    char socket_buffer[SOCKET_BUFFER_SIZE];
    while (quit == 0) {
        portnum = 5000;
        sockfd = socket(AF_INET, SOCK_STREAM, 0);   // TCP
        if (sockfd < 0) 
            cerr << "ERROR opening socket" << endl;
        server = gethostbyname("localhost");
        if (server == NULL) {
            cerr << "ERROR, no such host\n" << endl;
            exit(0);
        }
        bzero((char *) &serv_addr, sizeof(serv_addr));
        serv_addr.sin_family = AF_INET;
        bcopy((char *)server->h_addr,(char *)&serv_addr.sin_addr.s_addr,server->h_length);
        serv_addr.sin_port = htons(portnum);
        if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0) 
            cerr << "ERROR connecting" << endl;
        printf("Please enter the message: ");
        bzero(socket_buffer,SOCKET_BUFFER_SIZE);
        fgets(socket_buffer,SOCKET_BUFFER_SIZE-1,stdin);
        n = write(sockfd,socket_buffer,strlen(socket_buffer));
        if (n < 0) 
             cerr << "ERROR writing to socket" << endl;
        bzero(socket_buffer,SOCKET_BUFFER_SIZE);
        n = read(sockfd,socket_buffer,SOCKET_BUFFER_SIZE-1);
        if (n < 0) 
             cerr << "ERROR reading from socket" << endl;
        printf("%s\n",socket_buffer);
    }
}

void * user_client_thread_func(void){
    int sockfd, portnum, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    char socket_buffer[SOCKET_BUFFER_SIZE];
    while (quit == 0) {
        portnum = 5000;
        sockfd = socket(AF_INET, SOCK_STREAM, 0);   // TCP
        if (sockfd < 0) 
            cerr << "ERROR opening socket" << endl;
        server = gethostbyname("localhost");
        if (server == NULL) {
            cerr << "ERROR, no such host\n" << endl;
            exit(0);
        }
        bzero((char *) &serv_addr, sizeof(serv_addr));
        serv_addr.sin_family = AF_INET;
        bcopy((char *)server->h_addr,(char *)&serv_addr.sin_addr.s_addr,server->h_length);
        serv_addr.sin_port = htons(portnum);
        if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0) 
            cerr << "ERROR connecting" << endl;
        printf("Please enter the message: ");
        bzero(socket_buffer,SOCKET_BUFFER_SIZE);
        fgets(socket_buffer,SOCKET_BUFFER_SIZE-1,stdin);
        n = write(sockfd,socket_buffer,strlen(socket_buffer));
        if (n < 0) 
             cerr << "ERROR writing to socket" << endl;
        bzero(socket_buffer,SOCKET_BUFFER_SIZE);
        n = read(sockfd,socket_buffer,SOCKET_BUFFER_SIZE-1);
        if (n < 0) 
             cerr << "ERROR reading from socket" << endl;
        printf("%s\n",socket_buffer);
    }
}


// Producer thread function
void * producer_thread_func(void){
    struct timeval produced_i_blocked_start_time;
    struct timeval produced_i_blocked_end_time;
    struct timeval produced_i_blocked_total_time;
    struct timeval produced_i_blocked_temp_time;
    produced_i_blocked_total_time.tv_sec=0;
    produced_i_blocked_total_time.tv_usec=0;
    int blocked=0;
    int is_blocked_time=0;
    
     int sockfd, newsockfd, portnum, clilen;
     char socket_buffer[SOCKET_BUFFER_SIZE];
     struct sockaddr_in serv_addr, cli_addr;
     int n;
     
     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0) 
        cerr << "ERROR opening socket" << endl;
     bzero((char *) &serv_addr, sizeof(serv_addr));
     portnum = 5000;
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(portnum);
     if (bind(sockfd, (struct sockaddr *) &serv_addr,
              sizeof(serv_addr)) < 0) 
              cerr << "ERROR on binding" << endl;
     
    while(quit==0){
        listen(sockfd,5);
        clilen = sizeof(cli_addr);
        newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
        if (newsockfd < 0) 
             cerr << "ERROR on accept" << endl;
        bzero(socket_buffer,SOCKET_BUFFER_SIZE);
        n = read(newsockfd,socket_buffer,SOCKET_BUFFER_SIZE-1);
        if (n < 0) cerr << "ERROR reading from socket" << endl;
        
        if (!producer_is_client_cmd_valid (socket_buffer)){
            n = write(newsockfd,"your command is invalid",23);
            if (n < 0) cerr << "ERROR writing to socket" << endl;
            continue;
        }
        else {
            int msgsize = get_msg_size (&socket_buffer);

            pthread_mutex_lock(&lock);
            produced_request++;

            // for sending randomly n number of integers at a time
            if (buffer_msg_count + msgsize  < B_BUFFER_SIZE){
                // for caculateing time 
                if (blocked == 1){
                    is_blocked_time = 1;
                    gettimeofday(&produced_i_blocked_end_time,NULL);
                    blocked = 0;
                }

                // cerr << "Trace: PRODUCER_data: %d msg_size = %d\n", (int)pthread_self(), msg_size);
                for (int i = 0; i < msgsize; i=((i+1)%B_BUFFER_SIZE)){
                    int wr_i = (buffer_index_write+i) % B_BUFFER_SIZE;
                    buffer[wr_i] = socket_buffer[i]; // sending the value of msg_size as msg msg_size number of times
                    // cerr << "Trace: PRODUCER_data: %d SENT: %d\n",(int)pthread_self(),buffer[wr_i]);
                }

                buffer_msg_sizes[buffer_index_write] = msgsize;
                buffer_index_write = (buffer_index_write + msgsize) % B_BUFFER_SIZE;
                buffer_msg_count += msgsize;
                // cerr << "Trace: PRODUCER_data: %d buffer_index_write: %d\n",(int)pthread_self(), buffer_index_write);
            }
            else {
                gettimeofday(&produced_i_blocked_start_time,NULL);
                blocked = 1;
                producer_block++;
                // cerr << "Trace: PRODUCER_data: %d thread - buffer has NOT ENOUGH SPACE\n",(int)pthread_self());  
            }

            if (is_blocked_time == 1){            
                is_blocked_time = 0;
                timersub(&produced_i_blocked_end_time,
                         &produced_i_blocked_start_time, 
                         &produced_i_blocked_temp_time);

                timeradd(&produced_i_blocked_total_time,
                         &produced_i_blocked_temp_time, 
                         &produced_i_blocked_total_time);

                total_block_t += (double)produced_i_blocked_temp_time.tv_sec + 
                                 (double)produced_i_blocked_temp_time.tv_usec/1000000.0;
            }

            pthread_mutex_unlock(&lock);
        }
    }
    

    cerr << "Trace: Producer: end : " <<  pthread_self() << endl);
    pthread_exit(NULL);
}




// Consumer thread function
void * consumer_thread_func (void){
   cerr << "Trace: Consumer: start: " << pthread_self() << endl);
    while (quit==0) {
        pthread_mutex_lock(&lock);
        
        // for sending randomly n number of integers at a time
        if (buffer_msg_count > 0){
            int rev_data;
            int msg_size = buffer_msg_sizes[buffer_index_read];
            // cerr << "Trace: consume_data: %d RECEIVED msg_size = %d\n", (int)pthread_self(), msg_size);
            
            for (int i=0; i < msg_size; i=((i+1)%B_BUFFER_SIZE)){
                int rd_i = (buffer_index_read + i) % B_BUFFER_SIZE;
                rev_data = buffer[rd_i];
                // cerr << "Trace: consume_data: %d RECEIVED: %d\n",(int)pthread_self(),rev_data);
            }
            
            // simulating time to consume data
            useconds_t t =get_consumer_delay(P_i_PROBABILITY_OF_C_t1);
            usleep(t);
            
            buffer_msg_sizes[buffer_index_read] = 0;
            buffer_index_read = (buffer_index_read + msg_size) % B_BUFFER_SIZE;
            buffer_msg_count -= msg_size;
            
            // cerr << "Trace: consume_data: %d buffer_index_read: %d\n",(int)pthread_self(),buffer_index_read);
            
            processed_request++;
        } else {
            consumer_idle++;
            // cerr << "Trace: consume_data: %d thread - buffer has NO DATA\n",(int)pthread_self()); 
        }    
        pthread_mutex_unlock(&lock);
        // cerr << "Trace: consume_data: %d end\n", (int)pthread_self());  
        
    }
    // cerr << "Trace: Consumer: %d end\n", (int)pthread_self());
    pthread_exit(NULL);
}



int producer_is_client_cmd_valid (string s){
    // need to parse the input string and get the method name and parameters
    // return 1 if valid
    // else return 0 and send back INVALID_CMD to client
    
    
    
    
}

void sighandler(int signum) {
    if (signum==SIGINT){
        cerr << "Trace: SIGNAL = SIGINT" << endl;
        quit=1;
    }
    if (signum==SIGALRM){
        cerr << "Trace: SIGNAL = SIGALRM" << endl);
        quit=1;
    }
}



int get_msg_size (char *s){
    int i = 0;
    while (s[i] != '\0')
        i++;
    return i;
}
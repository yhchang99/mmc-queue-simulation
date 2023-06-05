#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>
#include <float.h>

// Define customer 
typedef struct customer{
	struct customer *next;
	double it;  // interarrival time
	double st;  // service time
	double at;  // arrival time
	double dt;	// departure time
	double wt;  // waiting time
	int which_server;
} customer;

// Define server
typedef struct server{
	customer *head;
	customer *tail;
	int size;
	double t;	// server time
	double dt;	// the departure time of last person
} server;

// Define system

typedef struct _system{
	double time; 			// Time
	double waiting_time;	// Waiting time in system
	double system_time; 	// Total time in system 
	double total_service_time;
	server **servers;	 	
} _system;

//customer function
customer *customer_new(double it, double st);	// Create a new customer

//server function
server *q_new(); 								// Create a new server
customer *q_pop(server *q);						// Pop a customer out 

//system function
_system *system_new(int n, double arrival_rate, double service_rate);	// create a new system
bool q_insert(server *q, customer *n);									// Insert customer into server

//some math function
double rand_exp(double arrival_rate);			// Generate exponential RV with arrival_rate rate
int factorial(int n);							// To Calculate n!

//
void scheduling(_system *s, customer *n,  FILE *fp);
double get_dt(_system *s);

/* Global variables */
double arrival_rate, service_rate;			// Parameters for generate exp rv
double samples;							// Number of events
int count =0;
int num_server;
server *q_system;						// server which stores all events

//math function


double rand_exp(double arrival_rate){
	double u;
	u = rand()/(RAND_MAX + 1.0);// generate random number between 0~1
	return -log(1-u)/arrival_rate;
}

int factorial(int n){
	if(n == 0)
		return 1;
	else
		return n*factorial(n-1);
}
//queue function 
server *q_new(){
	server *q = malloc(sizeof(server));
	if(q){
		q->head = NULL;
		q->tail = NULL;
		q->size = 0;
		q->t = 0;
		q->dt = 0;
		return q;
	}
	else
		return NULL;
}

bool q_insert(server *q, customer *n){
	if(!q | !n){
		return false;
	}
	else{
		customer *tmp = malloc(sizeof(customer));
		tmp->it = n->it;
		tmp->st = n->st;
		tmp->dt = n->dt;
		tmp->wt = n->wt;
		
		/* First time entering the system */
		if( n->at == -1 ){
			q->t += n->it;	
			tmp->at = q->t;
		}
		else
			tmp->at = n->at;

		if(q->head && q->tail){
			q->tail->next = tmp;
			q->tail = tmp;
		}
		else{
			q->head = tmp;
			q->tail = tmp;
		}		
		q->size++;
		return true;
	}
}

customer *q_pop(server *q){
	customer *tmp = q->head;
	if(q->size == 1){
		q->head = NULL;
		q->tail = NULL;
	}
	else{
		q->head = q->head->next;
	}
	q->size--;
	return tmp;
}

// Create a customer
customer *customer_new(double it, double st){
	customer *tmp = malloc(sizeof(customer));
	if(tmp){
		tmp->it = it;
		tmp->st = st;
		tmp->at = -1;			// initialize
		tmp->which_server = -1;	// initialize
		return tmp;
	}
	else
		return NULL;
}

// Create a _system
_system *system_new(int n, double arrival_rate, double service_rate){
	_system *tmp = malloc(sizeof(_system));
	if(tmp){
		tmp->time = 0;
		tmp->waiting_time = 0;
		tmp->system_time = 0;
		tmp->total_service_time =0;
		tmp->servers = malloc(num_server * sizeof(server *));
		for(int i=0;i<n;i++){
			tmp->servers[i] = q_new();
		}
		return tmp;
	}
	else
		return NULL;
}

void scheduling(_system *s, customer *n, FILE *fp){
	if(n){
		bool if_insert = false;
		while( !if_insert ){
			/* Update system time and update states of all servers */
			s->time = n->at;
			for(int i=0;i<num_server;i++){
				if( s->servers[i]->size > 0 && s->servers[i]->dt <= s->time ){
					//s->servers[i]->dt = s->servers[i]->head->at;
					q_pop(s->servers[i]);
				}
			}

			/* Check status of all servers and find the min_dt in all busy servers */
			server *min_q = q_new();
			min_q->dt = DBL_MAX;

			bool need_wait = true;
			for(int i=0;i<num_server;i++){
				/* Server isn't busy, assign customer to it */
				if(s->servers[i]->size == 0){
					s->servers[i]->dt = n->at + n->st;
					s->system_time += n->st;
					s->total_service_time += n->st;
					n->dt = n->at + n->st;
					n->at = n->at - n->wt;	// shift back
					/* Assign customer to server */
					n->which_server = i;
					if_insert = q_insert(s->servers[i], n);
					need_wait = false;
					break;
				}
                /* Server is busy, find the min dt*/
				else if( min_q->dt > s->servers[i]->dt ){
					min_q = s->servers[i];
				}
			}
			/* If all server are busy, find the min_dt to calculate the waiting time */
			if(need_wait){
				s->waiting_time += min_q->dt - n->at;
				n->wt = min_q->dt - n->at;
				s->system_time += min_q->dt - n->at;
				n->at = min_q->dt;	// shift real arrival time to min_q->dt
			}
		}
		if(!(n->next)){
			s->time = get_dt(s);
			for(int i=0;i<num_server;i++){
				if( s->servers[i]->size > 0 ){
					if( s->servers[i]->dt <= s->time ){
						q_pop(s->servers[i]);
					}
				}
			}
		}
		if(n->wt ==0)
			count++;
        fprintf(fp, "%f\n",n->wt);
		//fprintf(fp, "%f %f %f %f %f %d\n", n->it, n->st, n->at, n->dt, n->wt, n->which_server);
	}
}

double get_dt(_system *s){
	double max_dt = s->servers[0]->dt;
	for(int i=0;i<num_server;i++){
		if( max_dt <= s->servers[i]->dt )
			max_dt = s->servers[i]->dt;
	}
	return max_dt;
}
void theoretical(){
	if(num_server ==1){
		double rho = arrival_rate/service_rate;
		double E_W = rho/(service_rate*(1-rho));
		printf("Mean of System  Time: %f\n", E_W+1/service_rate);
		printf("Mean of Waiting Time: %f\n", E_W);
	}
	else if(num_server > 1){
		double r = arrival_rate/service_rate;
		double rho = r/num_server;
		
		double p0 = 0;
		for(int i =0; i<num_server; i++)
			p0 += pow(r, i)/factorial(i);
		
		p0 += pow(r,num_server)/(factorial(num_server)*(1-rho));
		p0 = 1/p0;

		double E_Wq = p0*pow(r, num_server)/(factorial(num_server)*(num_server)*service_rate*pow(1-rho,2));
		double Wt 	= 1-pow(r,num_server)*p0/(factorial(num_server)*(1-rho));
		printf("Mean of System  Time: %f\n", E_Wq+ 1/service_rate);
		printf("Mean of Waiting Time: %f\n", E_Wq);
		printf("probibility of W(0) : %f\n" , Wt);
	}
}

int main(){
	srand((unsigned)time(NULL));
		
    // Define parameters
	samples = 1000000;
    arrival_rate = 0.9;
    service_rate = 1;
    num_server = 2;

	double avg_service_time  =0;
	double avg_waiting_time  =0;
	double avg_system_time   =0;
    int repeat = 10;

	FILE *fp = fopen("data/wait_time.txt", "w");
	
	for(int i=0;i<repeat;i++){
		_system * s = system_new(num_server, arrival_rate, service_rate);
		q_system = q_new();

		// Create events and add to server
		for(int i=0;i<samples;i++)
			q_insert(q_system, customer_new(rand_exp(arrival_rate),rand_exp(service_rate)));

		customer *head = q_system->head;
		while(head){
			scheduling(s, head, fp);
			head = head->next;
		}
		avg_service_time += s->total_service_time /samples;
		avg_waiting_time += s->waiting_time /samples;
		avg_system_time  += s->system_time /samples;
	}
	printf("\n*********Parameter*********\n");
    printf("arrival_rate: %f\n",arrival_rate);
    printf("service_rate: %f\n",service_rate);
    printf("Number of servers: %d\n",(int)num_server);
	printf("Number of samples: %d\n",(int)samples);
    printf("Number of simulation: %d\n",repeat);
    printf("\n*********Theoretical*******\n");
	theoretical();
	printf("\n*********Simulated*********\n");
	printf("Mean of System  Time: %f\n",  avg_system_time/repeat);
    printf("Mean of Waiting Time: %f\n", avg_waiting_time/repeat);
	printf("probibility of W(0) : %f\n", count/(repeat*samples));
	
	return 0;
}
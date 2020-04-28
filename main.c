#define _GNU_SOURCE
#include <unistd.h>
#include <stdio.h>
#include <sched.h>
#include <time.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#define schcpu 2
#define chdcpu 3

void timeunit(){
volatile unsigned long i; for(i=0;i<1000000UL;i++);
}
int chdpid[10];
char name[10][50];
int begin[10];
int length[10];
int tu = 0;
int started = 0;
cpu_set_t chdset;
int primin;
int primax;
char policy[20];
void fifo(int proccnt){
	int cnt = proccnt;
	for(;cnt>0;){
		for(int i = 0;i<proccnt;i++)if(begin[i] == tu){
			cnt--;
			started++;
			int pid = fork();
			if(pid==0){
					pid = getpid();
					sched_setaffinity(0,sizeof(chdset),&chdset);
					struct sched_param para;
					para.sched_priority = primax-started;
					sched_setscheduler(0,SCHED_FIFO,&para);
					// proc ready
					struct timespec st,et;
					int len = length[i];
					clock_gettime(CLOCK_REALTIME,&st);
					for(volatile int j = 0;j<len;j++){
						volatile unsigned long i; for(i=0;i<1000000UL;i++);
					}
					clock_gettime(CLOCK_REALTIME,&et);
					// print to output
					// require root
					FILE* f = fopen("/dev/kmsg","w");
					fprintf(f, "[Project1] %d %lu.%09lu %lu.%09lu\n",pid,st.tv_sec,st.tv_nsec,et.tv_sec,et.tv_nsec );
					// done
					exit(EXIT_SUCCESS);			
			}else{
				// proc detached
				chdpid[i] = pid;

			}
			//fprintf(stderr, "process %s started\n",name[i] );
		}
		timeunit();
		tu++;
		
	}
	fprintf(stderr, "All process started.\n");
	for(int i = 0;i<proccnt;i++){waitpid(chdpid[i],NULL,0);
		fprintf(stdout, "%s %d\n",name[i],chdpid[i] );
	}
}
int main(){
	cpu_set_t schset;
	CPU_ZERO(&schset);
	CPU_ZERO(&chdset);
	CPU_SET(schcpu,&schset);
	CPU_SET(chdcpu,&chdset);
	sched_setaffinity(0,sizeof(schset),&schset);
	primin = sched_get_priority_min(SCHED_FIFO);
	primax = sched_get_priority_max(SCHED_FIFO);
	scanf("%s",policy);
	int processin;
	scanf("%d" ,&processin);
	for(int i = 0;i<processin;i++){
		scanf("%s %d %d",name[i],&begin[i],&length[i]);
	}
	fifo(processin);

}


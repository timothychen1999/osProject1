#include <unistd.h>
#include <stdio.h>
#include <sched.h>
#include <time.h>
#define schcpu 2
#define chdcpu 12

void timeunit(){
volatile unsigned long i; for(i=0;i<1000000UL;i++);
}
int chdpid[10];
char name[10][50];
int begin[10];
int length[10];
int tu = 0;
int started = 0;
struct cpu_set_t chdset;
int primin;
int primax;

void fifo(int proccnt){
	int cnt = proccnt;
	for(;cnt>0;){
		for(int i = 0;i<proccnt;i++)if(begin[i] == tu){
			cnt--;
			started++;
			int pid = fork();
			if(pid==0){
					pid = getpid();
					sched_setaffinty(0,sizeof(chdset),&chdset);
					struct sched_param para;
					para.sched_priority = primax-started;
					sched_setscheduler(0,SCHED_FIFO,&para);
					// proc ready
					struct timespec st,et;
					clock_gettime(CLOCK_REALTIME,&st);
					for(int j = 0;j<length[i];j++)timeunit();
					clock_gettime(CLOCK_REALTIME,&et);
					// print to output
					// require root
					FILE* f = fopen("/dev/kmsg","w");
					fprintf(f, "[Project1] %d %lu.%lu %lu.%lu\n",pid,st.tv_sec,st.tv_nsec,et.tv_sec,et.tv_nsec );
					// done
					exit(EXIT_SUCCESS);			
			}else{
				// proc detached
				chdpid[i] = pid;

			}
		}
		timeunit();
		tu++;
		
	}

}
int main(){
	struct cpu_set_t schset;
	CPU_ZERO(&schset);
	CPU_ZERO(&chdset);
	CPU_SET(schcpu,&schset);
	CPU_SET(chdcpu,&chdset);
	sched_setaffinty(0,sizeof(schset),&schset);
	primin = sched_get_priority_min(SCHED_FIFO);
	primax = sched_get_priority_max(SCHED_FIFO);


}


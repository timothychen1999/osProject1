#define _GNU_SOURCE
#include <unistd.h>
#include <stdio.h>
#include <sched.h>
#include <time.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>
#include <sys/types.h>
#define schcpu 22
#define chdcpu 23

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
					
					struct sched_param para;
					para.sched_priority = primax-started;
					sched_setscheduler(0,SCHED_FIFO,&para);
					sched_setaffinity(0,sizeof(chdset),&chdset);
					struct sched_param runningpara;
					runningpara.sched_priority = primin;
					// proc ready
					struct timespec st,et;
					int len = length[i];
					clock_gettime(CLOCK_REALTIME,&st);
					for(volatile int j = 0;j<len;j++){
						volatile unsigned long i; for(i=0;i<1000000UL;i++);
					}
					clock_gettime(CLOCK_REALTIME,&et);
					sched_setscheduler(0,SCHED_FIFO,&runningpara);
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
	//fprintf(stderr, "All process started.\n");
	for(int i = 0;i<proccnt;i++){waitpid(chdpid[i],NULL,0);
		fprintf(stdout, "%s %d\n",name[i],chdpid[i] );
	}
}
void psjf(int proccnt){
	int procpri[10];
	int mxlen = -1;
	for(int i = 0;i<proccnt;i++)mxlen = mxlen>length[i]?mxlen:length[i];
	int shorter = 0;
	for(int i = 0;i<=mxlen+20;i++){
		for(int j = 0;j<proccnt;j++)if(length[j]==i)procpri[j] = shorter++;
	}
	//fprintf(stderr,"%d %d %d\n",procpri[0],procpri[1],procpri[2]);

	int cnt = proccnt;
	for(;cnt>0;){
		for(int i = 0;i<proccnt;i++)if(begin[i] == tu){
			cnt--;
			started++;
			int pid = fork();
			if(pid==0){
					pid = getpid();
					
					struct sched_param para;
					para.sched_priority = primax-procpri[i];
					sched_setscheduler(0,SCHED_FIFO,&para);
					sched_setaffinity(0,sizeof(chdset),&chdset);
					struct sched_param runningpara;
					runningpara.sched_priority = primin;
					// proc ready
					struct timespec st,et;
					int len = length[i];
					clock_gettime(CLOCK_REALTIME,&st);
					for(volatile int j = 0;j<len;j++){
						volatile unsigned long i; for(i=0;i<1000000UL;i++);
					}
					clock_gettime(CLOCK_REALTIME,&et);
					sched_setscheduler(0,SCHED_FIFO,&runningpara);
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
	//fprintf(stderr, "All process started.\n");
	for(int i = 0;i<proccnt;i++){waitpid(chdpid[i],NULL,0);
		fprintf(stdout, "%s %d\n",name[i],chdpid[i] );
	}


}
void sjf(int proccnt){
	int procpri[10];
	int mxlen = -1;
	for(int i = 0;i<proccnt;i++)mxlen = mxlen>length[i]?mxlen:length[i];
	int shorter = 1;
	for(int i = 0;i<=mxlen+20;i++){
		for(int j = 0;j<proccnt;j++)if(length[j]==i)procpri[j] = shorter++;
	}
	//fprintf(stderr,"%d %d %d\n",procpri[0],procpri[1],procpri[2]);

	int cnt = proccnt;
	for(;cnt>0;){
		for(int i = 0;i<proccnt;i++)if(begin[i] == tu){
			cnt--;
			started++;
			int pid = fork();
			if(pid==0){
					pid = getpid();
					
					struct sched_param para;
					para.sched_priority = primax-procpri[i];
					sched_setscheduler(0,SCHED_FIFO,&para);
					sched_setaffinity(0,sizeof(chdset),&chdset);
					// proc ready
					// proc sellected, set to highest priority
					struct sched_param runningpara;
					runningpara.sched_priority = primax;
					sched_setscheduler(0,SCHED_FIFO,&runningpara);
					
					struct timespec st,et;
					int len = length[i];
					clock_gettime(CLOCK_REALTIME,&st);
					for(volatile int j = 0;j<len;j++){
						volatile unsigned long i; for(i=0;i<1000000UL;i++);
					}
					clock_gettime(CLOCK_REALTIME,&et);
					// print to output
					// require root
					runningpara.sched_priority = primin;
					sched_setscheduler(0,SCHED_FIFO,&runningpara);
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
	//fprintf(stderr, "All process started.\n");
	for(int i = 0;i<proccnt;i++){waitpid(chdpid[i],NULL,0);
		fprintf(stdout, "%s %d\n",name[i],chdpid[i] );
	}


}
void high(int pid){
	struct sched_param para;
	para.sched_priority = primax;
	sched_setscheduler(pid,SCHED_FIFO,&para);
}
void low(int pid){
	struct sched_param para;
	para.sched_priority = primin;
	sched_setscheduler(pid,SCHED_FIFO,&para);
}
void rr(int proccnt){
	int cnt = proccnt;
	int turn = -1;
	int runninglst[10];
	int isrunnging = 0;
	int nxtchk = -1;
	int flag = 0;
	//int runninglst[10];
	memset(runninglst,0,sizeof(runninglst));

	for(;cnt>0;){
		for(int i = 0;i<proccnt;i++)if(begin[i] == tu){
			started++;
			if(isrunnging++ == 0){
				nxtchk = tu+500;
				turn = i;
				flag = 1;
			}
			runninglst[i] = 1;
			int pid = fork();
			if(pid==0){
					pid = getpid();
					
					struct sched_param para;
					para.sched_priority = primin;
					sched_setscheduler(0,SCHED_FIFO,&para);
					sched_setaffinity(0,sizeof(chdset),&chdset);

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
		//rr handler
		if(flag){
			flag = 0;
			high(chdpid[turn]);
		}
		int hp;
		if(isrunnging&&(hp = waitpid(chdpid[turn],NULL,WNOHANG))!=0){
			if(hp==-1){
				perror("waitpid:");
				exit(EXIT_FAILURE);
			}
			
			runninglst[turn] = 0;
			isrunnging--;
			cnt--;

			//find next to run
			if(isrunnging==0)continue;
			while(runninglst[turn]==0){
				turn = turn+1;
				if(turn>=proccnt)turn -= proccnt;
			}
			nxtchk = tu+500;
			high(chdpid[turn]);
			//fprintf(stderr, "switch, pid: %d end, %d left.\n",hp,cnt );
		}
		if(isrunnging&&tu >= nxtchk){
			int pturn = turn;
			//next run
			low(chdpid[turn]);
			do{
				turn = turn+1;
				if(turn>=proccnt)turn -= proccnt;
			}while(runninglst[turn]==0);
			nxtchk = tu+500;
			high(chdpid[turn]);
			nxtchk = tu+500;
			//fprintf(stderr, "switch from %d to %d\n",pturn,turn );

		}
		timeunit();
		tu++;
		
	}
	for(int i = 0;i<proccnt;i++)
		fprintf(stdout, "%s %d\n",name[i],chdpid[i] );

	//fprintf(stderr, "All process started.\n");

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
#ifdef MinMax
	fprintf(stderr, "%d %d\n",primax,primin);
	exit(EXIT_SUCCESS);
#endif 
	scanf("%s",policy);
	int processin;
	scanf("%d" ,&processin);
	for(int i = 0;i<processin;i++){
		scanf("%s %d %d",name[i],&begin[i],&length[i]);
	}
	switch(policy[0]){
		case 'P':
		case 'p':
			psjf(processin);
			break;
		case 'F':
		case 'f':
			fifo(processin);
			break;
		case 's':
		case 'S':
			sjf(processin);
			break;
		case 'R':
		case 'r':
			rr(processin);
			break;
		default:
			fprintf(stderr,"No such policy : %s\n",policy);
			exit(-1);

	}

}


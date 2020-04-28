#include <unistd.h>
#include <stdio.h>
#define schcpu 2
#define chdcpu 12
void timeunit(){
volatile unsigned long i; for(i=0;i<1000000UL;i++);
}
int pid[10];
char name[10][50];
int begin[10];
int tu = 0;
int 



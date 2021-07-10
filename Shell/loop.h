#ifndef __PROMPT_H
#define __PROMPT_H

void wloop();
void execcd(char *path,char *home); 
void echoc(char *str);
void list(char *str,char *home);
void proc(char *base,char *comm);
void bhandlerfun();
void printinfo(int x);
void killbgs();
void findjobs();
void bgtofg(int x);
void fgtobg(int x);
void killproc(int x,int sig);
#endif
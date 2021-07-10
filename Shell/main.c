#include "headers.h"
#include "loop.h"
int main(){
	signal(SIGINT,SIG_IGN);
	signal(SIGTSTP,SIG_IGN);
	wloop();
	return 0;
}
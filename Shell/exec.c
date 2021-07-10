#include "headers.h"
#include "loop.h"
struct node{
	pid_t id;
	char str[1025];
	struct node *next,*prev;
};
struct node *head = NULL;
struct node *tail = NULL;
struct node *curr = NULL;
void addnode(pid_t aid,char *str1)
{
	struct node *new_node = (struct node*)malloc(sizeof(struct node)); 
	new_node->id = aid;
	strcpy(new_node->str,str1);
	new_node->prev = NULL;
	new_node->next = NULL;
	if(head==NULL){
		head=new_node;
		return;
	}
	curr = head;
	while(curr->next!=NULL)curr = curr->next;
	curr->next = new_node;
	tail = new_node;
	new_node->prev = curr;
}
void delnode(struct node* dnode)
{
	struct node *temp1;
	if(head==NULL)return;
	if(head==dnode){
		if(head->next != NULL){
			head->next->prev = NULL;
			head = head->next;
			return;
		}
		else{
			head=NULL;
			return;
		}
	}
	curr = head;
	while(curr->next != NULL && curr != dnode){
		temp1 = curr;
		curr = curr->next;
	}
	if(curr==dnode){
		temp1->next = temp1->next->next;
		if(temp1->next != NULL){
			temp1->next->prev = temp1;
		}
		else tail = temp1;
		free(curr);
	}
}
void bhandlerfun()
{	
	curr = head;	
	int stats;
	while(curr != NULL)
	{	
		if(waitpid(curr->id,&stats,WNOHANG)>0)
		{
		if(WIFEXITED(stats))
		{
			printf("\n%s with pid %d exited normally\n",curr->str,curr->id);
			delnode(curr);
		}
		else if(WIFSIGNALED(stats)){
			printf("\n%s with pid %d exited abnormally\n",curr->str,curr->id);
			delnode(curr);
		}
		}
		curr = curr->next;
	} 
}
char* fdate(char* str, time_t val)
{
        strftime(str, 36, "%b%e %H:%M", localtime(&val));
        return str;
}
void pathcal(char *ans,char *path,char *name)
{
	int size = strlen(path);
	int i;
	ans[size] = '/';
	for(i=size+1;i<(size+strlen(name)+1);i++)ans[i] = name[i-(size+1)];
	ans[i] = '\0';
}
void execcd(char path[],char home[]){
	int dir;
	char path1[1025] = "\0";
	int hmsize = strlen(home);
	if(strlen(path)==0)dir = chdir(home);
	else if(path[0] == '~'){
		strcpy(path1,home);
		for(int i=1;i<strlen(path);i++)path1[hmsize-1+i] = path[i];
		dir = chdir(path1);
	}
	else dir = chdir(path);
	if(dir==-1)perror("Shell");
}
void echoc(char str[])
{
	printf("%s\n", str);
}
void list(char str[],char home[])
{
	int size = strlen(str);
	int hmsize = strlen(home);
	char path1[1025]="\0";
	int a=0,l=0;
	for(int i=0;i<size;i++)
	{
		if(str[i] == '.' || str[i]=='/'){
			for(int j=i;j<size;j++){
				path1[j-i] = str[j];
			}
			break;
		}
		else if(str[i] == '~')
		{
			strcpy(path1,home);
			if(i<(size-1))
			{
				int z=0;
				for(int j=i+1;j<size;j++){
					z++;
					path1[hmsize+z-1] = str[j];
				}
			}
			break;
		}
		else if(str[i] == '-')
		{
			if(str[i+1] == 'a')a=1;
			if(str[i+1] == 'l')l=1;
			if((i+2)<size && str[i+2]=='a')a=1;
			if((i+2)<size && str[i+2]=='l')l=1;
		}
	}
	DIR *dip;   
	struct dirent *dipst;
	char dpath[1025]=".";
	if(strlen(path1) == 0){
		dip = opendir("./");
		if(dip == NULL)perror("Shell");
		strcpy(path1,".");
	}
	else{
	 dip = opendir(path1);
	 if(dip == NULL)perror("Shell");
	 strcpy(dpath,path1);
	}
	if(dip!=NULL){
	if(l==0 && a==0)
	{
		while ((dipst = readdir(dip)) != NULL){
		if(dipst->d_name[0] != '.' && dipst->d_name[strlen(dipst->d_name)-1] != '~')printf("%s  ", dipst->d_name);
		}
			printf("\n");
	}
	if(l==0 && a==1)
	{
		while ((dipst = readdir(dip)) != NULL)printf("%s  ", dipst->d_name);
		printf("\n");	
	}
	if(l==1 && a==0){
		struct stat fstat;
		struct passwd *userid;
		struct group *grp;
		while ((dipst = readdir(dip)) != NULL){
		if(dipst->d_name[0] != '.' && dipst->d_name[strlen(dipst->d_name)-1] != '~'){
			pathcal(dpath,path1,dipst->d_name);
			if(stat(dpath,&fstat) < 0)perror("Shell");
			userid = getpwuid(fstat.st_uid);
    		printf( (S_ISDIR(fstat.st_mode)) ? "d" : "-");
    		printf( (fstat.st_mode & S_IRUSR) ? "r" : "-");
    		printf( (fstat.st_mode & S_IWUSR) ? "w" : "-");
    		printf( (fstat.st_mode & S_IXUSR) ? "x" : "-");
    		printf( (fstat.st_mode & S_IRGRP) ? "r" : "-");
    		printf( (fstat.st_mode & S_IWGRP) ? "w" : "-");
    		printf( (fstat.st_mode & S_IXGRP) ? "x" : "-");
    		printf( (fstat.st_mode & S_IROTH) ? "r" : "-");
    		printf( (fstat.st_mode & S_IWOTH) ? "w" : "-");
    		printf( (fstat.st_mode & S_IXOTH) ? "x" : "-");
    		printf(" %ld",fstat.st_nlink);
    		grp = getgrgid(fstat.st_gid);
    		printf(" %s %s",userid->pw_name,grp->gr_name);
    		printf(" %ld",fstat.st_size);
    		char md[36];
    		printf(" %s",fdate(md,fstat.st_mtime));
			printf(" %s\n", dipst->d_name);
		}
	}			
	}
		if(l==1 && a==1){
		struct stat fstat;
		struct passwd *userid;
		struct group *grp;
		while ((dipst = readdir(dip)) != NULL){
			pathcal(dpath,path1,dipst->d_name);
			if(stat(dpath,&fstat) < 0)perror("Shell");
			userid = getpwuid(fstat.st_uid);
    		printf( (S_ISDIR(fstat.st_mode)) ? "d" : "-");
    		printf( (fstat.st_mode & S_IRUSR) ? "r" : "-");
    		printf( (fstat.st_mode & S_IWUSR) ? "w" : "-");
    		printf( (fstat.st_mode & S_IXUSR) ? "x" : "-");
    		printf( (fstat.st_mode & S_IRGRP) ? "r" : "-");
    		printf( (fstat.st_mode & S_IWGRP) ? "w" : "-");
    		printf( (fstat.st_mode & S_IXGRP) ? "x" : "-");
    		printf( (fstat.st_mode & S_IROTH) ? "r" : "-");
    		printf( (fstat.st_mode & S_IWOTH) ? "w" : "-");
    		printf( (fstat.st_mode & S_IXOTH) ? "x" : "-");
    		printf(" %ld",fstat.st_nlink);
    		grp = getgrgid(fstat.st_gid);
    		printf(" %s %s",userid->pw_name,grp->gr_name);
    		printf(" %ld",fstat.st_size);
    		char md[36];
    		printf(" %s",fdate(md,fstat.st_mtime));
			printf(" %s\n", dipst->d_name);
		}
	}		
	closedir(dip);
}
}
void printinfo(int x)
{	char path[1025],line[101];
	struct stat st;
	if(x==0)x = getpid();
	sprintf(path,"/proc/%d",x);
	if(stat(path, &st) == -1 && errno == ENOENT){
		printf("Shell:No such process exists\n");
	}
	else{
	int id;
	FILE *fp1;
	char st;
	sprintf(path,"/proc/%d/stat",x);
	fp1 = fopen(path,"r");
	fscanf(fp1,"%d %s %c",&id,line,&st);
	printf("pid -- %d\nStatus -- %c\n",id,st);
	sprintf(path,"/proc/%d/statm",x);
	fp1 = fopen(path,"r");
	int mem;
	fscanf(fp1,"%d",&mem);
	printf("memory -- %d\n",mem);
	fclose(fp1);
	sprintf(path,"/proc/%d/exe",x);
	strcpy(line,"\0");
	int rd = readlink(path,line,101);
	line[rd] = '\0';
	printf("Executable path -- %s\n",line);
	}
}
struct fullpipe{
	char *args[1024];
	char *infilename,*outfilename;
	int bg,inred,outred;
};
void proc(char *base,char* comm)
{
	char *pipes[1024];
	char *pipetok = strtok(comm,"|");
	int pipe_ind=0;
	while(pipetok != NULL)
	{
		pipes[pipe_ind++] = pipetok;
		pipetok = strtok(NULL,"|");
	}
	int pipeno = 0,i;
	struct fullpipe pipecomms[10];
	while(pipeno!=pipe_ind){
		pipecomms[pipeno].bg=0;
		pipecomms[pipeno].inred=0;
		pipecomms[pipeno].outred=0;
		char *commtok = strtok(pipes[pipeno]," \t");
		i=0;
		while(commtok != NULL)
		{
			if(!strcmp(commtok,"<"))
			{
				pipecomms[pipeno].inred = 1;
				commtok = strtok(NULL, " \t");
				pipecomms[pipeno].infilename = commtok;
			}
			else if(!strcmp(commtok,">"))
			{
				pipecomms[pipeno].outred = 1;
				commtok = strtok(NULL, " \t");
				pipecomms[pipeno].outfilename = commtok;
			}
			else if(!strcmp(commtok,">>"))
			{
				pipecomms[pipeno].outred = 2;
				commtok = strtok(NULL, " \t");
				pipecomms[pipeno].outfilename = commtok;
			}
			else if(!strcmp(commtok,"&"))
			{
				pipecomms[pipeno].args[i] = NULL;
				pipecomms[pipeno].bg=1;
				break;
			}
			else {
				pipecomms[pipeno].args[i++] = commtok;
			}
			commtok = strtok(NULL," \t");
		}
		pipecomms[pipeno].args[i] = NULL;
		pipeno++;
	}
	pipeno=0;
	int fdp[2],exfd = 0,stats;
	pid_t pid;
	while(pipeno!=pipe_ind)
	{
		int pipeflag=0;
		pipe(fdp);
		if((pid=fork())<0)perror("Shell");
		else if(pid==0 && pipecomms[pipeno].bg==1){
			setpgid(0,0);
			execvp(pipecomms[pipeno].args[0],pipecomms[pipeno].args);
		}
		else if(pid!=0 && pipecomms[pipeno].bg==1){
			signal(SIGCHLD,bhandlerfun);
			setpgid(pid,pid);
			addnode(pid,base);
		}
		else if(pid==0 && pipecomms[pipeno].bg!=1)
		{
			setpgid(0,0);
			signal(SIGINT,SIG_DFL);
			signal(SIGTSTP,SIG_DFL);
    		if(pipecomms[pipeno].inred){
        	int fd0 = open(pipecomms[pipeno].infilename, O_RDONLY, 0);
        	dup2(fd0, STDIN_FILENO);
        	close(fd0);
        	}	
			if (pipecomms[pipeno].outred==1) {
        	int fd1 = creat(pipecomms[pipeno].outfilename, 0644);
        	dup2(fd1, STDOUT_FILENO);
        	pipeflag = 1;
        	close(fd1);
    		}
    		if(pipecomms[pipeno].outred==2){
        	int fd2 = open(pipecomms[pipeno].outfilename,O_WRONLY|O_APPEND);
        	if(fd2<0){
        		fd2 = creat(pipecomms[pipeno].outfilename, 0644);
        	}
        	dup2(fd2, STDOUT_FILENO);
        	close(fd2); 
        	pipeflag = 1;  			
			}
			dup2(exfd,0);
			if(pipeno != (pipe_ind-1) && pipeflag==0)
			{
				dup2(fdp[1],1);
			}
			close(fdp[0]);
			if(execvp(pipecomms[pipeno].args[0],pipecomms[pipeno].args)!=-1)perror("Shell");
		}
		else if (pid!=0 && pipecomms[pipeno].bg!=1){
			setpgid(pid,pid);
			signal(SIGTTOU,SIG_IGN);
			signal(SIGTTIN,SIG_IGN);
			int ex = tcsetpgrp(STDIN_FILENO,pid);
			if(ex==-1)perror("Shell");
			waitpid(pid,&stats,WUNTRACED);
			if(WIFSTOPPED(stats))addnode(pid,pipecomms[pipeno].args[0]);
			ex = tcsetpgrp(STDIN_FILENO,getpgrp());
			signal(SIGTTOU,SIG_DFL);
			signal(SIGTTIN,SIG_DFL);
			close(fdp[1]);
			exfd = fdp[0];
		}
		pipeno++;
	}
}
void findjobs()
{
	struct node *excurr = head;
	char path[1024],st,line[1024];
	int id;
	int ind=1;
	while(excurr!=NULL)
	{
		FILE *fp1;
		sprintf(path,"/proc/%d/stat",excurr->id);
		fp1 = fopen(path,"r");
		fscanf(fp1,"%d %s %c",&id,line,&st);
		if(st=='R')printf("[%d] Running %s [%d]\n",ind,curr->str,id);
		else printf("[%d] Stopped %s [%d]\n",ind,excurr->str,id);
		excurr= excurr->next;
		ind++;
	}	
}
void bgtofg(int x)
{
	curr = head;
	int ind=1;
	int flag=0;
	int stat;
	pid_t expid;
	struct node *excurr;
	while(curr!=NULL && ind<=x)
	{
		expid = curr->id;
		if(ind==x){
			flag=1;
			excurr = curr;
			int ex = tcsetpgrp(STDIN_FILENO,expid);
			kill(expid,SIGCONT);
			signal(SIGTTOU,SIG_IGN);
			signal(SIGTTIN,SIG_IGN);
			waitpid(expid,&stat,WUNTRACED);
			ex = tcsetpgrp(STDIN_FILENO,getpgrp());
			signal(SIGTTOU,SIG_DFL);
			signal(SIGTTIN,SIG_DFL);
			break;
		}
		curr = curr->next;
		ind++;
	}
	if(WIFEXITED(stat))delnode(excurr);
	if(flag==0)printf("Shell:No such process exists");
}
void fgtobg(int x)
{
	struct node *excurr = head;
	int ind=1;
	int flag=0;
	int stat;
	while(excurr!=NULL && ind<=x)
	{

		if(ind==x){
			flag=1;
			kill(excurr->id,SIGCONT);
			printf("%d",excurr->id);
			break;
		}
		excurr = excurr->next;
		ind++;
	}
	if(flag==0)printf("Shell:No such process exists");
}
void killproc(int x,int sig)
{
	struct node *excurr = head;
	int ind=1;
	int flag=0;
	char killcomm[100];
	while(excurr!=NULL && ind<=x)
	{
		if(ind==x){
			flag=1;
			sprintf(killcomm,"kill -%d %d",sig,excurr->id);
			proc("kill",killcomm);
		}
		excurr = excurr->next;
		ind++;
	}
	if(flag==0)printf("Shell:No such process exists");
}
void killbgs()
{
	char comm[100];
	char base[] = "kill";
	struct node *excurr = head;
	while(excurr!=NULL)
	{
		sprintf(comm,"kill -9 %d",excurr->id);
		proc("kill",comm);
		excurr = excurr->next;
	}
}
#include "headers.h"
#include "loop.h"
void trimspace(char *str){
	int ind=0;
	while(str[ind]==' ' || str[ind]=='\t')ind++;
	if(ind!=0)
	{
        int i=0;
        while(str[i+ind] != '\0')
        {
            str[i] = str[i+ind];
            i++;
        }
        str[i] = '\0'; 
	}
	ind = strlen(str)-1;
	while(str[ind]==' ' || str[ind]=='\t')ind--;
	str[ind+1]='\0';
}
void stringbreak(char *comm1,char *a1,char *a2)
{
	int i;
	int size = strlen(comm1);
	int flag = 0;
	for(i=0;i<size;i++)
	{
		if(comm1[i] == ' ' || comm1[i] == '\t'){
			int j;
			for(j=0;j<i;j++)a1[j] = comm1[j];
			flag = 1;
			a1[j] = '\0';
			break;
		}
	}
	int k;
	if(flag == 0){
		for(k=0;k<size;k++)a1[k] = comm1[k];
		a1[k] = '\0';
	}
	while(comm1[i]==' ' || comm1[i]=='\t')i++;
	for(k=i;k<size;k++){
		a2[k-i] = comm1[k];
	}
	a2[k-i] = '\0';
}
int strtoint(char *str)
{
	int sz = strlen(str),ans=0;
	for(int i=0;i<sz;i++)
	{
		ans = (10*ans) + (str[i]-'0');
	}
	return ans;
}
void wloop(){
	char *user;
	user = getlogin();
	struct utsname sys_inf;
	int sys=uname(&sys_inf);
	char ussys[10000];
	char comm[1025];
	char pd[1025];
	char home[1025];
	getcwd(home,1025);
	int homeflag,homelen=strlen(home),dirind=0;
	char exdir[2][1024];
	while(1)
	{
		getcwd(pd,1025);
		char pdx[1025];
		int pdlen = strlen(pd);
		if(pdlen>=homelen)
		{
			homeflag=1;
			for(int i=0;i<homelen;i++)
			{
				if(pd[i]!=home[i]){
					homeflag = 0;
					break;
				}
			}
			if(homeflag){
				pdx[0]='~';
				int index;
				for(index=homelen;index<pdlen;index++)pdx[(index+1)-homelen] = pd[index];
				pdx[(index+1)-homelen] = '\0';
			}
			else strcpy(pdx,pd);
		}
		else strcpy(pdx,pd);		
		sprintf(ussys, "%s@%s:%s$", user,sys_inf.nodename, pdx);
		printf("%s", ussys);
		if(fgets(comm,1025,stdin)==NULL){
			printf("\nBye\n");
			exit(0);
		}
		comm[strlen(comm)-1] = '\0';
		char *tok = strtok(comm,";");
		char command[1025],arg1[1025],arg2[1025];
		char carray[11][1025],forpip[1024];
		int uls = 0;
		while(tok!=NULL)
		{
			strcpy(carray[uls],tok);
			tok = strtok(NULL, ";");
			uls++;
		}
		int cmind =0; 
		while(uls!=cmind)
		{
			strcpy(forpip,carray[cmind]);
			char *tokpip = strtok(forpip,">|<");
			int piping = 0;
			while(tokpip!=NULL)
			{
				tokpip = strtok(NULL,">|<");
				piping++;
			}
			if(piping ==1)piping=0;
			else piping=1;
			getcwd(pd,1025);
			strcpy(command,carray[cmind]);
			trimspace(command);
			stringbreak(command,arg1,arg2);
			if(!strcmp(arg1,"cd")){
				trimspace(arg2);
				strcpy(exdir[dirind],pd);
				if(!strcmp(arg2,"-"))
				{	
					execcd(exdir[1-dirind],home);
				}
				else execcd(arg2,home);
				dirind++;
				dirind=dirind%2;	
			}
			else if(!strcmp(arg1,"echo")){
				trimspace(arg2);
				echoc(arg2);
			}
			else if(!strcmp(arg1,"pwd")){
				printf("%s\n", pdx);
			}
			else if(!strcmp(arg1,"ls") && piping==0){
				trimspace(arg2);
				list(arg2,home);
			}
			else if(!strcmp(arg1,"pinfo")){
				trimspace(arg2);
				int argval=0;
				for(int i=0;i<strlen(arg2);i++)argval = argval*10 + (arg2[i]-'0');
				printinfo(argval);
			}
			else if(!strcmp(arg1,"setenv"))
			{
				trimspace(arg2);
				char *tokeniser = strtok(arg2," \t"),*setargs[2];
				int setind=0,setflag=0;
				while(tokeniser!=NULL)
				{
					if(setind>1){
						setflag=1;
						break;
					}
					setargs[setind++] = tokeniser;
					tokeniser = strtok(NULL," \t");
				}
				if(!setflag){
					if(setenv(setargs[0],setargs[1],1)==-1)perror("Shell");
				}
				else printf("Shell:Incorrect number of arguments\n");
			}
			else if(!strcmp(arg1,"unsetenv"))
			{
				trimspace(arg2);
				char *tokeniseru = strtok(arg2," \t"),*unsetarg;
				int unsetind=0,unsetflag=0;
				while(tokeniseru!=NULL)
				{
					if(unsetind>0){
						unsetflag=1;
						break;
					}
					unsetarg = tokeniseru;
					tokeniseru = strtok(NULL," \t");
					unsetind++;
				}
				if(!unsetflag){
					if(unsetenv(unsetarg)==-1)perror("Shell");
				}
				else printf("Shell:Incorrect number of arguments\n");
			}
			else if(!strcmp(arg1,"jobs"))
			{
				findjobs();
			}
			else if(!strcmp(arg1,"kjobs"))
			{
				char *killtok = strtok(arg2," \t");
				char *killarg[2],killcomm[100];
				int kind=0;
				while(killtok != NULL)
				{
					killarg[kind++] = killtok;
					killtok = strtok(NULL," \t");
				}
				killproc(strtoint(killarg[0]),strtoint(killarg[1]));
			}
			else if(!strcmp(arg1,"overkill"))
			{
				killbgs();
			}
			else if(!strcmp(arg1,"fg"))
			{
				trimspace(arg2);
				bgtofg(strtoint(arg2));
			
			}
			else if(!strcmp(arg1,"bg"))
			{
				trimspace(arg2);
				fgtobg(strtoint(arg2));
			}
			else{
				trimspace(command);
				trimspace(arg1);
				proc(arg1,command);
			}
			cmind++;
		}
	}
}
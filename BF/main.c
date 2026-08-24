#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#define TAPE_LEN 30000
int main(int argc,char* argv[]){
	if (argc==1) exit(1);
	FILE* f = fopen(argv[1],"r");
	fseek(f,0,SEEK_END);
	unsigned long filelen = ftell(f);	
	if (filelen < 2) exit(1);
	rewind(f);
	unsigned char program[filelen];
	fread(program,1,filelen,f);
	unsigned char dataarr[TAPE_LEN];
	memset(dataarr,0,TAPE_LEN);
	unsigned long dataptr = 0;
	signed long instrptr = 0;
	unsigned long loopnum = 0;
	unsigned long ignbrck = 0; 
	signed long looppos[1024];
	char prv = '\n';
	char c = '\n';
	while(instrptr < filelen){	
		switch(program[instrptr]){
			case '>':
				dataptr++;
				instrptr++;
			break;
			case '<':
				if(dataptr!=0) dataptr--;
				instrptr++;
			break;
			case '+':
				dataarr[dataptr]++;
				instrptr++;
			break;
			case '-':
				dataarr[dataptr]--;
				instrptr++;
			break;
			case '.':
				//printf("-%d-\n",dataarr[dataptr]);
				putchar(dataarr[dataptr]);
				instrptr++;
			break;
			case '[':
				ignbrck = 0;
				if(dataarr[dataptr]==0){
					while(1){
						instrptr++;
						if(program[instrptr]=='[') {
							ignbrck++;
						}else if(program[instrptr]==']'&&ignbrck==0){
							instrptr++;
							break;
						}else if(program[instrptr]==']'&&ignbrck!=0){
							ignbrck--;
						}
					}	
				}else{
					loopnum++;
					looppos[loopnum-1] = instrptr;
					instrptr++;	
				}
			break;
			case ',':
				do{
				prv = c;
				c = getchar();
				}while((c == '\n' || c == '\r') && !(prv == '\r' || prv == '\n'));
				dataarr[dataptr] = c;
				instrptr++;
			break;
			case ']':
				if(loopnum == 0){
					printf("excess ']' character");
					exit(1);
				}
				if(dataarr[dataptr]==0){
					instrptr++;
					loopnum--;
				}else{
					instrptr=looppos[loopnum-1];
					instrptr++;
				}
			break;
			default:
				instrptr++;
			break;
			
			if(instrptr == filelen && loopnum != 0){
				printf("excess '[' character");
				exit(1);
			}
		}
	}
	return 0;
}

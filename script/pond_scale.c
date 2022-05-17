#include <stdio.h>
#include <argp.h>
#include "data.h"
#include "stream.h"
#include "t.h"

int main(int argc,const char **argv){
	argv_pra pra[5];
	int aip=-1,aipp=-1,aio=-1,aiop=-1,asw=-1,aswp=-1,ahpp=-1;
        pra[0].position=&aipp;pra[0].value_position=&aip;memcpy(pra[0].item_name,"-input1",ARGV_STR_MAX);
        pra[1].position=&aiop;pra[1].value_position=&aio;memcpy(pra[1].item_name,"-input2",ARGV_STR_MAX);
        pra[2].position=&aswp;pra[2].value_position=&asw;memcpy(pra[2].item_name,"-width",ARGV_STR_MAX);
        pra[3].position=&ahpp;pra[3].value_position=NULL;memcpy(pra[3].item_name,"-help",ARGV_STR_MAX);
	pra[4].position=&ahpp;pra[4].value_position=NULL;memcpy(pra[4].item_name,"h",ARGV_STR_MAX);
	int are=getArgvValue(pra,4,0,NULL,0,argc,argv);
        if(are<0){
                printf("unkown item: %s\n",argv[-1*are]);
                ahpp=0;
        }else if(are){
                printf("-%s requires value\n",pra[are-1].item_name);//note
                ahpp=0;
        }
        if(aip<0 || asw<0 || ahpp>=0){
                fprintf(stderr,"pond v0.2\n");
                fprintf(stderr,"  --input1    file    matrix of number\n");
                fprintf(stderr,"  --input2    file    matrix of number\n");
                fprintf(stderr,"  --width     int     the size of window\n");
                fprintf(stderr,"  -h/--help           show this message and exit\n");
                exit(0);
        }
	int X0=0,Y0=0,X1=0,Y1=0;
	float **matrix0=NULL,**matrix1=NULL;
	if(getDataFromFile(argv[aip],&matrix0,&X0,&Y0) || getDataFromFile(argv[aio],&matrix1,&X1,&Y1) || X0!=X1)exit(-1);
	int width=atoi(argv[asw]);
	int i=0;
	for(i=0;i<Y0;i++){
		scale(matrix0[i],X0);
	}
	for(i=0;i<Y1;i++){
		scale(matrix1[i],X1);
	}
	stream(matrix0,X0,Y0,matrix1,X1,Y1,width);
	clearMatrix(&matrix0,&X0,&Y0);
	clearMatrix(&matrix1,&X1,&Y1);
	return 0;
}

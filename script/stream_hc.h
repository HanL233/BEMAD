#include <string.h>
#include <math.h>
#include "t.h"

#define MAX_DISTANCE 100.0f
#define MIN_RATIO 0.5f
#define MIN_PATTERN 100
#define MAX_PATTERN 200
#define GAP 8
#define  MAX_ROUND 50000

typedef struct sPOND{
	int *pos;
	int num;
	float center;
	float *scaled_wave;
	float score;
	struct sPOND *previous;
	struct sPOND *next;
}POND;

POND *getHere(POND *head,POND *tail,POND *here,float center){//TODO TEST
        if(here==NULL)here=head;
        while(here && here->center<center)here=here->next;
        while(here && here->center>center)here=here->previous;
        if(here==NULL)here=tail;
        return here;
}

POND *insertPOND(POND **head,POND **tail,POND *here,POND *pond){
        here=getHere(*head,*tail,here,pond->center);
        if(here){
                pond->next=here->next;
                if(here->next){
                        here->next->previous=pond;
                }else{
                        *tail=pond;
                }
                here->next=pond;
                pond->previous=here;
        }else{
                pond->next=*head;
                if(*head){
                        (*head)->previous=pond;
                }else{
                        *tail=pond;
                }
                pond->previous=NULL;
                *head=pond;
        }
        return pond;
}

void freePOND(POND *here){
	free(here->pos);
	free(here->scaled_wave);
	free(here);
}

void deletePOND(POND **head,POND **tail,POND *here){
        if(*head!=*tail){
                if(here->previous){
                        here->previous->next=here->next;
                }else{
                        *head=(*head)->next;
                        if(*head)(*head)->previous=NULL;
                }
                if(here->next){
                        here->next->previous=here->previous;
                }else{
                        *tail=(*tail)->previous;
                        if(*tail)(*tail)->next=NULL;
                }
        }else{
                *head=NULL;
                *tail=NULL;
        }
	freePOND(here);
}

void clearPOND(POND **head,POND **tail){
        *tail=NULL;
        POND *tmp=NULL;
        while(*head){
                *head=(tmp=*head)->next;
                free(tmp->pos);
                free(tmp->scaled_wave);
                free(tmp);
        }
}

void showSinglePOND(POND *here,float *wave,float *wind){
        int i=0;
        printf("---\nscore=%g,center=%g,num=%d\nposition=",here->score,here->center,here->num);
        for(i=0;i<here->num;i++){
                if(i)printf(",");
                printf("%d",(here->pos)[i]);
        }
        printf("\nwave=");
        for(i=0;i<here->num;i++){
                if(i)printf(",");
                printf("%g",wave[(here->pos)[i]]);
        }
        printf("\nscaled_wave=");
        for(i=0;i<here->num;i++){
                if(i)printf(",");
                printf("%g",(here->scaled_wave)[i]);
        }
        printf("\nwind=");
        for(i=0;i<here->num;i++){
                if(i)printf(",");
                printf("%g",wind[(here->pos)[i]]);
        }
	printf("\n\n");
}

void showMiniSinglePOND(POND *here,float *wave,float *wind){
        fprintf(stderr,"score=%g,center=%g,num=%d\n\n",here->score,here->center,here->num);
}

void showPOND(POND *head,float *wave,float *wind){
	int i=0;
        while(head){
		printf("--- %d ---\n",i++);
                showSinglePOND(head,wave,wind);
                head=head->next;
        }
}

void showMiniPOND(POND *head,float *wave,float *wind){
	int i=0;
        while(head){
		fprintf(stderr,"--- %d ---\n",i++);
                showMiniSinglePOND(head,wave,wind);
                head=head->next;
        }
}

float *getDiff(float **M,int X,int Y,int *pos,int len,float *scaled_wave,float *wind){
	int i=0,j=0;
	float *diff=(float*)calloc(Y,sizeof(float));
	float *d=(float*)calloc(len,sizeof(float));
	for(i=0;i<Y;i++){
		for(j=0;j<len;j++){
			d[j]=M[i][pos[j]];
		}
		scale(d,len);
		diff[i]=0;
		for(j=0;j<len;j++){
			diff[i]+=(d[j]-scaled_wave[j])*wind[pos[j]]*wind[pos[j]];//TODO confirm
		}
		diff[i]=fabs(diff[i]);
	}
	free(d);
	return diff;
}

float *_getDiff(float **M,int X,int Y,int *pos,int len,float *scaled_wave,float *wind){
	int i=0,j=0;
	float *diff=(float*)calloc(Y,sizeof(float));
	float *d=(float*)calloc(len,sizeof(float));
	fprintf(stderr,"\nwind");
	for(j=0;j<len;j++){
		fprintf(stderr,",%f",wind[pos[j]]);
	}
	fprintf(stderr,"\n");
	for(i=0;i<Y;i++){
		fprintf(stderr,"\ndata%d",i);
		for(j=0;j<len;j++){
			d[j]=M[i][pos[j]];
			fprintf(stderr,",%f",d[j]);
		}
		fprintf(stderr,"\nscaled_data%d",i);
		scale(d,len);
		for(j=0;j<len;j++){
			fprintf(stderr,",%f",d[j]);
		}
		fprintf(stderr,"\n");
		diff[i]=0;
		for(j=0;j<len;j++){
			diff[i]+=(d[j]-scaled_wave[j])*wind[pos[j]]*wind[pos[j]];//TODO confirm
		}
		diff[i]=fabs(diff[i]);
	}
	free(d);
	return diff;
}

float getScore(float **MA,int XA,int YA,float **MB,int XB,int YB,int *pos,int len,float *scaled_wave,float *wind){
	float *DA=getDiff(MA,XA,YA,pos,len,scaled_wave,wind);
	float *DB=getDiff(MB,XB,YB,pos,len,scaled_wave,wind);
	float t=tValue(DA,YA,DB,YB);
	/*if(t!=t){
	int i=0;
	fprintf(stderr,"A");
	for(i=0;i<YA;i++){
		fprintf(stderr,",%f",DA[i]);
	}
	fprintf(stderr,"\nB");
	for(i=0;i<YB;i++){
		fprintf(stderr,",%f",DB[i]);
	}
	fprintf(stderr,"\nwave");
	for(i=0;i<len;i++){
		fprintf(stderr,",%f",scaled_wave[i]);
	}
	fprintf(stderr,"\nPOS");
	for(i=0;i<len;i++){
		fprintf(stderr,",%d",pos[i]);
	}
	fprintf(stderr,"\n");
	_getDiff(MA,XA,YA,pos,len,scaled_wave,wind);
	_getDiff(MB,XB,YB,pos,len,scaled_wave,wind);
	exit(0);
	}*/
	//printf("t=%g\n",t);
	free(DA);
	free(DB);
	return t;
}

int isTheSame(POND *PA,POND *PB){
	if(PA->center!=PB->center || PA->score!=PB->score || PA->num!=PB->num)return 0;
	int i=0;
	for(i=0;i<PA->num;i++){
		if((PA->pos)[i]!=(PB->pos)[i])return 0;
	}
	return 1;
}

POND *mergePOND(float **MA,int XA,int YA,float **MB,int XB,int YB,float *wave,float *wind,POND *PA,POND *PB){
	if(isTheSame(PA,PB)){
		return NULL;
	}
	POND *pond=(POND*)malloc(sizeof(POND));

	//merge pos/wave/wind from PA&PB by removing overlap pos nodes
	int pa=0,pb=0,p=0,i=0;
	int num=PA->num+PB->num;
	int *pos=(int*)malloc(num*sizeof(int));
	float *scaled_wave=(float*)malloc(num*sizeof(float));
	float center=0.0f;
	while(pa<PA->num || pb<PB->num){
		//pos in PA&PB is always sorted increasely
		if(pa>=PA->num){
			pos[p]=(PB->pos)[pb];
			scaled_wave[p]=wave[(PB->pos)[pb]];
			p++;
			pb++;
		}else if(pb>=PB->num){
			pos[p]=(PA->pos)[pa];
			scaled_wave[p]=wave[(PA->pos)[pa]];
			p++;
			pa++;
		}else{
			if((PA->pos)[pa]>(PB->pos)[pb]){
				pos[p]=(PB->pos)[pb];
				scaled_wave[p]=wave[(PB->pos)[pb]];
				p++;
				pb++;
			}else if((PA->pos)[pa]<(PB->pos)[pb]){
				pos[p]=(PA->pos)[pa];
				scaled_wave[p]=wave[(PA->pos)[pa]];
				p++;
				pa++;
			}else{
				pos[p]=(PA->pos)[pa];
				scaled_wave[p]=wave[(PA->pos)[pa]];
				p++;
				pa++;
				pb++;
			}
		}
	}
	for(i=0;i<p;i++){
		center+=(float)pos[i];
	}
	center/=(float)p;
	scale(scaled_wave,p);
	pond->num=p;
	pond->pos=pos;
	pond->center=center;
	pond->scaled_wave=scaled_wave;
	pond->previous=NULL;
	pond->next=NULL;
	pond->score=getScore(MA,XA,YA,MB,XB,YB,pos,p,scaled_wave,wind);//Notice the scalaed wave

	return pond;
}

void getWaveAndWind(float **MA,int XA,int YA,float **MB,int XB,int YB,float **_wave,float **_wind){
	float *wave=(float*)malloc(XA*sizeof(float));
	float *wind=(float*)malloc(XA*sizeof(float));
	int i=0,j=0;
	float s=0.0f,m=0.0f,v=0.0f,t=0.0f;
	for(i=0;i<XA;i++){
		s=0.0f;
		for(j=0;j<YA;j++){
			s+=MA[j][i];
		}
		for(j=0;j<YB;j++){
			s+=MB[j][i];
		}
		m=s/((float)(YA+YB));
		v=0.0f;
		for(j=0;j<YA;j++){
			t=MA[j][i]-m;
			//printf("%g\n",MA[j][i]);
			v+=t*t;
		}
		//printf("----\n");
		for(j=0;j<YB;j++){
			t=MB[j][i]-m;
			//printf("%g\n",MB[j][i]);
			v+=t*t;
		}
		v/=((float)(YA+YB-1));
		wave[i]=m;
		wind[i]=v;//TODO check
		//exit(0);
	}
	if(*_wave)free(*_wave);
	if(*_wind)free(*_wind);
	*_wave=wave;
	*_wind=wind;
}

POND *createPOND(float **MA,int XA,int YA,float **MB,int XB,int YB,float *wave,float *wind,int from,int to,int from2,int to2){
	//NOTICE make sure to is no larger than XA&XB
	int i=0;
	float *scaled_wave=(float*)malloc((to+to2-from-from2)*sizeof(float));
	memcpy(scaled_wave,wave+from,(to-from)*sizeof(float));
	memcpy(scaled_wave+(to-from),wave+from2,(to2-from2)*sizeof(float));
	/*for(i=0;i<to-from;i++){
		printf("%g,",wave[i+from]);
	}
	printf("\n");
	for(i=0;i<to-from;i++){
		printf("%g,",scaled_wave[i]);
	}
	printf("\n");*/
	scale(scaled_wave,to+to2-from-from2);//TODO check the var
	/*for(i=0;i<to-from;i++){
		printf("%g,",scaled_wave[i]);
	}
	printf("\n");*/
	int *pos=(int*)malloc((to+to2-from-from2)*sizeof(int));
	for(i=from;i<to;i++){
		pos[i-from]=i;
	}
	for(i=from2;i<to2;i++){
		pos[i+to-from-from2]=i;
	}
	//printf("get\n");
	float score=getScore(MA,XA,YA,MB,XB,YB,pos,to+to2-from-from2,scaled_wave,wind);
	POND *pond=(POND*)malloc(sizeof(POND));
	pond->pos=pos;
	pond->num=to+to2-from-from2;
	pond->center=((float)(to+to2+from+from2))/2.0f;
	pond->scaled_wave=scaled_wave;
	pond->score=score;
	pond->previous=NULL;
	pond->next=NULL;
	return pond;
}

int compareFloat(const void *_a,const void *_b){
	float a=*((const float*)_a),b=*((const float*)_b);
	return a>b?-1:(a<b?1:0);
}

float filterPOND(POND **head,POND **tail,float ratio){
	POND *here=NULL,*tmp=*head;
	int i=0,num=0;
	tmp=*head;
	while(tmp){
		num++;
		tmp=tmp->next;
	}
	float *score=(float*)malloc(num*sizeof(float));
	tmp=*head;
	while(tmp){
		score[i++]=fabs(tmp->score);
		tmp=tmp->next;
	}
	/*for(i=0;i<num;i++){
		printf("%g\n",score[i]);
	}
	printf("------\n");*/
	qsort(score,num,sizeof(float),compareFloat);//TODO check ordered in/decreasely
	/*for(i=0;i<num;i++){
		printf("%g\n",score[i]);
	}
	printf("------\n");*/
	int _cut=(int)(((float)num)*ratio);
	if(_cut>MAX_PATTERN)_cut=MAX_PATTERN;
	float cutoff=score[_cut];
	tmp=*head;
	while(tmp){
		tmp=(here=tmp)->next;
		if(fabs(here->score)<cutoff){
			deletePOND(head,tail,here);
		}
	}
	/*for(i=0;i<num;i++){
		printf("[%d] %g%s\n",i,score[i],(fabs(score[i])>=cutoff?" <--":""));
	}
	exit(0);*/
	free(score);
	return cutoff;
}

float transScore(float score){
	return fabs(score*score);
}

float transPos(int distance){
	float dis=fabs(distance);
	return (1.0f/(sqrt(dis)+1.0f));
}

float frand(){
	static float maxRand=(float)RAND_MAX;
	return (float)(rand())/maxRand;
}

int stream(float **MA,int XA,int YA,float **MB,int XB,int YB,int width){
	printf("XA=%d,YA=%d,XB=%d,YB=%d,width=%d\n",XA,YA,XB,YB,width);
	POND *head=NULL,*tail=NULL,*here=NULL,*tmp=NULL;
	float *wave=NULL,*wind=NULL;
	getWaveAndWind(MA,XA,YA,MB,XB,YB,&wave,&wind);
	int i=0,j=0,num=0;
	/*for(i=0;i<10;i++){
		printf("%g,",wave[i]);
	}
	printf("\n");
	for(i=0;i<10;i++){
		printf("%g,",wind[i]);
	}
	printf("\n");*/
	float maxScore=0.0f,sumScore=0.0f,ss=0.0f,_ss=0.0f;
	for(i=0;i<XA-(width+width);i+=width){//NOTICE -width
	//for(i=0;i<width*20;i+=width){
		for(j=0;j<GAP;j++){
			if(i+width*(1<<j)+width<XA){
				POND *pond=createPOND(MA,XA,YA,MB,XB,YB,wave,wind,i,i+width,i+width*(1<<j),i+width*(1<<j)+width);
				if(fabs(pond->score)>0.0f){
					if(maxScore<fabs(pond->score)){
						maxScore=fabs(pond->score);
					}
					sumScore+=transScore(pond->score);//fabs(pond->score);Notice: to amplificate the score
					here=insertPOND(&head,&tail,here,pond);
					num++;
					//fprintf(stderr,"create: %f\n",maxScore);
				}else{
					freePOND(pond);
				}
			}
		}
	}
	int n=0,ava=0;
	float goal=0.0f;
	POND *p0=NULL,*p1=NULL;
	while(n++<MAX_ROUND){
		goal=sumScore*frand();
		ss=0.0f;
		p0=head;
		while(p0 && p0->next && (ss+=transScore(p0->score))<goal){
			p0=p0->next;
		}
		tmp=head;
		_ss=0.0f;
		while(tmp){
			if(tmp!=p0){
				_ss+=transScore(tmp->score)*transPos(p0->center-tmp->center);
			}
			tmp=tmp->next;
		}
		goal=_ss*frand();
		ss=0.0f;
		p1=head;
		while(p1 && p1->next){
			if(p1!=p0 && (ss+=transScore(p1->score)*transPos(p0->center-p1->center))>=goal){
				break;
			}
			p1=p1->next;
		}
		POND *pond=mergePOND(MA,XA,YA,MB,XB,YB,wave,wind,p0,p1);
		if(pond->score){
			ava=1;
			tmp=head;
			while(tmp){
				if(isTheSame(tmp,pond)){
					ava=0;
					break;
				}
				tmp=tmp->next;
			}
		}else{
			ava=0;
		}
		if(ava){
			sumScore+=transScore(pond->score);
			insertPOND(&head,&tail,p0,pond);
			if(maxScore<fabs(pond->score)){
				maxScore=fabs(pond->score);
			}
			//fprintf(stderr,"------\n");
			//showMiniSinglePOND(p0,wave,wind);
			//showMiniSinglePOND(p1,wave,wind);
			//showMiniSinglePOND(pond,wave,wind);
			//fprintf(stderr,"%d %f\n",n,maxScore);
		}else{
			freePOND(pond);
		}
	}
	showPOND(head,wave,wind);
	free(wave);
	free(wind);
	clearPOND(&head,&tail);
	return 0;
}

#include "SB-decoder.h"

/*====================================================================================*/
int next_event(int **bp, int *end, counters *c, int* e)
{
  if((c->event%10000)==0)
    printf("Number of analyzed events if %10ld\r",c->event);
  if(**bp!=(int)EVENT_TAG)
    return -1;
 
  if((*bp+*(*bp+1))<=end)
    {
      memset(e,0,BUFFSIZE*sizeof(int));
      memcpy(e,*bp,(*(*bp+1)*sizeof(int)));
      *bp+=*(*bp+1);
      c->event++;
      return 1;
    }
 
    return -1;
}
/*====================================================================================*/
int process_event(int* e, raw_event* data)
{
  int *ep,*end;
  int s,sh,ss,sc,snf,sf,sd;
  int n,m,id;
 
  sh=(int)(sizeof(rheader)/sizeof(int));
  ss=(int)(sizeof(sheader)/sizeof(int));
  sc=(int)(sizeof(channel)/sizeof(int));
  snf=(int)(sizeof(nimfit)/sizeof(int));
  sf=(int)(sizeof(float)/sizeof(int));
  sd=(int)(sizeof(double)/sizeof(int));
  
  ep=&e[0];
  if(*ep!=(int)EVENT_TAG)
    {
      printf("Wrong event tag  0x%8.8x\n",*ep);
      printf("Terminating sort\n");
      exit(-1);
    }
  ep++;
  end=&e[*ep];
  ep++;
  while(ep<end)
    {
      switch(*ep)
	{
	case (int)HEADER_TAG:
	  ep++; s=*ep-2;ep++;
	  memcpy(&data->h,ep,sh*sizeof(int));
	  ep+=s;
	  break;

 	case (int)SBT_TAG:
	  ep++; s=*ep-2;ep++;	 	  
	  memcpy(&data->sbt.h,ep,ss*sizeof(int));
	  ep+=ss;
	  n=(s-ss)/(sc+snf+3);
	  
	  for(m=0;m<n;m++)
	    {
	      if(*ep!=(int)CHANNEL_TAG)
		{
		  printf("Sandobox TRIG decoding\n");
		  printf("Wrong channel tag  0x%8.8x\n",*ep);
		  printf("Terminating sort\n");
		  exit(-1);
		}
	      ep++;
	      id=*ep;
	      ep++;
	      memcpy(&data->sbt.trig[id],ep,sc*sizeof(int));
	      ep+=sc;
	      if(*ep!=(int)SBT_FIT_TAG)
		{
		  printf("Sandobox TRIG decoding\n");
		  printf("Wrong fit tag  0x%8.8x\n",*ep);
		  printf("Terminating sort\n");
		  exit(-1);
		}
	      ep++;
	      memcpy(&data->sbt.fit[id],ep,snf*sizeof(int));
	      ep+=snf;
	    }
	  break;
	  
	case (int)SBC_TAG:
	  ep++; s=*ep-2;ep++;	 	  
	  memcpy(&data->sbc.h,ep,ss*sizeof(int));
	  ep+=ss;
	  n=(s-ss)/(sc+2);
	  
	  for(m=0;m<n;m++)
	    {
	      if(*ep!=(int)CHANNEL_TAG)
		{
		  printf("Sandobox HPGe decoding\n");
		  printf("Wrong channel tag  0x%8.8x\n",*ep);
		  printf("Terminating sort\n");
		  exit(-1);
		}
	      ep++;
	      id=*ep;
	      ep++;
	      memcpy(&data->sbc.hpge[id],ep,sc*sizeof(int));
	      ep+=sc;
	      memcpy(&data->sbc.t0[id],ep,sf*sizeof(int));
	      ep+=sf;
	    }
	  break;

	case (int)SBA_TAG:
	  ep++; s=*ep-2;ep++;	 	  
	  memcpy(&data->sba.h,ep,ss*sizeof(int));
	  ep+=ss;
	  n=(s-ss)/(sc+2);
	  
	  for(m=0;m<n;m++)
	    {
	      if(*ep!=(int)CHANNEL_TAG)
		{
		  printf("Sandobox Aux decoding\n");
		  printf("Wrong channel tag  0x%8.8x\n",*ep);
		  printf("Terminating sort\n");
		  exit(-1);
		}
	      ep++;
	      id=*ep;
	      ep++;
	      memcpy(&data->sba.aux[id],ep,sc*sizeof(int));
	      ep+=sc;
	      memcpy(&data->sba.t0[id],ep,sf*sizeof(int));	      
	      ep+=sf;
	      memcpy(&data->sba.q[id],ep,sd*sizeof(int));	      
	      ep+=sd;	      
	    }
	  break;
	  
	default:
	  printf("Wrong tag  0x%8.8x\n",*ep);
	  printf("Terminating sort\n");
	  exit(-1);
	  break;
	}
    }
  return 1;
}
/*====================================================================================*/
void print_data(int* d,int limit)
{
  int i,j;
  for(i=0;i<limit;i+=16)
    {
      printf(" %4.4d ",i);
      for(j=0;j<16;j++)
	if((i+j)<limit)
	  printf(" 0x%8.8x ",d[i+j]);
      printf("\n");
    }
}
/*====================================================================================*/
void sort(input_names_type *name)
{
  FILE *input=NULL,*output=NULL;
  int state=END_OF_RECORD;
  int   stop;
  counters   *c;
  int         b[BUFFSIZE],e[BUFFSIZE],sb[BUFFSIZE];
  int         *bp,*end;
  raw_event   data;  
  
  if((input=fopen(name->fname.inp_data,"r"))==NULL)
    {
      printf("\nI can't open input file %s\n",name->fname.inp_data);
      exit(-2);
    }
   else
   printf("Sorting data from file %s\n",name->fname.inp_data);
  if(name->flag.separate==1)
    {
      if((output=fopen(name->fname.out_data,"w"))==NULL)
	{
	  printf("\nI can't open output file %s\n",name->fname.out_data);
	  exit(-2);
	}
      else
	{
	  printf("Separated data written to file %s\n",name->fname.out_data);
	  memset(sb,0,BUFFSIZE*sizeof(int));
	  sb[0]=BUFFER_TAG;
	  sb[1]++;
	  sb[1]++;
	}
    }
  /*initialize*/
  c=(counters *) malloc(sizeof(counters));
  memset(c,0,sizeof(counters));
  bp=NULL;
  end=NULL;

  /*do sorting */
  stop=0;
  while(stop==0){
    switch(state){
    case END_OF_RECORD:
      if( fread(&b,sizeof(int),BUFFSIZE,input) != BUFFSIZE ){state = END_OF_FILE;}
      else 
	{
	  bp=&b[0];
	  end=&(b[b[1]]);

	  if(*bp!=(int)BUFFER_TAG)
	    {
	      printf("Wrong buffer tag  0x%8.8x\n",*b);
	      printf("Terminating sort\n");
	      exit(-1);
	    }
	  c->buffer++;
	  bp+=2;
	  state = NEXT_EVENT;
	}
      break;
    case NEXT_EVENT:
      if( next_event(&bp,end,c,e) < 0 ){state = END_OF_RECORD;}
      else {state = PROCESSING_EVENT;}
      break;
    case PROCESSING_EVENT:
      memset(&data,0,sizeof(raw_event));
      if( process_event(e,&data) < 0 ){ state = NEXT_EVENT;}
      else { state = ANALYZE_DATA;  }
      break;
    case ANALYZE_DATA:
      if(analyze_data(&data)==SEPARATOR_KEEP)
	if(name->flag.separate==1)
	  {
	    if(e[1]<BUFFSIZE-2)
	      {
		if((sb[1]+e[1])<BUFFSIZE) /* separator buffer has space */
		  {
		    memcpy(&sb[sb[1]],&e[0],e[1]*sizeof(int));
		    sb[1]+=e[1];
		  }
		else /* no space, save current and start a new buffer */
		  {
		    fwrite(sb,sizeof(int),BUFFSIZE,output);
		    memset(sb,0,BUFFSIZE*sizeof(int));
		    sb[0]=BUFFER_TAG;
		    sb[1]++;
		    sb[1]++;
		    memcpy(&sb[sb[1]],&e[0],e[1]*sizeof(int));
		    sb[1]+=e[1];
		  }
	      }
	    else
	      {
		printf("Event dropped due to the length restriction\n");
		printf("Event length is 0x%8.8x\n",e[1]);
	      }
	  }
      state=NEXT_EVENT;
      break;
    case END_OF_FILE:
      printf("End of file %s\n",name->fname.inp_data);
      fclose(input);
      if(name->flag.separate==1)
	{
	  fwrite(sb,sizeof(int),BUFFSIZE,output);
	  fclose(output);
	}
      printf("Number of analyzed buffers is %8ld\n",c->buffer);
      printf("Number of analyzed events  is %8ld\n",c->event);
      stop=1;
      break;
    default:
      printf("Unknown case\n");
      exit(0);
      break;
    }
  }
  free(c);
}

/*====================================================================================*/

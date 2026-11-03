#include "SB-common.h"
#include "SB-format.h"
int verify_ts(raw_event *d,unsigned long long int *min, unsigned long long int *max )
{
  unsigned long long int one=1,ts_min=one<<63,ts_max=0,x,y;

  for(int i=0;i<MAX_DET_BITS;i++)
    switch(d->h.setupHP&(one<<i))
      {
      case SB_TBIT:
	verify_ts_trig(&d->sbt,&x,&y);
	if(x<ts_min) ts_min=x;
	if(y>ts_max) ts_max=y;	
	break;
     case SB_DBIT:
	verify_ts_hpge(&d->sbc,&x,&y);
	if(x<ts_min) ts_min=x;
	if(y>ts_max) ts_max=y;	
	break;
	
      default:
	break;
      }
  
  *min=ts_min;
  *max=ts_max;
  return (int)(*max-*min);
}
/*===============================================================*/
int verify_ts_trig(SBTRG*d,unsigned long long int *min, unsigned long long int *max )
{
  unsigned long long int ts;
  unsigned long long int one=1;
  int k;
  
  *min=(one<<63);
  *max=0;
  
  if(d->h.TSfold>0)
    for(k=0;k<NTRIG;k++)
      if((d->h.TSHP&(1<<k))!=0)  
	{
	  ts=((unsigned long long)d->trig[k].timestamp_up&0x00ffffff)<<24;
	  ts|=((unsigned long long)d->trig[k].timestamp&0x00ffffff);
	  if(ts>*max) *max=ts;
	  if(ts<*min) *min=ts;	  
	}
  
  return (int)(*max-*min);
}
/*===============================================================*/
int verify_ts_hpge(SBCSS *d,unsigned long long int *min, unsigned long long int *max )
{
  unsigned long long int ts;
  unsigned long long int one=1;
  int k;
  
  *min=(one<<63);
  *max=0;

  if(d->h.TSfold>0)
    for(k=0;k<NPOS;k++)
      if((d->h.TSHP&(1<<k))!=0)
	{
	  ts=((unsigned long long)d->hpge[k].timestamp_up&0x00ffffff)<<24;
	  ts|=((unsigned long long)d->hpge[k].timestamp&0x00ffffff);
	  if(ts>*max) *max=ts;
	  if(ts<*min) *min=ts;     
	}
  
  return (int)(*max-*min);
}
/*===============================================================*/
int verify_ts_aux(SBAUX *d,unsigned long long int *min, unsigned long long int *max )
{
  unsigned long long int ts;
  unsigned long long int one=1;
  int k;
  
  *min=(one<<63);
  *max=0;

  if(d->h.TSfold>0)
    for(k=0;k<NAUX;k++)
      if((d->h.TSHP&(1<<k))!=0)
	{
	  ts=((unsigned long long)d->aux[k].timestamp_up&0x00ffffff)<<24;
	  ts|=((unsigned long long)d->aux[k].timestamp&0x00ffffff);
	  if(ts>*max) *max=ts;
	  if(ts<*min) *min=ts;     
	}
  
  return (int)(*max-*min);
}
/*===============================================================*/
void display_raw_event(raw_event* d)
{
  int k;
  display_rheader(&d->h);
  printf("--- TRIGGER HIT INFO------------------------------------\n");
  display_sheader(&d->sbt.h);
  display_channel_legend();
  printf("\n");
  for(k=0;k<NTRIG;k++)
    if((d->sbt.h.TSHP&(1<<k))!=0)
      {
	display_channel(k,&d->sbt.trig[k]);
	printf("\n");
      }
  display_NIMFit_legend();
  for(k=0;k<NTRIG;k++)
    if((d->sbt.h.TSHP&(1<<k))!=0)
      {
	display_NIMFit(k,&d->sbt.fit[k]);
	printf("\n");
      }
  
  printf("---    HPGe HIT INFO------------------------------------\n");
  display_sheader(&d->sbc.h);
  display_channel_legend();
  printf("      T0\n");
  for(k=0;k<NPOS;k++)
    if((d->sbc.h.TSHP&(1<<k))!=0)
      {
	display_channel(k,&d->sbc.hpge[k]);
	printf(" %8.2f\n",d->sbc.t0[k]);
	printf("\n");
      }
  printf("---     AUX HIT INFO------------------------------------\n");
  display_sheader(&d->sba.h);
  display_channel_legend();
  printf("      T0        Q\n");
  for(k=0;k<NAUX;k++)
    if((d->sbc.h.TSHP&(1<<k))!=0)
      {
	display_channel(k,&d->sba.aux[k]);
	printf(" %8.2f",d->sba.t0[k]);
	printf(" %8.2f\n",d->sba.q[k]);
	printf("\n");
      }
}
/*===============================================================*/
void display_rheader(rheader* d)
{
  printf("================================================================\n");
  printf("Event number                      %8d\n",d->trig_num&0x7fffffff);
  printf("Fragment fold                     %8d\n",d->frag_fold);
  printf("Trigger bits                      %8d 0x%8.8x\n",d->setupHP,d->setupHP);
  printf("Time Stamp High Min               %8.8x\n",d->timestamp_up_min&0x00ffffff);
  printf("Time Stamp High Max               %8.8x\n",d->timestamp_up_max&0x00ffffff);
  printf("Time Stamp Low Min                %8.8x\n",d->timestamp_min&0x00ffffff);
  printf("Time Stamp Low Max                %8.8x\n",d->timestamp_max&0x00ffffff);  ;  
}
/*===============================================================*/
void display_channel(int i,channel* d)
{
  printf(" %4d     %8.8x  %8.8x  %8.8x  %8.8x ",i,d->timestamp_up&0x00ffffff,d->timestamp&0x00ffffff,d->cfd,d->charge);  
}
/*===============================================================*/
void display_channel_legend()
{
  printf("Channel    TS high   TS low     CFD      Charge");
}
/*===============================================================*/
void display_sheader(sheader* d)
{
  printf("Energy     fold                   %4.4x\n",d->Efold);
  printf("CFD        fold                   %4.4x\n",d->Tfold);
  printf("Time Stamp fold                   %4.4x\n",d->TSfold);  
  printf("Energy hit pattern                %4.4x\n",d->EHP);
  printf("CFD hit pattern                   %4.4x\n",d->THP);
  printf("Time Stamp hit pattern            %4.4x\n",d->TSHP);  
 
}
/*===============================================================*/
void display_NIMFit_legend()
{
  printf("Channel    T rise    T fall     Ampl.     Base.\n");
}
/*===============================================================*/
void display_NIMFit(int i,nimfit* d)
{
  printf(" %4d     %8d  %8d  %8.2f  %8.2f ",i,d->tr,d->tf,d->h,d->b);  
}

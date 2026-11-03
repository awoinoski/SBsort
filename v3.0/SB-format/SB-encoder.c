#include "SB-encoder.h"

/*================================================================*/
int encode(raw_event* data, FILE* encoded_output,int* enb)
{
  int e[BUFFSIZE],pck[BUFFSIZE];
  int k;
 
  if(data->h.trig_num!=0)
    { 
      memset(e,0,sizeof(e));
      e[0]=EVENT_TAG;
      e[1]++;
      e[1]++;
      /* pack the header */
      memset(pck,0,sizeof(pck));
      pck[0]=HEADER_TAG;
      pck[1]=2+sizeof(rheader)/sizeof(int);
      memcpy(&pck[2],&data->h,sizeof(rheader));
      //pack to the single event buffer
      memcpy(&e[e[1]],&pck[0],pck[1]*sizeof(int));
      e[1]+=pck[1];
      
      /* pack TRIGGER */
      if((data->h.setupHP&SB_TBIT)!=0)
	{	
	  //pack the header
	  memset(pck,0,sizeof(pck));
	  pck[0]=SBT_TAG;
	  pck[1]=2+sizeof(sheader)/sizeof(int);
	  memcpy(&pck[2],&data->sbt.h,sizeof(sheader));
	  //pack channels
	  for(k=0;k<NTRIG;k++)
	    if((data->sbt.h.TSHP&(1<<k))!=0)
	      {
		pck[pck[1]]=CHANNEL_TAG;
		pck[1]++;
		pck[pck[1]]=k;
		pck[1]++;	
		memcpy(&pck[pck[1]],&data->sbt.trig[k],sizeof(channel));
		pck[1]+=sizeof(channel)/sizeof(int);
		pck[pck[1]]=SBT_FIT_TAG;
		pck[1]++;
		memcpy(&pck[pck[1]],&data->sbt.fit[k],sizeof(nimfit));
		pck[1]+=sizeof(nimfit)/sizeof(int);
	      }
	  //pack to the single event buffer
	  memcpy(&e[e[1]],&pck[0],pck[1]*sizeof(int));
	  e[1]+=pck[1];
	}

       /* pack HPGe */
      if((data->h.setupHP&SB_DBIT)!=0)
	{	
	  //pack the header
	  memset(pck,0,sizeof(pck));
	  pck[0]=SBC_TAG;
	  pck[1]=2+sizeof(sheader)/sizeof(int);
	  memcpy(&pck[2],&data->sbc.h,sizeof(sheader));
	  //pack channels
	  for(k=0;k<NPOS;k++)
	    if((data->sbc.h.TSHP&(1<<k))!=0)
	      {
		pck[pck[1]]=CHANNEL_TAG;
		pck[1]++;
		pck[pck[1]]=k;
		pck[1]++;	
		memcpy(&pck[pck[1]],&data->sbc.hpge[k],sizeof(channel));
		pck[1]+=sizeof(channel)/sizeof(int);
		memcpy(&pck[pck[1]],&data->sbc.t0[k],sizeof(float));
		pck[1]+=sizeof(float)/sizeof(int);
	      }
	  //pack to the single event buffer
	  memcpy(&e[e[1]],&pck[0],pck[1]*sizeof(int));
	  e[1]+=pck[1];
	}

      /* pack AUX */
      if((data->h.setupHP&SB_ABIT)!=0)
	{	
	  //pack the header
	  memset(pck,0,sizeof(pck));
	  pck[0]=SBA_TAG;
	  pck[1]=2+sizeof(sheader)/sizeof(int);
	  memcpy(&pck[2],&data->sba.h,sizeof(sheader));
	  //pack channels
	  for(k=0;k<NAUX;k++)
	    if((data->sba.h.TSHP&(1<<k))!=0)
	      {
		pck[pck[1]]=CHANNEL_TAG;
		pck[1]++;
		pck[pck[1]]=k;
		pck[1]++;	
		memcpy(&pck[pck[1]],&data->sba.aux[k],sizeof(channel));
		pck[1]+=sizeof(channel)/sizeof(int);
		memcpy(&pck[pck[1]],&data->sba.t0[k],sizeof(float));
		pck[1]+=sizeof(float)/sizeof(int);
		memcpy(&pck[pck[1]],&data->sba.q[k],sizeof(double));
		pck[1]+=sizeof(double)/sizeof(int);
	      }
	  //pack to the single event buffer
	  memcpy(&e[e[1]],&pck[0],pck[1]*sizeof(int));
	  e[1]+=pck[1];
	}  
     
      //save the buffer if needed      
      if(e[1]<BUFFSIZE-2)
	{
	  if((enb[1]+e[1])<BUFFSIZE) /* buffer has space */
	    {
	      memcpy(&enb[enb[1]],&e[0],e[1]*sizeof(int));
	      enb[1]+=e[1];
	    }
	  else /* no space, save current and start a new buffer */
	    {
	      fwrite(enb,sizeof(int),BUFFSIZE,encoded_output);
	      memset(enb,0,BUFFSIZE*sizeof(int));
	      enb[0]=BUFFER_TAG;
	      enb[1]++;
	      enb[1]++;
	      memcpy(&enb[enb[1]],&e[0],e[1]*sizeof(int));
	      enb[1]+=e[1];
	    }
	}
      else
	{
	  printf("Event dropped due to the length restriction\n");
	  printf("Event length is 0x%8.8x\n",e[1]);
	}
    }
  
  return 0;
}

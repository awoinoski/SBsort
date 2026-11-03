#include "map.h"
/*======================================================================*/
void print_map(sbmap* map)
{
  int i;
  printf("=======    ==========================================\n");
  printf("TDAQMAP    SANDBOX\n");
  printf("=======    ==========================================\n");
  
  printf("MDGG      CHANNEL\n");
  printf("O1        %d\n",map->sb_trig_map[0]);
  printf("O2        %d\n",map->sb_trig_map[1]);
  printf("O3        %d\n",map->sb_trig_map[2]);
  printf("O4        %d\n",map->sb_trig_map[3]);
  
  if(map->sb_trig_fit==1)
    printf("FIT       YES\n");
  else
    printf("FIT        NO\n");
  printf("=======    ==========================================\n");
      
      

 if((map->sb_hpge_max-map->sb_hpge_min)>=0)
    {
      printf("HPGe      CHANNEL\n");
      printf("MIN        %d\n",map->sb_hpge_min);
      printf("MAX        %d\n",map->sb_hpge_max);
      printf("KPAR       %d\n",map->sb_hpge_kpar);
      if(map->sb_hpge_fit==1)
	printf("FIT       YES\n");
      else
	printf("FIT        NO\n");
      printf("=======    ==========================================\n");
      
      printf("HPGe       GROUP2POSITION\n");
      printf("Group      Position\n");
      for(i=0;i<NPOS;i++)
	if(map->sb_hpge_map[i]!=-1)
	  printf(" %2d         %2d\n",i,map->sb_hpge_map[i]);      
      printf("=======    ==========================================\n");
    }

 if((map->sb_aux_max-map->sb_aux_min)>=0)
    {
      printf("AUX      CHANNEL\n");
      printf("MIN        %d\n",map->sb_aux_min);
      printf("MAX        %d\n",map->sb_aux_max);
      printf("KPAR       %d\n",map->sb_aux_kpar);
      if(map->sb_aux_fit==1)
	printf("FIT       YES\n");
      else
	printf("FIT        NO\n");
      printf("=======    ==========================================\n");
      
      printf("AUX        GROUP2POSITION\n");
      printf("Group      Position\n");
      for(i=0;i<NAUX;i++)
	if(map->sb_aux_map[i]!=-1)
	  printf(" %2d         %2d\n",i,map->sb_aux_map[i]);      
      printf("=======    ==========================================\n");
    }
    
}
/*======================================================================*/
void read_map(char* name,sbmap* map)
{
  FILE *inp;
  char str1[132],str2[132];
  int take;
 
  
  if((inp=fopen(name,"r"))==NULL)
      {
         printf("\nFile %s can not be opened\n",name);
         exit(EXIT_FAILURE);
      }
  else
    printf("TDAQ map read from file %s\n",name);

  //initialize map to -1 to avoid mis-labeling channels
  memset(map,-1,sizeof(sbmap));

  while(fscanf(inp,"%s %s",str1,str2)!=EOF)
    {
      printf("===> %s %s\n",str1,str2);
     if(strcmp(str1,"MDGG")==0)
	if(strcmp(str2,"CHANNEL")==0)
	  {
	    if(fscanf(inp,"%s %s",str1,str2)!=EOF)
	      if(strcmp(str1,"O1")==0)
		map->sb_trig_map[0]=atoi(str2);

	    if(fscanf(inp,"%s %s",str1,str2)!=EOF)
	      if(strcmp(str1,"O2")==0)
		map->sb_trig_map[1]=atoi(str2);
	    
	    if(fscanf(inp,"%s %s",str1,str2)!=EOF)
	      if(strcmp(str1,"O3")==0)
		map->sb_trig_map[2]=atoi(str2);
	    
	    if(fscanf(inp,"%s %s",str1,str2)!=EOF)
	      if(strcmp(str1,"O4")==0)
		map->sb_trig_map[3]=atoi(str2);
	    
	    if(fscanf(inp,"%s %s",str1,str2)!=EOF)
	      if(strcmp(str1,"FIT")==0)
		if(strcmp(str2,"YES")==0)
		  map->sb_trig_fit=1;	     
	  }
     
     if(strcmp(str1,"HPGe")==0)
       if(strcmp(str2,"CHANNEL")==0)
	 {
	   if(fscanf(inp,"%s %s",str1,str2)!=EOF)
	     if(strcmp(str1,"MIN")==0)
	       map->sb_hpge_min=atoi(str2);
	   
	   if(fscanf(inp,"%s %s",str1,str2)!=EOF)
	     if(strcmp(str1,"MAX")==0)
	       map->sb_hpge_max=atoi(str2);
	   
	   if(fscanf(inp,"%s %s",str1,str2)!=EOF)
	     if(strcmp(str1,"KPAR")==0)
	       map->sb_hpge_kpar=atoi(str2);   
	   
	   if(fscanf(inp,"%s %s",str1,str2)!=EOF)
	     if(strcmp(str1,"FIT")==0)
	       if(strcmp(str2,"YES")==0)
		 map->sb_hpge_fit=1;
	 }

     take=1;
     if(strcmp(str1,"HPGe")==0)
	if(strcmp(str2,"GROUP2POSITION")==0)
	  {
	    if(fscanf(inp,"%s %s",str1,str2)!=EOF)
	      if(strcmp(str1,"Group")==0)
		if(strcmp(str2,"Position")==0)
		  while(take==1)
		      if(fscanf(inp,"%s %s",str1,str2)!=EOF)
			{
			  if((strcmp(str1,"END")==0)||(strcmp(str1,"MAP")==0))
			    take=0;
			  else
			    map->sb_hpge_map[atoi(str1)]=atoi(str2);	   
			}
	  }

    if(strcmp(str1,"AUX")==0)
       if(strcmp(str2,"CHANNEL")==0)
	 {
	   if(fscanf(inp,"%s %s",str1,str2)!=EOF)
	     if(strcmp(str1,"MIN")==0)
	       map->sb_aux_min=atoi(str2);
	   
	   if(fscanf(inp,"%s %s",str1,str2)!=EOF)
	     if(strcmp(str1,"MAX")==0)
	       map->sb_aux_max=atoi(str2);
	   
	   if(fscanf(inp,"%s %s",str1,str2)!=EOF)
	     if(strcmp(str1,"KPAR")==0)
	       map->sb_aux_kpar=atoi(str2);   
	   
	   if(fscanf(inp,"%s %s",str1,str2)!=EOF)
	     if(strcmp(str1,"FIT")==0)
	       if(strcmp(str2,"YES")==0)
		 map->sb_aux_fit=1;
	 }

    take=1;
    if(strcmp(str1,"AUX")==0)
	if(strcmp(str2,"GROUP2POSITION")==0)
	  {
	    if(fscanf(inp,"%s %s",str1,str2)!=EOF)
	      if(strcmp(str1,"Group")==0)
		if(strcmp(str2,"Position")==0)
		  while(take==1)
		      if(fscanf(inp,"%s %s",str1,str2)!=EOF)
			{
			  if((strcmp(str1,"END")==0)||(strcmp(str1,"MAP")==0))
			    take=0;
			  else
			    map->sb_aux_map[atoi(str1)]=atoi(str2);	   
			}
	  }
     
    }
  fclose(inp);
  print_map(map);
}
/*======================================================================*/
void map_event(Tig10_event *ptr, short* waveform, raw_event *data,sbmap* map,int turn)
{

  int d,pos;
  WaveFormPar wpar;
  int take=0;
  int nw;
  
  if(turn>-1)
    {
      if(data->h.frag_fold==0)
	data->h.trig_num=(ptr->trigger_num&0x00ffffff)+turn*0x01000000;
      else
	{
	  if(data->h.trig_num!=((ptr->trigger_num&0x00ffffff)+turn*0x01000000))
	    {
	      printf("Reconstruction error: inconsistent trigger number\n");
	      printf("Event trigger number    %8d\n",(data->h.trig_num));
	      printf("Fragment trigger number %8d\n",(ptr->trigger_num));
	      printf("Sort forced to terminate\n");
	      exit(0);
	    }
	}
    }

  data->h.frag_fold++;
  if(data->h.timestamp_up_min==0)
    data->h.timestamp_up_min=ptr->timestamp_up;
  if(ptr->timestamp_up<data->h.timestamp_up_min)
    data->h.timestamp_up_min=ptr->timestamp_up;
  
  if(data->h.timestamp_up_max==0)
    data->h.timestamp_up_max=ptr->timestamp_up;
  if(ptr->timestamp_up>data->h.timestamp_up_max)
    data->h.timestamp_up_max=ptr->timestamp_up;
  
  if(data->h.timestamp_min==0)
    data->h.timestamp_min=ptr->timestamp;
  if(ptr->timestamp<data->h.timestamp_min)
    data->h.timestamp_min=ptr->timestamp;
  
  if(data->h.timestamp_max==0)
    data->h.timestamp_max=ptr->timestamp;
  if(ptr->timestamp>data->h.timestamp_max)
    data->h.timestamp_max=ptr->timestamp; 

  /***************************** Start TRIGGER ***********************/
  take=0;
  pos=-1;
  for(d=0;d<NTRIG;d++)
    if(ptr->channel==map->sb_trig_map[d])
      {
	take=1;
	pos=d;
      }
  
  if(take==1)
    {
      if(ptr->charge_flag==1)
	if(ptr->charge!=0)
	  {
	    data->sbt.trig[pos].charge=ptr->charge;
	    data->sbt.h.EHP|=(1<<(pos));
	    data->sbt.h.Efold++;
	  }
      if(ptr->cfd_flag==1)
	if(ptr->cfd!=0)
	  {
	    data->sbt.trig[pos].cfd=ptr->cfd;
	    data->sbt.h.THP|=(1<<(pos));
	    data->sbt.h.Tfold++;
	  }
      
      data->sbt.trig[pos].timestamp=ptr->timestamp;
      data->sbt.trig[pos].timestamp_up=ptr->timestamp_up;
      data->sbt.h.TSHP|=(1<<(pos));
      data->sbt.h.TSfold++;
      data->h.setupHP|=SB_TBIT;
      
      if(map->sb_trig_fit==1)
	if(waveform!=NULL)
	  if(ptr->waveform_length!=0)
	    {
	      if(fit_NIM(ptr->waveform_length,waveform,&wpar)==1)
		{
		  data->sbt.fit[pos].tr=wpar.nimtr;
		  data->sbt.fit[pos].tf=wpar.nimtf;
		  data->sbt.fit[pos].h=wpar.nimh;
		  data->sbt.fit[pos].b=wpar.baseline;
		}
	      else
		{
		  data->sbt.fit[pos].tr=-1;
		  data->sbt.fit[pos].tf=-1;
		  data->sbt.fit[pos].h=-1;
		data->sbt.fit[pos].b=-1;
		}
	  }
    }
      
  /***************************** End TRIGGER ***********************/

    /***************************** Start HPGe ***********************/
  if(ptr->channel>=map->sb_hpge_min)
    if(ptr->channel<=map->sb_hpge_max)      
      {
	d=(ptr->channel-map->sb_hpge_min);
	pos=map->sb_hpge_map[d];

	
	if(pos>0)
	  if(pos<=(NPOS+1))
	    {
	      if(ptr->charge_flag==1)
		if(ptr->charge!=0)
		  {
		    data->sbc.hpge[pos-1].charge=ptr->charge/map->sb_hpge_kpar;
		    data->sbc.h.EHP|=(1<<(pos-1));
		    data->sbc.h.Efold++;
		  }
	      
	      if(ptr->cfd_flag==1)
		if(ptr->cfd!=0)
		  {
		    data->sbc.hpge[pos-1].cfd=ptr->cfd;
		    data->sbc.h.THP|=(1<<(pos-1));
		    data->sbc.h.Tfold++;
		  }
	      data->sbc.hpge[pos-1].timestamp=ptr->timestamp;
	      data->sbc.hpge[pos-1].timestamp_up=ptr->timestamp_up;
	      data->sbc.h.TSHP|=(1<<(pos-1));
	      data->sbc.h.TSfold++;
	      data->h.setupHP|=SB_DBIT;

	      if(map->sb_hpge_fit==1)
		if(waveform!=NULL)
		  if((nw=ptr->waveform_length)!=0)
		    {
		      memset(&wpar,0,sizeof(WaveFormPar));
		      fit_T0(nw,waveform,&wpar);
		      data->sbc.t0[pos-1]=wpar.t0;
		      take|=1;
		    }
	    }
      }
  /***************************** End HPGe ***********************/

  
  /***************************** Start AUX ***********************/
  if(ptr->channel>=map->sb_aux_min)
    if(ptr->channel<=map->sb_aux_max)      
      {
	d=(ptr->channel-map->sb_aux_min);
	pos=map->sb_aux_map[d];

	
	if(pos>0)
	  if(pos<=(NAUX+1))
	    {
	      if(ptr->charge_flag==1)
		if(ptr->charge!=0)
		  {
		    data->sba.aux[pos-1].charge=ptr->charge;
		    data->sba.h.EHP|=(1<<(pos-1));
		    data->sba.h.Efold++;
		  }
	      
	      if(ptr->cfd_flag==1)
		if(ptr->cfd!=0)
		  {
		    data->sba.aux[pos-1].cfd=ptr->cfd;
		    data->sba.h.THP|=(1<<(pos-1));
		    data->sba.h.Tfold++;
		  }
	      data->sba.aux[pos-1].timestamp=ptr->timestamp;
	      data->sba.aux[pos-1].timestamp_up=ptr->timestamp_up;
	      data->sba.h.TSHP|=(1<<(pos-1));
	      data->sba.h.TSfold++;
	      data->h.setupHP|=SB_ABIT;

	      if(map->sb_aux_fit==1)
		if(waveform!=NULL)
		  if((nw=ptr->waveform_length)!=0)
		    {
		      memset(&wpar,0,sizeof(WaveFormPar));
		      fit_T0(nw,waveform,&wpar);
		      data->sba.t0[pos-1]=wpar.t0;
		      data->sba.q[pos-1]=get_QDC3(nw,waveform,&wpar)/map->sb_aux_kpar;
		      take|=1;
		    }
	    }
      }
  /***************************** End AUX ***********************/
  return;
}

/*======================================================================*/

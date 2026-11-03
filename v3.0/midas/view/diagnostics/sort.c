#include "sort.h"
int color(int ch)
{
  if(ch==780) return 2;
  if(ch==790) return 4;
  if(ch==70) return 64;
  if(ch==71) return 8;

  return -1;
}
/*================================================================*/
int map(int ch)
{
  if(ch==780) return 0;
  if(ch==790) return 3;
  if(ch==70) return 1;
  if(ch==71) return 2;

  return -1;
}
/*================================================================*/
int analyze_fragment(Tig10_event* ptr, short* waveform)
{
  int time;
  int k;
  if((ptr->trigger_num&0x0fffffff)==chn)
    {
      time=ptr->cfd&0x00ffffff;
      time-=(ptr->timestamp*16)&0x00ffffff;
      printf("=========================================================\n");
      printf("Trigger  number : %8d 0x%8.8x\n",ptr->trigger_num&0x0fffffff,ptr->trigger_num);
      printf("           Port : %8d 0x%8.8x\n",ptr->port,ptr->port);
      printf("          Tig10 : %8d 0x%8.8x\n",ptr->tig10,ptr->tig10);
      printf("      Collector : %8d 0x%8.8x\n",ptr->collector,ptr->collector);
      printf("        Channel : %8d 0x%8.8x\n",ptr->channel,ptr->channel);
      printf("         Charge : %8d 0x%8.8x\n",ptr->charge,ptr->charge);
      printf("            CFD : %8d 0x%8.8x\n",ptr->cfd,ptr->cfd);
      printf("            LED : %8d 0x%8.8x\n",ptr->led,ptr->led);
      printf("     Time Stamp : %8d 0x%8.8x\n",ptr->timestamp,ptr->timestamp);
      printf("  Time Stamp Up : %8d 0x%8.8x\n",ptr->timestamp_up,ptr->timestamp_up);
      printf("           Time : %8d 0x%8.8x\n",time+S16K, time+S16K);
      printf("Waveform length : %8d 0x%8.8x\n",ptr->waveform_length,ptr->waveform_length);

      k=map(ptr->channel);
      if(k>=0)
	if(k<4)
	  {
	    if(ptr->waveform_length!=0)
	      {	  
		for(Int_t i=0;i<ptr->waveform_length;i++)
		  h[k]->Fill(i,waveform[i]);
	      }
	    h[k]->SetLineColor(color(ptr->channel));
	  }
    }
  return 0;
}
/*================================================================*/
int main(int argc, char *argv[])
{
  int ac;
  char* av[1];
  int i,N;
    
  ac=1;
  av[0]=(char*)malloc(sizeof(char));
  theApp=new TApplication("App", &ac, av);
  
  if(argc!=4)
    {
      printf("view_diagnostics midas_data_file waveform_length trigger_number \n");
      exit(-1);
    }
  N=atoi(argv[2]);
  for(i=0;i<4;i++)
    {
      if(h[i]!=NULL) delete h[i];
      h[i]=new TH1D("Waveform","Waveform",N,0,N);
    }
  chn=atoi(argv[3]);

  /* do sorting */
  sort_but_not_assemble(argv[1]);
  /* display results */
  printf("Program provides information on diagnostic signals for the selected trigger number.\n");
  
  if(c!=NULL) delete c;
  c = new TCanvas("Diagnostics", "Diagnostics",10,10, 700, 500);

  h[3]->Draw("HIST");
  for(i=0;i<3;i++)
     h[i]->Draw("HIST SAME");
      
  
  theApp->Run(kTRUE);

}

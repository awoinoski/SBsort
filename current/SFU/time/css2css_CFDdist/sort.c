#include "sort.h"

int analyze_data(raw_event *data)
{
  int pos;
  long diff;

  if(data->sbc.h.Efold>1)
    {
    for(pos=2;pos<NPOS;pos++)
      if((data->sbc.h.EHP&(1<<(pos-1)))!=0)
	{
	  diff=(data->sbc.hpge[0].cfd-data->sbc.hpge[pos-1].cfd)*0.625;
	  h->Fill((int)(diff));
	}
    }

  return SEPARATOR_DISCARD;
}
/*============================================================================*/

int main(int argc, char *argv[])
{
  input_names_type* name;
  TCanvas *canvas;
  TApplication *theApp;
  
  if(argc!=2)
    {
      printf("css2css_CFDdist sfu_data_file\n");
      exit(-1);
    }
  
  h = new TH1D("Time Difference [ns]","Time Difference [ns]",S4K,-S2K,S2K-1);
  h->Reset();
  
  printf("Program sorts histogram of time differences CSS CFD timestamps from the same event.\n");

  name=(input_names_type*)malloc(sizeof(input_names_type));
  memset(name,0,sizeof(input_names_type));
  strcpy(name->fname.inp_data,argv[1]);
  sort(name); 
  
  theApp=new TApplication("App", &argc, argv);
  canvas = new TCanvas("Time Difference", "Time Difference",10,10, 500, 300);
  canvas->cd();
  // gPad->SetLogy(1);
  h->Draw();
  
  theApp->Run(kTRUE);
}

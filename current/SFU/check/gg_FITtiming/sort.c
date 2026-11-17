#include "sort.h"

int analyze_data(raw_event *data)
{
  int pos;
  float diff;

  if(data->sbc.h.Efold>1)
    {
    for(pos=2;pos<NPOS;pos++)
      if((data->sbc.h.EHP&(1<<(pos-1)))!=0)
	{
	  diff=(data->sbc.t0[0]-data->sbc.t0[pos-1])*10;
	  h->Fill((int)(diff));

	  //printf("CSS1: %f, CSS%d: %f, Diff: %f \n",data->sbc.t0[0],pos, data->sbc.t0[pos-1], diff);
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
      printf("check_gg_FITtiming sfu_data_file\n");
      exit(-1);
    }
  
  h = new TH1D("Time Difference","Time Difference [ns]",S4K,-S2K,S2K-1);
  h->Reset();
  
  printf("Program sorts histogram of time differences of t0 from CSS waveform fitting from the same event.\n");

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

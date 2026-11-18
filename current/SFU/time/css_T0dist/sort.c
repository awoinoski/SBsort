#include "sort.h"

int analyze_data(raw_event *data)
{
  int pos, t0;

  if(data->sbc.h.Efold>0)
    {
    for(pos=1;pos<NPOS;pos++)
      if (pos==p)
	if((data->sbc.h.EHP&(1<<(pos-1)))!=0)
	  {
	    t0=data->sbc.t0[pos-1]*10;
	  
	    h->Fill((int)(t0));
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
  char title[132];
  
  if(argc!=3)
    {
      printf("css_T0dist sfu_data_file positon(1-8)\n");
      exit(-1);
    }

  printf("Program sorts histogram of t0 from CSS waveform fitting. \n");

  p=atoi(argv[2]);
  snprintf(title, sizeof(title), "CSS %d T0 Distribution",p);
  
  h = new TH1D("T0",title,300,0,(300-1)*10);
  h->Reset();

  name=(input_names_type*)malloc(sizeof(input_names_type));
  memset(name,0,sizeof(input_names_type));
  strcpy(name->fname.inp_data,argv[1]);
  sort(name); 
  
  theApp=new TApplication("App", &argc, argv);
  canvas = new TCanvas("T0", "T0",10,10, 500, 300);
  canvas->cd();
  gPad->SetLogy(1);
  h->GetXaxis()->SetTitle("T0 Time [ns]");
  h->GetYaxis()->SetTitle("Frequency");
  h->Draw();
  
  theApp->Run(kTRUE);
}

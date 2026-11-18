#include "sort.h"

int analyze_data(raw_event *data)
{
  h->Fill(data->sbt.fit[0].tr*10);
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
      printf("trig_T0dist sfu_data_file\n");
      exit(-1);
    }
  
  h = new TH1D("T0","NIM Trigger T0 Distribution",300,0,(300-1)*10);
  h->Reset();
  
  printf("Program sorts histogram of t0 from the trigger NIM waveform fitting. \n");

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

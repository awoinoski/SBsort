#include "sort.h"

int analyze_data(raw_event *data)
{
  int d;
  if(data->sbt.h.TSHP==0xe)
    {
      d=data->sbt.fit[1].tr-data->sbt.fit[2].tr;
      h->Fill(d);
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
      printf("time_MDGG_O2_O3 sfu_input_data\n");
      exit(-1);
    }
  
  h = new TH1D("MDGG O2-O3 time","MDGG O2-O3 time",512,-256,256);
  h->Reset();
  h->SetLineColor(kBlue);
  h->GetXaxis()->SetTitle("Clock Ticks [10 ns]");
  h->GetYaxis()->SetTitle("Counts");

  
  printf("Program display histograms of the time difference between MDGG O2 and O3 signals \n");
  
  name=(input_names_type*)malloc(sizeof(input_names_type));
  memset(name,0,sizeof(input_names_type)); 
  strcpy(name->fname.inp_data,argv[1]);
  sort(name); 
  
  theApp=new TApplication("App", &argc, argv);
  canvas = new TCanvas("MDGG time", "MDGG time",10,10, 500, 300);
  canvas->cd();
  h->Draw("HIST");
   
  theApp->Run(kTRUE);
}

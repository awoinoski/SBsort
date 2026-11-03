#include "sort.h"

int analyze_data(raw_event *data)
{
 
  h0->Fill(data->sbt.fit[0].tr);
  h1->Fill(data->sbt.fit[1].tr);
  h2->Fill(data->sbt.fit[2].tr);
  h3->Fill(data->sbt.fit[3].tr);
    
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
      printf("check_NIMTr sfu_input_data\n");
      exit(-1);
    }
  
  h0 = new TH1D("Trig","Trig",S1K,0,S1K-1);
  h0->Reset();
  h0->SetLineColor(kBlue);
  h0->GetXaxis()->SetTitle("Clock Ticks [10 ns]");
  h0->GetYaxis()->SetTitle("Counts");
  
  h1 = new TH1D("XLM E2","XLM E2",S1K,0,S1K-1);
  h1->Reset();
  h1->SetLineColor(kRed);
  h1->GetXaxis()->SetTitle("Clock Ticks [10 ns]");
  h1->GetYaxis()->SetTitle("Counts");
  
  h2 = new TH1D("XLM E3","XLM E3",S1K,0,S1K-1);
  h2->Reset();
  h2->SetLineColor(kGreen);
  h2->GetXaxis()->SetTitle("Clock Ticks [10 ns]");
  h2->GetYaxis()->SetTitle("Counts");
  
  h3 = new TH1D("XLM E4","XLM E4",S1K,0,S1K-1);
  h3->Reset();
  h3->SetLineColor(kMagenta);
  h3->GetXaxis()->SetTitle("Clock Ticks [10 ns]");
  h3->GetYaxis()->SetTitle("Counts");
 
  
  printf("Program display histograms of rise times for NIM trigger signals \n");
  
  name=(input_names_type*)malloc(sizeof(input_names_type));
  memset(name,0,sizeof(input_names_type)); 
  strcpy(name->fname.inp_data,argv[1]);
  sort(name); 
  
  theApp=new TApplication("App", &argc, argv);
  canvas = new TCanvas("NIM Trigs", "NIM Trigs",10,10, 500, 1200);
  canvas->Divide(1,4);
  canvas->cd(1);
  h0->Draw("HIST");
  canvas->cd(2);
  h1->Draw("HIST");
  canvas->cd(3);
  h2->Draw("HIST");
  canvas->cd(4);
  h3->Draw("HIST");
    
  theApp->Run(kTRUE);
}

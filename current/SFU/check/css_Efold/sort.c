#include "sort.h"

int analyze_data(raw_event *data)
{

  if(data->sbc.h.Efold>0)
    {
      hist[data->sbc.h.Efold]++;
      h->Fill(data->sbc.h.Efold);
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
      printf("check_css_Efold sfu_data_file\n");
      exit(-1);
    }
  
  h = new TH1D("Efold","EFold",8,0,8);
  h->Reset();
  
  printf("Program sorts histogram of Efold for CSS events.\n");

  name=(input_names_type*)malloc(sizeof(input_names_type));
  memset(name,0,sizeof(input_names_type));
  strcpy(name->fname.inp_data,argv[1]);
  sort(name); 
  
  theApp=new TApplication("App", &argc, argv);
  canvas = new TCanvas("EFold", "EFold",10,10, 500, 300);
  canvas->cd();
  gPad->SetLogy(1);
  h->Draw();
  
  theApp->Run(kTRUE);
}

#include "sort.h"

int analyze_data(raw_event *data)
{
  int e,t;

  if((data->sbc.h.EHP&1<<(pos-1))==0)
    return SEPARATOR_DISCARD;

  if((data->sbt.h.TSHP&1<<(start-1))==0)
    return SEPARATOR_DISCARD;

  if((data->sbt.h.TSHP&1<<(stop-1))==0)
    return SEPARATOR_DISCARD;
  
 
  t=data->sbt.fit[stop-1].tr-data->sbt.fit[start-1].tr; 
  e=data->sbc.hpge[pos-1].charge;
  
  h->Fill(t,e);

  return SEPARATOR_DISCARD;
}
/*==============================================================================*/
int main(int argc, char *argv[])
{
  input_names_type *name;
  TCanvas *canvas;
  TApplication *theApp;
  char HistName[128];
  char id[4][3]={"O1","O2","O3","O4"};
  int bad;

  bad=0;
  
  if(argc!=5)
    {
      printf("diag_ET sfu_input_data position_for_E start_for_T stop_for_T\n");
      exit(-1);
    }

 
  pos=atoi(argv[2]);
  start=atoi(argv[3]);
  stop=atoi(argv[4]);

  if(start<=0) bad=1;
  if(start>4)  bad=1;
  if(stop<=0)  bad=1;
  if(stop>4)   bad=1;
  if(pos<=0)   bad=1;
  if(pos>NPOS)  bad=1;

  if(bad==1)
    {
      printf("Ranges are: 1-8 for HPGe position 1-4 for timing of diagnostic signals\n");
      printf("Selection out of range\n");
      printf("Exiting\n");
      exit(0);
    }
 
  sprintf(HistName,"E %1d start %3s stop %3s",pos,id[start-1],id[stop-1]);
  h = new TH2D(HistName,HistName,256,-128,128,S2K,0,S16K);
  printf("Program sorts energy-time histogram for given HPGe position and diagnostic signals.\n");
  
  name=(input_names_type*)malloc(sizeof(input_names_type));
  memset(name,0,sizeof(input_names_type));
  strcpy(name->fname.inp_data,argv[1]);
  sort(name);

  theApp=new TApplication("App", &argc, argv);
 
  canvas = new TCanvas("Diag_ET","Diag_ET",10,10, 500, 300);
  gPad->SetLogz(1);
  gStyle->SetPalette(1);
  canvas->cd();

  
  h->Draw("COLZ");

  theApp->Run(kTRUE); 
}

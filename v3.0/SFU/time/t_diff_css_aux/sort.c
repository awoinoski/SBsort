#include "sort.h"

int analyze_data(raw_event *data)
{
  int pos, t;

  // if((data->h.setupHP&SB_ABIT)!=0)
  //{
  //   t=(int)((float)data->sbc.t0[1]-data->sba.t0[pos-1]);
  //   h->Fill(t);     
  // }


  if(data->sba.h.Efold>0)
    for(pos=1;pos<NAUX+1;pos++)
      if((data->sba.h.EHP&(1<<(pos-1)))!=0)
	{

	  // t=(int)((float)data->sbc.t0[0]-data->sba.t0[pos-1]);
	  t=(int)((float)data->sbc.t0[0]-data->sba.t0[0]);
	  h->Fill(t);

	}

  
  return SEPARATOR_DISCARD;
}
/*=======================================================================*/
int main(int argc, char *argv[])
{
  input_names_type* name;
  TCanvas *canvas;
  TApplication *theApp;
  
  if(argc!=2)
    {
      printf("t_diff_css_aux sfu_input_data_file_name\n");
      exit(-1);
    }
  

  h = new TH1D("CSS AUX Coincidence","Coincidence timing",512,-256,256);
  h->Reset();
  h->SetLineColor(kBlue);
  h->GetXaxis()->SetTitle("Clock Ticks [10 ns]");
  h->GetYaxis()->SetTitle("Counts");

  
  printf("Program sorts a time coincidence spectrum from CSS and AUX histogram for SANDBOX HPGe \n");

  
  name=(input_names_type*)malloc(sizeof(input_names_type));
  memset(name,0,sizeof(input_names_type));
  strcpy(name->fname.inp_data,argv[1]);
  sort(name);

  theApp=new TApplication("App", &argc, argv);
  canvas = new TCanvas("Coincidence time", "Coincidence time",10,10, 500, 300);
  canvas->cd();
  h->Draw("HIST");
   
  theApp->Run(kTRUE);

}

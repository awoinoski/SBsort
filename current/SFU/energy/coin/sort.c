#include "sort.h"

int analyze_data(raw_event *data)
{
  int pos, e_aux=0, e_hpge=0;

 if(data->sbc.h.Efold>0)
    for(pos=1;pos<NPOS+1;pos++)
      if((data->sbc.h.EHP&(1<<(pos-1)))!=0)
	{
	  // hist[0][100+pos]++;
	  e_hpge=data->sbc.hpge[pos-1].charge;
	}

  if(data->sba.h.Efold>0)
    for(pos=1;pos<NAUX+1;pos++)
      if((data->sba.h.EHP&(1<<(pos-1)))!=0)
	{
	  // hist[0][100+pos]++;
	  e_aux=data->sba.q[pos-1];

	  

	}
	  	  

  // energy calibration
  
  
 
  h->Fill(ahpge + bhpge*e_hpge + chpge*e_hpge*e_hpge, aaux + baux*e_aux + caux*e_aux*e_aux);

 
  
  return SEPARATOR_DISCARD;
}
/*=======================================================================*/
int main(int argc, char *argv[])
{
  input_names_type* name;
  TCanvas *canvas;
  TApplication *theApp;
  char title[132];
  
  canvas=NULL;
  
  if(argc!=2)
    {
      printf("coin sfu_input_data_file_name\n");
      exit(-1);
    }
  
  printf("Program sorts energy into a coincidence matrix for SANDBOX HPGe and NaI Aux \n");

  name=(input_names_type*)malloc(sizeof(input_names_type));
  memset(name,0,sizeof(input_names_type)); 
  strcpy(name->fname.inp_data,argv[1]);

  
  
  h=new TH2D("","",S4K,0,S4K,S4K,0,S4K);
  sprintf(title,"Coincidence Matrix");
  h->Reset();
  h->SetName(title);
  h->SetTitle(title);
  
  sort(name);


  theApp=new TApplication("App", &argc, argv);
  canvas = new TCanvas(title, title,10,10, 1400, 1000);
  h->GetXaxis()->SetTitle("HPGe Energy (keV)");
  h->GetYaxis()->SetTitle("AUX Energy (keV)");
  h->GetXaxis()->SetTitleOffset(1.4);
  h->GetYaxis()->SetTitleOffset(1.4);
  canvas->SetLogz();
  h->Draw();
  gStyle->SetPalette(1);
  theApp->Run(kTRUE);
}

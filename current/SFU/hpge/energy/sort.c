#include "sort.h"

int analyze_data(raw_event *data)
{
  cal_event* cev;
  float e;

  cev=(cal_event*)malloc(sizeof(cal_event));
  memset(cev,0,sizeof(cal_event));
  calibrate_HPGe(data,cal_par,cev);

  if(cev->sbc.h.Efold>0)
    for(pos=1;pos<NPOS+1;pos++)
      if((cev->sbc.h.EHP&(1<<(pos-1)))!=0)
	{
	  
	  e=cev->sbc.hpge[pos].charge/cal_par->contr_e;
	  if(e>=0 && e<S32K-10) hist[pos][(int)(e)]++;
	  else
	    hist[pos][S32K-5]++;

	}
  free(cev);
  return SEPARATOR_DISCARD;
}
/*=======================================================================*/
int main(int argc, char *argv[])
{
  FILE * output;
  input_names_type* name;
  int pos;
  
  if(argc!=2)
    {
      printf("css_energy sfu_input_data_file_name\n");
      exit(-1);
    }
  
  printf("Program sorts energy histogram for SANDBOX HPGe \n");

  name=(input_names_type*)malloc(sizeof(input_names_type));
  memset(name,0,sizeof(input_names_type));
  cal_par=(HPGe_calibration_parameters*)malloc(sizeof(HPGe_calibration_parameters));
  memset(cal_par,0,sizeof(HPGe_calibration_parameters));
  memset(hist,0,sizeof(hist));
  read_master(argv[1],name);

  sort(name);

  if((output=fopen("css_energy.mca","w"))==NULL)
    {
      printf("ERROR! I cannot open file css_energy.mca\n");
      exit(EXIT_FAILURE);
    }

  for(pos=0;pos<NPOS+1;pos++)
    fwrite(hist[pos],S32K*sizeof(int),1,output);
	
   fclose(output);
}

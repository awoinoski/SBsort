#include "sort.h"

int analyze_data(raw_event *data)
{
  cal_event* cev;
  double e;

  cev=(cal_event*)malloc(sizeof(cal_event));
  memset(cev,0,sizeof(cal_event));
  calibrate_HPGe(data,cal_par,cev);


  if(cev->sbc.h.Efold>0)
    for(pos=1;pos<NPOS;pos++)
      if((cev->sbc.h.EHP&(1<<(pos-1)))!=0)
	{
	  hist[0][100+pos]++;

	  
	  //e=cev->sbc.hpge[pos].charge/cal_par->contr_e;
	  e=cev->sbc.hpge[pos-1].charge;

	  printf("Event number: %d\n",data->h.trig_num);
	  printf("RAW: The energy %d, position %d, Efold %d , EHP %d\n",data->sbc.hpge[pos-1].charge,pos,data->sbc.h.Efold,data->sbc.h.EHP);
	  printf("Cal: The energy %f, position %d, Efold %d , EHP %d\n",e,pos,cev->sbc.h.Efold,cev->sbc.h.EHP);
	  
	  

	  if(e>0 && e<S32K-10)
	    {
	      hist[pos][(int)(e)]++;
	      printf("Making hist! \n");
	    }
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
      printf("css_energy master_file_name\n");
      exit(-1);
    }
  
  printf("Program sorts energy histogram for SANDBOX HPGe \n");

  name=(input_names_type*)malloc(sizeof(input_names_type));
  memset(name,0,sizeof(input_names_type));
  cal_par=(HPGe_calibration_parameters*)malloc(sizeof(HPGe_calibration_parameters));
  memset(cal_par,0,sizeof(HPGe_calibration_parameters));
  memset(hist,0,sizeof(hist));
  read_master(argv[1],name);

  if(name->flag.inp_data!=1)
    {
      printf("ERROR! Input data file not defined.\n");
      exit(EXIT_FAILURE);
    }

  if(name->flag.hpge_cal_par==1)
    {
      printf("HPGe calpar read from: %s\n",name->fname.hpge_cal_par);
      initialize_HPGe_calibration(cal_par,name->fname.hpge_cal_par);
    }

  else
    {
      printf("ERROR! HPGe calibration parameters not defined\n");
      exit(EXIT_FAILURE);
    }

  sort(name);

  
  if((output=fopen("css_energy.mca","w"))==NULL)
    {
      printf("ERROR! I cannot open file css_energy.mca\n");
      exit(EXIT_FAILURE);
    }

  for(pos=0;pos<NPOS;pos++)
    {
      fwrite(hist[pos],S32K*sizeof(int),1,output);
      printf("Writing data to spectrum %d \n", pos);
    }
   fclose(output);
}

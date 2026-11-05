#include "sort.h"

int analyze_data(raw_event *data)
{
  int pos,e;

 if(data->sba.h.Efold>0)
    for(pos=1;pos<NAUX+1;pos++)
      if((data->sba.h.EHP&(1<<(pos-1)))!=0)
	{
	  hist[0][100+pos]++;
	  e=data->sba.q[pos-1];
	  if(e>=0 && e<S32K-10)
	    hist[pos][e]++;
	  else
	    hist[pos][S32K-5]++;

	}
  
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
      printf("aux_energy_raw sfu_input_data_file_name\n");
      exit(-1);
    }
  
  printf("Program sorts raw energy histogram for SANDBOX HPGe \n");

  name=(input_names_type*)malloc(sizeof(input_names_type));
  memset(name,0,sizeof(input_names_type));
  strcpy(name->fname.inp_data,argv[1]);
  memset(hist,0,sizeof(hist));

  sort(name);

  if((output=fopen("aux_energy_raw.mca","w"))==NULL)
    {
      printf("ERROR! I cannot open file energy_raw.mca\n");
      exit(EXIT_FAILURE);
    }

  for(pos=0;pos<NAUX+1;pos++)
    fwrite(hist[pos],S32K*sizeof(int),1,output);
	
   fclose(output);
}

#include "sort.h"

int analyze_data(raw_event *data)
{
  int pos,t;

 if(data->sbc.h.TSfold>0)
    for(pos=1;pos<NPOS+1;pos++)
      if((data->sbc.h.TSHP&(1<<(pos-1)))!=0)
	{
	  hist[0][100+pos]++;
	  t=(int)((float)data->sbt.fit[0].tr-data->sbc.t0[pos-1]);
	  if(t>=0 && t<S4K-10)
	    hist[pos][t]++;
	  else
	    hist[pos][S4K-5]++;

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
      printf("time_t0_O1 sfu_input_data_file_name\n");
      exit(-1);
    }
  
  printf("Program sorts t0 to trigger time histogram for SANDBOX HPGe \n");

  name=(input_names_type*)malloc(sizeof(input_names_type));
  memset(name,0,sizeof(input_names_type));
  strcpy(name->fname.inp_data,argv[1]);
  memset(hist,0,sizeof(hist));

  sort(name);

  if((output=fopen("time_t0_O1.spn","w"))==NULL)
    {
      printf("ERROR! I cannot open file time_t0_O1\n");
      exit(EXIT_FAILURE);
    }

  for(pos=0;pos<NPOS+1;pos++)
    fwrite(hist[pos],S4K*sizeof(int),1,output);
	
   fclose(output);
}

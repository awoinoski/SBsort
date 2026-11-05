#include "sort.h"

int analyze_data(raw_event *data)
{
  int c,e;

  c=data->sbt.h.TSHP;
  e=data->sbc.hpge[pos-1].charge;
  
  if(c>=0)
    if(c<16)
      {
	if(e>=0 && e<S32K-10)
	  hist[c][e]++;
	else
	  hist[pos][S32K-5]++;
      }

  return SEPARATOR_DISCARD;
}
/*==============================================================================*/
int main(int argc, char *argv[])
{
  input_names_type *name;
  int k;
  FILE *output;
  
  if(argc!=3)
    {
      printf("energy_Diag_Class sfu_input_data position\n");
      exit(-1);
    }

  memset(hist,0,sizeof(hist));
  pos=atoi(argv[2]);
  
  printf("Program displays classes for diagnostic signals.\n");
  
  name=(input_names_type*)malloc(sizeof(input_names_type));
  memset(name,0,sizeof(input_names_type));
  strcpy(name->fname.inp_data,argv[1]);
  sort(name);

  if((output=fopen("energy_Diag_Class.mca","w"))==NULL)
    {
      printf("ERROR! I cannot open file energy_Diag_Class.mca\n");
      exit(EXIT_FAILURE);
    }

  for(k=0;k<16;k++)
    fwrite(hist[k],S32K*sizeof(int),1,output);
	
   fclose(output);

}

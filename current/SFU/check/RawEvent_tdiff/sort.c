#include "sort.h"

int analyze_data(raw_event *data)
{
  int pos, t;

  for(pos=1;pos<NAUX+1;pos++)
      if((data->sba.h.EHP&(1<<(pos-1)))!=0)
	{

	  // compute the time difference between HPGe and AUX
	  // t=(int)((float)data->sbc.t0[0]-data->sba.t0[pos-1]);
	  t=(int)((float)data->sbc.t0[0]-data->sba.t0[0]);
       

	  // print out raw event information that meets the condition
	  if((data->h.setupHP)!=0)
	    if(t > low)
	      if(t < high)
		{
		  
		  display_raw_event(data);
		  getc(stdin);
		}
	}
  

  return SEPARATOR_DISCARD;
}
/*==============================================================================*/
int main(int argc, char *argv[])
{
  input_names_type *name;
  
  if(argc!=4)
    {
      printf("check_RawEvent sfu_input_data low_gate high_gate\n");
      exit(-1);
    }
  
  printf("Program displays raw events on a condition of a range of the time difference between HPGe and AUX.\n");

  low = atoi(argv[2]);
  high = atoi(argv[3]);

  
  name=(input_names_type*)malloc(sizeof(input_names_type));
  memset(name,0,sizeof(input_names_type));
  strcpy(name->fname.inp_data,argv[1]);
  sort(name); 
}

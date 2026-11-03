#include "sort.h"

int analyze_data(raw_event *data)
{
  int trig,dt;
  
  trig=data->h.trig_num&0x7fffffff;
  dt=data->sbt.fit[stop-1].tr-data->sbt.fit[start-1].tr;

  if(dt>low && dt<high && data->sbt.h.TSHP==c)
    {
      printf("Event# %10d is in time gate.\n",trig);
      count++;
    }
  return SEPARATOR_DISCARD;
}
/*==============================================================================*/
int main(int argc, char *argv[])
{
  input_names_type *name;

  
  if(argc!=7)
    {
      printf("check_Triggers sfu_input_data evt_class start_for_T stop_for_T dt_low dt_high\n");
      exit(-1);
    }

  c=atoi(argv[2]);
  start=atoi(argv[3]);
  stop=atoi(argv[4]);
  low=atoi(argv[5]);
  high=atoi(argv[6]);
  
  name=(input_names_type*)malloc(sizeof(input_names_type));
  memset(name,0,sizeof(input_names_type));
  printf("Program prints triggers of a diagnostic class (evt_class) inside of a time gate (dt_high, dt_low) for time difference between two timing signals (start, stop) \n");
  
  strcpy(name->fname.inp_data,argv[1]);
  sort(name); 
  printf("Number of triggers in gate and class            is %10d\n",count);
}

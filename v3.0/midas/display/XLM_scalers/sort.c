#include "sort.h"
void print_xlm_scalers(xlm_event* xlm)
{
 
  int k,kk;
  int h,s,c,g;

  printf("\e[1;1H\e[2J");
  fflush(stdout);
  printf("\n--------------------------------------------");  
  printf("\n Rates in counts/sec ");
  printf("\n--------------------------------------------\n");
  printf("Cumulative live trigger         : %8d\n",xlm->MDGG_scalers[5]);
  printf("Cumulative XLM  trigger         : %8d\n",xlm->MDGG_scalers[6]);
  printf("Live trigger   rate             : %8d\n",xlm->MDGG_scalers[0]);
  printf("XLM  trigger   rate             : %8d\n",xlm->MDGG_scalers[1]);
  printf("XLM  output E1 rate             : %8d\n",xlm->MDGG_scalers[2]);
  printf("XLM  output E2 rate             : %8d\n",xlm->MDGG_scalers[3]);
  printf("XLM  output E3 rate             : %8d\n",xlm->MDGG_scalers[4]);
  printf("\n--------------------------------------------\n");
  printf("XLM trigger to MDGG             : %8d",xlm->XLM_scalers[74]);
  printf("\n--------------------------------------------");
  printf("\n Channel   HPGe   Shield   Catcher   Good Ge");

  for(k=0;k<NCSS;k++)       
    {
      h=xlm->XLM_scalers[k];
      s=xlm->XLM_scalers[NHPGe-1+k];
      c=xlm->XLM_scalers[2*(NHPGe-1)+k];
      kk=k%2;
      if(kk==0)
	g=(xlm->XLM_scalers[64+k/2]&0xffff);
      if(kk==1)
	g=((xlm->XLM_scalers[64+k/2]&0xffff0000)>>16);
      printf("\n %4d %8d %8d %8d %8d",k+1,h,s,c,g);
    }
  printf("\n--------------------------------------------");
  printf("\n Auxiliary   1        2        3        4");
  printf("\n      %8d %8d %8d %8d",xlm->XLM_scalers[60],xlm->XLM_scalers[61],xlm->XLM_scalers[62],xlm->XLM_scalers[63]);
  printf("\n--------------------------------------------\n");
  
  printf("Press ENTER to continue\n");getc(stdin);

  fflush(stdout);
}
/*================================================================*/
int get_fragments(long* proc)
{
   int *bank_data, items;
   char *bank_name;

  /* loop over banks, looking for the good ones (or others ...)  */
   while(1){
      if( (items = next_bank( &bank_name, &bank_data )) < 0 ){ return(-1); }
   
 
      if(strcmp(bank_name,"MXLM")==0)
	{ 
	  swapInt( (char *)bank_data, items*sizeof(int) );

	  unpack_xlm_bank(bank_data, items);
	}
      
      (*proc)++;
   }
   
   return(-1); /* go to next event */
}

      
/*================================================================*/
int main(int argc, char *argv[])
{
  FILE* input;
  long proc=0;
  int stop=0;
  int state=END_OF_RECORD;
  if(argc!=2)
    {
      printf("display_XLM_scalers midas_file_name\n");
      printf("Program prints MDGG/HPGeXLM scalers from a midas file\n");
      exit(-1);
    }

   /* open the input file*/
  if((input=fopen(argv[1],"r"))==NULL)
    {
      printf("\nI can't open input file %s\n",argv[1]);
      exit(-2);
    }

  while(stop==0){
    switch(state){
    case END_OF_RECORD:
      if( next_record(input) <= 0 ){ state = END_OF_FILE;}
      else {state = NEXT_EVENT;}
      break;
    case NEXT_EVENT:
      if( next_event() < 0 ){ state = END_OF_RECORD; }
      else { state = GET_FRAGMENTS; }
      break;
    case GET_FRAGMENTS:
      if( get_fragments(&proc) < 0 ){ state = NEXT_EVENT;}
      else { state = GET_FRAGMENTS;  }
      break;
    case END_OF_FILE:
      fclose(input);
      printf("\n Number of analyzed fragments is %8ld\n",proc);
      printf("\n");
      stop=1;
      break;
    default:
      printf("Unknown case\n");
      exit(0);
      break;
    }
  }
  
  return 0; 
}

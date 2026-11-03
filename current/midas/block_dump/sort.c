#include "sort.h"
int  dump_block_data(int *data, int n)
{

  for(int i=0;i<n;i++)
    {
      printf(" 0x%8.8x ",data[i]);
      if(i%8==7) printf("\n");
    }
  getc(stdin);
  
  return 0;
}
/*================================================================*/
int get_fragments(long* proc,char* block_id)
{
   int *bank_data, items;
   char *bank_name;

  /* loop over banks, looking for the good ones (or others ...)  */
   while(1){
      if( (items = next_bank( &bank_name, &bank_data )) < 0 ){ return(-1); }
   
 
      if(strcmp(bank_name,block_id)==0)
	{ 
	  swapInt( (char *)bank_data, items*sizeof(int) );
	  printf("%s block #%ld\n",block_id,*proc);
	  getc(stdin);
	  dump_block_data(bank_data, items);
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
  if(argc!=3)
    {
      printf("midas_block_dump midas_input_data block_id\n");
      printf("Program dumps data from blocks identified in a midas file\n");
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
      if( get_fragments(&proc,argv[2]) < 0 ){ state = NEXT_EVENT;}
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

#include "SB-cal.h"
/********************************************************************/
void initialize_TIGRESS_calibration(HPGe_calibration_parameters *HPGe_cal_par, char *name)
{
  FILE *inp;
  char str1[256],str2[256];

  memset(HPGe_cal_par,0,sizeof(*HPGe_cal_par));

  
  if((inp=fopen(name,"r"))==NULL)
      {
         printf("\nFile %s can not be opened\n",name);
         exit(EXIT_FAILURE);
      }
  else
        printf("calibration parameters read from file %s\n",name);

  
}
/********************************************************************/
void read_HPGe_e_cal_par(HPGe_calibration_parameters *HPGe_cal_par, char *filename)
{
  FILE *inp;
  char line[132];
  int pos,i;

  if((inp=fopen(filename,"r"))==Null)
    {
      printf("\nI can't open file %s\n",filename);
      exit(EXIT_FAILURE);
      
    }

  printf("\nEnergy calibration parameters read from the file:\n %s\n",filename);

  if(fgets(line,132,inp)!=NULL)
    {
      if(fgets(line,132,inp)!=NULL)
	while(fscanf(inp,"%d %lf %lf %lf",&pos,&a[0],&a[1],&a[2])!=EOF)
	  if(pos>0 && pos<NPOS)
	    {
	      HPGe_cal_par->ce[pos]=1;
	      for(i=0;i<3;i++) HPGe_cal_par->ce[pos][i]=a[i];
	    }
    }

  else
    {
      printf("Wrong structure of file %s\n",filename);
      printf("Aborting sort\n");
      exit(1);
    }
  fclose(inp);
  
}

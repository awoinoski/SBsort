#include "SB-cal.h"
/********************************************************************/
void initialize_HPGe_calibration(HPGe_calibration_parameters *HPGe_cal_par, char *name)
{
  FILE *inp;
  char str1[256],str2[256];

  memset(HPGe_cal_par,0,sizeof(*HPGe_cal_par));
  HPGe_cal_par->contr_e=1.;

  
  if((inp=fopen(name,"r"))==NULL)
      {
         printf("\nFile %s can not be opened\n",name);
         exit(EXIT_FAILURE);
      }
  
  else
        printf("calibration parameters read from file %s\n",name);

  if(fgets(str1,256,inp)!=NULL)
    {
      if(fgets(str1,256,inp)!=NULL)
	{
	  while(fscanf(inp,"%s %s",str1,str2)!=EOF)
	    {
	      if(strcmp(str1,"Energy_spectra_contraction_factor")==0)
		HPGe_cal_par->contr_e=atof(str2);

	      if(strcmp(str1,"Energy_calibration_parameters")==0)
		read_HPGe_e_cal_par(HPGe_cal_par,str2);
	    }
	}
    }

  else
    {
      printf("Wrong structure of file %s\n",name);
      printf("Aborting sort\n");
      exit(1);
    }
  
  fclose(inp);
    
}
/********************************************************************/
void read_HPGe_e_cal_par(HPGe_calibration_parameters *HPGe_cal_par, char *filename)
{
  FILE *inp;
  char line[132];
  int pos,i;
  double a[3];

  if((inp=fopen(filename,"r"))==NULL)
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
	      HPGe_cal_par->ceflag[pos]=1;
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
/********************************************************************/
void summarize_HPGe_calibration(HPGe_calibration_parameters *HPGe_cal_par, char *filename)
{
  FILE *out;
  int pos;

  if((out=fopen(filename,"w"))==NULL)
    {
      printf("\nFile %s can not be opened\n",filename);
      exit(EXIT_FAILURE);
    }

  fprintf(out,"HPGe energy calibration parameters\n");
  fprintf(out,"  pos#  col#    a0             a1             a2\n");
  for(pos=1;pos<NPOS;pos++)
    if(HPGe_cal_par->ceflag[pos]==1)
      {
	fprintf(out,"   %02d    %12.5e   %12.5e   %12.5e\n",pos,HPGe_cal_par->ce[pos][0],HPGe_cal_par->ce[pos][1],HPGe_cal_par->ce[pos][2]);
      }

  fclose(out);
}

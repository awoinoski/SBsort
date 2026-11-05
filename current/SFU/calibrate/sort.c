#include "functions.h"

gf3str gf3gd;
float derivs[51];
int   gotsignal;
int   ready = 0;



//=====================================================================================
int main(int argc, char *argv[])
{
  FILE* sou;
  data** dat;
  int  ndat=0;
  int i;
  calpar cp,cl,ch,cw;
  float e,de,x,dx;
  float en,den,xn,dxn;
  caldat** res;
  int    nres;
  float delta;
  double eth,pos,dp,w,dw;
  
  dat=(data**)malloc(sizeof(data*));
  nres=0;
  res=(caldat**)malloc(sizeof(caldat*));

  gfinit();
  
  if(argc!=2)
    {
      printf("\n ./OCTOPI_calibrator parameter_file_name\n");
      exit(-1);
    }
 
  ndat=read_par_file(argv[1],dat);

  //res=(caldat**)malloc(ndat*sizeof(caldat*));


  if(ndat<2)
    {
      printf("At least two spectra required for calibration\n");
      printf("Number of spectra read successfully is %d\n",ndat);
      printf("Exiting\n");
    }

  first_pass_cal(dat,ndat,&cp);
  printf("a[0]  %12.5e a[1]  %12.5e a[2]  %12.5e\n",cp.a,cp.b,cp.c);
  set_gf3_cal(&cp);

  for(i=0;i<ndat;i++)
    {
      copy_to_gf3str(dat[i]->spn);    
      strncpy(gf3gd.namesp,dat[i]->src,7);
      strncpy(gf3gd.filnam,dat[i]->fnm,79);
   
      sou=fopen(dat[i]->sou,"r");
      if(sou==NULL)
	{
	  printf("File %s can not be opened.\n",dat[i]->sou);
	  printf("Exiting\n");
	  exit(0);
	}

 
      if(fscanf(sou,"%f %f %f %f",&e,&de,&x,&dx)!=4) break;
      
      while(1)
	{
	 
	  if(fscanf(sou,"%f %f %f %f",&en,&den,&xn,&dxn)!=4)
	    {
	      nres=fit_singlet(e,&cp,res,nres,i);
	      if(nres>3)
		{
		  printf("i am here");
		  get_cal_par(&cp,res,nres);
		  //print_list(res,nres,dat);

		}
	      break;
	    }

	  delta=0.005*(e+en);
	  if(2*DE>delta) delta=2*DE;
	  if(fabs(e-en)>delta)//check if singlet
	      {
		nres=fit_singlet(e,&cp,res,nres,i);
		if(nres>3)
		  {
		    get_cal_par(&cp,res,nres);
		    print_list(res,nres,dat);
		  }
		e=en;de=den;x=xn;dx=dxn;

	      }
	    else
	      {
		nres=fit_doublet(e,en,&cp,res,nres,i);
		if(nres>3)
		  {
		    get_cal_par(&cp,res,nres);
		    print_list(res,nres,dat);
		  }
		if(fscanf(sou,"%f %f %f %f",&e,&de,&x,&dx)!=4) break;
	      }


	}
      
      fclose(sou);
    }


  sort_list(res,nres);
  
  print_list(res,nres,dat);

  cal_par(&cp,res,nres);

  inv_fit(res,nres,&eth,&cl,&ch);

  FWHM_fit(res,nres,&cw);
  
  printf("Results of the final fit\n");
  printf(" Source   Position   Error     Energy   Calc.Ene   Res.En.Sq. Calc.Pos  Res.Pos.Sq. FWHM   Calc.FWHM Res.FWHM.Sq\n");
  for(i=0;i<nres;i++)
    {
      e=res[i]->e;
      de=cp.a+cp.b*res[i]->ch+cp.c*res[i]->ch*res[i]->ch;
      x=e-de;
         x=x*x/res[i]->dch/res[i]->dch;

	 if(e<=eth)
	   pos=cl.a+cl.b*e+cl.c*e*e;
	 else
	   pos=ch.a+ch.b*e+ch.c*e*e;
	 dp=res[i]->ch-pos;

	 w=sqrt(cw.a+cw.b*e+cw.c*e*e);
	 dw=res[i]->w-w;
	 printf("%7s %10.4f %7.4f %10.4f %10.4f %10.4f %10.4f %10.4f %6.2f %8.2f %8.2f\n",dat[res[i]->sid]->src,res[i]->ch,res[i]->dch,res[i]->e,de,x,pos,dp*dp,res[i]->w, w,dw*dw);
    }

  printf("==========================================================\n");
  printf("Final parameters for channel to energy RADWARE calibration\n");
  printf("a[0]  %12.5e a[1]  %12.5e a[2]  %12.5e\n",cp.a,cp.b,cp.c);

 
  printf("==========================================================\n");
  printf("Final parameters for energy to channel RSOAPY calibration\n");
  printf("eth  %10.3f [keV]\n",eth);
  printf("al[0] %12.5e al[1] %12.5e al[2] %12.5e\n",cl.a,cl.b,cl.c);
  printf("ah[0] %12.5e ah[1] %12.5e ah[2] %12.5e\n",ch.a,ch.b,ch.c);
  printf("==========================================================\n");
  printf("Final parameters for energy to FWHM RSOAPY calibration\n");
  printf("  F   %12.5e   G   %12.5e   H   %12.5e\n",cw.a,cw.b,cw.c);
  printf("==========================================================\n");
  for(i=0;i<ndat;i++)
    free(dat[i]);
  free(dat);

  for(i=0;i<nres;i++)
    free(res[i]);
 
}
 

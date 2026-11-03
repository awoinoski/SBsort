#include "waveform_analyzer.h"
/*================================================================*/
double fit_NIM(int N, short* waveform, WaveFormPar* wpar)
{

  int t,w;
  double s;
  
  wpar->baseline_range=T0RANGE; //only 8 samples!
  get_baseline(wpar->baseline_range,waveform,wpar);
  
  wpar->max=-1;
  for(t=0;t<N;t++)
    if(waveform[t]>wpar->max)
      {
	wpar->max=waveform[t];
	wpar->tmax=t;
      }
  if(wpar->max!=-1)
    wpar->mflag=1;
  else
    return -1;
    
  if(wpar->tmax<BASELINE_RANGE)
    return BAD_BASELINE_RANGE;

  w=0.5*(wpar->max+wpar->baseline);

  for(t=T0RANGE;t<wpar->tmax;t++)
      if(waveform[t+1]>=w)
	if(waveform[t]<w)
	  break;
  
  wpar->nimtr=t+1;

  for(t=wpar->tmax;t<N;t++)
    if(waveform[t]>=w)
	if(waveform[t+1]<w)
	  break;
    
  wpar->nimtf=t+1;

  t=wpar->nimtf-wpar->nimtr;

  if(t<4)
    return BADCHISQ_LIN_T0;
    
  s=0;
  w=0;
  for(t=wpar->nimtr+1;t<wpar->nimtf-1;t++)
    {
      s+=waveform[t];
      w++;
    }
  s/=w;

  wpar->nimh=s-wpar->baseline;
  
  return 1.;
}
/*================================================================*/
double fit_NIMT0(int N, short* waveform, WaveFormPar* wpar)
{
  LinePar lp;
  int t,w;
  double a,b;


  wpar->baseline_range=T0RANGE; //only 8 samples!
  get_baseline(wpar->baseline_range,waveform,wpar);
  
  wpar->max=-1;
  for(t=0;t<N;t++)
    if(waveform[t]>wpar->max)
      {
	wpar->max=waveform[t];
	wpar->tmax=t;
      }
  if(wpar->max!=-1)
    wpar->mflag=1;
  else
    return -1;
    
  if(wpar->tmax<BASELINE_RANGE)
    return BAD_BASELINE_RANGE;


  //fit crude line to the baseline
  fit_line(waveform,0,T0RANGE,&lp);
  
  for(t=T0RANGE;t<wpar->tmax;t++)
    {
      w=0.5*(wpar->max+lp.slope*t+lp.intercept);
      if(waveform[t+1]>=w)
	if(waveform[t]<w)
	  break;
    }

  //fit fine line to the baseline
  fit_line(waveform,0,t-T0RANGE,&lp);
  t++;
  wpar->temin=t-2;
  wpar->temax=t+2;
  wpar->thigh=t;
  
  a=waveform[t]-waveform[t-1];
  b=waveform[t]-a*t;

  wpar->t0=-(b-lp.intercept)/(a-lp.slope);

  wpar->b0=lp.intercept;
  wpar->b1=lp.slope;
  wpar->s0=b;
  wpar->s1=a;
  wpar->s2=0.;

  return 1.;
}
/*======================================================*/
void get_baseline(int N, short *waveform,WaveFormPar *wpar)
{
  wpar->baseline=0.;
  wpar->baselineStDev=0.;
 
  //error if waveform length N is shorter than baseline range
  if(N<wpar->baseline_range)
    {
      printf("Baseline range (%d) larger than waveform length!\n",
	     wpar->baseline_range);
      printf("Terminating program\n");
      exit(0);
    }
  
  for(int i=0;i<wpar->baseline_range;i++)
    {
      wpar->baseline+=waveform[i];
      wpar->baselineStDev+=waveform[i]*waveform[i];
    }
  
  wpar->baselineStDev/=wpar->baseline_range;
  wpar->baseline/=wpar->baseline_range; 
  wpar->baselineStDev-=wpar->baseline*wpar->baseline;
  wpar->baselineStDev=sqrt(wpar->baselineStDev); 
  wpar->bflag=1; //flag after establishing baseline
}
/*================================================================*/
int fit_parabola(short* waveform,int low, int high,ParPar* pp)
{
  int i,ndf;
  lin_eq_type e;
  double chisq;
  memset(&e,0,sizeof(lin_eq_type));
  memset(pp,0,sizeof(ParPar));
  e.dim=3;
  chisq=0.;
  ndf=0;
  for(i=low;i<high;i++)
    {
      e.matrix[0][0]+=1;
      e.matrix[0][1]+=i;
      e.matrix[0][2]+=i*i;
      e.matrix[1][2]+=i*i*i;
      e.matrix[2][2]+=i*i*i*i;
      e.vector[0]+=waveform[i];
      e.vector[1]+=waveform[i]*i;
      e.vector[2]+=waveform[i]*i*i;
      ndf++;
      chisq+=waveform[i]*waveform[i];
    }
  e.matrix[1][0]=e.matrix[0][1];
  e.matrix[1][1]=e.matrix[0][2];
  e.matrix[2][0]=e.matrix[0][2];
  e.matrix[2][1]=e.matrix[1][2];
 
  if(solve_lin_eq(&e)==0)
    {
      pp->chisq=BADCHISQ_MAT;
      return -1;
    }
  else
    {
      chisq-=e.vector[0]*e.solution[0];
      chisq-=e.vector[1]*e.solution[1];
      chisq-=e.vector[2]*e.solution[2];
      pp->constant=e.solution[0];
      pp->linear=e.solution[1];
      pp->quadratic=e.solution[2];
      pp->chisq=chisq;
      pp->ndf=ndf;
      return 1;
    }
}
/*================================================================*/
int fit_line(short* waveform,int low, int high,LinePar* lp)
{
  int i,ndf;
  lin_eq_type e;
  double chisq;
  memset(&e,0,sizeof(lin_eq_type));
  memset(lp,0,sizeof(LinePar));
  e.dim=2;
  chisq=0.;
  ndf=0;
  for(i=low;i<high;i++)
    {
      e.matrix[0][0]+=1;
      e.matrix[0][1]+=i;
      e.matrix[1][1]+=i*i;
      e.vector[0]+=waveform[i];
      e.vector[1]+=waveform[i]*i;
      ndf++;
      chisq+=waveform[i]*waveform[i];
    }
  e.matrix[1][0]=e.matrix[0][1];
 
  if(solve_lin_eq(&e)==0)
    {
      lp->chisq=BADCHISQ_MAT;
      return -1;
    }
  else
    {
      chisq-=e.vector[0]*e.solution[0];
      chisq-=e.vector[1]*e.solution[1];
      lp->slope=e.solution[1];
      lp->intercept=e.solution[0];
      lp->chisq=chisq;
      lp->ndf=ndf;
      return 1;
    }
}
/*================================================================*/
int fit_smooth_parabola(short* waveform,int low, int high, double x0, ParPar* pp)
{
  int i,ndf,k;
  lin_eq_type e;
  double chisq;
  double x;
  memset(&e,0,sizeof(lin_eq_type));
  memset(pp,0,sizeof(ParPar));
  e.dim=2;
  chisq=0.;
  ndf=0;
  k=(int)rint(x0);

  for(i=low;i<k;i++)
    {
      e.matrix[0][0]+=1;
      e.vector[0]+=waveform[i];
      ndf++;
      chisq+=waveform[i]*waveform[i];
    }

  for(i=k;i<high;i++)
    {
      x=(i-x0)*(i-x0);
      e.matrix[0][0]+=1;
      e.matrix[0][1]+=x;
      e.matrix[1][1]+=x*x;
      e.vector[0]+=waveform[i];
      e.vector[1]+=waveform[i]*x;
      ndf++;
      chisq+=waveform[i]*waveform[i];
    }
  e.matrix[1][0]=e.matrix[0][1];
 
 
  if(solve_lin_eq(&e)==0)
    {
      pp->chisq=BADCHISQ_MAT;
      return -1;
    }
  else
    {
      chisq-=e.vector[0]*e.solution[0];
      chisq-=e.vector[1]*e.solution[1];
    
      pp->constant=e.solution[0];
      pp->linear=0.;
      pp->quadratic=e.solution[1];
      pp->chisq=chisq;
      pp->ndf=ndf;
      
      return 1;
    }

  return -1;
}
/*======================================================*/
void print_WavePar(WaveFormPar *wpar)
{
  printf("== Currently established waveform parameters ============\n");
  printf("baseline         : %10.2f\n",wpar->baseline);
  printf("baseline st. dev.: %10.2f\n",wpar->baselineStDev);
  printf("max              : %10.2f\n",(double)wpar->max);
  printf("tmax             : %10.2f\n",(double)wpar->tmax);
  printf("temin            : %10.2f\n",(double)wpar->temin);
  printf("temax            : %10.2f\n",(double)wpar->temax);
  printf("t0               : %10.2f\n",(double)wpar->t0);
}
/*======================================================*/
void print_NIMWavePar(WaveFormPar *wpar)
{
  printf("== Currently established waveform parameters ============\n");
  printf("baseline         : %10.2f\n",wpar->baseline);
  printf("baseline st. dev.: %10.2f\n",wpar->baselineStDev);
  printf("max              : %10.2f\n",(double)wpar->max);
  printf("tmax             : %10.2f\n",(double)wpar->tmax);
  printf("tr               : %10.2f\n",(double)wpar->nimtr);
  printf("tf               : %10.2f\n",(double)wpar->nimtf);
  printf("h                : %10.2f\n",(double)wpar->nimh);
}
/*================================================================*/
void  display_NIM_fit(int N, short* waveform, WaveFormPar* wpar,TApplication* theApp)
{
  TH1D *h=new TH1D("Waveform","Waveform",N,0,N);
  TH1D *g=new TH1D("Fit","Fit",N,0,N);
  h->Reset();
  for(Int_t i=0;i<N;i++)
    h->Fill(i,waveform[i]);

  g->Reset();
  for(Int_t i=0;i<wpar->nimtr;i++)
    g->Fill(i,wpar->baseline);

  for(Int_t i=wpar->nimtr;i<wpar->nimtf;i++)
    g->Fill(i,wpar->baseline+wpar->nimh);

  for(Int_t i=wpar->nimtf;i<N;i++)
    g->Fill(i,wpar->baseline);

  TCanvas *c=(TCanvas*)gROOT->FindObject("WaveformFit");
  if(c!=NULL) delete c;
  c = new TCanvas("WaveformFit", "WaveformFit",10,10, 700, 500);
  
  h->Draw("HIST");
  g->Draw("HIST SAME");
  g->SetLineColor(kRed);
  theApp->Run(kTRUE); 	      
  h->Delete();
  g->Delete();
}
/*================================================================*/
void  display_T0_fit(int N, short* waveform, WaveFormPar* wpar,TApplication* theApp)
{
  TH1D *h=new TH1D("Waveform","Waveform",N,0,N); 
  h->Reset();
  for(Int_t i=0;i<N;i++)
    h->Fill(i,waveform[i]);

  TCanvas *c=(TCanvas*)gROOT->FindObject("WaveformFit");
  if(c!=NULL) delete c;
  c = new TCanvas("WaveformFit", "WaveformFit",10,10, 700, 500);
  
  h->Draw("HIST");

  TF1* g=new TF1("fit","[0]+[1]*x",0,wpar->temax);
  TF1* f=new TF1("fit","[0]+[1]*x+[2]*x*x",wpar->temin,wpar->thigh);
 
  g->SetParameter(0,wpar->b0);
  g->SetParameter(1,wpar->b1);
  g->SetLineColor(kRed);
  g->Draw("same");

  f->SetParameter(0,wpar->s0);
  f->SetParameter(1,wpar->s1);
  f->SetParameter(2,wpar->s2);

  f->SetLineColor(8);
  f->Draw("same");
  
  theApp->Run(kTRUE); 	      
  h->Delete();
  f->Delete();
  g->Delete();
}
/*================================================================*/
double  fit_T0(int N, short* waveform, WaveFormPar* wpar)
{
  double chisq[3],chimin;
  WaveFormPar w[3];
  size_t swp;
  int  i,imin;

  swp=sizeof(WaveFormPar);

  wpar->baseline_range=T0RANGE; //only 8 samples!
  get_baseline(wpar->baseline_range,waveform,wpar);
  get_tmax(N,waveform,wpar);
 
  if(wpar->tmax<BASELINE_RANGE)
    return BAD_BASELINE_RANGE;

  get_t30(N,waveform,wpar);
  get_t50(N,waveform,wpar);
  wpar->thigh=wpar->t30;

  for(i=0;i<3;i++)
    chisq[i]=LARGECHISQ;

  memcpy(&w[0],wpar,swp);
  chisq[0]=get_smooth_T0(N,waveform,&w[0]);
  memcpy(&w[1],wpar,swp);
  chisq[1]=get_parabolic_T0(N,waveform,&w[1]);
  memcpy(&w[2],wpar,swp);
  chisq[2]=get_linear_T0(N,waveform,&w[2]);

  chimin=LARGECHISQ;
  imin=0;

  for(i=0;i<3;i++)
    if(chisq[i]>0)
      if(chisq[i]<chimin)
	{
	  chimin=chisq[i];
	  imin=i;
	}

  memcpy(wpar,&w[imin],swp);

 
  return chisq[imin];
 
}
/*======================================================*/
void get_tmax(int N, short *waveform,WaveFormPar *wpar)
{
  int i,j,sum;
  int D=FILTER/2;

  wpar->max=0;

  //applies the filter to the waveform
  for(i=D;i<N-D;i++)
    {
      sum=0;
      for(j=i-D;j<i+D;j++)
	sum+=waveform[j];
      sum/=FILTER; //the value of the filtered waveform at i
      if(sum>wpar->max)
	{
	  //if the value of the filtered waveform at i is larger than the current maximum, max=value and tmax = i
	  wpar->max=sum;
	  wpar->tmax=i;
	}
    }
  wpar->mflag=1; //flag after finding tmax
}
/*===========================================================*/
void get_t30(int N, short *waveform,WaveFormPar *wpar)
{
  int t;

  t=get_tfrac( N, waveform, wpar,0.3,0.15,0.45);
  if((t>0)&&(t<MAX_SAMPLES))
    {
      wpar->t30_flag=1;
      wpar->t30=t;
    }
  else
    {
      wpar->t30_flag=-1;
      wpar->t30=-1;
    }
}
/* ==================================================== */
void get_t50(int N, short *waveform,WaveFormPar *wpar)
{
  int t;
  
  t=get_tfrac( N, waveform, wpar,0.5,0.3,0.8);

  if((t>0)&&(t<MAX_SAMPLES))
    {
      wpar->t50_flag=1;
      wpar->t50=t;
    }
  else
    {
      wpar->t50_flag=-1;
      wpar->t50=-1;
    }
}/*===========================================================*/
double get_tfrac(int N, short *waveform,WaveFormPar *wpar,double frac,double fraclow, double frachigh)
{
  int t;
  double f,flow,fhigh;
  lin_eq_type e;
  int i,imax,imin;
  long long int a;
  double p,q,r,d;

  if(wpar->bflag!=1)
    {
      printf("Baseline not deterimned for the tfraction\n");
      exit(1);
    }

  if(wpar->mflag!=1)
    {
      printf("Maximum not deterimned for the tfraction\n");
      exit(1);
    }
  t=wpar->tmax;
  f=wpar->baseline+frac*(wpar->max-wpar->baseline);
  flow=wpar->baseline+fraclow*(wpar->max-wpar->baseline);
  fhigh=wpar->baseline+frachigh*(wpar->max-wpar->baseline);

  while(waveform[t]>f)
    {
      t--;
      if(t<=4) break;
    }
  imin=t;
  while(waveform[imin]>flow)
    {
      imin--;
      if(imin<=1) break;
    }
  
  imax=t;

   while(waveform[imax]<fhigh)
    {
      imax++;
      if(imax>=N-1) break;
    }

  e.dim=3;

  i=imax-imin;
  a=i;
  e.matrix[0][0]=a+1;
  e.matrix[0][1]=0.5*a;
  e.matrix[2][0]=a/6.;
  e.matrix[2][2]=-a/30.;
  a*=i;
  e.matrix[0][1]+=0.5*a;
  e.matrix[2][0]+=0.5*a;
  e.matrix[2][1]=0.25*a;
  a*=i;
  e.matrix[2][0]+=a/3.;
  e.matrix[2][1]+=0.5*a;
  e.matrix[2][2]+=a/3.;
  a*=i;
  e.matrix[2][1]+=0.25*a;
  e.matrix[2][2]+=0.5*a;
  a*=i;
  e.matrix[2][2]+=0.2*a;

  e.matrix[1][0]=e.matrix[0][1];
  e.matrix[1][1]=e.matrix[2][0];
  e.matrix[0][2]=e.matrix[2][0];
  e.matrix[1][2]=e.matrix[2][1];

 for(i=0;i<e.dim;i++)
    e.vector[i]=0;

  for(i=imin;i<imax+1;i++)
    {
      a=i-imin;
      e.vector[0]+=waveform[i];
      e.vector[1]+=waveform[i]*a;
      e.vector[2]+=waveform[i]*a*a;
    }
     

    if(solve_lin_eq(&e)==0)
      return -4;
    else
      {
    	p=e.solution[0]-f;
    	q=e.solution[1];
    	r=e.solution[2];

    	if(r!=0)
    	  {
    	    d=q*q-4*r*p;
    	    if(d<0)
		return -5;
    	    else
    	      {

    		f=-q+sqrt(d);
    		f*=0.5;
    		f/=r;
    		f+=imin;
	      
    		return f;
    	      }
    	  }
    	else
    	  {
    	    if(q!=0)
    	      {
    		f=-p/q;
    		return f;
    	      }
    	    else
    	      return -6;
    	  }
      }
    return -7;
}
/*================================================================*/
double  get_smooth_T0(int N, short* waveform, WaveFormPar* wpar)
{

  ParPar pp,ppmin;
  int k,kmin;
  double chit,chitmin;
  double c,t;
 
 
  chitmin=LARGECHISQ;
  kmin=0;
  //corse search first
  for(k=T0RANGE/2;k<wpar->thigh-T0RANGE/2;k++)
    {
      fit_smooth_parabola(waveform,0,wpar->thigh,(double)k,&pp);
      chit=pp.chisq;
      if(chit<chitmin)
	{
	  chitmin=chit;
	  kmin=k;
	}
    }	// end of the corse search loop over k
 
  c=kmin;
  chitmin=LARGECHISQ;
  //fine search next
  for(t=kmin-1;t<kmin+1;t+=0.1)
    {
      fit_smooth_parabola(waveform,0,wpar->thigh,t,&pp);
      chit=pp.chisq;
      if(chit<chitmin)
	{
      	  memcpy(&ppmin,&pp,sizeof(ParPar));
	  chitmin=chit;
	  c=t;
	}
    }	// end of the fine search loop over k
 

  memcpy(&pp,&ppmin,sizeof(ParPar));
  t=c;
  wpar->s0=pp.constant+pp.quadratic*t*t;
  wpar->s1=-2.*pp.quadratic*t;
  wpar->s2=pp.quadratic;

  
  wpar->b0=pp.constant;
  wpar->b1=0.;

  wpar->t0=-1;
  wpar->temin=0;
  wpar->temax=wpar->thigh;
  if(t>0)
    if(t<N)
      {
	wpar->t0=t;
	wpar->temin=(int)rint(wpar->t0)-2;
	wpar->temax=(int)rint(wpar->t0)+2;
	return (double)(chitmin/(wpar->thigh-2));
      } 
  return BADCHISQ_SMOOTH_T0;
}
/*================================================================*/
double  get_parabolic_T0(int N, short* waveform, WaveFormPar* wpar)
{

  LinePar lp;
  ParPar pp;
  int k;
  //int kmin;
  double chit,chitmin;
  double a,b,c,d,t;
 
  chitmin=LARGECHISQ;
  //kmin=0;
  for(k=T0RANGE/2;k<wpar->thigh-T0RANGE/2;k++)
    {
      //fit line to the baseline
      fit_line(waveform,0,k,&lp);
      
      //fit parabola to the risetime
      fit_parabola(waveform,k,wpar->thigh,&pp);
      
      chit=lp.chisq+pp.chisq;
      
      if(chit<chitmin)
	{
	  chitmin=chit;
	  wpar->b0=lp.intercept;
	  wpar->b1=lp.slope;
	  wpar->s0=pp.constant;
	  wpar->s1=pp.linear;
	  wpar->s2=pp.quadratic;
	}
    }
  
  a=wpar->s2;
  b=wpar->s1-wpar->b1;
  c=wpar->s0-wpar->b0;
  d=b*b-4*a*c;
  
  t=-1.;
  if(a==0.)
    t=-c/b;
  else
    {
      if(d>=0)
	{
	  if(d==0.)
	    t=-0.5*b/a;
	  else
	    {
	      d=sqrt(d);
	      t=0.5*(-b+d)/a;
	    }
	}
      else
	{
	  return BADCHISQ_PAR_T0;
	}
    }

  wpar->t0=-1;
  wpar->temin=0;
  wpar->temax=wpar->thigh;
  if(t>0)
    if(t<N)
      {
	wpar->t0=t;
	wpar->temin=(int)rint(wpar->t0)-2;
	wpar->temax=(int)rint(wpar->t0)+2;
	return (double)(chitmin/(wpar->thigh-5));
      } 
  return BADCHISQ_PAR_T0;
 	
}
/*================================================================*/
double  get_linear_T0(int N, short* waveform, WaveFormPar* wpar)
{

  LinePar lp,lpl;
  int k;
  //int kmin;
  double chit,chitmin;
  double b,c,t;
  
  chitmin=LARGECHISQ;
  //kmin=0;
  for(k=T0RANGE/2;k<wpar->thigh-T0RANGE/2;k++)
    {
      //fit line to the baseline
      fit_line(waveform,0,k,&lp);
      
      //fit line to the risetime
      fit_line(waveform,k,wpar->thigh,&lpl);
      
      
      chit=lp.chisq+lpl.chisq;
	  
      if(chit<chitmin)
	{
	  chitmin=chit;
	  wpar->b0=lp.intercept;
	  wpar->b1=lp.slope;
	  wpar->s0=lpl.intercept;
	  wpar->s1=lpl.slope;
	  wpar->s2=0.;
	}
    }	// end of the loop over k
  
 

  b=wpar->s1-wpar->b1;
  c=wpar->s0-wpar->b0;
  t=-c/b;

  wpar->t0=-1;
  wpar->temin=0;
  wpar->temax=wpar->thigh;
  if(t>0)
    if(t<N)
      {
	wpar->t0=t;
	wpar->temin=(int)rint(wpar->t0)-2;
	wpar->temax=(int)rint(wpar->t0)+2;
	return (double)(chitmin/(wpar->thigh-5));
      }
  
  return BADCHISQ_LIN_T0;

}
/*================================================================*/
double  get_QDC3(int N, short* waveform, WaveFormPar* wpar)
{//uses baseline from get_baseline
  int i;
  float t0;
  float tot,range;
  t0 = wpar->t0;
  if(wpar->t0<10){
    t0=10;
  }

  range=N-t0;
  // printf("N %i t0 %f range: %f\n",N,t0, range);
  tot=0;
  for(i=wpar->t0;i<N;i++)
    tot+=waveform[i];
  
  tot-=(wpar->baseline)*range;
  return tot;
}

#include "functions.h"
//=====================================================================================
void FWHM_fit(caldat** res,int nres, calpar *cw )
{
  int k,l,m;
  lin_eq_type x;
  double w,e,f;
  
  memset(&x,0,sizeof(lin_eq_type));
  x.dim=3;

  for(k=0;k<nres;k++)
    {
      f=res[k]->w*res[k]->w;
      e=res[k]->e;
      w=1./res[k]->dw;
      for(l=0;l<x.dim;l++)
	{
	  x.vector[l]+=w*w*f*pow(e,l);
	  for(m=0;m<x.dim;m++)
	     x.matrix[l][m]+=w*w*pow(e,l+m);
	}
    }
  solve_lin_eq(&x);
  cw->a=x.solution[0];
  cw->b=x.solution[1];
  cw->c=x.solution[2];

  return;
}
//=====================================================================================
void inv_fit(caldat** res,int nres,double* eth,calpar* cpl, calpar *cph )
{
  double chisq,e0,e0min,chisqmin;
  int k,l,m;
  double e[10],ch[10];
  lin_eq_type x,y;

  
  chisqmin=1e9;
  e0min=10000.;
  
  for(e0=1.1*res[0]->e;e0<=0.9*res[nres-1]->e;e0+=10.)
    {
      
      chisq=chisq_e0(e0,res,nres,&y);     
      if(chisq<chisqmin)
	{
	  e0min=e0;
	  chisqmin=chisq;
	}
    }
  e0=e0min;
  chisq=chisqmin;
  //  printf("e0 %6.0f chisq %8.2f chisq/ndf %8.2f\n",e0,chisq,chisq/(nres-4));

  memset(&x,0,sizeof(lin_eq_type));
  x.dim=3;
  
  for(k=0;k<10;k++)
    {
      e[k]=e0+(k-5)*5.;
      ch[k]=chisq_e0(e[k],res,nres,&y);
      for(l=0;l<x.dim;l++)
	{
	  x.vector[l]+=ch[k]*pow(e[k],l);
	  for(m=0;m<x.dim;m++)
	    x.matrix[l][m]+=pow(e[k],l+m);
	}
    }

  solve_lin_eq(&x);

  for(k=0;k<10;k++)
    {
      chisq=x.solution[2]*e[k]*e[k]+x.solution[1]*e[k]+x.solution[0];
      //  printf("e0 %6.0f chisq %8.2f chisq/ndf %8.2f fit %8.2f\n",e[k],ch[k],ch[k]/(nres-4),chisq/(nres-4));
    }
  
  e0=-x.solution[1]/2./x.solution[2];
  chisq=chisq_e0(e0,res,nres,&y); 
  *eth=e0;
  cpl->a=y.solution[1]+e0*e0*(y.solution[0]-y.solution[3]);
  cpl->b=y.solution[2]-2.*e0*(y.solution[0]-y.solution[3]);
  cpl->c=y.solution[0];
  cph->a=y.solution[1];
  cph->b=y.solution[2];
  cph->c=y.solution[3];
  //  printf("e0 %8.2f chisq %8.2f chisq/ndf %8.2f\n",e0,chisq,chisq/(nres-4));
  return;
}
//=====================================================================================
double chisq_e0(double e0, caldat** res,int nres, lin_eq_type* x )
{
  double e,p,w,ev[4],chisq;
  int i,k,l;


  memset(x,0,sizeof(lin_eq_type));
  chisq=0.;
  x->dim=4;
  
  for(i=0;i<nres;i++)
    {
      e=res[i]->e;
      p=res[i]->ch;
      w=1./res[i]->dch;
      chisq+=w*w*p*p;
      if(e<=e0)
	{
	  ev[0]=(e-e0)*(e-e0);
	  ev[1]=1.;
	  ev[2]=e;
	  ev[3]=e0*(2*e-e0);
	}
      else
	{
	  ev[0]=0.;
	  ev[1]=1.;
	  ev[2]=e;
	  ev[3]=e*e;
	}
      for(k=0;k<x->dim;k++)
	{
	  x->vector[k]+=w*w*p*ev[k];
	  for(l=0;l<x->dim;l++)
	    x->matrix[k][l]+=w*w*ev[k]*ev[l];
	}
    }

  solve_lin_eq(x);
 
  for(k=0;k<x->dim;k++)
    chisq-=x->vector[k]*x->solution[k];
  return chisq;
}
//=====================================================================================

void sort_list(caldat** list, int n)
{
  int i,j;
  caldat store;
  size_t s;

  s=sizeof(caldat);
  
  for(i=0;i<n-1;i++)
    for(j=0;j<n-i-1;j++)
      if(list[j]->e>list[j+1]->e)
	{
	  memcpy(&store,list[j],s);
	  memcpy(list[j],list[j+1],s);
	  memcpy(list[j+1],&store,s); 
	}
}
//=====================================================================================
void cal_par(calpar* cp,caldat **cdat,int ncdat)
{
  double s1,sx,sxx,sxxxx,sxxx,sy,sxy,sxxy,w,wa,wb,wc;
  int i;

  if(ncdat<3)
    {
      printf("Insufficient number of data points for calibration\n");
      printf("Exiting\n");
      exit(0);
    }
 

  s1=0.;sx=0.;sxx=0.;sxxx=0.;sxxxx=0.;sy=0.;sxy=0.;sxxy=0.;
      
  for(i=0;i<ncdat;i++)
    {
      w=1./cdat[i]->dch/cdat[i]->dch;
      s1+=w;
      sx+=w*cdat[i]->ch;
      sxx+=w*cdat[i]->ch*cdat[i]->ch;
      sxxx+=w*cdat[i]->ch*cdat[i]->ch*cdat[i]->ch;
      sxxxx+=w*cdat[i]->ch*cdat[i]->ch*cdat[i]->ch*cdat[i]->ch;
      sy+=w*cdat[i]->e;
      sxy+=w*cdat[i]->e*cdat[i]->ch;
      sxxy+=w*cdat[i]->e*cdat[i]->ch*cdat[i]->ch;
    }
  w=s1*sxx*sxxxx+sx*sxxx*sxx+sxx*sx*sxxx-s1*sxxx*sxxx-sx*sx*sxxxx-sxx*sxx*sxx;
  wa=sy*sxx*sxxxx+sx*sxxx*sxxy+sxx*sxy*sxxx-sy*sxxx*sxxx-sx*sxy*sxxxx-sxxy*sxx*sxx;
  wb=s1*sxy*sxxxx+sy*sxxx*sxx+sxx*sx*sxxy-s1*sxxx*sxxy-sy*sx*sxxxx-sxx*sxy*sxx;
  wc=s1*sxx*sxxy+sx*sxy*sxx+sy*sx*sxxx-s1*sxy*sxxx-sx*sx*sxxy-sxx*sxx*sy;
  cp->a=wa/w;
  cp->b=wb/w;
  cp->c=wc/w;
  

  
  for(i=0;i<ncdat;i++)
    {
      cdat[i]->res=cdat[i]->e-cp->a-cp->b*cdat[i]->ch-cp->c*cdat[i]->ch*cdat[i]->ch; 
    }
}
//=====================================================================================
void print_list(caldat** res,int nres, data** dat )
{
  int i;
  
  printf(" Source    Energy   Position   Error  Width   Error     Area   Error  Chisq/NDF\n");
  for(i=0;i<nres;i++){
    printf("%7s %10.4f %10.4f %6.4f %6.2f %6.2f %10.1f %6.1f %8.2f\n",dat[res[i]->sid]->src,res[i]->e,res[i]->ch,res[i]->dch,res[i]->w,res[i]->dw,res[i]->a,res[i]->da,res[i]->chisq);
  }
    return;
}
//=====================================================================================
int fit_singlet(float e, calpar* cp,caldat** res,int nres,int id)
{
  int lo,hi,c;
  float cent;
  float em;
  int n,i,max;
  printf("Fitting singlet at energy %10.4f\n",e);//return 0;
  em=0.995*e;
  if((e-DE)<em) em=e-DE;
  lo=(int)floor(cp->a+cp->b*(em)+cp->c*em*em);

  em=1.005*e;
  if((e+DE)>em) em=e+DE;
  hi=(int)ceil(cp->a+cp->b*em+cp->c*em*em);

  gf3gd.mch[0]=lo;
  gf3gd.mch[1]=hi;

  gf3gd.npks=1;
  cent=cp->a+cp->b*e+cp->c*e*e;
  gf3gd.ppos[0]=cent;
  
  c=(int)rint(cent);
  
  max=gf3gd.spec[c];
  
  for(i=lo+1;i<hi;i++)
    if(gf3gd.spec[i]>max)
      {
	max=gf3gd.spec[i];
	gf3gd.ppos[0]=i;
      }
  
 
  gfset();
  dofit();
  
  n=add_cdat_point(res,nres,e,gf3gd.pars[6],gf3gd.errs[6],gf3gd.pars[7],gf3gd.errs[7],gf3gd.pars[8],gf3gd.errs[8],gf3gd.chisq,id);

  return n; 

}
//=====================================================================================
int fit_doublet(float e, float en, calpar* cp,caldat** res,int nres,int id)
{
  int lo,hi;
  float cent;
  float em;
  int n;
  
  printf("Fitting doublet at energies %10.4f %10.4f\n",e,en);// return 0;
  if(e<en)
    em=e;
  else
    em=en;
  em*=0.995;
  if((e-DE)<em) em=e-DE;
  
  lo=(int)floor(cp->a+cp->b*em+cp->c*em*em);

  if(e>en)
    em=e;
  else
    em=en;
  em*=1.005;
  if((e+DE)>em) em=e+DE;
    
  hi=(int)ceil(cp->a+cp->b*em+cp->c*em*em);

  gf3gd.mch[0]=lo;
  gf3gd.mch[1]=hi;
  
  gf3gd.npks=2;
  cent=cp->a+cp->b*e+cp->c*e*e;
  gf3gd.ppos[0]=cent;

  cent=cp->a+cp->b*en+cp->c*en*en;
  gf3gd.ppos[1]=cent;

  gfset();
  dofit();

  n=add_cdat_point(res,nres,e,gf3gd.pars[6],gf3gd.errs[6],gf3gd.pars[7],gf3gd.errs[7],gf3gd.pars[8],gf3gd.errs[8],gf3gd.chisq,id);
  n=add_cdat_point(res,n,en,gf3gd.pars[9],gf3gd.errs[9],gf3gd.pars[10],gf3gd.errs[10],gf3gd.pars[10],gf3gd.errs[10],gf3gd.chisq,id);
  return n;
}
/* ======================================================================= */
void set_gf3_cal(calpar* cp)
{
  gf3gd.ical = 1;
  gf3gd.gain[0] = -cp->a/cp->b*(1+cp->c*cp->c/cp->b/cp->b);
  gf3gd.gain[1] = 1./cp->b*(1+2*cp->a*cp->c/cp->b);
  gf3gd.gain[2] = -cp->c/cp->b/cp->b/cp->b;
  gf3gd.nterms = 3;
}

/* ======================================================================= */
int parset(int mode)
{
  float x, y, x0;
  int   i, j, resetp, resetw, ihi, ilo, ipp;

  ilo = gf3gd.mch[0];
  ihi = gf3gd.mch[1];
  x0 = (float) ((ihi + ilo) / 2);
  for (i = 0; i < gf3gd.npars; ++i) {
    gf3gd.errs[i] = 0.f;
  }
  x = 0.f;
  for (i = 0; i < gf3gd.npks; ++i) {
    x += gf3gd.ppos[i];
  }
  x /= (float) gf3gd.npks;

  if (gf3gd.freepars[0] == 1)
    gf3gd.pars[0] = (gf3gd.spec[ihi] + gf3gd.spec[ilo]) / 2.f;
  if (gf3gd.freepars[1] == 1)
    gf3gd.pars[1] = (gf3gd.spec[ihi] - gf3gd.spec[ilo]) / (float) (ihi-ilo);
  if (gf3gd.freepars[2] == 1) gf3gd.pars[2] = 0.f;
  if (gf3gd.freepars[3] == 1) gf3gd.pars[3] = gf3gd.finest[0] + gf3gd.finest[1] * x;
  if (gf3gd.freepars[4] == 1) gf3gd.pars[4] = gf3gd.finest[2] + gf3gd.finest[3] * x;
  if (gf3gd.pars[4] == 0.f)
    gf3gd.pars[4] = sqrt(gf3gd.swpars[0] +
			 gf3gd.swpars[1] * x + 
			 gf3gd.swpars[2] * x * x) * .5f;
  if (gf3gd.freepars[5] == 1) gf3gd.pars[5] = gf3gd.finest[4];
  if (mode < 0) {
    /* come here only during set-up */
    resetp = 1;
    resetw = 1;
    gf3gd.nfp = 3 - gf3gd.infix[0] - gf3gd.infix[1] - gf3gd.infix[2];
    gf3gd.freepars[3] = gf3gd.infix[0];
    gf3gd.freepars[4] = gf3gd.infix[1];
    gf3gd.freepars[5] = gf3gd.infix[2];
    if (gf3gd.freepars[3] == 0 && gf3gd.pars[3] == 0.f) {
      gf3gd.nfp += gf3gd.freepars[4];
      gf3gd.freepars[4] = 0;
    }
    gf3gd.irelw = gf3gd.infixrw;
    if (gf3gd.infixw == 0) {
      for (i = 1; i <= gf3gd.npks; ++i) {
	gf3gd.freepars[i*3 + 4] = 0;
      }
      gf3gd.nfp += gf3gd.npks;
    }
  } else {
    resetp = 1;
    if (gf3gd.irelpos == 0)
      resetp = 1;
    resetw = 1;
    if (gf3gd.irelw == 0)
      resetw = 1;
  }
  for (i = 0; i < gf3gd.npks; ++i) {
    gf3gd.areas[i] = 0.f;
    gf3gd.dareas[i] = 0.f;
    j = i*3 + 6;
    if (resetp && gf3gd.freepars[j] == 1) gf3gd.pars[j] = gf3gd.ppos[i];
    if (mode < 0 || (resetw && gf3gd.freepars[j+1] == 1))
      gf3gd.pars[j+1] = sqrt(gf3gd.swpars[0] +
			     gf3gd.swpars[1] * gf3gd.ppos[i] +
			     gf3gd.swpars[2] * gf3gd.ppos[i] * gf3gd.ppos[i]);
    if (gf3gd.freepars[j+2] == 1) {
      x = gf3gd.ppos[i] - x0;
      y = gf3gd.pars[0] + gf3gd.pars[1] * x;
      ipp = gf3gd.ppos[i] + 0.5f;
      gf3gd.pars[j+2] = gf3gd.spec[ipp] - y;
      gf3gd.cents[i] = gf3gd.pars[j];
    }
  }
  return 0;
} /* parset */
/* ======================================================================= */
void gfinit(void)
{

  int i;
  float sw1,sw2,sw3;
  
  gf3gd.maxch = S32K;
  gf3gd.wtmode = -1;

  if (!gf3gd.ical) {
    gf3gd.ical = 1;
    gf3gd.gain[0] = 0.0;
    gf3gd.gain[1] = 0.5;
    gf3gd.nterms = 2;
  }

  gf3gd.irelpos = 0;
  gf3gd.irelw = 1;

  for(i=0;i<5;i++) gf3gd.finest[i]=0;
  for(i=0;i<3;i++) gf3gd.infix[i]=0;
  gf3gd.infixw=1;
  gf3gd.infixrw=0;
  sw1=4.0;
  sw2=3.0;
  sw3=0.0;
  
  gf3gd.swpars[0] = sw1 * sw1;
  gf3gd.swpars[1] = sw2 * sw2 / 1e3f;
  gf3gd.swpars[2] = sw3 * sw3 / 1e6f;

}
/* ======================================================================= */
void gfset(void)
{
  int i,param;
  /* int lo; */
  /* char fixtag[53][4]; */
  /* static char parc[53][4] = { */
  /*   " A "," B "," C "," R ","BTA","STP","P1 ","W1 ","H1 ","P2 ","W2 ","H2 ", */
  /*   "P3 ","W3 ","H3 ","P4 ","W4 ","H4 ","P5 ","W5 ","H5 ","P6 ","W6 ","H6 ", */
  /*   "P7 ","W7 ","H7 ","P8 ","W8 ","H8 ","P9 ","W9 ","H9 ","PA ","WA ","HA ", */
  /*   "PB ","WB ","HB ","PC ","WC ","HC ","PD ","WD ","HD ","PE ","WE ","HE ", */
  /*   "PF ","WF ","HF ","RW ","RP "}; */

 


  /* ask for fixed pars */
  gf3gd.npars = (gf3gd.npks + 2) * 3;
  for (i = 0; i <  gf3gd.npars; ++i) {
    gf3gd.freepars[i] = 1;
  }
  
  gf3gd.irelpos = 0;
  gf3gd.nfp=1;
  parset(-1);
  //fix background to linear
  param=3;
  gf3gd.nfp += gf3gd.freepars[param-1];
  gf3gd.freepars[param-1] = 0;
  gf3gd.pars[param-1] = 0.;
 

  //list info about fixed

  /*   for (i = 0; i < gf3gd.npars; ++i) { */
  /*   if (gf3gd.freepars[i] == 0) { */
  /*     strcpy(fixtag[i], "  *"); */
  /*   } else { */
  /*     sprintf(fixtag[i], "%3d", i+1); */
  /*     fixtag[i][3] = '\0'; */
  /*   } */
  /* } */
  /* strcpy(fixtag[51], "   "); */
  /* if (!gf3gd.irelw) strcpy(fixtag[51], "  *"); */
  /* strcpy(fixtag[52], "   "); */
  /* if (!gf3gd.irelpos) strcpy(fixtag[52], "  *"); */

  /* lo = 0; */
  /* if (gf3gd.npars > 26) { */
  /*   for (i = lo; i < 26; ++i) { */
  /*     printf(" %s", fixtag[i]); */
  /*   } */
  /*   printf("\n "); */
  /*   for (i = lo; i < 26; ++i) { */
  /*     printf(" %s", parc[i]); */
  /*   } */
  /*   lo = 26; */
  /* } */
  /* printf("\n"); */
  /* for (i = lo; i < gf3gd.npars; ++i) { */
  /*   printf(" %s", fixtag[i]); */
  /* } */
  /* printf(" %s %s", fixtag[51], fixtag[52]); */
  /* printf("\n "); */
  /* for (i = lo; i < gf3gd.npars; ++i) { */
  /*   printf(" %s", parc[i]); */
  /* } */
  /* printf(" %s %s", parc[51], parc[52]); */
  return;
} /* gfset */
/* ======================================================================= */
int gffin(void)
{
  float a, d, r, y, r1, eb, eh, er, ew, bet;
  int   i, ic;

  /* calc. areas, centroids and errors */
  r = gf3gd.pars[3] / 50.f;
  r1 = 1.f - r * .5f;
  bet = gf3gd.pars[4];
  for (i = 0; i < gf3gd.npks; ++i) {
    ic = i*3 + 7;
    y = gf3gd.pars[ic] / (bet * 3.33021838f);
    if (y > 4.0f) {
      d = 0.0f;
    } else {
      d = exp(-y * y) / erfc(y);
    }
    a = r * bet * d + gf3gd.pars[ic] * 1.06446705f * r1;
    gf3gd.areas[i] = a * gf3gd.pars[ic + 1];
    eh = a * gf3gd.errs[ic + 1];
    er = (bet * 2.f * d - gf3gd.pars[ic] * 1.06446705f) * gf3gd.errs[3] / 100.f;
    eb = r * d * (y * 2.f * y + 1.f - d * 1.12837917f * y) * gf3gd.errs[4];
    ew = (r1 * 1.06446705f + r * .600561216f * d *
	  (d / 1.77245385f - y)) * gf3gd.errs[ic];
    gf3gd.dareas[i] = sqrt(eh * eh + gf3gd.pars[ic+1] * 
			   gf3gd.pars[ic+1] * (er * er + eb * eb + ew * ew));
    gf3gd.cents[i] = gf3gd.pars[ic-1] - r * bet * d * bet / a;
  }
 
  typeit(2);
  return 0;
} /* gffin */


//=====================================================================================
int dofit(void)
{
  int  k, maxits;
  maxits = 50;

  /* do fit */
  k = fitter(maxits);
  if (k == 2) printf(" WARNING - convergence failed.\n"
		     "  Do not believe quoted parameter values!\n");
  /* display fit and list parameters */
  if (k != 1) gffin();
  return 0;
} /* dofit */

/* ======================================================================= */
int eval(float *pars, int ichan, float *fit, int npks, int mode)
{
  /* calculate the fit using present values of the pars */

  float a, h, r, r1, r2, u, u1, u2, u3, u5, u6, u7, u8, w, x, x1, z; 
  static float y[15], y1[15], y2[15], beta, width;
  static int   i, notail, ix0;

  if (mode == -9) {
    /* mode = -9 ; initialise, i.e calculate  IX0,NOTAIL,Y,Y1,Y2 */
    ix0 = (gf3gd.mch[0] + gf3gd.mch[1]) / 2;
    notail = 1;
    if (gf3gd.freepars[3] == 0 && pars[3] == 0.f) return 0;
    notail = 0;
    for (i = 0; i < npks; ++i) {
      y[i] = pars[i*3 + 7] / (pars[4] * 3.33021838f);
      if (y[i] > 4.f) {
	y1[i] = 0.f;
	notail = 1;
      } else {
	y1[i] = erfc(y[i]);
	y2[i] = exp(-y[i] * y[i]) * 1.12837917f / y1[i];
      }
    }
    return 0;
  }
  x = (float) (ichan - ix0);
  *fit = pars[0] + pars[1]*x + pars[2]*x*x;
  if (mode >= 1) {
    derivs[1] = x;
    derivs[2] = x*x;
    derivs[3] = 0.f;
    derivs[4] = 0.f;
    derivs[5] = 0.f;
  }
  x1 = (float) (ichan);
  for (i = 0; i < npks; ++i) {
    x = x1 - pars[i*3 + 6];
    width  = pars[i*3 + 7] / 2.35482f;
    h      = pars[i*3 + 8];
    w = x / (width*1.41421356f);
    if (fabs(w) > 4.f) {
      u1 = 0.f;
      u3 = 0.f;
      if (x < 0.f) u3 = 2.f;
    } else {
      u1 = exp(-w*w);
      u3 = erfc(w);
    }
    if (mode == -1) {
      /* mode = -1; calculate background only */
      *fit += h * pars[5] * u3 / 200.f;
      continue;
    }
    if (notail) {
      /* notail = true; pure gaussians only */
      u = u1 + pars[5] * u3 / 200.f;
      *fit += h * u;
      /* calculate derivs only for mode.ge.1 */
      if (mode >= 1) {
	derivs[5] += h * u3 / 200.f;
	a = u1 * (w + pars[5] / 354.49077f) * 2.f;
	derivs[i*3 + 6] = h * a / (width * 1.41421356f);
	derivs[i*3 + 7] = h * w * a * 1.41421356f / (width * 2.35482f);
	derivs[i*3 + 8] = u;
      }
      continue;
    }
    r = pars[3] / 100.f;
    r1 = 1.f - r;
    beta = pars[4];
    z = w + y[i];
    if ((r2 = x / beta, fabs(r2)) > 12.f) {
      u5 = 0.f;
      u6 = 0.f;
      u7 = 0.f;
    } else {
      u7 = exp(x / beta) / y1[i];
      if (fabs(z) > 4.f) {
	u5 = 0.f;
	if (z < 0.f) u5 = 2.f;
	u6 = 0.f;
      } else {
	u5 = erfc(z);
	u6 = exp(-z * z) * 1.12837917f;
      }
    }
    u2 = u7 * u5;
    u = r1 * u1 + r * u2 + pars[5] * u3 / 200.f;
    *fit += h * u;
    /* calculate derivs only for mode.ge.1 */
    if (mode >= 1) {
      u8 = u5 * y2[i];
      derivs[3] += h * (u2 - u1) / 100.f;
      derivs[4] += r * h * u7 * (y[i] * (u6 - u8) - u5 * x / beta) / beta;
      derivs[5] += h * u3 / 200.f;
      a = u1 * (r1 * w + pars[5] / 354.49077f) * 2.f;
      derivs[i*3 + 6] = h * (a + r*u7*(u6 - u5*2.f*y[i])) / (width*1.41421356f);
      derivs[i*3 + 7] = h * (w*a + r*u7*(u6 * (w - y[i]) + u8*y[i])) *
                        1.41421356f / (width*2.35482f);
      derivs[i*3 + 8] = u;
    }
  }
  return 0;
} /* eval */

/* ======================================================================= */
int fitter(int maxits)
{
  /* this subroutine is a modified version of 'CURFIT', in Bevington
     see page 237 */

  double alpha[51][51], array[51][51], ddat;
  float  diff, beta[51], b[51], delta[51], fixed[51], ers[51];
  float  chisq=0.0f, chisq1, flamda, dat, fit, r1;
  int    i, j, k, l, m, conv = 0, nits, test, nipos, nextp[51];
  int    ndf, ihi, ilo, niw, rpfixed, rwfixed, mip=0, nip=0, miw=0, nip1=0;
  char   dattim[20];
  static char wtc[3][16] = {"fit.","data.","sp.         "};

  ilo = gf3gd.mch[0];
  ihi = gf3gd.mch[1];
  nip = gf3gd.npars - gf3gd.nfp;
  /* nip   = no. of independent (non-fixed) pars
     nfp   = no. of fixed pars
     npars = total no. of pars = 3 * no.of peaks + 6
     ndf   = no. of degrees of freedom */
  if (gf3gd.wtmode > 0) strncpy(&wtc[2][4], gf3gd.nwtsp, 8);
  for (i = 6; i < gf3gd.npars; ++i) {
    fixed[i] = (float) gf3gd.freepars[i];
  }
  /* set up fixed relative widths */
  rwfixed = 0;
  if (gf3gd.irelw <= 0) {
    niw = 0;
    for (j = 7; j < gf3gd.npars; j += 3) {
      if (gf3gd.freepars[j] == 1) miw = j;
      /* miw = highest fitted (non-fixed) width par. no. */
      niw += gf3gd.freepars[j];
    }
    /* niw = no. of fitted (non-fixed) widths */
    if (niw > 1) {
      for (j = 7; j <= miw; j += 3) {
	gf3gd.freepars[j] = 0;
      }
      rwfixed = 1;
      nip = nip - niw + 1;
      nip1 = nip;
    }
  }
  /* set up fixed relative positions */
  rpfixed = 0;
  if (gf3gd.irelpos <= 0) {
    nipos = 0;
    for (j = 6; j < gf3gd.npars; j += 3) {
      if (gf3gd.freepars[j] == 1) mip = j;
      /* mip = highest fitted (non-fixed) position par. no. */
      nipos += gf3gd.freepars[j];
    }
    /* nipos = no. of fitted (non-fixed) positions */
    if (nipos > 1) {
      for (j = 6; j <= mip; j += 3) {
	gf3gd.freepars[j] = 0;
      }
      rpfixed = 1;
      nip = nip - nipos + 1;
      nip1 = nip - 1;
    }
  }
  ndf = ihi - ilo + 1 - nip;
  if (ndf < 1) {
    printf("No d.o.f.\n");
    goto QUIT;
  }
  if (nip < 2) {
    printf("Too many fixed pars.\n");
    goto QUIT;
  }
  /* set up array nextp, pointing to free pars */
  k = 0;
  for (j = 0; j < gf3gd.npars; ++j) {
    if (gf3gd.freepars[j] != 0) nextp[k++] = j;
  }
  if (rwfixed) nextp[k++] = miw;
  if (rpfixed) nextp[k++] = mip;
  if (k != nip) {
    printf("nip != sum(freepars)\n");
    goto QUIT;
  }
  /* initialise for fitting */
  flamda = .001f;
  nits = 0;
  test = 0;
  derivs[0] = 1.f;
  for (i = 0; i < gf3gd.npars; ++i) {
    gf3gd.errs[i] = 0.f;
    b[i] = gf3gd.pars[i];
  }
  gotsignal = 0;
  signal(SIGINT, breakhandler);
  /* printf("You can press control-C to interrupt this fit...\n"); */

  /* evaluate fit, alpha & beta matrices, & chisq */
 NEXT_ITERATION:
  for (j = 0; j < nip; ++j) {
    beta[j] = 0.f;
    for (k = 0; k <= j; ++k) {
      alpha[k][j] = 0.f;
    }
  }
  chisq1 = 0.f;
  eval(gf3gd.pars, 0, &fit, gf3gd.npks, -9);
  for (i = ilo; i <= ihi; ++i) {
    eval(gf3gd.pars, i, &fit, gf3gd.npks, 1);
    diff = gf3gd.spec[i] - fit;
    /* weight with fit/data/weight sp. for wtmode=-1/0/1 */
    if (gf3gd.wtmode < 0) {
      dat = fit;
    } else if (gf3gd.wtmode == 0) {
      dat = gf3gd.spec[i];
    } else {
      dat = gf3gd.wtsp[i];
    }
    if (dat < 1.f) dat = 1.f;
    ddat = (double) dat;
    chisq1 += diff * diff / dat;
    if (rwfixed) {
      for (k = 7; k < miw; k += 3) {
	derivs[miw] += fixed[k] * derivs[k];
      }
    }
    if (rpfixed) {
      for (k = 6; k < mip; k += 3) {
	derivs[mip] += fixed[k] * derivs[k];
      }
    }
    for (l = 0; l < nip; ++l) {
      j = nextp[l];
      beta[l] += diff * derivs[j] / dat;
      for (m = 0; m <= l; ++m) {
	alpha[m][l] += (double)derivs[j] * (double)derivs[nextp[m]] / ddat;
      }
    }
  }
  chisq1 /= (float) ndf;
  /* invert modified curvature matrix to find new parameters */
 INVERT_MATRIX:
  for (j = 0; j < nip; ++j) {
    if (alpha[j][j] * alpha[j][j] == 0.) {
      printf("Cannot - diagonal element no. %d equal to zero.\n", j);
      goto QUIT;
    }
  }
  array[0][0] = flamda + 1.f;
  for (j = 1; j < nip; ++j) {
    for (k = 0; k < j; ++k) {
      array[k][j] = alpha[k][j] / sqrt(alpha[j][j] * alpha[k][k]);
      array[j][k] = array[k][j];
    }
    array[j][j] = flamda + 1.f;
  }
  matinv(array[0], nip, 51);
  if (!test) {
    for (j = 0; j < nip; ++j) {
      delta[j] = 0.f;
      for (k = 0; k < nip; ++k) {
	delta[j] += beta[k] * array[k][j] / sqrt(alpha[j][j] * alpha[k][k]);
      }
    }
    /* calculate new par. values */
    for (l = 0; l < nip; ++l) {
      j = nextp[l];
      b[j] = gf3gd.pars[j] + delta[l];
    }
    if (rwfixed) {
      for (j = 7; j <= miw - 3; j += 3) {
	b[j] = gf3gd.pars[j] + fixed[j] * delta[nip1-1];
      }
    }
    if (rpfixed) {
      for (j = 6; j <= mip - 3; j += 3) {
	b[j] = gf3gd.pars[j] + fixed[j] * delta[nip-1];
      }
    }
    for (j = 7; j <= gf3gd.npars; j += 3) {
      if (b[j] < 0.0f) b[j] = fabs(b[j]);
    }
    /* if chisq increased, increase flamda and try again */
    chisq = 0.f;
    eval(b, gf3gd.freepars[3], &fit, gf3gd.npks, -9);
    for (i = ilo; i <= ihi; ++i) {
      eval(b, i, &fit, gf3gd.npks, 0);
      diff = gf3gd.spec[i] - fit;
      /* weight with fit/data/weight sp. for wtmode=-1/0/1 */
      if (gf3gd.wtmode < 0) {
	dat = fit;
      } else if (gf3gd.wtmode == 0) {
	dat = gf3gd.spec[i];
      } else {
	dat = gf3gd.wtsp[i];
      }
      if (dat < 1.f) dat = 1.f;
      chisq += diff * diff / dat;
    }
    chisq /= (float) ndf;
    if (chisq > chisq1 && flamda < 2.f) {
      flamda *= 10.f;
      goto INVERT_MATRIX;
    }
  }
  /* evaluate parameters and errors
     test for convergence */
  conv = 1;
  for (j = 0; j < nip; ++j) {
    if (array[j][j] < 0.) array[j][j] = 0.;
    ers[j] = sqrt(array[j][j] / alpha[j][j]) * sqrt(flamda + 1.f);
    if ((r1 = delta[j], fabs(r1)) >= ers[j] / 100.f) conv = 0;
  }
  if (!test && !gotsignal) {
    for (j = 0; j < gf3gd.npars; ++j) {
      gf3gd.pars[j] = b[j];
    }
    flamda /= 10.f;
    ++nits;
    if (!conv && nits < maxits) goto NEXT_ITERATION;
    /* re-do matrix inversion with FLAMDA=0
       to calculate errors */
    flamda = 0.f;
    test = 1;
    goto INVERT_MATRIX;
  }

  if (gotsignal) {
    printf("**** Fit Interrupted (Control-C) ****\n"
	   "**** WARNING: Parameter values and errors may be undefined!\n");
  }
  signal(SIGINT, SIG_DFL);
  /* list data and exit */
  for (l = 0; l < nip; ++l) {
    gf3gd.errs[nextp[l]] = ers[l];
  }
  if (rwfixed) {
    for (j = 7; j <= miw; j += 3) {
      gf3gd.freepars[j] = fixed[j];
      gf3gd.errs[j] = fixed[j] * ers[nip1 - 1];
    }
  }
  if (rpfixed) {
    for (j = 6; j <= mip; j += 3) {
      gf3gd.freepars[j] = fixed[j];
      gf3gd.errs[j] = fixed[j] * ers[nip - 1];
    }
  }
  datetime(dattim);
  printf(" File %s  Spectrum %.8s  %.18s\n"
	 " Fitted chs %d to %d, %d peaks\n"
	 " %d indept. pars,  %d degrees of freedom,  weighted with %s\n",
	 gf3gd.filnam, gf3gd.namesp, dattim, gf3gd.mch[0], gf3gd.mch[1],
	 gf3gd.npks, nip, ndf, wtc[gf3gd.wtmode+1]);
  if (rpfixed) printf("Relative peak positions fixed.\n");
  if (rwfixed) printf("Relative widths fixed.\n");

  if (conv) {
    printf(" %d iterations,  Chisq/d.o.f.= %.3f\n", nits, chisq);
    gf3gd.chisq=chisq;
    return 0;
  }
  printf("Failed to converge after %d iterations,  Chisq/d.o.f.= %.3f\n"
	 "  WARNING - do not believe quoted parameter values!\n", nits, chisq);
  gf3gd.chisq=chisq;
  return 2;

 QUIT:
  if (rwfixed || rpfixed) {
    for (i = 6; i < gf3gd.npars; ++i) {
      gf3gd.freepars[i] = fixed[i];
    }
  }
  return 1;
} /* fitter */

/* ====================================================================== */
int datetime(char *dattim)
{
  /* encode current date and time into character string dattim
     desired format 22-Mar-94 HH:MM:SS
     dattim must be at least 20 chars long */
  time_t   now;

  now = time((time_t *)NULL);
  strftime(dattim, 19, "%d-%b-%y %H:%M:%S", localtime(&now));
  dattim[18] = '\0';
  return 0;
} /* datetime */

/* ======================================================================= */
void breakhandler(int dummy)
{
  gotsignal = 1;
}

/* ======================================================================= */
int energy(float x, float dx, float *eg, float *deg)
{
  int jj;

  if (gf3gd.ical == 0) return 1;

  *deg = 0.f;
  *eg = gf3gd.gain[gf3gd.nterms - 1];
  for (jj = gf3gd.nterms - 1; jj >= 1; --jj) {
    *deg = (float) jj * gf3gd.gain[jj] + *deg * x;
    *eg = gf3gd.gain[jj-1] + *eg * x;
  }
  *deg *= dx;
  return 0;
} /* energy */

/* ======================================================================= */
int wrresult(char *out, float value, float err, int minlen)
{
  int   n;
  char  *c;

  /* append nicely-formatted " value(error)" to string out
     minlen = requested minimum length of out at completion
     returns new length of out string */

  c = out + strlen(out);
  if (err < 0.00005f) {
    sprintf(c, " %.4f", value);
    c = out + strlen(out);
    while (*(c-1) == '0' && *(c-2) == '0') c--;
    strcpy(c, "(0)");
  } else if (err < 0.0025f) {
    sprintf(c, " %.4f(%.0f)", value, err * 10000.0f);
  } else if (err < 0.025f) {
    sprintf(c, " %.3f(%.0f)", value, err * 1000.0f);
  } else if (err < 0.25f) {
    sprintf(c, " %.2f(%.0f)", value, err * 100.0f);
  } else if (err < 2.5f) {
    sprintf(c, " %.1f(%.0f)", value, err * 10.0f);
  } else {
    sprintf(c, " %.0f(%.0f)", value, err);
  }
  for (n = strlen(out); n < minlen; n++) {
    strcat(out," ");
  }
  return n;
} /* wrresult */

/* ======================================================================= */
int typeit(int mode)
{
  /* print results of fit - parameters etc. */
  float eg, deg;
  int   i, k;
  char  l[120], *c;

  if (mode == 1) {
    printf(" Mkr chs: limits %5d %5d\n           peaks",
	   gf3gd.mch[0], gf3gd.mch[1]);
 
    for (i = 0; i < gf3gd.npks; i++) {
      printf(" %.2f", gf3gd.ppos[i]);
   
    }
    printf("\n");
 
  }
  strcpy(l, " Background: A =");
  wrresult(l, gf3gd.pars[0], gf3gd.errs[0], 0);
  strcat(l, ",  B =");
  wrresult(l, gf3gd.pars[1], gf3gd.errs[1], 0);
  strcat(l, ",  C =");
  wrresult(l, gf3gd.pars[2], gf3gd.errs[2], 0);
  printf("%s\n", l);

  strcpy(l, "      Shape: R =");
  wrresult(l, gf3gd.pars[3], gf3gd.errs[3], 0);
  strcat(l, ",  Beta =");
  wrresult(l, gf3gd.pars[4], gf3gd.errs[4], 0);
  strcat(l, ",  Step =");
  wrresult(l, gf3gd.pars[5], gf3gd.errs[5], 0);
  printf("%s\n", l);
 

  if (gf3gd.ical == 0) {
     printf("   position      width       height"
	    "        area          centroid\n");
  } else {
     printf("   position      width       height"
	    "        area          centroid  energy\n");
  }
  for (i = 0; i < gf3gd.npks; ++i) {
    k = i*3 + 6;
    *l= '\0';
    wrresult(l, gf3gd.pars[k], gf3gd.errs[k], 14);
    wrresult(l, gf3gd.pars[k+1], gf3gd.errs[k+1], 26);
    wrresult(l, gf3gd.pars[k+2], gf3gd.errs[k+2], 40);
    c = l + wrresult(l, gf3gd.areas[i], gf3gd.dareas[i], 54);
    wrresult(l, gf3gd.cents[i], gf3gd.errs[k], 0);
    /* remove (error) from centroid */
    c = (strrchr(c,'('));
    *c = '\0';
    if (!energy(gf3gd.cents[i], gf3gd.errs[k], &eg, &deg)) {
      while (c < l + 64) {
	strcat(c++, " ");
      }
      wrresult(l, eg, deg, 0);
    }
    printf("%2d%s\n", i+1, l);
  
  }
  return 0;
} /* typeit */

//=====================================================================================
void get_cal_par(calpar* cp,caldat **cdat,int ncdat)
{
  double s1,sx,sxx,sxxxx,sxxx,sy,sxy,sxxy,w,wa,wb,wc;
  int i;

  if(ncdat<2)
    {
      printf("Insufficient number of data points for calibration\n");
      printf("Exiting\n");
      exit(0);
    }
  
  if(ncdat==2)
    {
    
      s1=0.;sx=0.;sxx=0.;sy=0.;sxy=0.;
      
      for(i=0;i<ncdat;i++)
	{
	  w=1./cdat[i]->dch/cdat[i]->dch;
	  s1+=w;
	  sx+=w*cdat[i]->e;
	  sxx+=w*cdat[i]->e*cdat[i]->e;
	  sy+=w*cdat[i]->ch;
	  sxy+=w*cdat[i]->e*cdat[i]->ch;
	}
      w=s1*sxx-sx*sx;
      wa=sy*sxx-sxy*sx;
      wb=s1*sxy-sx*sy;
      cp->a=wa/w;
      cp->b=wb/w;
      cp->c=0;
    }

  if(ncdat>2)
    {
      s1=0.;sx=0.;sxx=0.;sxxx=0.;sxxxx=0.;sy=0.;sxy=0.;sxxy=0.;

      
      for(i=0;i<ncdat;i++)
	{   
	  w=1./cdat[i]->dch/cdat[i]->dch;
	  s1+=w;
	  sx+=w*cdat[i]->e;
	  sxx+=w*cdat[i]->e*cdat[i]->e;
	  sxxx+=w*cdat[i]->e*cdat[i]->e*cdat[i]->e;
	  sxxxx+=w*cdat[i]->e*cdat[i]->e*cdat[i]->e*cdat[i]->e;
	  sy+=w*cdat[i]->ch;
	  sxy+=w*cdat[i]->e*cdat[i]->ch;
	  sxxy+=w*cdat[i]->e*cdat[i]->e*cdat[i]->ch;
	};
      
      w=s1*sxx*sxxxx+sx*sxxx*sxx+sxx*sx*sxxx-s1*sxxx*sxxx-sx*sx*sxxxx-sxx*sxx*sxx;
      wa=sy*sxx*sxxxx+sx*sxxx*sxxy+sxx*sxy*sxxx-sy*sxxx*sxxx-sx*sxy*sxxxx-sxxy*sxx*sxx;
      wb=s1*sxy*sxxxx+sy*sxxx*sxx+sxx*sx*sxxy-s1*sxxx*sxxy-sy*sx*sxxxx-sxx*sxy*sxx;
      wc=s1*sxx*sxxy+sx*sxy*sxx+sy*sx*sxxx-s1*sxy*sxxx-sx*sx*sxxy-sxx*sxx*sy;
      cp->a=wa/w;
      cp->b=wb/w;
      cp->c=wc/w;

    }


  
  for(i=0;i<ncdat;i++)
    {
      cdat[i]->res=cdat[i]->ch-cp->a-cp->b*cdat[i]->e-cp->c*cdat[i]->e*cdat[i]->e; 
    }
}
//=====================================================================================
void first_pass_cal(data** dat,int ndat,calpar* cp)
{
  caldat **cdat;
  int ncdat=0;
  int i;
  float e,ch,dch;

  // cdat=(caldat**)malloc(sizeof(caldat*));

  
  cdat=(caldat**)malloc(ndat*sizeof(caldat*));
 
  
  for(i=0;i<ndat;i++)
    {
      if(strcmp(dat[i]->src,"eu152")==0)
      	{
	  printf("Data for 152Eu calibration identified at ID %d\n",i);
	  e=121.783;
	  cent_max_peak(e,dat[i]->spn,&ch,&dch);
	  ncdat=add_cdat_point(cdat,ncdat,e,ch,dch,0.,0.,0.,0.,0.,i);
	  printf("Find peak from 152Eu, 121.8 keV\n");
	  printf("Energy  %f, channel  %f\n\n",e, ch);
	}
      if(strcmp(dat[i]->src,"co56")==0)
	{
	  printf("Data for  56Co calibration identified at ID %d\n",i);
	  e=846.772;
	  cent_max_peak(e,dat[i]->spn,&ch,&dch);
	  ncdat=add_cdat_point(cdat,ncdat,e,ch,dch,0.,0.,0.,0.,0.,i);
	  printf("Find peak from 56Co, 846.8 keV\n");
	  printf("Energy  %12.5e, channel  %12.5e\n\n",e, ch);
	}
      if(strcmp(dat[i]->src,"co60")==0)
	{
	  printf("Data for  60Co calibration identified at ID %d\n",i);
	  e=1173.228;
	  cent_max_peak(e,dat[i]->spn,&ch,&dch);
	  ncdat=add_cdat_point(cdat,ncdat,e,ch,dch,0.,0.,0.,0.,0.,i);
	  printf("Find peak from 60Co, 1173 keV\n");
	  printf("Energy  %f, channel  %f\n\n",e, ch);
	}
      if(strcmp(dat[i]->src,"ba133")==0)
	{
	  printf("Data for 133Ba calibration identified at ID %d\n",i);
	  e=80.9979;
	  cent_max_peak(e,dat[i]->spn,&ch,&dch);
	  ncdat=add_cdat_point(cdat,ncdat,e,ch,dch,0.,0.,0.,0.,0.,i);
	  printf("Find peak from 133Ba, 81 keV\n");
	  printf("Energy  %fe, channel  %f\n\n",e, ch);
	}
      if(strcmp(dat[i]->src,"cs137")==0)
	{
	  printf("Data for 137Cs calibration identified at ID %d\n",i);
	  e=661.659;
	  cent_max_peak(e,dat[i]->spn,&ch,&dch);
	  ncdat=add_cdat_point(cdat,ncdat,e,ch,dch,0.,0.,0.,0.,0.,i);
	  printf("Find peak from 137Cs, 661.7 keV\n");
	  printf("Energy  %f, channel  %f\n\n",e, ch);
	}
      if(strcmp(dat[i]->src,"am241")==0)
	{
	  printf("Data for 241Am calibration identified at ID %d\n",i);
	  e=59.5409;
	  cent_max_peak(e,dat[i]->spn,&ch,&dch);
	  ncdat=add_cdat_point(cdat,ncdat,e,ch,dch,0.,0.,0.,0.,0.,i);
	  printf("Find peak from 241Am, 59.5 keV\n");
	  printf("Energy  %f, channel  %f\n\n",e, ch);
	}
   
    }

  
  get_cal_par(cp,cdat,ndat);
  /* print_list(cdat,ndat,dat);  */  
  /* getc(stdin);  */
  for(i=0;i<ncdat;i++)
    free(cdat[i]);
  free(cdat);
}

//==============================================================================
int add_cdat_point(caldat **cdat,int ncdat,float e,float ch,float dch,float w,float dw,float a,float da,float chisq,int id)
{

  cdat[ncdat]=(caldat*) malloc(sizeof(caldat));
  cdat[ncdat]->sid=id;
  cdat[ncdat]->e=e;
  cdat[ncdat]->ch=ch;
  cdat[ncdat]->dch=dch;
  cdat[ncdat]->w=w;
  cdat[ncdat]->dw=dw;
  cdat[ncdat]->a=a;
  cdat[ncdat]->da=da;
  cdat[ncdat]->chisq=chisq;
  
  ncdat++;
  
 
  return ncdat;
}
//==============================================================================
void cent_max_peak(float e,int* spn,float* cent, float* dcent)
{
  int lo,hi,chmax;
  float area,fwhm,darea;
  
  chmax=find_ch_max(spn);
  lo=(int)floor((1.-DE/e)*chmax);
  hi=(int)ceil((1.+DE/e)*chmax);

  copy_to_gf3str(spn);
  find_ge_cent_mod(lo, hi, &area, cent, &fwhm, &darea, dcent);

}

//==============================================================================
void copy_to_gf3str(int* spn)
{
  int j;
  
  for(j=0;j<S32K;j++)
    gf3gd.spec[j]=(float)spn[j];
  gf3gd.maxch=S32K;
}
//==============================================================================
int read_par_file(char* fname, data **dat)
{
  FILE *inp;
  FILE *spn;
  char *line;
  int  nline=512;
  size_t sline;
  int ndat;
  int  spid,i,len;
 

  ndat=0;
  sline=nline*sizeof(char);
  line=(char*)malloc(sline);
  
  inp=fopen(fname,"r");

  while(getline(&line,&sline,inp)!=EOF)
    if(strchr(line,'#')==NULL)
      {
	dat[ndat]=(data*)malloc(sizeof(data));
	dat[ndat]->src=(char*)malloc(STRSZ*sizeof(char));
	dat[ndat]->sou=(char*)malloc(STRSZ*sizeof(char));
	dat[ndat]->fnm=(char*)malloc(STRSZ*sizeof(char));
	dat[ndat]->spn=(int*)malloc(S32K*sizeof(int));
	
	sscanf(line,"%s %s %s %d",dat[ndat]->src,dat[ndat]->sou,dat[ndat]->fnm,&spid);

	spn=fopen(dat[ndat]->fnm,"r");
	for(i=0;i<=spid;i++)
	  len=fread(dat[ndat]->spn,sizeof(int),S32K,spn);
	fclose(spn);
	
	if(len==S32K)
	  {
	    ndat++;
	  }
	else
	  {
	    printf("Spectrum %2d for spn file %s read incorrectly\n",spid,dat[ndat]->fnm);
	    printf("Length read is %d\n",len);
	    printf("Exiting\n");
	    exit(0);
	  }
      }
  free(line);
  fclose(inp);
 
  return ndat;
}
//=======================================================================
int find_ch_max(int* spn)
{
  int  chmax,cmax,i;

  chmax=S32K;
  cmax=0;
  for(i=THR;i<S32K-THR;i++)
    if(spn[i]>cmax)
      {
	cmax=spn[i];
	chmax=i;
      }

  return chmax;
}

//==============================================================================
//taken from radware, modified to eliminate display
int find_ge_cent_mod(int loch, int hich, 
		 float *area, float *cent, float *fwhm, float *darea, float *dcent)
{
  static float factor = 1.5f;
  float x, y, x1, y1, x2, y2, sx2y, dcsum, sigma;
  float dc, bg, oldcen, xm, flo, fhi, xhi, yhi, xlo, ylo, sxy, r1;
  int   maxw2, i, iteration, hi, lo;

  
  /* integrate peak over Cent +/- Factor*FWHM */
  lo = loch;
  hi = hich;
  maxw2 = (hi - lo) * (hi - lo) / 10;
  *area = 0.f;
  *cent = 0.f;
  *fwhm = 0.f;
  find_bg_mod(lo, hi, &x1, &y1, &x2, &y2);
  //printf("background %f %f %f %f\n",x1,y1,x2,y2);
  /* integrate peak over Cent +/- Factor*FWHM */
  oldcen = (float) (lo + hi) / 2.f;
  r1 = oldcen - (hich - loch) / 7.f;
  lo = rint(r1);
  r1 = oldcen + (hich - loch) / 7.f;
  hi = rint(r1);
  if (hi - lo < 2) {
    r1 = oldcen - 3;
    lo = rint(r1);
    hi = lo + 6;
  }

  flo = 1.f;
  fhi = 1.f;
  for (iteration = 1; iteration <= 20; ++iteration) {
    xm = oldcen;
    xlo = (float) lo + (1.f - flo) * .5f - xm;
    xhi = (float) hi - (1.f - fhi) * .5f - xm;
    bg = y1 - (x1 - (float) lo) * (y1 - y2) / (x1 - x2);
    ylo = (gf3gd.spec[lo] - bg) * flo;
    if(ylo<0)ylo=0.;// KS modification
    //ylo=gf3gd.spec[lo];
    dc = flo * flo * (gf3gd.spec[lo] + bg / 25.f);
    *darea = dc;

    dcsum = dc * (xlo * xlo);
    bg = y1 - (x1 - (float) hi) * (y1 - y2) / (x1 - x2);
    //    yhi=gf3gd.spec[hi];
    yhi = (gf3gd.spec[hi] - bg) * fhi;
    if(yhi<0) yhi=0.;//KS modification
    dc = fhi * fhi * (gf3gd.spec[hi] + bg / 25.f);
    *darea += dc;

    dcsum += dc * (xhi * xhi);
    *area = ylo + yhi;
    sxy = ylo * xlo + yhi * xhi;
    sx2y = ylo * xlo * xlo + yhi * xhi * xhi;
    //     printf("init xlo= %f ylo=%f xhi=%f yhi=%f sxy=%f sx2y=%f\n",xlo,ylo,xhi,yhi,sxy,sx2y);
    for (i = lo + 1; i <= hi-1; ++i) {
      x = (float) i - xm;
      bg = y1 - (x1 - (float) i) * (y1 - y2) / (x1 - x2);
      y = gf3gd.spec[i] - bg;
      if(y<0)y=0.;//KS modification
      *area += y;
      dc = gf3gd.spec[i] + bg / 25.f;
      *darea += dc;
      dcsum += dc * (x * x);
      sxy += y * x;
      sx2y += y * x * x;
      //      printf("x= %f y=%f sxy=%f sx2y=%f\n",x,y,sxy,sx2y);
    }

    if (*area < 10.f) {
      *area = 0.f;
      *darea = 0.f;
      *cent = 0.f;
      *fwhm = 0.f;
      //      printf("case 1\n");
      return 0;
    }
    *cent = sxy / *area;
    sigma = sx2y / *area - *cent * *cent;
    *cent += xm;
    //      printf("sx2y %f sigma %f cent %f area %f\n",sx2y,sigma,*cent,*area);
    if (sigma <= 0.f || sigma > (float) maxw2) {
      *area = 0.f;
      *darea = 0.f;
      *cent = 0.f;
      *fwhm = 0.f;
      //      printf("case 2\n");
      return 0;
    }

    sigma = sqrt(sigma);
    *fwhm = sigma * 2.355f;
    *dcent = sqrt(dcsum) / *area;
    *darea = sqrt(*darea);
    if ((r1 = *cent - oldcen, fabs(r1)) < *dcent / 20.f ||
	iteration == 20) {
      return 0;
    }

    oldcen = *cent;
    lo = (int) (*cent - factor * *fwhm);
    hi = (int) (*cent + factor * *fwhm);
    if (lo < 1 || hi > gf3gd.maxch) {
      *area = 0.f;
      *darea = 0.f;
      *cent = 0.f;
      *fwhm = 0.f;
      //      printf("case 4\n");
      return 0;
    }
    flo = factor * *fwhm - *cent + (float) (lo + 1);
    fhi = factor * *fwhm + *cent - (float) hi;
  }
  //  printf("case 5\n");
  return 0;
} /* find_ge_cent */
/* ======================================================================= */
int find_bg_mod(int loch, int hich, 
	    float *x1, float *y1, float *x2, float *y2)
{

  find_bg2_mod(loch, hich, x1, y1, x2, y2);
  if (*y1 <= 1.f) {
    *y1 = 1.5f;
  } else {
    *y1 += sqrt(*y1) * .7f;
  }
  if (*y2 <= 1.f) {
    *y2 = 1.5f;
  } else {
    *y2 += sqrt(*y2) * .7f;
  }

  
  return 0;
} /* find_bg */
/* ======================================================================= */
int find_bg2_mod(int loch, int hich, float *x1, float *y1, float *x2, float *y2)
{
  float a, y;
  int   i, mid;

  mid = (loch + hich) / 2;


  /* find channel with least counts on each side of middle */
  *y1 = (gf3gd.spec[loch] + gf3gd.spec[loch + 1] + gf3gd.spec[loch + 2]) / 3.f;
  *x1 = (float) (loch + 1);
  for (i = loch + 2; i <= mid - 2; ++i) {
    a = (gf3gd.spec[i-1] + gf3gd.spec[i] + gf3gd.spec[i+1]) / 3.f;
    if (*y1 > a) {
      *y1 = a;
      *x1 = (float) i;
    }
  }
  *y2 = (gf3gd.spec[mid + 2] + gf3gd.spec[mid + 3] + gf3gd.spec[mid + 4]) / 3.f;
  *x2 = (float) (mid + 3);
  for (i = mid + 4; i <= hich - 1; ++i) {
    a = (gf3gd.spec[i-1] + gf3gd.spec[i] + gf3gd.spec[i+1]) / 3.f;
    if (*y2 > a) {
      *y2 = a;
      *x2 = (float) i;
    }
  }
  /* check that there are no channels between loch and hich
     that are below the background. */
  if (*y2 > *y1) {
    for (i = rint(*x1) + 1; i <= mid - 2; ++i) {
      y = *y1 - (*x1 - (float) i) * (*y1 - *y2) / (*x1 - *x2);
      a = (gf3gd.spec[i-1] + gf3gd.spec[i] + gf3gd.spec[i+1]) / 3.f;
      if (y > a) {
	*y1 = a;
	*x1 = (float) i;
      }
    }
    for (i = rint(*x2) + 1; i <= hich - 1; ++i) {
      y = *y1 - (*x1 - (float) i) * (*y1 - *y2) / (*x1 - *x2);
      a = (gf3gd.spec[i-1] + gf3gd.spec[i] + gf3gd.spec[i+1]) / 3.f;
      if (y > a) {
	*y2 = a;
	*x2 = (float) i;
      }
    }
  } else {
    for (i = rint(*x1) - 1; i >= loch + 1; --i) {
      y = *y1 - (*x1 - (float) i) * (*y1 - *y2) / (*x1 - *x2);
      a = (gf3gd.spec[i-1] + gf3gd.spec[i] + gf3gd.spec[i+1]) / 3.f;
      if (y > a) {
	*y1 = a;
	*x1 = (float) i;
      }
    }
    for (i = rint(*x2) - 1; i >= mid + 3; --i) {
      y = *y1 - (*x1 - (float) i) * (*y1 - *y2) / (*x1 - *x2);
      a = (gf3gd.spec[i-1] + gf3gd.spec[i] + gf3gd.spec[i+1]) / 3.f;
      if (y > a) {
	*y2 = a;
	*x2 = (float) i;
      }
    }
  }
  if (*y1 < 1.f) *y1 = 1.f;
  if (*y2 < 1.f) *y2 = 1.f;
  return 0;
} /* find_bg2 */
/* ======================================================================= */
int matinv(double *array, int norder, int dim)
{
  double amax, save, d1;
  int i, j, k, ik[1000], jk[1000];

  for (k = 0; k < norder; ++k) {
    /* find largest element array(i,j) in rest of matrix */
    amax = 0.f;
    while (1) {
      for (i = k; i < norder; ++i) {
	for (j = k; j < norder; ++j) {
	  if (fabs(amax) - (d1 = array[i + j*dim], fabs(d1)) <= 0.f) {
	    amax = array[i + j*dim];
	    ik[k] = i;
	    jk[k] = j;
	  }
	}
      }
      if (amax == 0.f) return 0;
      /* interchange rows and columns to put amax in array(k,k) */
      i = ik[k];
      if (i < k) continue;
      if (i > k) {
	for (j = 0; j < norder; ++j) {
	  save = array[k + j*dim];
	  array[k + j*dim] = array[i + j*dim];
	  array[i + j*dim] = -save;
	}
      }
      j = jk[k];
      if (j >= k) break;
    }
    if (j > k) {
      for (i = 0; i < norder; ++i) {
	save = array[i + k*dim];
	array[i + k*dim] = array[i + j*dim];
	array[i + j*dim] = -save;
      }
    }
    /* accumulate elements of inverse matrix */
    for (i = 0; i < norder; ++i) {
      if (i != k) array[i + k*dim] = -array[i + k*dim] / amax;
    }
    for (i = 0; i < norder; ++i) {
      for (j = 0; j < norder; ++j) {
	if (i != k && j != k)
	  array[i + j*dim] += array[i + k*dim] * array[k + j*dim];
      }
    }
    for (j = 0; j < norder; ++j) {
      if (j != k) array[k + j*dim] /= amax;
    }
    array[k + k*dim] = 1.f / amax;
  }
  /* restore ordering of matrix */
  for (k = norder-1; k >= 0; --k) {
    j = ik[k];
    if (j > k) {
      for (i = 0; i < norder; ++i) {
	save = array[i + k*dim];
	array[i + k*dim] = -array[i + j*dim];
	array[i + j*dim] = save;
      }
    }
    i = jk[k];
    if (i > k) {
      for (j = 0; j < norder; ++j) {
	save = array[k + j*dim];
	array[k + j*dim] = -array[i + j*dim];
	array[i + j*dim] = save;
      }
    }
  }
  return 0;
} /* matinv */

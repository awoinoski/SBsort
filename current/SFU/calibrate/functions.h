#ifndef _FUNC_
#define _FUNC_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <signal.h>
#include <time.h>

#include "lin_eq_solver.h"

#define S32K   32768
#define STRSZ  128
#define THR    20
#define DE     10.0

typedef struct data
{
  char *src;
  char *sou;
  char *fnm;
  int  *spn;
} data;

typedef struct caldat
{
  int  sid;
  float e;
  float ch;
  float dch;
  float w;
  float dw;
  float a;
  float da;
  float res;
  float chisq;
} caldat;


typedef struct gf3str
{
  double gain[6];
  float  finest[5], swpars[3];              /* initial parameter estimates */
  int    infix[3], infixrw, infixw;         /*  and flags */
  int    ical,nterms;
  int    maxch;
  float  ppos[15];
  int    mch[2]; 
  float  pars[51], errs[51];                
  float  areas[15], dareas[15], cents[15];
  int    npars, nfp, freepars[51], npks, irelw, irelpos;
  int    wtmode;           /* fit weight mode and spectrum */
  float  wtsp[32768];
  char   nwtsp[8];
  char   namesp[8], filnam[80];
  float  spec[32768];
  float  chisq;
} gf3str;

typedef struct calpar
{
  double a;
  double b;
  double c;
} calpar;



extern gf3str gf3gd;
extern float derivs[51];
extern int   gotsignal;
extern int   ready;

void gfset(void);
int dofit(void);
void gfinit(void);
void cal_par(calpar*,caldat**,int);
void get_cal_par(calpar*,caldat**,int);
void first_pass_cal(data**,int ,calpar*);
int add_cdat_point(caldat**,int,float,float,float,float,float,float,float,float,int);
int read_par_file(char*, data**);
int find_ch_max(int*);
void copy_to_gf3str(int*);
void cent_max_peak(float,int*,float*, float*);
int find_ge_cent_mod(int, int,float*,float*, float *, float *, float *);//from radware
int find_bg_mod(int, int, float *, float *, float *, float *);//from radware
int find_bg2_mod(int, int, float *, float *, float *, float *);//from radware
int typeit(int);//from radware
int wrresult(char*, float, float, int);//from radware
int energy(float, float, float*, float *);//from radware
void breakhandler(int);//from radware
int datetime(char *);//from radware
int eval(float *, int, float *, int, int);//from radware
int fitter(int);//from radware
int matinv(double*, int, int);//from radware
int gffin(void);//from radware
int parset(int);//from radware
void set_gf3_cal(calpar *);
void print_list(caldat**,int, data**);
int fit_singlet(float, calpar*,caldat**,int,int);
int fit_doublet(float, float, calpar*,caldat**,int,int);
void cal_par(calpar*,caldat**,int);
void sort_list(caldat**, int);
void inv_fit(caldat**,int,double*,calpar*,calpar*);
double chisq_e0(double,caldat**,int,lin_eq_type*);
void FWHM_fit(caldat**,int , calpar *);
#endif

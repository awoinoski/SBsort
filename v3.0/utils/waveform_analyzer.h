#ifndef WAVEFORM_ANALYZER_H
#define WAVEFORM_ANALYZER_H

#define NSHAPE     5  //number of trial functions for waveform fit
#define BADCHISQ_SMOOTH_T0   -1024-2 //smooth_t0 gives bad result
#define BADCHISQ_PAR_T0      -1024-3 //parabolic_t0 gives bad result
#define BADCHISQ_LIN_T0      -1024-4 //linear_t0 gives bad result
#define BADCHISQ_MAT         -1024-5 //matrix for fit is not invertable
#define BAD_BASELINE_RANGE   -1024-11
#define BASELINE_RANGE 16
#define T0RANGE            1
#define LARGECHISQ         1E111
#define FILTER 8 //integration region for noise reduction (in samples)


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
//For ROOT
#include "TH1D.h"
#include "TF1.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TApplication.h"
#include "TROOT.h"
#include "TLine.h"

#include "lin_eq_solver.h"
#include "tig-format.h"

typedef struct
{
  //parameters for baseline
  int    baseline_range;
  double baseline; //baseline
  double baselineStDev; //baseline variance
  int    bflag; //flag for baseline determiniation

  //paremeters for exclusion zone
  double max; //max value of the filtered waveform 
  double tmax; //x position of the max value of the filtered waveform
  int    temax; //x position, exclusion zone upper limit
  int    temin; //x position, exclusion zone lower limit
  int    mflag; //flag for tmax found

  int   nimtr;
  int   nimtf;
  double   nimh;
  
  double t50;
  double t30;  
  int t30_flag;
  int t50_flag;

  double t0; 
  double b0;
  double b1;
  long double s0;
  long double s1;
  long double s2;
  int    thigh;
}WaveFormPar;

typedef struct
{
  double slope;
  double intercept;
  double chisq;
  double ndf;
}LinePar;

typedef struct
{
  double constant;
  double linear;
  double quadratic;
  double chisq;
  double ndf;
}ParPar;

void     display_T0_fit(int, short*, WaveFormPar* ,TApplication*);
void     display_NIM_fit(int, short*, WaveFormPar* ,TApplication*);

int      fit_line(short*,int,int,LinePar*);
int      fit_parabola(short*,int,int,ParPar*);
int      fit_smooth_parabola(short*,int,int,double,ParPar*);
double   fit_NIMT0(int, short*, WaveFormPar*);
double   fit_NIM(int, short*, WaveFormPar*);
double   fit_T0(int, short*, WaveFormPar*);

void     get_baseline(int, short*, WaveFormPar*);
void     get_t30(int, short*, WaveFormPar*);
void     get_t50(int, short*, WaveFormPar*);
void     get_tmax(int, short*, WaveFormPar*);
double   get_tfrac(int, short*, WaveFormPar*,double,double,double);
double   get_linear_T0(int,short*,WaveFormPar*);
double   get_parabolic_T0(int,short*,WaveFormPar*);
double   get_smooth_T0(int,short*,WaveFormPar*);

void     print_WavePar(WaveFormPar*);
void     print_NIMWavePar(WaveFormPar*);

double   get_QDC3(int,short*, WaveFormPar*);
#endif

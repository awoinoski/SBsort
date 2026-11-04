#ifndef __SB_CAL_H
#define __SB_CAL_H

#include "SB-format.h"


#define NPOS           8
#define NTRIG          4
#define NAUX           4
#define SB_TBIT   0x1000
#define SB_DBIT   0x2000
#define SB_ABIT   0x2000
#define MAX_DET_BITS 16

#define BUFFER_TAG        0xfafafafa
#define EVENT_TAG         0xabcdabcd
#define CHANNEL_TAG       0xabcd0000
#define HEADER_TAG        0xabcd0001
#define SBT_TAG           0xabcd00a1
#define SBC_TAG           0xabcd00a2
#define SBT_FIT_TAG       0xabcd00a3
#define SBA_TAG           0xabcd00a4

typedef struct cChannel
{
  int timestamp;
  int timestamp_up;
  int charge;
  int cfd;
}cChannel;


typedef struct cSheader
{
  int Efold;
  int Tfold;
  int TSfold;
  int EHP;
  int THP;
  int TSHP;
} cSheader;

typedef struct cRheader
{
  int   trig_num;
  int   timestamp_up_min;
  int   timestamp_up_max;
  int   timestamp_min;
  int   timestamp_max;
  int   setupHP;
  int   frag_fold;
} cRheader;

typedef struct cNimfit
{
  int      tr;
  int      tf;
  double   h;
  double   b;
} cNimfit;

typedef struct cSBTRG
{
  cSheader  h;
  cChannel  trig[NTRIG];
  cNimfit   fit[NTRIG];  
}cSBTG;

typedef struct cSBCSS
{
  cSheader  h;
  cChannel  hpge[NPOS];
  float     t0[NPOS];
}cSBCSS;

typedef struct cSBAUX
{
  cSheader  h;
  cChannel  aux[NAUX];
  float     t0[NAUX];
  double    q[NAUX];
}cSBAUX;

typedef struct cal_event
{
  cRheader h;
  cSBTRG   sbt;
  cSBCSS   sbc;
  cSBAUX   sba;
}cal_event;


typedef struct HPGe_cal_par
{
  int   ceflag[NPOS];
  float ce[NPOS][3];
  float contr_e;
}HPGe_cal_par;


void display_raw_event(cal_event*);
void display_rheader(cRheader*);
void display_sheader(cSheader*);
void display_channel(int,cChannel*);
void display_channel_legend();
void display_NIMFit_legend();
void display_NIMFit(int,cNimfit*);

int verify_ts(cal_event*,unsigned long long int*,unsigned long long int*);
int verify_ts_trig(cSBTRG*,unsigned long long int*,unsigned long long int*);
int verify_ts_hpge(cSBCSS*,unsigned long long int*,unsigned long long int*);
int verify_ts_aux(cSBAUX*,unsigned long long int*,unsigned long long int*);
#endif

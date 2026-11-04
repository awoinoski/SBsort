#ifndef __SB_FORMAT_H
#define __SB_FORMAT_H

#include "waveform_analyzer.h"

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

typedef struct channel
{
  int timestamp;
  int timestamp_up;
  int charge;
  int cfd;
}channel;


typedef struct sheader
{
  int   Efold;
  int   Tfold;
  int   TSfold;
  int EHP;
  int THP;
  int TSHP;
} sheader;

typedef struct rheader
{
  int   trig_num;
  int   timestamp_up_min;
  int   timestamp_up_max;
  int   timestamp_min;
  int   timestamp_max;
  int   setupHP;
  int   frag_fold;
} rheader;

typedef struct nimfit
{
  int      tr;
  int      tf;
  double   h;
  double   b;
} nimfit;

typedef struct SBTRG
{
  sheader  h;
  channel  trig[NTRIG];
  nimfit   fit[NTRIG];  
}SBTG;

typedef struct SBCSS
{
  sheader  h;
  channel  hpge[NPOS];
  float    t0[NPOS];
}SBCSS;

typedef struct SBAUX
{
  sheader  h;
  channel  aux[NAUX];
  float    t0[NAUX];
  double   q[NAUX];
}SBAUX;

typedef struct raw_event
{
  rheader h;
  SBTRG   sbt;
  SBCSS   sbc;
  SBAUX   sba;
}raw_event;

void display_raw_event(raw_event*);
void display_rheader(rheader*);
void display_sheader(sheader*);
void display_channel(int,channel*);
void display_channel_legend();
void display_NIMFit_legend();
void display_NIMFit(int,nimfit*);

int verify_ts(raw_event*,unsigned long long int*,unsigned long long int*);
int verify_ts_trig(SBTRG*,unsigned long long int*,unsigned long long int*);
int verify_ts_hpge(SBCSS*,unsigned long long int*,unsigned long long int*);
int verify_ts_aux(SBAUX*,unsigned long long int*,unsigned long long int*);
#endif

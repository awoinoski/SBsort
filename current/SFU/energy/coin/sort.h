#ifndef SORT_H
#define SORT_H

#define S16K 16384
#define S8K 8192
#define S4K 4096

#include "SB-common.h"
#include "SB-format.h"
#include "SB-decoder.h"

#include "TH2D.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TApplication.h"


int  hist[NPOS+1][S32K];
TH2D *h;

float aaux=-18.584691149774244;
float baux=0.6003301649401459;
float caux=5.621641307404634e-06;

float ahpge=0.25299465971934615;
float bhpge=0.10846973408260442;
float chpge=-3.118749375785498e-09;

#endif

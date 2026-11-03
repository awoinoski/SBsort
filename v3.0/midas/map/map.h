#ifndef __MAP_H
#define __MAP_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tig-format.h"
#include "SB-format.h"
#include "waveform_analyzer.h"

typedef struct sbmap
{
  int sb_trig_fit;
  int sb_trig_map[NTRIG];
  int sb_hpge_min;  
  int sb_hpge_max;
  int sb_hpge_fit;
  int sb_hpge_kpar;
  int sb_hpge_map[NPOS];
  int sb_aux_min;  
  int sb_aux_max;
  int sb_aux_fit;
  int sb_aux_kpar;
  int sb_aux_map[NAUX];
} sbmap;

void map_event(Tig10_event *, short*, raw_event*,sbmap*,int);
void read_map(char*,sbmap*);
void print_map(sbmap*);
#endif

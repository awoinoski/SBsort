#ifndef _XLM_H
#define _XLM_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "SB-common.h"

#define  NSCLMDGG 8
#define  NSCLXLM 75


typedef struct xlm_scalar_struct{
  int MDGG_scalers[NSCLMDGG];
  int XLM_scalers[NSCLXLM];
} xlm_event;


int  unpack_xlm_bank(int*, int);
extern void print_xlm_scalers(xlm_event*);

#endif

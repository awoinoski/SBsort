#ifndef __SB_COMMON_H
#define __SB_COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>



#define S1K     1024
#define S2K     2048
#define S4K     4096
#define S8K     8192
#define S16K   16384
#define S32K   32768
#define S65K   65536

#define BUFFSIZE S8K

typedef struct counters
{
  long buffer;
  long event;
}counters;

#endif

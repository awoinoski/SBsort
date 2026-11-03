#ifndef STATE_MACHINE_SBNA_H
#define STATE_MACHINE_SBNA_H

#define NEXT_EVENT        0
#define GET_FRAGMENTS     1
#define END_OF_RECORD     2
#define END_OF_FILE       3
#define ANALYZE_DATA      4



#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "midas-format.h"
#include "xlm-format.h"
#include "tig-format.h"

void sort_but_not_assemble(char*);
int  get_fragments(long*,int, int*);
extern int analyze_fragment(Tig10_event*,short*);
#endif

#ifndef READ_MASTER_H
#define READ_MASTER_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

struct fname
{
  char inp_data[132];
  char out_data[132];
  char out_data_list[132]; 
  char cluster_file[132];
  char root_output_file[132];
  char root_gate_file[132];
  char gate_name_file[132];
};


struct flag
{
  int inp_data;
  int out_data;
  int out_data_list;
  int separate;
  int cluster_file;
  int root_output_file;
  int root_gate_file;
  int gate_name_file;
};

typedef struct
{
  struct fname fname;
  struct flag flag;
}input_names_type;

void read_master(char *name, input_names_type *inp_names);


#endif

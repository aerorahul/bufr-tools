#include <iostream>
#include "bufr_tools.h"


#ifdef FORTRAN_UNDERSCORE
#define __FORTRAN_FN_NAME(func_name) func_name ##_
#else
#define __FORTRAN_FN_NAME(func_name) func_name
#endif

#define FORTRAN_FN_NAME(func_name) __FORTRAN_FN_NAME(func_name)


extern "C"
{
  void FORTRAN_FN_NAME(count_messages)(const char* finput, int* nimsg, int* nirep, int finput_len);
  void read_bufrdata(const char* finput, int nchanl, BufrData** bdata);
  void print_bufrdata(int num_msgs, BufrData** bdata);
}


void count_messages(string filepath, int* num_messages, int* num_sections)
{
  FORTRAN_FN_NAME(count_messages)(filepath.c_str(), num_messages, num_sections, (int)filepath.length());
}


BufrData* read_bufrdata(string filepath, int num_channels, int num_msgs, int num_reps)
{
  //Initialize memory for BufrData
  BufrData* bufrData = new BufrData[num_reps];
  for (unsigned int rep_idx = 0; rep_idx<num_reps; rep_idx++)
  {
    bufrData[rep_idx].nchanl = num_channels;
    bufrData[rep_idx].bufr_data = new double[num_channels];
  };

  read_bufrdata(filepath.c_str(), num_channels, &bufrData);

  return bufrData;
}

void print_bufrdata(int num_msgs, BufrData* bdata)
{
  print_bufrdata(num_msgs, &bdata);
}


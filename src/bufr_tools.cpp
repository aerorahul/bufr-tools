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
  void FORTRAN_FN_NAME(count_messages)(const char* finput, f_int* nimsg, f_int* nirep, f_int finput_len);
  void read_bufrdata(const char* finput, f_int* nchanl, BufrData** bdata);
  void print_bufrdata(f_int* nirep, BufrData** bdata);
}


void count_messages(string filepath, f_int* num_messages, f_int* num_sections)
{
  FORTRAN_FN_NAME(count_messages)(filepath.c_str(), num_messages, num_sections, (f_int)filepath.length());
}


BufrDataList read_bufrdata(string filepath, f_int num_channels, f_int num_msgs, f_int num_reps)
{

  //Initialize memory for BufrData
  BufrData* bufrData = new BufrData[num_reps];
  for (unsigned int rep_idx = 0; rep_idx<num_reps; rep_idx++)
  {
    bufrData[rep_idx].nchanl = num_channels;
    bufrData[rep_idx].bufr_data = new f_real[num_channels];
  };

  cout << "C++ " << sizeof(BufrData) << endl;

  read_bufrdata(filepath.c_str(), &num_channels, &bufrData);
  print_bufrdata(&num_reps, &bufrData);

  return BufrDataList();
}


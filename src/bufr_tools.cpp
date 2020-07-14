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
	void FORTRAN_FN_NAME(read_bufrdata)(const char* finput, f_int* nchanl, f_int* nimsg, f_int* nirep, BufrData* bdata, f_int finput_len);
}


void count_messages(string filepath, f_int* num_messages, f_int* num_sections)
{
	FORTRAN_FN_NAME(count_messages)(filepath.c_str(), num_messages, num_sections, (f_int)filepath.length());
}

BufrDataList read_bufrdata(string filepath, f_int num_channels, f_int num_msgs, f_int num_reps)
{
	BufrData* bufrData = new BufrData[num_reps];
	FORTRAN_FN_NAME(read_bufrdata)(filepath.c_str(), &num_channels, &num_msgs, &num_reps, bufrData, (f_int)filepath.length());

	cout << "Num Channels: " << num_channels << " Num Msgs: " << num_msgs << " Num Reps " << num_reps;

	return BufrDataList();
}


//
//  bufrlib_wrapper.h
//  ioda-frontend
//
//  Created by Ronald McLaren on 7/6/20.
//  Copyright © 2020 NOAA. All rights reserved.
//

#ifndef bufrlib_wrapper_h
#define bufrlib_wrapper_h

#include <string>

using namespace std;

typedef long FortranFileUnit;

typedef enum
{
    In,
    Out,
    Sec3,
    Nul,
    Nodx,
    Quiet
} BufrMode;

void open_file(FortranFileUnit file_id, string filename, string action);
void close_file(FortranFileUnit file_id);

void open_bufr(FortranFileUnit bufr_id, BufrMode mode, FortranFileUnit bufr_table_id);
void close_bufr(FortranFileUnit bufr_id);

int read_next_msg(FortranFileUnit bufr_id, string& subset, int* date_id);
void open_next_msg(FortranFileUnit bufr_id, string& subset, FortranFileUnit bufr_table_id);

int read_msg_subset(FortranFileUnit bufr_id);
void dump_table_data(FortranFileUnit bufr_id, FortranFileUnit output_id);

#endif /* bufrlib_wrapper_h */

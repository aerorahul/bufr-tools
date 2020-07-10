//
//  bufrlib_wrapper.cpp
//  ioda-frontend
//
//  Created by Ronald McLaren on 7/6/20.
//  Copyright © 2020 NOAA. All rights reserved.
//

#include <stdio.h>
#include <iostream>

#include "bufrlib_wrapper.h"


//On some platforms the compiler for fortran will append an underscore character to
//preserve reverse compatility with older versions of python. The following macro
//adds an underscore when specified by the preprocessor flag FORTRAN_UNDERSCORE.

#ifdef FORTRAN_UNDERSCORE
#define __FORTRAN_FN_NAME(func_name) func_name ##_
#else
#define __FORTRAN_FN_NAME(func_name) func_name
#endif

#define FORTRAN_FN_NAME(func_name) __FORTRAN_FN_NAME(func_name)

#ifdef F77_INTSIZE_8
    typedef long f77int;
#else
    typedef int f77int;
#endif

static const int SUBSET_STR_LEN = 10;
//
//CALL UFBINT  ( LUBFR, R8ARR, MXMN, MXLV, NLV, CMNSTR )
//
//CALL UFBREP  ( LUBFR, R8ARR, MXMN, MXLV, NLV, CMNSTR )
//
//CALL UFBSEQ  ( LUBFR, R8ARR, MXMN, MXLV, NLV, CMNSTR )

extern "C"
{
    //From Fortran IO
    void FORTRAN_FN_NAME(open_fortran_file)(f77int* unit, const char* filepath, const char* action, f77int file_path_len, f77int action_str_len);
    void FORTRAN_FN_NAME(close_fortran_file)(f77int* unit);

    //From NCEP BUFR Lib
    void FORTRAN_FN_NAME(openrb)(f77int* UNIT, const char* FILE);
    void FORTRAN_FN_NAME(openbf)(f77int* LUBFR, const char* CIO, f77int* LUNDX, f77int cio_str_len);
    void FORTRAN_FN_NAME(closbf)(f77int* LUBFR);
    void FORTRAN_FN_NAME(readmg)(f77int* LUBFR, const char* CSUBSET, f77int* IDDATE, f77int* IRET, f77int subset_str_len);
    void FORTRAN_FN_NAME(openmb)(f77int* LUBFR, const char* CSUBSET, f77int* IDDATE, f77int subset_str_len);
    void FORTRAN_FN_NAME(readsb)(f77int* LUBFR, f77int* IRET);

    void FORTRAN_FN_NAME(ufbint)(f77int* LUBFR, double** R8ARR, f77int* MXMN, f77int* MXLV, f77int* NLV, const char* CMNSTR, f77int size_of_cmnstr);
    void FORTRAN_FN_NAME(ufbrep)(f77int* LUBFR, double** R8ARR, f77int* MXMN, f77int* MXLV, f77int* NLV, const char* CMNSTR, f77int size_of_cmnstr);
    void FORTRAN_FN_NAME(ufbseq)(f77int* LUBFR, double** R8ARR, f77int* MXMN, f77int* MXLV, f77int* NLV, const char* CMNSTR, f77int size_of_cmnstr);

    void FORTRAN_FN_NAME(dxdump)(f77int* LUBFR, f77int* OUTFILE);
}

void open_file(FortranFileUnit file_id, string filename, string action)
{
    f77int f_file_id = (f77int)file_id;
    FORTRAN_FN_NAME(open_fortran_file)(&f_file_id,
                                       filename.c_str(),
                                       action.c_str(),
                                       (f77int)filename.length(),
                                       (f77int)action.length());
}

void close_file(FortranFileUnit file_id)
{
    f77int f_file_id = (f77int)file_id;
    FORTRAN_FN_NAME(close_fortran_file)(&f_file_id);
}

void open_bufr(FortranFileUnit bufr_id, BufrMode mode, FortranFileUnit bufr_table_id)
{
    string mode_str = "";
    
    switch (mode)
    {
        case In:
            mode_str = "IN";
            break;
        case Out:
            mode_str = "OUT";
            break;
        case Sec3:
            mode_str = "SEC3";
            break;
        case Nul:
            mode_str = "NUL";
            break;
            
        default:
            break;
    }
    
    int str_len = (int)mode_str.length();
    
    f77int f_bufr_id = (f77int)bufr_id;
    f77int f_bufr_table_id = (f77int)bufr_table_id;
    FORTRAN_FN_NAME(openbf)(&f_bufr_id,
                            mode_str.c_str(),
                            &f_bufr_table_id,
                            str_len);
}

void close_bufr(f77int bufr_id)
{
    FORTRAN_FN_NAME(closbf)(&bufr_id);
}

int read_next_msg(FortranFileUnit bufr_id, string& subset, int* date_id)
{
    f77int result;
    char subset_str_buf[SUBSET_STR_LEN];
    f77int f_bufr_id = (f77int)bufr_id;
    f77int f_date_id;
    FORTRAN_FN_NAME(readmg)(&f_bufr_id, subset_str_buf, &f_date_id, &result, SUBSET_STR_LEN);
    subset.assign(subset_str_buf, SUBSET_STR_LEN);
    *date_id = (int) f_date_id;
    
    return (int)result;
}

void open_next_msg(FortranFileUnit bufr_id, string& subset, int* date_id)
{
    char subset_str_buf[SUBSET_STR_LEN];
    f77int f_bufr_id = (f77int)bufr_id;
    f77int f_date_id = (f77int)date_id;
    FORTRAN_FN_NAME(openmb)(&f_bufr_id, subset_str_buf, &f_date_id, SUBSET_STR_LEN);
    subset.assign(subset_str_buf, SUBSET_STR_LEN);
}

int read_msg_subset(FortranFileUnit bufr_id)
{
    f77int result;
    f77int f_bufr_id = (f77int)bufr_id;
    FORTRAN_FN_NAME(readsb)(&f_bufr_id, &result);
    return (int)result;
}

void dump_table_data(FortranFileUnit bufr_id, FortranFileUnit output_id)
{
    f77int f_bufr_id = (f77int)bufr_id;
    f77int f_output_id = (f77int)output_id;
    FORTRAN_FN_NAME(dxdump)(&f_bufr_id, &f_output_id);
}


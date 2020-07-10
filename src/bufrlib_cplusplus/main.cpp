//
//  main.cpp
//  ioda-frontend
//
//  Created by Ronald McLaren on 7/6/20.
//  Copyright © 2020 NOAA. All rights reserved.
//

#include <iostream>

#include "bufrlib_wrapper.h"

int main(int argc, const char * argv[])
{
    string bufr_path = "/Users/rmclaren/Work/ioda-frontend/sample-data/gdas/gdas.20200704/12/gdas.t12z.1bamua.tm00.bufr_d";
    
    FortranFileUnit bufr_id = 11;
    
    
    //Write table data
    open_file(bufr_id, bufr_path, "READWRITE");
    open_bufr(bufr_id, In, bufr_id);

    string output_path = "/Users/rmclaren/Work/ioda-frontend/1bamua.table.txt";
    FortranFileUnit out_id = 12;
    open_file(out_id, output_path, "READWRITE");
    dump_table_data(bufr_id, out_id);
    
    close_bufr(bufr_id);
    close_file(bufr_id);
    close_file(out_id);
    
    
    //Count the number of message subsets
    int num_subsets = 0;
    string subset;
    int idate;
    open_file(bufr_id, bufr_path, "");
    open_bufr(bufr_id, In, bufr_id);
    
    while (read_next_msg(bufr_id, subset, &idate) == 0)
    {
        while (read_msg_subset(bufr_id) == 0)
        {
            num_subsets++;
        }
    }
    
    cout << num_subsets << endl;
    
    close_bufr(bufr_id);
    close_file(bufr_id);
    
    return 0;
}


#include <string>
#include <iostream>

using namespace std;

#include "read_bufr.h"

int main(int argc, const char * argv[])
{
    string filepath = "/Users/rmclaren/Work/sample-bufr-data/gdas/gdas.20200704/12/gdas.t12z.1bamua.tm00.bufr_d";

    f_int num_msgs;
    f_int num_reps;
    count_messages(filepath, &num_msgs, &num_reps);
    cout << "Num msgs: " << num_msgs << " Num secs: " << num_reps << endl;

    f_int num_channels = 15;
    read_bufrdata(filepath, num_channels, num_msgs, num_reps);

    return 0;
}
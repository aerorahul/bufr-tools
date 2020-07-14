
#include <string>
#include <vector>

using namespace std;

typedef int f_int;
typedef double f_real;


typedef struct __attribute__((packed))
{
	f_int nchanl;
    f_int satid;
    f_int ifov;
    f_int dtime[6];
    f_real olat, olon;
    f_real terrain;
    f_real lza, sza;
    f_real sat_aziang, sol_aziang;
    f_real* bufrdata;
}  BufrData;

typedef vector<BufrData> BufrDataList;


void count_messages(string filepath, f_int* num_messages, f_int* num_sections);
BufrDataList read_bufrdata(string filepath, f_int num_channels, f_int num_messages, f_int num_reps);

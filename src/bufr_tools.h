
#include <string>
#include <vector>

using namespace std;


typedef struct
{
	int nchanl;
    int satid;
    int ifov;
    int dtime[6];
    double olat, olon;
    double terrain;
    double lza, sza;
    double sat_aziang, sol_aziang;
    double* bufr_data;
}  BufrData;


void count_messages(string filepath, int* num_messages, int* num_sections);
BufrData* read_bufrdata(string filepath, int num_channels, int num_messages, int num_reps);
void print_bufrdata(int num_msgs, BufrData* bdata);


#include <time.h>
#include "BufrFile.h"

using namespace BufrTools;
using namespace std;

const string INPUT_FILE = "/Users/rmclaren/Work/sample-bufr-data/gdas/gdas.20200704/12/gdas.t12z.1bhrs4.tm00.bufr_d";

int main(int argc, const char** argv) 
{
  //Read the Buffer Data
  auto bufrFile = BufrFile(INPUT_FILE, 15);

  clock_t tStart = clock();
  bufrFile.readData();
  printf("Time taken to read: %.2fs\n\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);

  bufrFile.printData(10);


  //Generate IODA Engines ObsSpace
  auto reports = bufrFile.getReports();



  return 0;
}
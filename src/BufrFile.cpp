#include <math.h>
#include <iostream>
#include <iomanip>

#include "BufrFile.h"
#include "bufr.interface.h"

using namespace BufrParser;
using namespace std;


typedef struct
{
  double satid;
  double ifov;
  double year;
  double month;
  double day;
  double hour;
  double minute;
  double second;
  double clat;
  double clon;
  double clath;
  double clonh;
  double terrain;
}  Header_1;

typedef struct
{
  double lza;
  double sza;
  double sat_aziang;
  double sol_aziang;
}  Header_2;

static const int FORTRAN_FILE_UNIT = 11;
static const char* HEADER_1_MNEMONIC = "SAID FOVN YEAR MNTH DAYS HOUR MINU SECO CLAT CLON CLATH CLONH HOLS";
static const char* HEADER_2_MNEMONIC = "SAZA SOZA BEARAZ SOLAZI";

BufrFile::BufrFile(string filepath, unsigned int numChannels) :
  filepath_(filepath),
  numChannels_(numChannels)
{
}

void BufrFile::readData()
{
  const int HEADER_1_SIZE = sizeof(Header_1)/sizeof(double);
  const int HEADER_2_SIZE = sizeof(Header_2)/sizeof(double);

  open_f(FORTRAN_FILE_UNIT, filepath_.c_str());
  openbf_f(FORTRAN_FILE_UNIT, "IN", FORTRAN_FILE_UNIT);

  char subset[25];
  int iddate;
  int result;


  while (ireadmg_f(FORTRAN_FILE_UNIT, subset, &iddate, 25) == 0)
  {
    while (ireadsb_f(FORTRAN_FILE_UNIT) == 0)
    {
      Report report;

      //Read header 1 data
      auto* header1 = new Header_1;
      ufbint_f(FORTRAN_FILE_UNIT, (void**)&header1, HEADER_1_SIZE, 1, &result, HEADER_1_MNEMONIC);

      report.nchanl = numChannels_;
      report.satid = (int)header1->satid;
      report.ifov = (int)header1->ifov;

      report.dtime[0] = (int)header1->year; //year
      report.dtime[1] = (int)header1->month; //month
      report.dtime[2] = (int)header1->day; //day
      report.dtime[3] = (int)header1->hour; //hour
      report.dtime[4] = (int)header1->minute; //minute
      report.dtime[5] = (int)header1->second; //second

      double lat = 0;
      double lon = 0;
      if (abs(header1->clath) <= 90 && abs(header1->clonh) <= 360)
      {
        lat = header1->clath;
        lon = header1->clon;
      }
      else if (abs(header1->clat) <= 90 && abs(header1->clon) <= 360)
      {
        lat = header1->clat;
        lon = header1->clon;
      }

      if (lon < 0) lon = lon + 360;
      if (lon >= 360) lon = lon - 360;

      report.olat = lat;
      report.olon = lon;

      report.terrain = 0.01 * abs(header1->terrain);

      free(header1);

      //Read header 2 data
      auto* header2 = new Header_2;

      ufbrep_f(FORTRAN_FILE_UNIT, (void**)&header2, HEADER_2_SIZE, 1, &result, HEADER_2_MNEMONIC);

      report.lza = header2->lza;
      report.sza = header2->sza;
      report.sat_aziang = header2->sat_aziang;
      report.sol_aziang = header2->sol_aziang;

      free(header2);

      //Read bufr data
      double* tmbr_data = new double[numChannels_];
      ufbrep_f(FORTRAN_FILE_UNIT, (void**)&tmbr_data, 1, numChannels_, &result, "TMBR");

      report.bufr_data.reset(tmbr_data);

      //Save the result
      reports_.push_back(report);
    }
  } 

  closbf_f(FORTRAN_FILE_UNIT);
  close_f(FORTRAN_FILE_UNIT);
}

void BufrFile::countMessages()
{
  open_f(FORTRAN_FILE_UNIT, filepath_.c_str());
  openbf_f(FORTRAN_FILE_UNIT, "IN", FORTRAN_FILE_UNIT);

  int num_msgs = 0;
  int num_reports = 0;

  char subset[25];
  int iddate;
  while (ireadmg_f(FORTRAN_FILE_UNIT, subset, &iddate, 25) == 0)
  {
      num_msgs++;

      while (ireadsb_f(FORTRAN_FILE_UNIT) == 0)
      {
          num_reports++;
      }
  } 

  closbf_f(FORTRAN_FILE_UNIT);
  close_f(FORTRAN_FILE_UNIT);

  cout << filepath_ << endl;
  cout << "contains " << num_msgs << " messages and " << num_reports << " reports." << endl;
}


void BufrFile::printData(int maxLinesToPrint)
{
  if  (!reports_.empty())
  {
    int idx = 0;
    cout.precision(5);
    for (const auto& report : reports_)
    {
      for (unsigned int channel_idx = 0; channel_idx < report.nchanl; channel_idx++)
      {
        cout << setw(7)  << report.bufr_data.get()[channel_idx] << " ";
      }
      cout << endl;

      if (maxLinesToPrint > 0 && ++idx >= maxLinesToPrint) break;
    }
  }
  else
  {
    cout << "No BUFR data to print. Do you need to call read?" << endl;
  }
}

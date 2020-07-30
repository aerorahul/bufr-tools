#include <time.h>
#include "BufrFile.h"

#include "Eigen/Dense"

#include "ioda/defs.h"
#include "ioda/ObsGroup.h"
#include "ioda/Engines/Factory.h"


using namespace std;
using namespace BufrTools;
using namespace ioda;

const string INPUT_FILE = "/Users/rmclaren/Work/sample-bufr-data/gdas/gdas.20200704/12/gdas.t12z.1bmhs.tm00.bufr_d";
const string OUTPUT_FILE = "/Users/rmclaren/Temp/ioda.nc";


ObsGroup createObsGroup(Reports reports, int numChannels)
{
  std::vector<int> nChans(numChannels);
	std::iota(nChans.begin(), nChans.end(), 1);

  std::vector<int> nlocs(reports.size());
	std::iota(nlocs.begin(), nlocs.end(), 1);

  string file_output = "/Users/rmclaren/Temp/obsgroup.dat";

  ObsGroup og = ObsGroup::createObsGroupFile(OUTPUT_FILE, true);
  og.createDimScale("nchans", nChans);
  og.createDimScale("nlocs", nlocs, true);

	Variable nchans_var = og.openDimScale("nchans");
  Variable nlocs_var = og.openDimScale("nlocs");

  VariableCreationParameters float_params = og.initVarCreateParams<float>(-999);

	Variable obs_var = og.createDbVar("ObsValue", "myObs", typeid(float), { "nlocs", "nchans" }, float_params);

  og.createDbVar("MetaData", "latitude", typeid(float), { "nlocs" }, float_params);
	Variable lat_var = og.openDbVar("MetaData", "latitude");

	og.createDbVar("MetaData", "longitude", typeid(float), { "nlocs" }, float_params);
	Variable lon_var = og.openDbVar("MetaData", "longitude");

  obs_var.atts
		.add<std::string>("coordinates", { "longitude latitude nchans" }, { 1 })
		.add<std::string>("long_name", { "obs I made up" }, { 1 })
		.add<std::string>("units", { "K" }, { 1 })
		.add<float>("valid_range", { 0.0, 500.0 }, { 2 });

  lat_var.atts
		.add<std::string>("long_name", { "latitude" }, { 1 })
		.add<std::string>("units", { "degrees_north" }, { 1 })
		.add<float>("valid_range", { -90.0, 90.0 }, { 2 });

	lon_var.atts
		.add<std::string>("long_name", { "longitude" }, { 1 })
		.add<std::string>("units", { "degrees_east" }, { 1 })
		.add<float>("valid_range", { -360.0, 360.0 }, { 2 });

  //Gather data from the list of reports
  vector<float> lats(reports.size());
  vector<float> lons(reports.size());
  Eigen::ArrayXXf bufrData(reports.size(), numChannels);
  for (unsigned int rpt_idx=0; rpt_idx < reports.size(); rpt_idx++)
  {
    lats[rpt_idx] = reports[rpt_idx].olat;
    lons[rpt_idx] = reports[rpt_idx].olon;

    //This is not efficient, should be creating eigen array from memory buffer.
    //Creating a vector of Reports was a mistake.
    double* data_bufr = reports[rpt_idx].bufr_data.get();
    for (unsigned int channel_idx = 0; channel_idx < numChannels; channel_idx++)
    {
      bufrData(rpt_idx, channel_idx) = data_bufr[channel_idx];
    }
  }

  lat_var.write(lats);
  lon_var.write(lons);
  obs_var.writeWithEigenRegular(bufrData);

  return og;
}

int main(int argc, const char** argv) 
{
  int numChannels = 15;

  //Read the Buffer Data
  auto bufrFile = BufrFile(INPUT_FILE, numChannels);

  clock_t tStart = clock();
  bufrFile.readData();
  printf("Time to read: %.2fs\n\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
  
  bufrFile.printData(10);

  //Generate IODA Engines ObsSpace
  tStart = clock();
  createObsGroup(bufrFile.getReports(), numChannels);
  printf("Time to create obs group: %.2fs\n\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);

  return 0;
}
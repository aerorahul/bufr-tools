
#include <math.h>
#include <string>
#include <iostream>
#include <typeinfo>

#include "Eigen/Dense"

#include "ioda/defs.h"
#include "ioda/ObsGroup.h"
#include "ioda/Engines/Factory.h"

using namespace std;
using namespace ioda;

#include "bufr_tools.h"


int main(int argc, const char * argv[])
{
  string filepath = "/Users/rmclaren/Work/sample-bufr-data/gdas/gdas.20200704/12/gdas.t12z.1bhrs4.tm00.bufr_d";

  int num_msgs;
  int num_reps;
  count_messages(filepath, &num_msgs, &num_reps);
  cout << "Num msgs: " << num_msgs << " Num secs: " << num_reps << endl;

  const int num_channels = 15;
  BufrData* bufrData = read_bufrdata(filepath, num_channels, num_msgs, num_reps);
  print_bufrdata(10, bufrData);

  std::vector<int> nChans(num_channels);
	std::iota(nChans.begin(), nChans.end(), 1);

  string file_output = "/Users/rmclaren/Temp/obsgroup.dat";

  ObsGroup og = ObsGroup::createObsGroupFile(file_output, true);
  og.createDimScale("nchans", nChans);
	Variable nchans_var = og.openDimScale("nchans");

  ioda::VariableCreationParameters float_params = og.initVarCreateParams<float>(-999);

	Variable obs_var =
		og.createDbVar("ObsValue", "myObs", typeid(float), { "nlocs", "nchans" }, float_params);

  obs_var.atts
		.add<std::string>("coordinates", { "MetaData/longitude MetaData/latitude nchans" }, { 1 })
		.add<std::string>("long_name", { "obs I made up" }, { 1 })
		.add<std::string>("units", { "K" }, { 1 })
		.add<float>("valid_range", { 0.0, 50.0 }, { 2 });



  // og.createDimScale("nchans", nChans);
	// Variable nchans_var = og.openDimScale("nchans");
	// ioda::VariableCreationParameters float_params = og.initVarCreateParams<float>(-999);

	// Variable obs_var = og.createDbVar("ObsValue", "myObs", typeid(float), { "nlocs", "nchans" }, float_params);

  return 0;
}
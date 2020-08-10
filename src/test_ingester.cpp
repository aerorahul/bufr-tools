#include <iostream>

#include <eckit/config/YAMLConfiguration.h>
#include <eckit/filesystem/PathName.h>

#include "BufrParser/BufrParser.h"
#include "BufrParser/BufrDescription.h"
#include "BufrParser/MnemonicSet.h"
#include "IngesterData.h"

using namespace Ingester;
using namespace std;
using namespace eckit;


static const string CONFIG_FILE = "/Users/rmclaren/Work/bufr-tools/src/test_ingester.yaml";
static const string INPUT_FILE = "/Users/rmclaren/Work/sample-bufr-data/gdas/gdas.20200704/12/gdas.t12z.1bmhs.tm00.bufr_d";
static const string OUTPUT_FILE = "/Users/rmclaren/Temp/ioda.nc";


const string PATH_SEPERATOR =
#if defined _WIN32 || defined __CYGWIN__
    "\\";
#else
    "/";
#endif


void createDescriptionManually()
{
//    auto description = BufrDescription();
//
//    auto set1 = MnemonicSet("SAID FOVN YEAR MNTH DAYS HOUR MINU SECO CLAT CLON CLATH CLONH HOLS", 1);
//    auto set2 = MnemonicSet("SAZA SOZA BEARAZ SOLAZI", 1);
//    auto set3 = MnemonicSet("TMBR", 15);
//
//    description.addMnemonicSet(set1);
//    description.addMnemonicSet(set2);
//    description.addMnemonicSet(set3);
//
//    auto bufrParser = BufrParser(description);
//    auto data = bufrParser.parse(INPUT_FILE, 5);
//
//    cout << data->get("TMBR") << endl;
}

void readDescriptionFromFile()
{
    auto yaml = YAMLConfiguration(PathName(CONFIG_FILE));

    auto dataPath = yaml.getString("dataBasepath");

    for (const auto& conf : yaml.getSubConfigurations("bufrData"))
    {
        auto description = BufrDescription(conf);
        auto bufrParser = BufrParser(description);

        auto filePath = dataPath + PATH_SEPERATOR + conf.getString("filename");
        auto data = bufrParser.parse(filePath, 4);

        cout << data->get("TMBR") << endl;
    }
}

int main(int, const char**)
{
    readDescriptionFromFile();

    return 0;
}


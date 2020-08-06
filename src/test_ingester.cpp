#include <iostream>

#include "BufrParser/BufrParser.h"
#include "BufrParser/BufrDescription.h"
#include "BufrParser/MnemonicSet.h"
#include "IngesterData.h"

using namespace Ingester;
using namespace std;


static const string CONFIG_FILE = "/Users/rmclaren/Work/bufr-tools/test_ingester.yaml";
static const string INPUT_FILE = "/Users/rmclaren/Work/sample-bufr-data/gdas/gdas.20200704/12/gdas.t12z.1bmhs.tm00.bufr_d";
static const string OUTPUT_FILE = "/Users/rmclaren/Temp/ioda.nc";


void createDescriptionManually()
{
    auto description = BufrDescription();

    auto set1 = MnemonicSet("SAID FOVN YEAR MNTH DAYS HOUR MINU SECO CLAT CLON CLATH CLONH HOLS", 1);
    auto set2 = MnemonicSet("SAZA SOZA BEARAZ SOLAZI", 1);
    auto set3 = MnemonicSet("TMBR", 15);

    description.addMnemonicSet(set1);
    description.addMnemonicSet(set2);
    description.addMnemonicSet(set3);

    auto bufrParser = BufrParser(description);
    auto data = bufrParser.parse(INPUT_FILE, 5);

    cout << data->get("TMBR") << endl;
}

void readDescriptionFromFile()
{
    auto description = BufrDescription(CONFIG_FILE);
    auto bufrParser = BufrParser(description);
    auto data = bufrParser.parse(INPUT_FILE, 5);

    cout << data->get("TMBR") << endl;
}

int main(int, const char**)
{
    readDescriptionFromFile();

    return 0;
}


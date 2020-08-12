#include <map>
#include "bufr.interface.h"

#include "BufrParser.h"
#include "BufrParser/BufrCollectors/BufrCollectors.h"
#include "BufrMnemonicSet.h"
#include "IngesterData.h"

using namespace Ingester;

static const unsigned int SUBSET_STR_LEN = 25;


BufrParser::BufrParser(BufrDescription& description) :
  description_(description)
{
}

shared_ptr<IngesterData> BufrParser::parse(const string& filepath, const unsigned int messagesToParse)
{
    //Parse the BUFR file
    const int fileUnit = openBufrFile(filepath);

    auto collectors = BufrCollectors(fileUnit);
    collectors.addMnemonicSets(description_.getMnemonicSets());

    char subset[SUBSET_STR_LEN];
    int iddate;

    unsigned int messageNum = 0;
    while (ireadmg_f(fileUnit, subset, &iddate, SUBSET_STR_LEN) == 0)
    {
        while (ireadsb_f(fileUnit) == 0)
        {
            collectors.collect();
        }

        if (messagesToParse > 0 && ++messageNum >= messagesToParse) break;
    }

    closeBufrFile(fileUnit);

    return collectors.finalize();
}

int BufrParser::openBufrFile(const string& filepath)
{
    static const int fileUnit = 11;

    open_f(fileUnit, filepath.c_str());
    openbf_f(fileUnit, "IN", fileUnit);

    return fileUnit;
}

void BufrParser::closeBufrFile(const unsigned int fileUnit)
{
    closbf_f(fileUnit);
    close_f(fileUnit);
}

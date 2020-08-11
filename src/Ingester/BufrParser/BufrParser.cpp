#include "BufrParser.h"

#include <map>
#include <iostream>

#include "bufr.interface.h"

#include "BufrMnemonicSet.h"
#include "IngesterData.h"
#include "BufrTypes.h"
#include "BufrAccumulator.h"

#include "BufrOperations/BufrOperation.h"
#include "BufrOperations/BufrIntOperation.h"
#include "BufrOperations/BufrRepOperation.h"


using namespace Ingester;
using namespace Eigen;

static const int FORTRAN_FILE_UNIT = 11;
static const unsigned int SUBSET_STR_LEN = 25;

typedef vector<BufrOperation*> Operations;


//Public Methods

BufrParser::BufrParser(BufrDescription& description) :
  description_(description)
{
}

shared_ptr<IngesterData> BufrParser::parse(const string& filepath, const unsigned int messagesToParse)
{
    //Parse the BUFR file
    const int fileUnit = openBufrFile(filepath);

    Operations operations;
    for (auto& mnemonicSet : description_.getMnemonicSets())
    {
        if (mnemonicSet.getMaxColumn() == 1)
        {
            operations.push_back(new BufrIntOperation(fileUnit, mnemonicSet));
        }
        else
        {
            operations.push_back(new BufrRepOperation(fileUnit, mnemonicSet));
        }
    }

    char subset[SUBSET_STR_LEN];
    int iddate;

    unsigned int messageNum = 0;
    while (ireadmg_f(fileUnit, subset, &iddate, SUBSET_STR_LEN) == 0)
    {
        while (ireadsb_f(fileUnit) == 0)
        {
            for (auto& op : operations)
            {
                op->execute();
            }
        }

        if (messagesToParse > 0 && ++messageNum >= messagesToParse) break;
    }

    closeBufrFile(fileUnit);

    //Create the IngesterData objeect
    auto ingesterData = make_shared<IngesterData>();
    unsigned int mnemonicSetIdx = 0;
    for (auto& mnemonicSet : description_.getMnemonicSets())
    {
        unsigned int mnemonicIdx = 0;
        for (auto& mnemonic : mnemonicSet.getMnemonics())
        {
            IngesterArray dataArr = operations[mnemonicSetIdx]->data(mnemonicIdx * mnemonicSet.getMaxColumn(),
                                                                 mnemonicSet.getChannels());

            ingesterData->add(mnemonic, dataArr);

            mnemonicIdx++;
        }
        mnemonicSetIdx++;
    }

    for (auto* operation : operations)
    {
        delete operation;
    }

    return ingesterData;
}

//Private Methods
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

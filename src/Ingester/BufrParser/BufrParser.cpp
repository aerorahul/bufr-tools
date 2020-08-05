
#include <map>
#include <iostream>
#include "Eigen/Dense"

#include "bufr.interface.h"
#include "BufrParser.h"
#include "MnemonicSet.h"
#include "IngesterData.h"
#include "IngesterAccumulator.h"


using namespace Ingester;
using namespace Eigen;

static const int FORTRAN_FILE_UNIT = 11;
static const unsigned int SUBSET_STR_LEN = 25;

typedef vector<IngesterAccumulator> AccumVec;
typedef vector<AccumVec> AccumVecVec;


BufrParser::BufrParser(BufrDescription& description) :
  description_(description)
{
}


shared_ptr<IngesterData> BufrParser::parse(const string& filepath, const unsigned int messagesToParse)
{
    //Initialize the data map
    AccumVecVec accumVecVec;
    for (const auto& mnemonicSet : description_.getMnemonicSets())
    {
        AccumVec accumVec;
        for (const auto& mnemonic : mnemonicSet.getMnemonics())
        {
            accumVec.push_back(IngesterAccumulator(mnemonicSet.getElementSize()));
        }

        accumVecVec.push_back(accumVec);
    }

    //Parse the BUFR file
    open_f(FORTRAN_FILE_UNIT, filepath.c_str());
    openbf_f(FORTRAN_FILE_UNIT, "IN", FORTRAN_FILE_UNIT);

    char subset[SUBSET_STR_LEN];
    int iddate;

    unsigned int messageNum = 0;
    while (ireadmg_f(FORTRAN_FILE_UNIT, subset, &iddate, SUBSET_STR_LEN) == 0)
    {
        while (ireadsb_f(FORTRAN_FILE_UNIT) == 0)
        {
            unsigned int mnemonicSetIdx = 0;
            for (const auto& mnemonicSet : description_.getMnemonicSets())
            {
                auto sbData = new double[mnemonicSet.getSize() * mnemonicSet.getElementSize()];
                int result;

                if (mnemonicSet.getElementSize() == 1)
                {
                    ufbint_f(FORTRAN_FILE_UNIT,
                            (void**) &sbData,
                            mnemonicSet.getSize(),
                            mnemonicSet.getElementSize(),
                            &result,
                            mnemonicSet.getMnemonicStr().c_str());
                }
                else
                {
                    ufbrep_f(FORTRAN_FILE_UNIT,
                             (void**) &sbData,
                             mnemonicSet.getSize(),
                             mnemonicSet.getElementSize(),
                             &result,
                             mnemonicSet.getMnemonicStr().c_str());
                }

                unsigned int offset = 0;
                for (unsigned int mnemonicIdx = 0; mnemonicIdx < mnemonicSet.getSize(); mnemonicIdx++)
                {
                    accumVecVec[mnemonicSetIdx][mnemonicIdx].addRow(sbData + offset);
                    offset += mnemonicSet.getElementSize();
                }

                delete[] sbData;
                mnemonicSetIdx++;
            }
        }

        if (messagesToParse > 0 && ++messageNum >= messagesToParse) break;
    }

    closbf_f(FORTRAN_FILE_UNIT);
    close_f(FORTRAN_FILE_UNIT);

    //Create the IngesterData objeect
    auto ingesterData = make_shared<IngesterData>();
    unsigned int mnemonicSetIdx = 0;
    for (const auto& mnemonicSet : description_.getMnemonicSets())
    {
        unsigned int mnemonicIdx = 0;
        for (const auto& mnemonic : mnemonicSet.getMnemonics())
        {
            ingesterData->add(mnemonic, accumVecVec[mnemonicSetIdx][mnemonicIdx].getData());
            mnemonicIdx++;
        }
        mnemonicSetIdx++;
    }

    return ingesterData;
}


#include <map>
#include <iostream>
#include "Eigen/Dense"

#include "BufrParser.h"
#include "MnemonicSet.h"
//#include "IngesterData.h"
#include "bufr.interface.h"


using namespace Ingester;
using namespace Eigen;

static const int FORTRAN_FILE_UNIT = 11;
static const unsigned int SUBSET_STR_LEN = 25;



class Accumulator
{
public:
    Accumulator() = default;

private:
};

typedef string Mnemonics;
typedef string Mnemonic;
typedef ArrayXd ArrayType;
typedef vector<ArrayType> DataVector;
typedef map<Mnemonic, DataVector> MnemonicData;
typedef map<Mnemonics, MnemonicData> DataMap;


BufrParser::BufrParser(BufrDescription& description) :
  description_(description)
{
}

void BufrParser::parse(const string& filepath)
{
//    auto data = shared_ptr<IngesterData>();

    open_f(FORTRAN_FILE_UNIT, filepath.c_str());
    openbf_f(FORTRAN_FILE_UNIT, "IN", FORTRAN_FILE_UNIT);

    char subset[SUBSET_STR_LEN];
    int iddate;

    vector<string> testVec;

    DataMap dataMap;
    for (const auto& mnemonicSet : description_.getMnemonicSets())
    {
        MnemonicData mnemonicData;
        for (const auto& mnemonic : mnemonicSet.getMnemonics())
        {
            mnemonicData.insert({mnemonic, DataVector()});
        }

        dataMap.insert({mnemonicSet.getMnemonicStr(), mnemonicData});
    }

    while (ireadmg_f(FORTRAN_FILE_UNIT, subset, &iddate, SUBSET_STR_LEN) == 0)
    {
        while (ireadsb_f(FORTRAN_FILE_UNIT) == 0)
        {
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

                for (const auto& mnemonic : mnemonicSet.getMnemonics())
                {
                    DataVector* dataVector = &(dataMap[mnemonicSet.getMnemonicStr()][mnemonic]);
                    ArrayType arr = Map<ArrayType>(sbData, mnemonicSet.getElementSize());
                    dataVector->push_back(arr);
                }

                delete[] sbData;
            }
        }
    }

    closbf_f(FORTRAN_FILE_UNIT);
    close_f(FORTRAN_FILE_UNIT);

    

//    return dataMap;
}

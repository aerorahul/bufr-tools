//
// Created by Ronald McLaren on 8/8/20.
//
#include "bufr.interface.h"

#include "BufrIntOperation.h"
#include "BufrParser/MnemonicSet.h"


using namespace Ingester;

BufrIntOperation::BufrIntOperation(const int fileUnit, const MnemonicSet mnemonicSet) :
    BufrOperation(fileUnit),
    accumulator_(BufrAccumulator(mnemonicSet.getSize() * mnemonicSet.getMaxColumn())),
    mnemonicSet_(mnemonicSet)
{
    scratchData_ = new double[mnemonicSet.getSize() * mnemonicSet.getMaxColumn()];
}

BufrIntOperation::~BufrIntOperation()
{
    delete[] scratchData_;
}

void BufrIntOperation::execute()
{
    int result;

    ufbint_f(fileUnit_,
             (void**) &scratchData_,
             mnemonicSet_.getSize(),
             mnemonicSet_.getMaxColumn(),
             &result,
             mnemonicSet_.getMnemonicStr().c_str());

    accumulator_.addRow(scratchData_);
}

IngesterArray BufrIntOperation::data(Index startCol, Index numCols)
{
    return accumulator_.getData(startCol, numCols);
}

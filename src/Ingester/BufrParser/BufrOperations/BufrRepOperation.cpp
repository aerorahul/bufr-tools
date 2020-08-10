//
// Created by Ronald McLaren on 8/8/20.
//
#include "bufr.interface.h"

#include "BufrRepOperation.h"
#include "BufrParser/MnemonicSet.h"


using namespace Ingester;

BufrRepOperation::BufrRepOperation(const int fileUnit, const MnemonicSet mnemonicSet) :
    BufrOperation(fileUnit),
    accumulator_(BufrAccumulator(mnemonicSet.getColumnSet().size())),
    mnemonicSet_(mnemonicSet)
{
    scratchData_ = new double[mnemonicSet.getMaxColumn()];
}

BufrRepOperation::~BufrRepOperation()
{
    delete[] scratchData_;
}

void BufrRepOperation::execute()
{
    int result;

    ufbrep_f(fileUnit_,
             (void**) &scratchData_,
             mnemonicSet_.getSize(),
             mnemonicSet_.getMaxColumn(),
             &result,
             mnemonicSet_.getMnemonicStr().c_str());

    accumulator_.addRow(scratchData_);
}

IngesterArray BufrRepOperation::data(Index startCol, Index numCols)
{
    return accumulator_.getData(startCol, numCols);
}

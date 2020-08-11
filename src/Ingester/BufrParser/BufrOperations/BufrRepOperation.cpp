//
// Created by Ronald McLaren on 8/8/20.
//
#include "bufr.interface.h"

#include "BufrRepOperation.h"
#include "BufrParser/BufrMnemonicSet.h"


using namespace Ingester;

BufrRepOperation::BufrRepOperation(const int fileUnit, const BufrMnemonicSet mnemonicSet) :
    BufrOperation(fileUnit),
    accumulator_(BufrAccumulator(mnemonicSet.getSize() * mnemonicSet.getMaxColumn())),
    mnemonicSet_(mnemonicSet)
{
    scratchData_ = new double[mnemonicSet.getSize() * mnemonicSet.getMaxColumn()];
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

IngesterArray BufrRepOperation::data(Index startCol, const Channels& channels)
{
    return accumulator_.getData(startCol, channels);
}

//
// Created by Ronald McLaren on 8/8/20.
//
#include "bufr.interface.h"

#include "BufrIntCollector.h"
#include "BufrParser/BufrMnemonicSet.h"


using namespace Ingester;

BufrIntCollector::BufrIntCollector(const int fileUnit, const BufrMnemonicSet mnemonicSet) :
    BufrCollector(fileUnit),
    accumulator_(BufrAccumulator(mnemonicSet.getSize() * mnemonicSet.getMaxColumn())),
    mnemonicSet_(mnemonicSet)
{
    scratchData_ = new double[mnemonicSet.getSize() * mnemonicSet.getMaxColumn()];
}

BufrIntCollector::~BufrIntCollector()
{
    delete[] scratchData_;
}

void BufrIntCollector::collect()
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

IngesterArray BufrIntCollector::data(Index startCol, const Channels& channels)
{
    return accumulator_.getData(startCol, channels);
}

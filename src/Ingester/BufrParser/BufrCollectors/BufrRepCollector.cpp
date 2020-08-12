//
// Created by Ronald McLaren on 8/8/20.
//
#include "bufr.interface.h"

#include "BufrRepCollector.h"
#include "BufrParser/BufrMnemonicSet.h"


using namespace Ingester;

BufrRepCollector::BufrRepCollector(const int fileUnit, const BufrMnemonicSet mnemonicSet) :
    BufrCollector(fileUnit, BufrAccumulator(mnemonicSet.getSize() * mnemonicSet.getMaxColumn())),
    mnemonicSet_(mnemonicSet)
{
    scratchData_ = new double[mnemonicSet.getSize() * mnemonicSet.getMaxColumn()];
}

BufrRepCollector::~BufrRepCollector()
{
    delete[] scratchData_;
}

void BufrRepCollector::collect()
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

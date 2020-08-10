//
// Created by Ronald McLaren on 8/8/20.
//

#include "BufrOperation.h"
#include "IngesterData.h"
#include "BufrParser/MnemonicSet.h"
#include "BufrParser/BufrAccumulator.h"

#pragma once

namespace Ingester
{
    class BufrRepOperation : public BufrOperation
    {
    public:
        BufrRepOperation(const int fileUnit, MnemonicSet mnemonicSet);
        ~BufrRepOperation() override;

        void execute() override final;
        IngesterArray data(Index startCol = 0, Index numCols = 1) override final;

    private:
        double *scratchData_;
        BufrAccumulator accumulator_;
        const MnemonicSet mnemonicSet_;
    };
}

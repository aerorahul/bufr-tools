//
// Created by Ronald McLaren on 8/8/20.
//

#include "BufrOperation.h"

#include "BufrParser/BufrMnemonicSet.h"
#include "BufrParser/BufrAccumulator.h"
#include "BufrParser/BufrTypes.h"

#pragma once

namespace Ingester
{
    class BufrIntOperation: public BufrOperation
    {
    public:
        explicit BufrIntOperation(const int fileUnit, const BufrMnemonicSet mnemonicSet);
        ~BufrIntOperation() override;

        void execute() override final;
        IngesterArray data(Index startCol, const Channels& channels) override final;

    private:
        double *scratchData_;
        BufrAccumulator accumulator_;
        const BufrMnemonicSet mnemonicSet_;
    };
}


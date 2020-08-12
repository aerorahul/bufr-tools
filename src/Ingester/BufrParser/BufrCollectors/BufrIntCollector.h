//
// Created by Ronald McLaren on 8/8/20.
//

#include "BufrCollector.h"

#include "BufrParser/BufrMnemonicSet.h"
#include "BufrAccumulator.h"
#include "BufrParser/BufrTypes.h"

#pragma once

namespace Ingester
{
    class BufrIntCollector: public BufrCollector
    {
    public:
        explicit BufrIntCollector(const int fileUnit, const BufrMnemonicSet mnemonicSet);
        ~BufrIntCollector() override;

        void collect() override final;
        inline BufrMnemonicSet getMnemonicSet() const { return mnemonicSet_; }
        IngesterArray data(Index startCol, const Channels& channels) override final;

    private:
        double *scratchData_;
        BufrAccumulator accumulator_;
        const BufrMnemonicSet mnemonicSet_;
    };
}


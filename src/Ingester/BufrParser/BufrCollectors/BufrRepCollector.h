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
    class BufrRepCollector : public BufrCollector
    {
    public:
        BufrRepCollector(const int fileUnit, BufrMnemonicSet mnemonicSet);
        ~BufrRepCollector() override;

        void collect() final;
        IngesterArrayMap finalize() final;

        inline BufrMnemonicSet getMnemonicSet() const { return mnemonicSet_; }

    private:
        double *scratchData_;
        const BufrMnemonicSet mnemonicSet_;
    };
}

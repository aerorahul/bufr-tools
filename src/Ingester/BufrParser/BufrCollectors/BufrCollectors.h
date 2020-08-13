//
// Created by Ronald McLaren on 8/11/20.
//

#pragma once

#include <string>
#include <vector>
#include <map>

#include "BufrParser/BufrTypes.h"

namespace Ingester
{
    class BufrMnemonicSet;
    class BufrAccumulator;
    class BufrCollector;
    class IngesterData;

    class BufrCollectors
    {
    public:
        explicit BufrCollectors(unsigned int fileUnit);
        ~BufrCollectors();

        void addMnemonicSets(const std::vector<BufrMnemonicSet>& mnemonicSets);
        void addMnemonicSet(const BufrMnemonicSet& mnemonicSet);
        void collect();
        std::shared_ptr<IngesterData> finalize();

    private:
        unsigned int fileUnit_;
        std::vector<BufrCollector*> collectors_;
    };
}

//
// Created by Ronald McLaren on 8/8/20.
//

#pragma once

#include <set>
#include "Eigen/Dense"

#include "BufrParser/BufrTypes.h"
#include "BufrAccumulator.h"

namespace Ingester
{
    using namespace Eigen;

    class BufrCollector
    {
    public:
        BufrCollector(const int fileUnit, const BufrAccumulator accumulator);
        virtual ~BufrCollector() = default;

        virtual void collect() = 0;
        IngesterArray data(Index startCol, const Channels& channels);

    protected:
        const int fileUnit_;
        BufrAccumulator accumulator_;
    };
}


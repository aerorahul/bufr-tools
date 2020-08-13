//
// Created by Ronald McLaren on 8/8/20.
//

#pragma once

#include <set>
#include <string>
#include "Eigen/Dense"

#include "BufrParser/BufrTypes.h"
#include "BufrAccumulator.h"

namespace Ingester
{
    class BufrCollector
    {
    public:
        BufrCollector(const int fileUnit, const BufrAccumulator accumulator);
        virtual ~BufrCollector() = default;

        virtual void collect() = 0;
        virtual IngesterArrayMap finalize() = 0;

    protected:
        const int fileUnit_;
        BufrAccumulator accumulator_;
    };
}


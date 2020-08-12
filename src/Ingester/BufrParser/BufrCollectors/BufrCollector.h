//
// Created by Ronald McLaren on 8/8/20.
//

#pragma once

#include <set>

#include "Eigen/Dense"
#include "BufrParser/BufrTypes.h"

namespace Ingester
{
    using namespace Eigen;

    class BufrCollector
    {
    public:
        BufrCollector(const int fileUnit);
        virtual ~BufrCollector() = default;

        virtual void collect() = 0;
        virtual IngesterArray data(Index startCol, const Channels& channels) = 0;

    protected:
        const int fileUnit_;
    };
}


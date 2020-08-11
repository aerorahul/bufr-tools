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

    class BufrOperation
    {
    public:
        BufrOperation(const int fileUnit);
        virtual ~BufrOperation() = default;

        virtual void execute() = 0;
        virtual IngesterArray data(Index startCol, const Channels& channels) = 0;

    protected:
        const int fileUnit_;
    };
}


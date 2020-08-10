//
// Created by Ronald McLaren on 8/8/20.
//

#pragma once

#include "Eigen/Dense"
#include "IngesterData.h"

namespace Ingester
{
    using namespace Eigen;

    class BufrOperation
    {
    public:
        BufrOperation(const int fileUnit);
        virtual ~BufrOperation() = default;

        virtual void execute() = 0;
        virtual IngesterArray data(Index startCol = 0, Index numCols = 1) = 0;

    protected:
        const int fileUnit_;
    };
}


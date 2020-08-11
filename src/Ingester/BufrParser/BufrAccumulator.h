//
// Created by Ronald McLaren on 8/5/20.
//

#pragma once

#include <set>

#include "Eigen/Dense"

#include "BufrTypes.h"

namespace Ingester
{
    using namespace Eigen;

    class BufrAccumulator
    {
    public:
        explicit BufrAccumulator(Index numColumns, Index blockSize = 50000);

        void addRow(double* newRow);

        IngesterArray getData(Index startCol, const Channels& channels);

    private:
        IngesterArray dataArray_;
        Index numColumns_;
        Index numDataRows_;
        Index blockSize_;
    };
}
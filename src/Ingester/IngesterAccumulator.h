//
// Created by Ronald McLaren on 8/5/20.
//

#pragma once

#include "Eigen/Dense"
#include "IngesterData.h"

namespace Ingester
{
    using namespace Eigen;

    class IngesterAccumulator
    {
    public:
        explicit IngesterAccumulator(Index numColumns, Index blockSize = 50000);

        void addRow(double* newRow);

        IngesterArray getData(Index startCol, Index numCols);

    private:
        IngesterArray dataArray_;
        Index numColumns_;
        Index numDataRows_;
        Index blockSize_;
    };
}
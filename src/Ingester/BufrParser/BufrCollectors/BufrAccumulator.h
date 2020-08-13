//
// Created by Ronald McLaren on 8/5/20.
//

#pragma once

#include <set>

#include "Eigen/Dense"

#include "BufrParser/BufrTypes.h"

namespace Ingester
{
    class BufrAccumulator
    {
    public:
        explicit BufrAccumulator(Eigen::Index numColumns, Eigen::Index blockSize = 50000);

        void addRow(double* newRow);

        IngesterArray getData(Eigen::Index startCol, const Channels& channels);
        void reset();

    private:
        IngesterArray dataArray_;
        Eigen::Index numColumns_;
        Eigen::Index numDataRows_;
        Eigen::Index blockSize_;
    };
}
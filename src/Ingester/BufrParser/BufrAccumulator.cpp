//
// Created by Ronald McLaren on 8/5/20.
//

#include "BufrAccumulator.h"

using namespace Ingester;

BufrAccumulator::BufrAccumulator(Index numColumns, Index blockSize) :
    dataArray_(blockSize, numColumns),
    numColumns_(numColumns),
    numDataRows_(0),
    blockSize_(blockSize)
{
}

void BufrAccumulator::addRow(double* newRow)
{
    if (numDataRows_ + 1 > dataArray_.rows())
    {
        dataArray_.conservativeResize(dataArray_.rows() + blockSize_, numColumns_);
    }

    dataArray_.row(numDataRows_) = Map<IngesterArray>(newRow, 1, numColumns_);
    numDataRows_++;
}

IngesterArray BufrAccumulator::getData(Index startCol, Index numCols)
{
    return dataArray_.block(0, startCol, numDataRows_, numCols);
}

//
// Created by Ronald McLaren on 8/8/20.
//

#include "BufrCollector.h"

using namespace Ingester;

BufrCollector::BufrCollector(const int fileUnit, const BufrAccumulator accumulator) :
    fileUnit_(fileUnit),
    accumulator_(accumulator)
{
}

IngesterArray BufrCollector::data(Index startCol, const Channels& channels)
{
    return accumulator_.getData(startCol, channels);
}

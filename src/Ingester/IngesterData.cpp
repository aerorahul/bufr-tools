
#include "IngesterData.h"

#include <utility>

Eigen::ArrayXXf Ingester::IngesterData::get(string& fieldName)
{
    return data_[fieldName];
}

void Ingester::IngesterData::add(string& fieldName, Eigen::ArrayXXf data)
{
    data_[fieldName] = std::move(data);
}

#pragma once

#include <string>
#include <vector>
#include <map>

#include "Eigen/Dense"
#include "IngesterTypes.h"

namespace Ingester
{
    class IngesterData
    {
    public:
        IngesterData() = default;

        void add(const std::string& fieldName, const IngesterArray& data);
        IngesterArray get(const std::string& fieldName);
        size_t size() const;

    private:
        IngesterArrayMap dataMap_;
    };
}

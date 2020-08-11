#pragma once

#include <string>
#include <vector>
#include <map>

#include "Eigen/Dense"
#include "IngesterTypes.h"

namespace Ingester
{
    using namespace std;
    using namespace Eigen;

    class IngesterData
    {
    public:
        IngesterData() = default;

        void add(const string& fieldName, const IngesterArray& data);
        IngesterArray get(const string& fieldName);

    private:
        map<string, IngesterArray> dataMap_;
    };
}

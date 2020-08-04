#pragma once

#include <string>
#include <vector>
#include <map>

#include "Eigen/Dense"

namespace Ingester
{
    using namespace std;
    using namespace Eigen;

    typedef Array<double, Dynamic, Dynamic, RowMajor> IngesterArray;

    class IngesterData
    {
    public:
        IngesterData() = default;

        void add(const string& fieldName, IngesterArray data);
        IngesterArray get(const string& fieldName);

    private:
        map<string, IngesterArray> dataMap_;
    };
}

#pragma once

#include <string>
#include <map>

#include "Eigen/Dense"

namespace Ingester
{
    using namespace std;

    class IngesterData
    {
    public:
        IngesterData() = default;
        Eigen::ArrayXXf get(string& fieldName);
        void add(string& fieldName, Eigen::ArrayXXf data);

    private:
        map<string, Eigen::ArrayXXf> data_;
    };
}
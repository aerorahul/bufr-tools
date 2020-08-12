//
// Created by Ronald McLaren on 8/11/20.
//

#pragma once

#include <string>
#include <map>

#include "Eigen/Dense"

typedef Eigen::Array<double, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> IngesterArray;
typedef std::map<std::string, IngesterArray> IngesterArrayMap;
#include <iostream>
#include <memory>
#include "IngesterData.h"


using namespace Ingester;
using namespace std;

void IngesterData::add(const string& fieldName, const IngesterArray& data)
{
    if (dataMap_.find(fieldName) != dataMap_.end())
    {
        cout << "WARNING: Adding duplicate field called " << fieldName << endl;
    }

    dataMap_.insert({fieldName, data});
}

IngesterArray IngesterData::get(const string& fieldName)
{
    if (dataMap_.find(fieldName) == dataMap_.end())
    {
        cout << "ERROR: Field called " << fieldName << " doesn't exist."<< endl;
        abort();
    }

    return dataMap_[fieldName];
}

size_t IngesterData::size() const
{
    size_t size = 0;
    if (dataMap_.size() > 0)
    {
        size = dataMap_.begin()->second.size();
    }

    return size;
}

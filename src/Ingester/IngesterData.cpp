#include <iostream>
#include <memory>
#include "IngesterData.h"


using namespace Ingester;

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

#pragma once

#include <vector>
#include <string>
#include <memory>

namespace Ingester
{
    using namespace std;

    class MnemonicSet;
    class BufrDescription
    
    class BufrParser
    {
        const string filepath_;
        const unsigned int numChannels_; 
        std::vector<ParameterSet> parameterSets_;

    public: 
        BufrFile(string filepath, unsigned int numChannels);
        void addParameterSet(MnemonicSet parameterSet);


        void readData();
        void printData(int numElements=-1);
        void countMessages();
        // Reports getReports();
    };
}

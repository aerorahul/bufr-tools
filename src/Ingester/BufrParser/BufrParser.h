#pragma once

#include <vector>
#include <string>
#include <memory>

#include "Eigen/Dense"

#include "BufrDescription.h"


namespace Ingester
{
    using namespace std;

    class MnemonicSet;
    class IngesterData;
    
    class BufrParser
    {
    public:
        explicit BufrParser(BufrDescription& description);
        shared_ptr<IngesterData> parse(const string& filepath, const unsigned int messagesToParse=0);

    private:
        BufrDescription description_;

        int openBufrFile(const string& filepath);
        void closeBufrFile(const unsigned int fileUnit);
    };
}

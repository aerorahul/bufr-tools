#pragma once

#include <vector>
#include <string>
#include <memory>

#include "Eigen/Dense"

#include "BufrDescription.h"


namespace Ingester
{
    using namespace std;

    class BufrMnemonicSet;
    class IngesterData;
    
    class BufrParser
    {
    public:
        explicit BufrParser(BufrDescription& description);
        ~BufrParser();
        shared_ptr<IngesterData> parse(const unsigned int maxMessagesToParse=0);

    private:
        BufrDescription description_;
        unsigned int fileUnit_;

        int openBufrFile(const string& filepath);
        void closeBufrFile(const unsigned int fileUnit);
    };
}

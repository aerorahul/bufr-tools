#pragma once

#include <vector>
#include <string>
#include <memory>
#include <map>
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
        void parse(const string& filepath);

    private:
        BufrDescription description_;
    };
}

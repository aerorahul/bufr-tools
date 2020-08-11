#pragma once

#include <eckit/config/LocalConfiguration.h>

#include <string>
#include <vector>

namespace Ingester
{
    using namespace std;

    class BufrMnemonicSet;
    
    class BufrDescription
    {
    public: 
        BufrDescription() = default;
        explicit BufrDescription(const eckit::Configuration& conf);

        void addMnemonicSet(BufrMnemonicSet mnemonicSet);
        inline vector<BufrMnemonicSet>& getMnemonicSets() { return mnemonicSets_; }

    private:
        vector<BufrMnemonicSet> mnemonicSets_;
    };
}

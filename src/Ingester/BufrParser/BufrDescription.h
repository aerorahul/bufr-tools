#pragma once

#include <eckit/config/LocalConfiguration.h>

#include <string>
#include <vector>

namespace Ingester
{
    using namespace std;

    class MnemonicSet;
    
    class BufrDescription
    {
    public: 
        BufrDescription() = default;
        explicit BufrDescription(const eckit::Configuration& conf);

        void addMnemonicSet(MnemonicSet mnemonicSet);
        inline vector<MnemonicSet>& getMnemonicSets() { return mnemonicSets_; }

    private:
        vector<MnemonicSet> mnemonicSets_;
    };
}

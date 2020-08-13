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
        explicit BufrDescription(const eckit::Configuration& conf, const string& basePath);

        void addMnemonicSet(BufrMnemonicSet mnemonicSet);

        inline void setFilepath(const string& filepath) { filepath_ = filepath; }
        inline vector<BufrMnemonicSet>& getMnemonicSets() { return mnemonicSets_; }
        inline string filepath() { return filepath_; }

    private:
        vector<BufrMnemonicSet> mnemonicSets_;
        string filepath_;
    };
}

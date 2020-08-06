#pragma once

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
        explicit BufrDescription(const string& filepath);

        void addMnemonicSet(MnemonicSet mnemonicSet);
        inline vector<MnemonicSet>& getMnemonicSets() { return mnemonicSets_; }

    private:
        vector<MnemonicSet> mnemonicSets_;
    };
}

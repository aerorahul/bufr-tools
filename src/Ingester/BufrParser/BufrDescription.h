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
        BufrDescription();
        void addMnemonicSet(MnemonicSet mnemonicSet);
        vector<MnemonicSet> getMnemonicSet();

    private:
        vector<MnemonicSet> mnemonicSets_;

        vector<string> tokenizeMnemonics(string mnemonics);
    };
}

#pragma once

#include <string>
#include <vector>

#include "BufrTypes.h"

namespace Ingester
{
    using namespace std;

    class BufrMnemonicSet
    {
    public:
        BufrMnemonicSet(const string mnemonics, const Channels elementSize);

        //Getters
        inline vector<string>& getMnemonics() { return mnemonics_; }
        inline string getMnemonicStr() const  { return mnemonicsStr_; }
        inline Channels getChannels() const  { return channels_; }
        inline size_t getMaxColumn() const { return maxColumn_; }
        inline size_t getSize() const  { return mnemonics_.size(); }

    private:
        string mnemonicsStr_;
        vector<string> mnemonics_;
        Channels channels_;
        size_t maxColumn_;

        static std::vector<string> tokenizeMnemonics(const string& mnemonics);
    };
}

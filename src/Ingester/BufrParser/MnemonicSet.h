#pragma once

#include <string>
#include <vector>

namespace Ingester
{
    using namespace std;

    class MnemonicSet
    {
    public:
        MnemonicSet(const string mnemonics, const size_t elementSize);

        //Getters
        inline vector<string>& getMnemonics() { return mnemonics_; }
        inline string getMnemonicStr() const  { return mnemonicsStr_; }
        inline size_t getElementSize() const  { return elementSize_; }
        inline size_t getSize() const  { return mnemonics_.size(); }

    private:
        string mnemonicsStr_;
        vector<string> mnemonics_;
        const size_t elementSize_;

        static std::vector<string> tokenizeMnemonics(const string& mnemonics);
    };
}

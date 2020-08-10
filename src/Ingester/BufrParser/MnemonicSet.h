#pragma once

#include <string>
#include <vector>
#include <set>

namespace Ingester
{
    using namespace std;

    class MnemonicSet
    {
    public:
        MnemonicSet(const string mnemonics, const set<int> elementSize);

        //Getters
        inline vector<string>& getMnemonics() { return mnemonics_; }
        inline string getMnemonicStr() const  { return mnemonicsStr_; }
        inline set<int> getColumnSet() const  { return columnSet_; }
        inline size_t getMaxColumn() const { return maxColumn_; }
        inline size_t getSize() const  { return mnemonics_.size(); }

    private:
        string mnemonicsStr_;
        vector<string> mnemonics_;
        set<int> columnSet_;
        size_t maxColumn_;

        static std::vector<string> tokenizeMnemonics(const string& mnemonics);
    };
}

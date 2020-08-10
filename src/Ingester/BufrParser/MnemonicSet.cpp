#include <iostream>
#include <vector>
#include <sstream>
#include <iterator>

#include "MnemonicSet.h"

using namespace Ingester;
using namespace std;

MnemonicSet::MnemonicSet(const string nmemonicsStr, const set<int> columnSet) :
    mnemonicsStr_(nmemonicsStr),
    columnSet_(columnSet)
{
    mnemonics_ = tokenizeMnemonics(nmemonicsStr);

    maxColumn_ = 0;
    for (auto col : columnSet_)
    {
        if (col > maxColumn_)
        {
            maxColumn_ = col;
        }
    }
}

vector<string> MnemonicSet::tokenizeMnemonics(const string& nmemonicsStr)
{
    //Tokenize the string into individual mnemonic strings
    istringstream buf(nmemonicsStr);
    istream_iterator<string> beg(buf), end;
    vector<string> tokens(beg, end);
    return tokens;
}

#include <iostream>
#include <vector>
#include <sstream>
#include <iterator>

#include "BufrMnemonicSet.h"
#include "BufrTypes.h"

using namespace Ingester;
using namespace std;

BufrMnemonicSet::BufrMnemonicSet(const string nmemonicsStr, const Channels channels) :
    mnemonicsStr_(nmemonicsStr),
    channels_(channels)
{
    mnemonics_ = tokenizeMnemonics(nmemonicsStr);

    maxColumn_ = 0;
    for (auto col : channels)
    {
        if (col > (int) maxColumn_)
        {
            maxColumn_ = col;
        }
    }
}

vector<string> BufrMnemonicSet::tokenizeMnemonics(const string& nmemonicsStr)
{
    //Tokenize the string into individual mnemonic strings
    istringstream buf(nmemonicsStr);
    istream_iterator<string> beg(buf), end;
    vector<string> tokens(beg, end);
    return tokens;
}
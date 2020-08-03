#include <iostream>
#include <vector>
#include <sstream>
#include <iterator>

#include "MnemonicSet.h"

using namespace Ingester;
using namespace std;

MnemonicSet::MnemonicSet(const string nmemonicsStr, const size_t elementSize) :
    mnemonicsStr_(nmemonicsStr),
    elementSize_(elementSize)
{
    mnemonics_ = tokenizeMnemonics(nmemonicsStr);
}

vector<string> MnemonicSet::tokenizeMnemonics(const string& nmemonicsStr)
{
    //Tokenize the string into individual mnemonic strings
    istringstream buf(nmemonicsStr);
    istream_iterator<string> beg(buf), end;
    vector<string> tokens(beg, end);
    return tokens;
}

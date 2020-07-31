#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <iterator>

#include "MnemonicSet.h"

using namespace Ingester;
using namespace std;

MnemonicSet::MnemonicSet(const string nmemonics, const size_t elementSize) :
    elementSize_(elementSize)
{
    mnemonics_ = tokenizeMnemonics(nmemonics);
}

vector<string> MnemonicSet::tokenizeMnemonics(const string mnemonics)
{
    //Tokenize the string into individual mnemonic strings
    istringstream buf(mnemonics);
    istream_iterator<string> beg(buf), end;
    vector<string> tokens(beg, end);
    return tokens;
}
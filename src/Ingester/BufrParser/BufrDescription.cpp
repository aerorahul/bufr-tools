
#include "BufrDescription.h"
#include "MnemonicSet.h"


using namespace Ingester;
using namespace std;

void BufrDescription::addMnemonicSet(MnemonicSet& mnemonicSet)
{
    mnemonicSets_.push_back(mnemonicSet);
}

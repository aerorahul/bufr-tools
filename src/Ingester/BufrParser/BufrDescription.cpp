
#include <iostream>

#include "oops/IntSetParser.h"
#include "BufrDescription.h"
#include "MnemonicSet.h"


using namespace Ingester;
using namespace std;

const string MNEMONIC_SETS_YAML_SECTION = "mnemonicSets";
const string MNEMONIC_STR_YAML_NAME = "mnemonics";
const string PER_ELEMENT_SIZE_YAML_NAME = "perElementSize";

BufrDescription::BufrDescription(const eckit::Configuration& conf)
{
    auto subConf = conf.getSubConfiguration(MNEMONIC_SETS_YAML_SECTION);

    for (const auto& mnemonicSetConf : subConf.getSubConfigurations())
    {
        auto channels = oops::parseIntSet(mnemonicSetConf.getString(PER_ELEMENT_SIZE_YAML_NAME));

        addMnemonicSet(MnemonicSet(mnemonicSetConf.getString(MNEMONIC_STR_YAML_NAME),
                                   channels));

    }
}

void BufrDescription::addMnemonicSet(MnemonicSet mnemonicSet)
{
    mnemonicSets_.push_back(mnemonicSet);
}

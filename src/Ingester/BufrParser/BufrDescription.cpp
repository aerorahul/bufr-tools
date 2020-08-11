
#include <iostream>

#include "oops/IntSetParser.h"
#include "BufrDescription.h"
#include "BufrMnemonicSet.h"
#include "BufrTypes.h"


using namespace Ingester;
using namespace std;

const string MNEMONIC_SETS_YAML_SECTION = "mnemonicSets";
const string MNEMONIC_STR_YAML_NAME = "mnemonics";
const string CHANNEL_NAME = "channels";

BufrDescription::BufrDescription(const eckit::Configuration& conf)
{
    auto subConf = conf.getSubConfiguration(MNEMONIC_SETS_YAML_SECTION);

    for (const auto& mnemonicSetConf : subConf.getSubConfigurations())
    {
        Channels channels;
        if (mnemonicSetConf.has(CHANNEL_NAME))
        {
            channels = oops::parseIntSet(mnemonicSetConf.getString(CHANNEL_NAME));
        }
        else
        {
            channels = {1};
        }

        addMnemonicSet(BufrMnemonicSet(mnemonicSetConf.getString(MNEMONIC_STR_YAML_NAME),
                                       channels));

    }
}

void BufrDescription::addMnemonicSet(BufrMnemonicSet mnemonicSet)
{
    mnemonicSets_.push_back(mnemonicSet);
}

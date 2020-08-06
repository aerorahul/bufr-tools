
#include <eckit/config/LocalConfiguration.h>
#include <eckit/config/YAMLConfiguration.h>
#include <eckit/filesystem/PathName.h>

#include "BufrDescription.h"
#include "MnemonicSet.h"


using namespace Ingester;
using namespace std;
using namespace eckit;


const string BUFR_PARSER_YAML_SECTION = "bufrParser";
const string MNEMONIC_SETS_YAML_SECTION = "mnemonicSets";
const string MNEMONIC_STR_YAML_NAME = "mnemonics";
const string PER_ELEMENT_SIZE_YAML_NAME = "perElementSize";

BufrDescription::BufrDescription(const string& filepath)
{
    auto conf = YAMLConfiguration(PathName(filepath)).getSubConfiguration(BUFR_PARSER_YAML_SECTION)
                                                     .getSubConfiguration(MNEMONIC_SETS_YAML_SECTION);


    for (const auto& mnemonicSetConf : conf.getSubConfigurations())
    {
        addMnemonicSet(MnemonicSet(mnemonicSetConf.getString(MNEMONIC_STR_YAML_NAME),
                                   mnemonicSetConf.getInt(PER_ELEMENT_SIZE_YAML_NAME)));

    }
}

void BufrDescription::addMnemonicSet(MnemonicSet mnemonicSet)
{
    mnemonicSets_.push_back(mnemonicSet);
}

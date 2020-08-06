
#include "ObsGroupDescription.h"

using namespace Ingester;

void ObsGroupDescription::addMetaKeys(FieldName& fieldName, ObsGroupName& obsGroupName)
{
    metaDataFieldMap_.insert({fieldName, obsGroupName});
}

void ObsGroupDescription::addObsValKeys(FieldName& fieldName, ObsGroupName& obsGroupName)
{
    obsValFieldMap_.insert({fieldName, obsGroupName});
}

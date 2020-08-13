#pragma once

#include <map>
#include <string>


namespace Ingester
{
    typedef std::string FieldName;
    typedef std::string ObsGroupName;
    typedef std::map<std::string, std::string> NameMap;

    class ObsGroupDescription
    {
    public:
        ObsGroupDescription() = default;

        void addMetaKeys(FieldName& fieldName, ObsGroupName& obsGroupName);
        void addObsValKeys(FieldName& fieldName, ObsGroupName& obsGroupName);

    private:
        NameMap metaDataFieldMap_;
        NameMap obsValFieldMap_;
    };
}

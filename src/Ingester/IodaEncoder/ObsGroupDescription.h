#pragma once

#include <map>
#include <string>


namespace Ingester
{
    using namespace std;

    typedef string FieldName;
    typedef string ObsGroupName;
    typedef map<string, string> NameMap;

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

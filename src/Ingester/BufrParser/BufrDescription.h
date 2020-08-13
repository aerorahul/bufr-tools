#pragma once

#include <eckit/config/LocalConfiguration.h>

#include <string>
#include <vector>

namespace Ingester
{
    class BufrMnemonicSet;
    
    class BufrDescription
    {
    public: 
        BufrDescription() = default;
        explicit BufrDescription(const eckit::Configuration& conf, const std::string& basePath);

        void addMnemonicSet(BufrMnemonicSet mnemonicSet);

        inline void setFilepath(const std::string& filepath) { filepath_ = filepath; }
        inline std::vector<BufrMnemonicSet>& getMnemonicSets() { return mnemonicSets_; }
        inline std::string filepath() { return filepath_; }

    private:
        std::vector<BufrMnemonicSet> mnemonicSets_;
        std::string filepath_;
    };
}

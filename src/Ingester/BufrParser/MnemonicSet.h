#pragma once

#include <string>
#include <vector>

namespace Ingester
{
    class MnemonicSet
    {

    public:
        MnemonicSet(const std::string nmemonics, const size_t elementSize);

    private:
        std::vector<std::string> mnemonics_;
        const size_t elementSize_;

        std::vector<std::string> tokenizeMnemonics(const std::string mnemonics);
    };
}

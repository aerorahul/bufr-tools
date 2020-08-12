//
// Created by Ronald McLaren on 8/11/20.
//

#include <memory>

#include "BufrCollectors.h"

#include "IngesterData.h"
#include "BufrRepCollector.h"
#include "BufrIntCollector.h"



using namespace Ingester;

BufrCollectors::BufrCollectors(unsigned int fileUnit) :
    fileUnit_(fileUnit)
{
}

BufrCollectors::~BufrCollectors()
{
    for (auto* collector : collectors_)
    {
        delete collector;
    }
}

void BufrCollectors::addMnemonicSets(const vector<BufrMnemonicSet>& mnemonicSets)
{
    for (const auto& set : mnemonicSets)
    {
        addMnemonicSet(set);
    }
}

void BufrCollectors::addMnemonicSet(const BufrMnemonicSet& mnemonicSet)
{
    if (mnemonicSet.getMaxColumn() == 1)
    {
        collectors_.push_back(new BufrIntCollector(fileUnit_, mnemonicSet));
    }
    else
    {
        collectors_.push_back(new BufrRepCollector(fileUnit_, mnemonicSet));
    }
}

void BufrCollectors::collect()
{
    for (auto* collector : collectors_)
    {
        collector->collect();
    }
}

shared_ptr<IngesterData> BufrCollectors::finalize()
{
    auto ingesterData = make_shared<IngesterData>();

    for (auto* collector : collectors_)
    {
        if (auto* intCollector = dynamic_cast<BufrIntCollector*>(collector))
        {
            unsigned int mnemonicIdx = 0;
            BufrMnemonicSet mnemonicSet = intCollector->getMnemonicSet();
            for (auto& mnemonic : mnemonicSet.getMnemonics())
            {
                IngesterArray dataArr = collector->data(mnemonicIdx * mnemonicSet.getMaxColumn(),
                                                                              mnemonicSet.getChannels());

                ingesterData->add(mnemonic, dataArr);

                mnemonicIdx++;
            }
        }
        else if (auto* bufrCollector = dynamic_cast<BufrRepCollector*>(collector))
        {
            unsigned int mnemonicIdx = 0;
            BufrMnemonicSet mnemonicSet = bufrCollector->getMnemonicSet();
            for (auto& mnemonic : mnemonicSet.getMnemonics())
            {
                IngesterArray dataArr = collector->data(mnemonicIdx * mnemonicSet.getMaxColumn(),
                                                        mnemonicSet.getChannels());

                ingesterData->add(mnemonic, dataArr);

                mnemonicIdx++;
            }
        }
    }

    return ingesterData;
}

//
//  BufrFile.cpp
//  ioda-frontend
//
//  Created by Ronald McLaren on 7/7/20.
//  Copyright © 2020 Ronald McLaren. All rights reserved.
//

#include "BufrRecord.hpp"


BufrRecord::BufrRecord(const string& bufrPath, const string& tablePath) :
bufrPath_(bufrPath),
tablePath_(tablePath),
bufrFileUnit_(0),
tableFileUnit_(0)
{
    open();
}

BufrRecord::~BufrRecord()
{
    close();
}

//Public Methods
vector<string> BufrRecord::getSubsets()
{
    return vector<string>();
}

BufrMsg nextMsg(string subset)
{
    return BufrMsg();
}

//Private Methods
void BufrRecord::open()
{
    open_file(bufrFileUnit_, bufrPath_, "r");
    
    if (tablePath_.length() > 0)
    {
        open_file(tableFileUnit_, bufrPath_, "r");
    }
    
    open_bufr(bufrFileUnit_, In, tableFileUnit_);
}

void BufrRecord::close()
{
    close_bufr(bufrFileUnit_);
    close_file(bufrFileUnit_);
    
    if (tablePath_.length() > 0)
    {
        close_file(tableFileUnit_);
    }
}

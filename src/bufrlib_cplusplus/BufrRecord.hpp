//
//  BufrFile.hpp
//  ioda-frontend
//
//  Created by Ronald McLaren on 7/7/20.
//  Copyright © 2020 Ronald McLaren. All rights reserved.
//

#ifndef BufrFile_hpp
#define BufrFile_hpp

#include <stdio.h>
#include <string>
#include <vector>

#include "bufrlib_wrapper.h"

using namespace std;

class BufrMsg
{
    string _subset;
};

class BufrRecord
{
    const string bufrPath_;
    const string tablePath_;
    FortranFileUnit bufrFileUnit_;
    FortranFileUnit tableFileUnit_;

public:
    BufrRecord(const string& bufrPath, const string& tablePath="");
    ~BufrRecord();
    
    vector<string> getSubsets();
    BufrMsg nextMsg(string subset);
    
private:
    void open();
    void close();
    
    static FortranFileUnit getAvailableFileUnit();
};

#endif /* BufrFile_hpp */

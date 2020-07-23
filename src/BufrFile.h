#include <vector>
#include <string>
#include <memory>

namespace BufrTools
{
    using namespace std;

    typedef struct
    {
        int nchanl;
        int satid;
        int ifov;
        int dtime[6];
        double olat, olon;
        double terrain;
        double lza, sza;
        double sat_aziang, sol_aziang;
        shared_ptr<double> bufr_data;
    }  Report;

    typedef vector<Report> Reports;

    class BufrFile
    {
        Reports reports_;
        const string filepath_;
        const unsigned int numChannels_; 

    public: 
        BufrFile(string filepath, unsigned int numChannels);
        void readData();
        void printData(int numElements=-1);
        void countMessages();
        Reports getReports();
    };
}

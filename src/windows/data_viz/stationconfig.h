/*  -----------------------------------------
 *  @levilliard
 *  2016
 *  -----------------------------------------
 */

#ifndef STATIONCONFIG_H
#define STATIONCONFIG_H

#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>

class StationConfig
{
    public:
        StationConfig();
        std::string getStationMetaData();
        std::vector<float> getStationData(std::string file_name);

        std::string getMetaData();

   private:
        std::string meta_data;
};

#endif // STATIONCONFIG_H

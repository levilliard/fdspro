/*-----------------------------------------
 * @levilliard
 * 2016
 *-----------------------------------------
 */

#include "stationconfig.h"

using namespace std;

StationConfig::StationConfig()
{
    meta_data = "";
}

string StationConfig::getStationMetaData(){
    return meta_data;
}

std::vector<float> StationConfig::getStationData(string file_name){

    vector<float> data;

    ifstream infile("data/stations_data/" + file_name);

    if(!infile){
        std::cout<<"data not found ....\n";
        return data;
    }

    string line;

    //read meta data
    int namp = 0;
    getline(infile, meta_data);

    while (getline(infile, line)) {
        istringstream iss(line);
        float temp;
        if(!(iss>>temp)){
            temp = 0;
        }

        namp++;
        data.push_back(temp);
    }

    return data;
}

std::string StationConfig::getMetaData(){
    return this->meta_data;
}

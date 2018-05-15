
/*
  @levilliard
  FDS Pro 2015
  -------------------------------
 */


#ifndef STATION_H
#define STATION_H

#include <string>

class Station
{
    public:
        Station(){}

        Station(const std::string& id, const std::string& code, double lat, double lon, const std::string& elv, const std::string& desc, const std::string& rm, const std::string& lm) {
            stationId = id;
            stationCode = code;
            stationLat = lat;
            stationLon = lon;
            stationElevation = elv;
            stationDesc = desc;
            stationRemark = rm;
            stationLastModified = lm;
        }

        void setStationId(const std::string& id){
            stationId = id;
        }

        std::string getStaionId(){
            return stationId;
        }

        void setStationCode(const std::string& code){
            stationCode = code;
        }

        std::string getStationCode(){
            return stationCode;
        }

        void setStationLat(const double lat){
            stationLat = lat;
        }

        double getStationLat(){
            return stationLat;
        }


        void setStationLon(const double lon){
            stationLon = lon;
        }

        double getStationLon(){
            return stationLon;
        }

        void setStationElevation(const std::string& elevation){
            stationElevation = elevation;
        }

        std::string getStationElevation(){
            return stationElevation;
        }



        void setStationDesc(const std::string& desc){
            stationDesc = desc;
        }

        std::string getStationDesc(){
            return stationDesc;
        }



        void setStationRemark(const std::string& remark){
            stationRemark = remark;
        }

        std::string getStationRemark(){
            return stationRemark;
        }



        void setStationLastModified(const std::string& lastModified){
            stationLastModified = lastModified;
        }

        std::string getStationLastModified(){
            return stationLastModified;
        }


    private:
        std::string stationId;
        std::string stationCode;
        double stationLat;
        double stationLon;
        std::string stationElevation;
        std::string stationDesc;
        std::string stationRemark;
        std::string stationLastModified;
};
#endif  //STATION_H

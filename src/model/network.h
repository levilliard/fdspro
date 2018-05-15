

#ifndef NETWORK_H
#define NETWORK_H

#include <string>

using namespace std;

class Network
{
   public:
        Network(){}

        Network(const string& id, const string& code, const string& inst, const string& reg, const string& modif, const string& rem){
            networkId = id;
            networkCode = code;
            networkInstitution = inst;
            networkRegion = reg;
            networkLastModified = modif;
            networkRemarks = rem;
        }

        void setNetworkId(const std:: string& id){
            networkId = id;
        }

        std::string getNeworkId(){
            return networkId;
        }

        void setNetworkCode(const std::string& code){
            networkCode = code;
        }

        std::string getNetworkCode(){
            return networkCode;
        }

        void setNwetworkInstitution(const std::string& institution){
            networkInstitution = institution;
        }

        std::string getNetworkInstitution(){
            return networkInstitution;
        }

        void setNetworkRegion(const std::string& region){
            networkRegion = region;
        }

        std::string getNetworkRegion(){
            return networkRegion;
        }

        void setNetworkLastModified(const std::string& lModified){
            networkLastModified = lModified;
        }

        std::string getNetworkLastModified(){
            return networkLastModified;
        }

        void setNetworkRemarks(const std::string& remarks){
            networkRemarks = remarks;
        }

        std::string getNetworkRemarks(){
            return networkRemarks;
        }

   private:
        std::string networkId;
        std::string networkCode;
        std::string networkInstitution;
        std::string networkRegion;
        std::string networkLastModified;
        std::string networkRemarks;
};


struct NetworkRequest
{
    std::string network_code;
    std::string date_start;
    std::string date_end;
};

#endif  //NETWORK_H

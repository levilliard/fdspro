#ifndef EVENT_H
#define EVENT_H

#include <string>

class Event{
private:
    std::string m_time;
    double latitude;
    double longitude;
    double mag;
    std::string magType;
    double nst;
    double gap;
    double dmin;
    double rms;
    std::string net;
    std::string id;
    std::string updated;
    std::string place;
    std::string eventType;


public:
    std::string getTime() const{
        return m_time;
    }

    void setTime(const std::string& tm){
        m_time = tm;
    }

    double getLatitude() const{
        return latitude;
    }

    void setLatitude(double lat){
        latitude = lat;
    }

    double getLongitude() const{
        return longitude;
    }

    void setLongitude(double lon){
        longitude = lon;
    }

    double getMag() const{
        return mag;
    }

    void setMag(double ma){
        mag = ma;
    }

    std::string getMagType() const{
        return magType;
    }

    void setMagType(const std::string& mt){
        magType = mt;
    }

    double getNST() const{
        return nst;
    }

    void setNST(double n){
        nst = n;
    }

    double getGap() const{
        return gap;
    }

    void setGap(double gp){
        gap = gp;
    }

    double getDMin() const{
        return dmin;
    }

    void setDMin(double dm){
        dmin = dm;
    }

    double getRMS() const{
        return rms;
    }

    void setRMS(double rm){
        rms = rm;
    }

    std::string getNet() const{
        return net;
    }

    void setNet(const std::string& n){
        net = n;
    }

    std::string getId() const{
        return id;
    }

    void setId(const std::string& id){
        this->id = id;
    }

    std::string getUpdated() const{
        return updated;
    }

    void setUpdated(const std::string& upd){
        updated = upd;
    }

    std::string getPlace() const{
        return place;
    }

    void setPlace(const std::string& pl){
        place = pl;
    }

    std::string getEventType() const{
        return eventType;
    }

    void setEventType(const std::string& evt){
        eventType = evt;
    }
};

//select sample events from database
struct  SampleEvent
{
     int sample_id;
     std::string sample_name;
     std::string sample_path;
     int sample_data_size;
     std::string created_by;
     std::string date_created;
     std::string modified_by;
     std::string date_modified;
     std::string periode;
};

#endif // EVENT_H

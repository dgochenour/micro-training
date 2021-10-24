
#ifndef Application_h
#define Application_h

#ifndef rti_me_cpp_hxx
#include "rti_me_cpp.hxx"
#endif

class Application
{
  public:
    DDSDomainParticipant *participant; 
    DDS_Long sleep_time;
    DDS_Long count;

    static void help(char *appname);

    DDS_ReturnCode_t initialize(
            DDS_Long domain_id, 
            char *udp_intf, 
            char *peer, 
            DDS_Long sleep_time, 
            DDS_Long count);

    DDS_ReturnCode_t enable();

    Application();
    ~Application();
};

#endif

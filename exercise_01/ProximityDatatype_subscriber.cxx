#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef rti_me_cpp_hxx
#include "rti_me_cpp.hxx"
#endif
#include "wh_sm/wh_sm_history.h"
#include "rh_sm/rh_sm_history.h"

#include "ProximityDatatype.h"
#include "ProximityDatatypeSupport.h"
#include "ProximityDatatypePlugin.h"
#include "ProximityDatatypeApplication.h"

class ProximityDataReaderListener : public DDSDataReaderListener
{

public:

    virtual void on_data_available(DDSDataReader *reader);
};

template <typename T>
void take_and_print(typename T::DataReader *reader)
{
    DDS_ReturnCode_t retcode;
    struct DDS_SampleInfo sample_info;
    T *sample = NULL;

    sample = T::TypeSupport::create_data();
    if (sample == NULL) {
        std::cout << "ERROR: Failed sample initialize" << std::endl;
        return;
    }

    retcode = reader->take_next_sample(*sample, sample_info);
    while (retcode == DDS_RETCODE_OK) {
        if (sample_info.valid_data) {
            std::cout << "\nValid sample received\n" << std::endl;
            /* TODO read sample attributes here */
        } else {
            std::cout << "\nSample received\n\tINVALID DATA" << std::endl;
        }
        retcode = reader->take_next_sample(*sample, sample_info);
    }

    T::TypeSupport::delete_data(sample);
}

void ProximityDataReaderListener::on_data_available(DDSDataReader * reader)
{
    ProximityDataDataReader *hw_reader = ProximityDataDataReader::narrow(reader);
    take_and_print<ProximityData>(hw_reader);
}

int subscriber_main_w_args(
        DDS_Long domain_id, 
        char *udp_intf, 
        char *peer,
        DDS_Long sleep_time, 
        DDS_Long count)
{
    ProximityDataReaderListener *listener  = new ProximityDataReaderListener();
    DDS_ReturnCode_t retcode;
    Application *application = NULL;

    try {

        application = new Application();
        if (application == NULL) {
            printf("failed Application new\n");
            throw "Application alloc";
        }

        retcode = application->initialize( 
                domain_id,
                udp_intf, 
                peer, 
                sleep_time, 
                count);
        if (retcode != DDS_RETCODE_OK) {
            throw "Application init";
        }

        DDSSubscriber *subscriber = NULL;
        subscriber = application->participant->create_subscriber(
            DDS_SUBSCRIBER_QOS_DEFAULT,NULL,
            DDS_STATUS_MASK_NONE);
        if (subscriber == NULL) {
            throw "subscriber";
        }

        DDS_DataReaderQos dr_qos;
        retcode = subscriber->get_default_datareader_qos(dr_qos);
        if (retcode != DDS_RETCODE_OK) {
            throw "dr_qos";
        }

        dr_qos.resource_limits.max_samples = 32;
        dr_qos.resource_limits.max_instances = 1;
        dr_qos.resource_limits.max_samples_per_instance = 32;
        dr_qos.reader_resource_limits.max_remote_writers = 10;
        dr_qos.reader_resource_limits.max_remote_writers_per_instance = 10;
        dr_qos.history.depth = 32;

        /* Reliability QoS */
        #ifdef USE_RELIABLE_QOS
        dr_qos.reliability.kind = DDS_RELIABLE_RELIABILITY_QOS;
        #else
        dr_qos.reliability.kind = DDS_BEST_EFFORT_RELIABILITY_QOS;
        #endif

        DDSDataReader *datareader = NULL;
        datareader = subscriber->create_datareader(
            application->topic,
            dr_qos,
            listener,
            DDS_DATA_AVAILABLE_STATUS);

        if (datareader == NULL) {
            throw "datareader";
        }

        retcode = application->enable();
        if (retcode != DDS_RETCODE_OK) {
            throw "application enable";
        }

    } catch (const char *msg) {

        if (application != NULL) {
            delete application;
        }
        if (listener != NULL) {
            delete listener;
        }
        //DataReaderQos is automatically disposed by destructor

        return DDS_RETCODE_ERROR;
    }


    if (application->count != 0) {

        std::cout << "Running for " << application->count << 
                "seconds, press Ctrl-C to exit" << std::endl;
        OSAPI_Thread_sleep(application->count * 1000);

    } else {

        int sleep_loop_count =  (24 * 60 * 60) / 2000;
        int sleep_loop_left = (24 * 60 * 60) % 2000;

        std::cout << "Running for 24 hours, press Ctrl-C to exit" << std::endl;

        while (sleep_loop_count) {
            OSAPI_Thread_sleep(2000  * 1000); /* sleep is in ms */
            --sleep_loop_count;
        }

        OSAPI_Thread_sleep(sleep_loop_left * 1000);
    }

    if (application != NULL) {
        delete application;
    }

    if (listener != NULL) {
        delete listener;
    }

    //DataReaderQos is automatically disposed by destructor

    return DDS_RETCODE_OK;
}


int main(int argc, char **argv)
{
    DDS_Long i = 0;
    DDS_Long domain_id = 0;
    char *peer = NULL;
    char *udp_intf = NULL;
    DDS_Long sleep_time = 1000;
    DDS_Long count = 0;

    for (i = 1; i < argc; ++i)
    {
        if (!strcmp(argv[i], "-domain"))
        {
            ++i;
            if (i == argc)
            {
                printf("-domain <domain_id>\n");
                return -1;
            }
            domain_id = strtol(argv[i], NULL, 0);
        }
        else if (!strcmp(argv[i], "-udp_intf"))
        {
            ++i;
            if (i == argc)
            {
                printf("-udp_intf <interface>\n");
                return -1;
            }
            udp_intf = argv[i];
        }
        else if (!strcmp(argv[i], "-peer"))
        {
            ++i;
            if (i == argc)
            {
                printf("-peer <address>\n");
                return -1;
            }
            peer = argv[i];
        }
        else if (!strcmp(argv[i], "-sleep"))
        {
            ++i;
            if (i == argc)
            {
                printf("-sleep_time <sleep_time>\n");
                return -1;
            }
            sleep_time = strtol(argv[i], NULL, 0);
        }
        else if (!strcmp(argv[i], "-count"))
        {
            ++i;
            if (i == argc)
            {
                printf("-count <count>\n");
                return -1;
            }
            count = strtol(argv[i], NULL, 0);
        }
        else if (!strcmp(argv[i], "-h"))
        {
            Application::help(argv[0]);
            return 0;
        }
        else
        {
            printf("unknown option: %s\n", argv[i]);
            return -1;
        }
    }

    return subscriber_main_w_args(domain_id, udp_intf, peer, sleep_time, count);
}


//#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>

#ifndef rti_me_cpp_hxx
#include "rti_me_cpp.hxx"
#endif

#include "wh_sm/wh_sm_history.h"
#include "rh_sm/rh_sm_history.h"

#include "Datatypes.h"
#include "DatatypesSupport.h"
#include "application_common.h"


int proximity_sensor_main(
        DDS_Long domain_id, 
        char *udp_intf, 
        char *peer, 
        DDS_Long sleep_time, 
        DDS_Long count)
{
    DDS_ReturnCode_t retcode;

    Application *application = NULL;
    ProximityData *sample = NULL;

    try {
        application = new Application();
        if (application == NULL) {
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

        DDSPublisher *publisher = NULL;
        publisher = application->participant->create_publisher(
                DDS_PUBLISHER_QOS_DEFAULT,
                NULL,
                DDS_STATUS_MASK_NONE);
        if (publisher == NULL) {
            throw "publisher";
        }

        // The *get_default_type_name() function is generated by rtiddsgen in 
        // the file DataTypesPlugin.cxx
        retcode = ProximityDataTypeSupport::register_type(
                    application->participant,
                    ProximityDataTypePlugin_get_default_type_name());
        if (retcode != DDS_RETCODE_OK) {
            throw "register ProximityData type";
        }

        DDSTopic *proximity_data_topic = NULL;
        proximity_data_topic = application->participant->create_topic(
                PROXIMITY_DATA_TOPIC_NAME, // this constant is defined in the IDL
                ProximityDataTypePlugin_get_default_type_name(),
                DDS_TOPIC_QOS_DEFAULT, 
                NULL,
                DDS_STATUS_MASK_NONE);
        if (proximity_data_topic == NULL) {
            throw "proximity_data_topic";
        } 

        DDS_DataWriterQos dw_qos;
        retcode = publisher->get_default_datawriter_qos(dw_qos);
        if (retcode != DDS_RETCODE_OK) {
            throw "dw_qos";
        }

        dw_qos.reliability.kind = DDS_BEST_EFFORT_RELIABILITY_QOS;
        dw_qos.resource_limits.max_samples = 32;
        dw_qos.resource_limits.max_samples_per_instance = 32;
        dw_qos.resource_limits.max_instances = 1;
        dw_qos.history.depth = 32;
        dw_qos.deadline.period.sec = 0;
        dw_qos.deadline.period.nanosec = 50000000;

        DDSDataWriter *datawriter = NULL;
        datawriter = publisher->create_datawriter(
                proximity_data_topic,
                dw_qos,
                NULL,
                DDS_STATUS_MASK_NONE);
        if (datawriter == NULL) {
            throw "datawriter";
        }

        retcode = application->enable();
        if (retcode != DDS_RETCODE_OK) {
            throw "application enable";
        }

        ProximityDataDataWriter *ProximityData_writer = NULL;
        ProximityData_writer = ProximityDataDataWriter::narrow(datawriter);
        if (ProximityData_writer == NULL) {
            throw "ProximityData_writer";
        }

        sample = ProximityDataTypeSupport::create_data();
        if (sample == NULL) {
            throw "ProximityData sample";
        }

        float proximity_step = 1;
        float sensor_proximity = 150;

        sample->sensor_id = DDS_String_dup("Sensor123");

        for (DDS_Long i = 0;
                (application->count > 0 && i < application->count) || (application->count == 0);
                ++i) {
            if (sensor_proximity == 0 || sensor_proximity == 300) {
                proximity_step *= -1;
            }

            sample->proximity = sensor_proximity;

            retcode = ProximityData_writer->write(*sample, DDS_HANDLE_NIL);
            if (retcode != DDS_RETCODE_OK) {
                std::cout << "ERR: Failed to write sample" << std::endl;
            } else {
                std::cout << "INFO: Wrote sample " << (i+1) << std::endl;
            } 

            OSAPI_Thread_sleep(application->sleep_time);
            sensor_proximity += proximity_step;
        }
    } catch (const char* msg) {
        std::cerr << msg << std::endl;
    }

    if (application != NULL) {
        delete application;
    }

    if (sample != NULL) {
        ProximityDataTypeSupport::delete_data(sample);
    } 

    return 0;
}


int main(int argc, char **argv)
{

    DDS_Long i = 0;
    DDS_Long domain_id = 0;
    char *peer = NULL;
    char *udp_intf = NULL;
    DDS_Long sleep_time = 50; // 50ms default
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

    return proximity_sensor_main(domain_id, udp_intf, peer, sleep_time, count);
}


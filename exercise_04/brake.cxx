#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#ifndef rti_me_cpp_hxx
#include "rti_me_cpp.hxx"
#endif
#include "wh_sm/wh_sm_history.h"
#include "rh_sm/rh_sm_history.h"

#include "Datatypes.h"
#include "DatatypesSupport.h"
#include "DatatypesPlugin.h"
#include "application_common.h"

class BrakeCommandReaderListener : public DDSDataReaderListener
{

public:
    virtual void on_data_available(DDSDataReader *reader);
};

void BrakeCommandReaderListener::on_data_available(DDSDataReader * reader) {

    DDS_ReturnCode_t retcode;

    BrakeCommandDataReader *BrakeCommand_dr_narrowed = NULL;
    BrakeCommand_dr_narrowed = BrakeCommandDataReader::narrow(reader);
    if (BrakeCommand_dr_narrowed == NULL) {
        std::cerr << "DataReader narrow error" << std::endl;
        return;
    }

    BrakeCommandSeq data_seq;
    DDS_SampleInfoSeq info_seq;
    retcode = BrakeCommand_dr_narrowed->take(
            data_seq,
            info_seq,
            DDS_LENGTH_UNLIMITED,
            DDS_ANY_SAMPLE_STATE,
            DDS_ANY_VIEW_STATE,
            DDS_ANY_INSTANCE_STATE);

    if (retcode == DDS_RETCODE_NO_DATA) {
        return;
    } else if (retcode != DDS_RETCODE_OK) {
        std::cerr << "ERROR: take error " << retcode << std::endl;
        return;
    }

    for (int i = 0; i < data_seq.length(); ++i) {
        if (info_seq[i].valid_data) {
            std::cout << "INFO: Received data" << std::endl;
            std::cout << "\tDevice ID = " << data_seq[i].device_id << std::endl;
            std::cout << "\tIntensity (%) = " << 
                    data_seq[i].brake_intensity << std::endl;
        } 
    }

    retcode = BrakeCommand_dr_narrowed->return_loan(data_seq, info_seq);
    if (retcode != DDS_RETCODE_OK) {
        std::cerr << "ERROR: return loan error: " << retcode << std::endl;
    }
}

int brake_main(
        DDS_Long domain_id, 
        char *udp_intf, 
        char *peer,
        DDS_Long sleep_time, 
        DDS_Long count)
{
    // general application
    Application *application = NULL;
    DDSWaitSet *waitset = NULL;
    DDS_ReturnCode_t retcode;
    
    // related to Brake Command Topic
    DDSDataReader *BrakeCommand_dr = NULL;
    BrakeCommandReaderListener *listener  = new BrakeCommandReaderListener();
    BrakeCommand *BrakeCommand_sample = NULL;
    BrakeCommand_sample = BrakeCommandTypeSupport::create_data();
    
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

        DDSPublisher *publisher = NULL;
        publisher = application->participant->create_publisher(
                DDS_PUBLISHER_QOS_DEFAULT,
                NULL,
                DDS_STATUS_MASK_NONE);
        if (publisher == NULL) {
            throw "publisher";
        }

        DDSSubscriber *subscriber = NULL;
        subscriber = application->participant->create_subscriber(
                DDS_SUBSCRIBER_QOS_DEFAULT,
                NULL,
                DDS_STATUS_MASK_NONE);
        if (subscriber == NULL) {
            throw "subscriber";
        }

        retcode = BrakeCommandTypeSupport::register_type(
                application->participant,
                BrakeCommandTypePlugin_get_default_type_name());
        if (retcode != DDS_RETCODE_OK) {
            throw "register BrakeCommand type";
        }

        DDSTopic *brake_command_topic = NULL;
        brake_command_topic = application->participant->create_topic(
                BRAKE_COMMAND_TOPIC_NAME, // this constant is defined in the IDL
                BrakeCommandTypePlugin_get_default_type_name(),
                DDS_TOPIC_QOS_DEFAULT, 
                NULL,
                DDS_STATUS_MASK_NONE);
        if (brake_command_topic == NULL) {
            throw "brake_command_topic";
        }

        DDS_DataReaderQos dr_qos;
        retcode = subscriber->get_default_datareader_qos(dr_qos);
        if (retcode != DDS_RETCODE_OK) {
            throw "dr_qos";
        }

        // TODO - QoS changes
        dr_qos.reliability.kind = DDS_RELIABLE_RELIABILITY_QOS;
        dr_qos.durability.kind = DDS_TRANSIENT_LOCAL_DURABILITY_QOS;

        BrakeCommand_dr = subscriber->create_datareader(
                brake_command_topic,
                dr_qos,
                listener,
                DDS_DATA_AVAILABLE_STATUS);
        if (BrakeCommand_dr == NULL) {
            throw "BrakeCommand_dr";
        }

        retcode = application->enable();
        if (retcode != DDS_RETCODE_OK) {
            throw "application enable";
        }

    } catch (const char *msg) {
        std::cerr << msg << std::endl;
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

    // we only get here after the while() loop above ends... after a long time    
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

    return brake_main(domain_id, udp_intf, peer, sleep_time, count);
}

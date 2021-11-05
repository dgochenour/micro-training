#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef rti_me_cpp_hxx
#include "rti_me_cpp.hxx"
#endif
#include "wh_sm/wh_sm_history.h"
#include "rh_sm/rh_sm_history.h"

#include "Datatypes.h"
#include "DatatypesSupport.h"
#include "DatatypesPlugin.h"
#include "application_common.h"

class ProximityDataReaderListener : public DDSDataReaderListener
{

public:

    virtual void on_requested_deadline_missed(
            DDSDataReader* reader,
            const DDS_RequestedDeadlineMissedStatus& status);
};

void ProximityDataReaderListener::on_requested_deadline_missed(
            DDSDataReader* reader,
            const DDS_RequestedDeadlineMissedStatus& status) 
{
    std::cout << "We missed a deadline!!" << std::endl;
}


bool is_brake_command_needed(DDS_Float received_proximity) {

    static DDS_Float previously_received_proximity = 1000;
    bool send_brake_command_sample = false;

    if (
            ((50 < previously_received_proximity && previously_received_proximity <= 100) &&
            (50 < received_proximity && received_proximity <= 100)) ||
            ((0 <= previously_received_proximity && previously_received_proximity <= 50) &&
            (0 <= received_proximity && received_proximity <= 50)) ||
            ((100 < previously_received_proximity) &&
            (100 < received_proximity))) {

        send_brake_command_sample = false;

    } else {
        send_brake_command_sample = true;
    }
    previously_received_proximity = received_proximity;
    return send_brake_command_sample;
}

void process_proximity_data(
        DDSDataReader *the_proximity_data_DataReader,
        DDSDataWriter *the_brake_command_DataWriter,
        BrakeCommand *BrakeCommand_sample) 
{

    DDS_ReturnCode_t retcode;

    ProximityDataDataReader *ProximityData_dr_narrowed = NULL;
    ProximityData_dr_narrowed = 
            ProximityDataDataReader::narrow(the_proximity_data_DataReader);
    if (ProximityData_dr_narrowed == NULL) {
        std::cerr << "DataReader narrow error" << std::endl;
        return;
    }

    BrakeCommandDataWriter *BrakeCommand_dw_narrowed = NULL;
    BrakeCommand_dw_narrowed = 
            BrakeCommandDataWriter::narrow(the_brake_command_DataWriter);
    if (BrakeCommand_dw_narrowed == NULL) {
            std::cerr << "DataWriter narrow error" << std::endl;
    }

    ProximityDataSeq data_seq;
    DDS_SampleInfoSeq info_seq;
    retcode = ProximityData_dr_narrowed->take(
            data_seq,
            info_seq,
            DDS_LENGTH_UNLIMITED,
            DDS_ANY_SAMPLE_STATE,
            DDS_ANY_VIEW_STATE,
            DDS_ANY_INSTANCE_STATE);

    if (retcode == DDS_RETCODE_NO_DATA) {
        return;
    } else if (retcode != DDS_RETCODE_OK) {
        std::cerr << "take error " << retcode << std::endl;
        return;
    }

    for (int i = 0; i < data_seq.length(); ++i) {
        if (info_seq[i].valid_data) {
            printf("Received data\n");
            printf("Sensor ID = %s\n", data_seq[i].sensor_id);
            printf("Proximity (m) = %f\n\n", data_seq[i].proximity);
        
            if(is_brake_command_needed(data_seq[i].proximity)) { 
                
                BrakeCommand_sample->device_id = "BRAKE01";              
                if (data_seq[i].proximity > 100) {
                    BrakeCommand_sample->brake_intensity = 0;
                } else if (data_seq[i].proximity > 50) {
                    BrakeCommand_sample->brake_intensity = 50;
                } else {
                    BrakeCommand_sample->brake_intensity = 100;
                    }
                retcode = BrakeCommand_dw_narrowed->write(*BrakeCommand_sample, DDS_HANDLE_NIL);
                if (retcode != DDS_RETCODE_OK) {
                    std::cerr << "take error " << retcode << std::endl;
                    return;
                }

                //BrakeCommandTypeSupport::delete_data(BrakeCommand_sample);
            }
        }
    }

    retcode = ProximityData_dr_narrowed->return_loan(data_seq, info_seq);
    if (retcode != DDS_RETCODE_OK) {
        std::cerr << "return loan error " << retcode << std::endl;
    }
}

int controller_main(
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

    // related to Proximity Data Topic
    DDSDataReader *ProximityData_dr = NULL;
    ProximityDataReaderListener *listener  = new ProximityDataReaderListener();
    
    // related to Brake Command Topic
    DDSDataWriter *BrakeCommand_dw = NULL;
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
            DDS_SUBSCRIBER_QOS_DEFAULT,NULL,
            DDS_STATUS_MASK_NONE);
        if (subscriber == NULL) {
            throw "subscriber";
        }

        //----------------------------------------------------------------------
        // Brake Command Topic related
        //----------------------------------------------------------------------

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

        DDS_DataWriterQos dw_qos;
        retcode = publisher->get_default_datawriter_qos(dw_qos);
        if (retcode != DDS_RETCODE_OK) {
            throw "dw_qos";
        }

        // TODO - QoS changes
        dw_qos.reliability.kind = DDS_RELIABLE_RELIABILITY_QOS;
        dw_qos.durability.kind = DDS_TRANSIENT_LOCAL_DURABILITY_QOS;

        BrakeCommand_dw = publisher->create_datawriter(
                brake_command_topic,
                dw_qos,
                NULL,
                DDS_STATUS_MASK_NONE);
        if (BrakeCommand_dw == NULL) {
            throw "BrakeCommand_dw";
        }

        //----------------------------------------------------------------------
        // Proximity Data Topic related
        //----------------------------------------------------------------------

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

        dr_qos.reliability.kind = DDS_BEST_EFFORT_RELIABILITY_QOS;
        dr_qos.deadline.period.sec = 0;
        dr_qos.deadline.period.nanosec = 100000000;

        ProximityData_dr = subscriber->create_datareader(
                proximity_data_topic,
                dr_qos,
                listener,
                DDS_REQUESTED_DEADLINE_MISSED_STATUS);
        if (ProximityData_dr == NULL) {
            throw "ProximityData_dr";
        }
        // Create a waitset where we'll wait for new data
        DDSStatusCondition *status_condition = NULL;
        status_condition = ProximityData_dr->get_statuscondition();
        if (!status_condition) {
            throw "status_condition";
        }
        retcode = status_condition->set_enabled_statuses(DDS_DATA_AVAILABLE_STATUS);
        if (retcode != DDS_RETCODE_OK) {              
            throw "set_enabled_statuses";
        }

        waitset = new DDSWaitSet();
        if(!waitset) {  
            throw "waitset";
        }
        retcode = waitset->attach_condition(status_condition);
        if (retcode != DDS_RETCODE_OK) {
            throw "attach_condition";  
        }             
        //----------------------------------------------------------------------
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

    // waitset initialization
    DDSConditionSeq active_conditions_seq;
    active_conditions_seq.maximum(1);
    DDS_Duration_t wait_timeout = { 10, 0 };

    while(1) {
        
        retcode = waitset->wait(active_conditions_seq, wait_timeout);
        if (retcode == DDS_RETCODE_OK) {
            // we got some data
            process_proximity_data(
                    ProximityData_dr, 
                    BrakeCommand_dw, 
                    BrakeCommand_sample);
        } else if (retcode == DDS_RETCODE_TIMEOUT) {
            // timeout
            std::cout << "INFO: wait timed out, no conditions triggered" << std::endl;
            continue;
        } else {
            // error
            std::cout << "ERROR: Wait error: " << retcode << std::endl;
            break;
        }
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

    return controller_main(domain_id, udp_intf, peer, sleep_time, count);
}

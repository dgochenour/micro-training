
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>

#ifndef rti_me_cpp_hxx
#include "rti_me_cpp.hxx"
#endif

#include "ProximityDatatypeApplication.h"
#include "ProximityDatatypePlugin.h"
#include "ProximityDatatypeSupport.h"

#include "dds_cpp/dds_cpp_dpde.hxx"
#include "dds_cpp/dds_cpp_wh_sm.hxx"
#include "dds_cpp/dds_cpp_rh_sm.hxx"
#include "dds_cpp/dds_cpp_netio.hxx"

void Application::help(char *appname)
{
    printf("%s [options]\n", appname);
    printf("options:\n");
    printf("-h                 - This text\n");
    printf("-domain <id>       - DomainId (default: 0)\n");
    printf("-udp_intf <intf>   - udp interface (no default)\n");
    printf("-peer <address>    - peer address (no default)\n");
    printf("-count <count>     - count (default -1)\n");
    printf("-sleep <ms>        - sleep between sends (default 1s)\n");
    printf("\n");
}

DDS_ReturnCode_t Application::initialize(
        DDS_Long domain_id,
        char *udp_intf,
        char *peer,
        DDS_Long sleep_time,
        DDS_Long count)
{
    DDS_ReturnCode_t retcode;

    this->sleep_time = sleep_time;
    this->count = count;

    DDSDomainParticipantFactory *factory = NULL; 
    factory = DDSDomainParticipantFactory::get_instance();
    RTRegistry *registry = NULL;
    registry = factory->get_registry();

    UDP_InterfaceFactoryProperty *udp_property = NULL;

    try {

        if (!registry->register_component(
                "wh", 
                WHSMHistoryFactory::get_interface(),
                NULL, 
                NULL)) {
            throw "WHSM";
        }
        if (!registry->register_component(
                "rh",
                RHSMHistoryFactory::get_interface(),
                NULL, 
                NULL)) {
            throw "RHSM";
        }

        /* Configure UDP transport's allowed interfaces */
        if (!registry->unregister(NETIO_DEFAULT_UDP_NAME, NULL, NULL)) {
            throw "unregister udp";
        }

        udp_property = new UDP_InterfaceFactoryProperty();
        if (udp_property == NULL) {
            throw "allocate udp properties";
        }

        if (!udp_property->allow_interface.maximum(2)) {
            throw "allow_interface maximum";
        }
        if (!udp_property->allow_interface.length(2)) {
            throw "allow_interface length";
        }

        /* loopback interface */
        *udp_property->allow_interface.get_reference(0) = DDS_String_dup("lo");

        if (udp_intf != NULL) { 
            /* use interface supplied on command line */
            *udp_property->allow_interface.get_reference(1) =
            DDS_String_dup(udp_intf);
        } else {
            *udp_property->allow_interface.get_reference(1) = 
            DDS_String_dup("wlp0s20f3");

        }

        if (!registry->register_component(
                NETIO_DEFAULT_UDP_NAME,
                UDPInterfaceFactory::get_interface(),
                &udp_property->_parent._parent,
                NULL)) {
            throw "register udp";
        }

        DDS_DomainParticipantFactoryQos dpf_qos;
        factory->get_qos(dpf_qos);
        dpf_qos.entity_factory.autoenable_created_entities = DDS_BOOLEAN_FALSE;
        factory->set_qos(dpf_qos);

        if (peer == NULL) {
            peer = (char *)"127.0.0.1"; /* default to loopback */
        }

        DPDE_DiscoveryPluginProperty dpde_properties;
        if (!registry->register_component(
                "dpde",
                DPDEDiscoveryFactory::get_interface(),
                &dpde_properties._parent,
                NULL)) {
            throw "register dpde";
        }
        DDS_DomainParticipantQos dp_qos;
        if (!dp_qos.discovery.discovery.name.set_name("dpde")) {
            throw "failed to set discovery plugin name";
        }

        dp_qos.discovery.initial_peers.maximum(1);
        dp_qos.discovery.initial_peers.length(1);
        *dp_qos.discovery.initial_peers.get_reference(0) = DDS_String_dup(peer);

        /* if there are more remote or local endpoints, you need to increase these limits */
        dp_qos.resource_limits.max_destination_ports = 32;
        dp_qos.resource_limits.max_receive_ports = 32;
        dp_qos.resource_limits.local_topic_allocation = 1;
        dp_qos.resource_limits.local_type_allocation = 1;
        dp_qos.resource_limits.local_reader_allocation = 1;
        dp_qos.resource_limits.local_writer_allocation = 1;
        dp_qos.resource_limits.remote_participant_allocation = 10; //was 8
        dp_qos.resource_limits.remote_reader_allocation = 32; //was 8
        dp_qos.resource_limits.remote_writer_allocation = 32; //was 8

        this->participant = factory->create_participant(
                (DDS_DomainId_t)domain_id,
                dp_qos, 
                NULL,
                DDS_STATUS_MASK_NONE);

        if (this->participant == NULL) {
            throw "DomainParticipant";
        }

        strcpy(this->type_name,"ProximityData"); //TODO - name lookup
        retcode = ProximityDataTypeSupport::register_type(this->participant,
                this->type_name);
        if (retcode != DDS_RETCODE_OK) {
            throw "register type";
        }

        strcpy(this->topic_name, "Example ProximityData"); //TODO - update Topic name
        this->topic = this->participant->create_topic(
                this->topic_name,
                this->type_name,
                DDS_TOPIC_QOS_DEFAULT, 
                NULL,
                DDS_STATUS_MASK_NONE);
        if (this->topic == NULL) {
            throw "Topic";
        }        

    } catch (const char* msg) {
        std::cerr << msg << std::endl;
        if (udp_property != NULL) {
            delete udp_property;
            udp_property = NULL;
        }
        return DDS_RETCODE_ERROR;
    }
    return DDS_RETCODE_OK;

}

DDS_ReturnCode_t Application::enable()
{
    DDS_ReturnCode_t retcode;

    retcode = this->participant->enable();
    if (retcode != DDS_RETCODE_OK) {
        printf("failed to enable entity\n");
    }
    return retcode;
}

Application::Application()
{
    this->participant = NULL;
    this->topic = NULL;
    this->topic_name[0] = '\0';
    this->type_name[0] = '\0';
    this->sleep_time = 1000;
    this->count = 0;
}

Application::~Application()
{
    DDS_ReturnCode_t retcode;

    if (this->participant != NULL) {
        retcode = this->participant->delete_contained_entities();
        if (retcode != DDS_RETCODE_OK) {
            printf("failed to delete contained entities (retcode=%d)\n",retcode);
        }

        retcode =
        DDSTheParticipantFactory->delete_participant(this->participant);
        if (retcode != DDS_RETCODE_OK) {
            printf("failed to delete participant: %d\n", retcode);
            return;
        }
        this->participant = NULL;
    }

    RTRegistry *registry = NULL;
    registry = (DDSDomainParticipantFactory::get_instance())->get_registry();

    UDP_InterfaceFactoryProperty *udp_property = NULL;
    if (!registry->unregister(
            NETIO_DEFAULT_UDP_NAME,
            (RT_ComponentFactoryProperty**)&udp_property, 
            NULL)) {
        printf("failed to unregister udp\n");
        return;
    }
    if (udp_property != NULL) {
        delete udp_property;
        udp_property = NULL;
    }
    if (!registry->unregister("dpde", NULL, NULL)) {
        printf("failed to unregister dpde\n");
        return;
    }
    if (!registry->unregister("rh", NULL, NULL)) {
        printf("failed to unregister rh\n");
        return;
    }
    if (!registry->unregister("wh", NULL, NULL)) {
        printf("failed to unregister wh\n");
        return;
    }

    retcode = DDSTheParticipantFactory->finalize_instance();
    if (retcode != DDS_RETCODE_OK) {
        printf("failed to finalize instance (retcode=%d)\n",retcode);
        return;
    }
}

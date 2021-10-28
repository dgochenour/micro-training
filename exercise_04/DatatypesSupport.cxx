/*
WARNING: THIS FILE IS AUTO-GENERATED. DO NOT MODIFY.

This file was generated from Datatypes.idl using "rtiddsgen".
The rtiddsgen tool is part of the RTI Data Distribution Service distribution.
For more information, type 'rtiddsgen -help' at a command shell
or consult the RTI Data Distribution Service manual.
*/

#include "DatatypesSupport.h"

/*** SOURCE_BEGIN ***/

/* =========================================================================== */

/* Requires */
#define TTYPENAME   ProximityDataTYPENAME

/* 
ProximityDataDataWriter (DDSDataWriter)   
*/

/* Defines */
#define TDataWriter ProximityDataDataWriter
#define TData       ProximityData

#ifdef __cplusplus
#include "dds_cpp/dds_cpp_tdatawriter_gen.hxx"
#endif

#undef TDataWriter
#undef TData

/* =========================================================================== */
/* 
ProximityDataDataReader (DDSDataReader)   
*/

/* Defines */
#define TDataReader ProximityDataDataReader
#define TDataSeq    ProximityDataSeq
#define TData       ProximityData
#ifdef __cplusplus
#include "dds_cpp/dds_cpp_tdatareader_gen.hxx"
#endif
#undef TDataReader
#undef TDataSeq
#undef TData

/* =========================================================================== */
/* 
ProximityDataTypeSupport

*/

#define TTypeSupport ProximityDataTypeSupport
#define TData        ProximityData

static struct DDS_TypeProgramNode ProximityData_gv_ProgramNode = DDS_TypeProgramNode_INITIALIZER;

static DDS_DataRepresentationId_t ProximityData_gv_AutoRepresentation = DDS_XCDR_DATA_REPRESENTATION;
static DDS_DataRepresentationId_t ProximityData_gv_XCDR1 = DDS_XCDR_DATA_REPRESENTATION;
static DDS_DataRepresentationId_t ProximityData_gv_XCDR2 = DDS_XCDR2_DATA_REPRESENTATION;

DDS_ReturnCode_t 
ProximityDataTypeSupport_serialize_data_to_cdr_buffer_ex(
    char *buffer,                                
    unsigned int *length,                        
    const ProximityData *a_data,                         
    DDS_DataRepresentationId_t  representation)
{
    struct DDS_TypePlugin tp;
    DDS_EncapsulationId_t encapsulation;
    const struct RTIXCdrInterpreterPrograms *programs = NULL;

    DDS_TypePlugin_initialize(&tp);

    programs = DDS_DomainParticipantFactory_assert_program(
        DDS_TheParticipantFactory,
        &ProximityData_gv_ProgramNode,
        ProximityData_get_typecode());

    if (programs == NULL)
    {
        return DDS_RETCODE_ERROR;
    }

    DDS_TypePlugin_initialize_static(
        &tp,
        ProximityDataTypePlugin_get(),
        programs);

    if (DDS_RETCODE_OK != DDS_TypeSupport_resolve_representation(
        &representation,
        ProximityData_gv_AutoRepresentation,
        ProximityData_gv_XCDR1,
        ProximityData_gv_XCDR2,
        &encapsulation))
    {
        return DDS_RETCODE_ERROR;
    }

    return XCDR_Interpreter_serialized_sample_to_buffer(
        &tp,
        buffer,
        length,
        (const void *)a_data,
        representation,
        encapsulation);
}

DDS_ReturnCode_t 
ProximityDataTypeSupport_serialize_data_to_cdr_buffer(
    char *buffer,
    unsigned int *length,
    const ProximityData *a_data)
{
    return ProximityDataTypeSupport_serialize_data_to_cdr_buffer_ex(
        buffer,
        length,
        a_data,
        DDS_AUTO_DATA_REPRESENTATION);

}

DDS_ReturnCode_t 
ProximityDataTypeSupport_deserialize_data_from_cdr_buffer(
    ProximityData *a_data,
    const char *buffer,
    unsigned int length)
{
    struct CDR_Stream_t stream;
    struct DDS_TypePlugin tp;
    const struct RTIXCdrInterpreterPrograms *programs = NULL;

    DDS_TypePlugin_initialize(&tp);

    if (!CDR_Stream_initialize_w_buffer(&stream,buffer,length))
    {
        return DDS_RETCODE_ERROR;
    }

    programs = DDS_DomainParticipantFactory_assert_program(
        DDS_TheParticipantFactory,
        &ProximityData_gv_ProgramNode,
        ProximityData_get_typecode());

    if (programs == NULL)
    {
        return DDS_RETCODE_ERROR;
    }

    DDS_TypePlugin_initialize_static(
        &tp,
        ProximityDataTypePlugin_get(),
        programs);

    if (!XCDR_Interpreter_deserialize(&tp,a_data,&stream,RTI_TRUE,RTI_TRUE,0))
    {
        return DDS_RETCODE_ERROR;
    }

    return DDS_RETCODE_OK;
}

#ifdef __cplusplus
#include "dds_cpp/dds_cpp_ttypesupport_gen.hxx"
#endif

#undef TTypeSupport
#undef TData

#undef TTYPENAME

/* =========================================================================== */

/* Requires */
#define TTYPENAME   BrakeCommandTYPENAME

/* 
BrakeCommandDataWriter (DDSDataWriter)   
*/

/* Defines */
#define TDataWriter BrakeCommandDataWriter
#define TData       BrakeCommand

#ifdef __cplusplus
#include "dds_cpp/dds_cpp_tdatawriter_gen.hxx"
#endif

#undef TDataWriter
#undef TData

/* =========================================================================== */
/* 
BrakeCommandDataReader (DDSDataReader)   
*/

/* Defines */
#define TDataReader BrakeCommandDataReader
#define TDataSeq    BrakeCommandSeq
#define TData       BrakeCommand
#ifdef __cplusplus
#include "dds_cpp/dds_cpp_tdatareader_gen.hxx"
#endif
#undef TDataReader
#undef TDataSeq
#undef TData

/* =========================================================================== */
/* 
BrakeCommandTypeSupport

*/

#define TTypeSupport BrakeCommandTypeSupport
#define TData        BrakeCommand

static struct DDS_TypeProgramNode BrakeCommand_gv_ProgramNode = DDS_TypeProgramNode_INITIALIZER;

static DDS_DataRepresentationId_t BrakeCommand_gv_AutoRepresentation = DDS_XCDR_DATA_REPRESENTATION;
static DDS_DataRepresentationId_t BrakeCommand_gv_XCDR1 = DDS_XCDR_DATA_REPRESENTATION;
static DDS_DataRepresentationId_t BrakeCommand_gv_XCDR2 = DDS_XCDR2_DATA_REPRESENTATION;

DDS_ReturnCode_t 
BrakeCommandTypeSupport_serialize_data_to_cdr_buffer_ex(
    char *buffer,                                
    unsigned int *length,                        
    const BrakeCommand *a_data,                         
    DDS_DataRepresentationId_t  representation)
{
    struct DDS_TypePlugin tp;
    DDS_EncapsulationId_t encapsulation;
    const struct RTIXCdrInterpreterPrograms *programs = NULL;

    DDS_TypePlugin_initialize(&tp);

    programs = DDS_DomainParticipantFactory_assert_program(
        DDS_TheParticipantFactory,
        &BrakeCommand_gv_ProgramNode,
        BrakeCommand_get_typecode());

    if (programs == NULL)
    {
        return DDS_RETCODE_ERROR;
    }

    DDS_TypePlugin_initialize_static(
        &tp,
        BrakeCommandTypePlugin_get(),
        programs);

    if (DDS_RETCODE_OK != DDS_TypeSupport_resolve_representation(
        &representation,
        BrakeCommand_gv_AutoRepresentation,
        BrakeCommand_gv_XCDR1,
        BrakeCommand_gv_XCDR2,
        &encapsulation))
    {
        return DDS_RETCODE_ERROR;
    }

    return XCDR_Interpreter_serialized_sample_to_buffer(
        &tp,
        buffer,
        length,
        (const void *)a_data,
        representation,
        encapsulation);
}

DDS_ReturnCode_t 
BrakeCommandTypeSupport_serialize_data_to_cdr_buffer(
    char *buffer,
    unsigned int *length,
    const BrakeCommand *a_data)
{
    return BrakeCommandTypeSupport_serialize_data_to_cdr_buffer_ex(
        buffer,
        length,
        a_data,
        DDS_AUTO_DATA_REPRESENTATION);

}

DDS_ReturnCode_t 
BrakeCommandTypeSupport_deserialize_data_from_cdr_buffer(
    BrakeCommand *a_data,
    const char *buffer,
    unsigned int length)
{
    struct CDR_Stream_t stream;
    struct DDS_TypePlugin tp;
    const struct RTIXCdrInterpreterPrograms *programs = NULL;

    DDS_TypePlugin_initialize(&tp);

    if (!CDR_Stream_initialize_w_buffer(&stream,buffer,length))
    {
        return DDS_RETCODE_ERROR;
    }

    programs = DDS_DomainParticipantFactory_assert_program(
        DDS_TheParticipantFactory,
        &BrakeCommand_gv_ProgramNode,
        BrakeCommand_get_typecode());

    if (programs == NULL)
    {
        return DDS_RETCODE_ERROR;
    }

    DDS_TypePlugin_initialize_static(
        &tp,
        BrakeCommandTypePlugin_get(),
        programs);

    if (!XCDR_Interpreter_deserialize(&tp,a_data,&stream,RTI_TRUE,RTI_TRUE,0))
    {
        return DDS_RETCODE_ERROR;
    }

    return DDS_RETCODE_OK;
}

#ifdef __cplusplus
#include "dds_cpp/dds_cpp_ttypesupport_gen.hxx"
#endif

#undef TTypeSupport
#undef TData

#undef TTYPENAME


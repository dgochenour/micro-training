/*
WARNING: THIS FILE IS AUTO-GENERATED. DO NOT MODIFY.

This file was generated from Datatypes.idl using "rtiddsgen".
The rtiddsgen tool is part of the RTI Data Distribution Service distribution.
For more information, type 'rtiddsgen -help' at a command shell
or consult the RTI Data Distribution Service manual.
*/

#ifndef Datatypes_1820407355_h
#define Datatypes_1820407355_h

#ifndef rti_me_cpp_hxx
#include "rti_me_cpp.hxx"
#endif

#include "dds_c/dds_c_typecode.h"

#if (defined(RTI_WIN32) || defined(RTI_WINCE)) && defined(NDDS_USER_DLL_EXPORT)
/* If the code is building on Windows, start exporting symbols. */
#undef NDDSUSERDllExport
#define NDDSUSERDllExport __declspec(dllexport)
#endif

static const char * const PROXIMITY_DATA_TOPIC_NAME = "Proximity Data";

static const char * const BRAKE_COMMAND_TOPIC_NAME = "Brake Command";

static const DDS_UnsignedLong SENSOR_ID_MAX_LENGTH = 16;

static const DDS_UnsignedLong DEVICE_ID_MAX_LENGTH = 16;

extern "C" {

    extern const char *ProximityDataTYPENAME;

}

struct ProximityDataSeq;
#ifndef NDDS_STANDALONE_TYPE
class ProximityDataTypeSupport;
class ProximityDataDataWriter;
class ProximityDataDataReader;
#endif
class ProximityData 
{
  public:
    typedef struct ProximityDataSeq Seq;
    #ifndef NDDS_STANDALONE_TYPE
    typedef ProximityDataTypeSupport TypeSupport;
    typedef ProximityDataDataWriter DataWriter;
    typedef ProximityDataDataReader DataReader;
    #endif

    DDS_String   sensor_id ;
    DDS_Float   proximity ;

};
#if (defined(RTI_WIN32) || defined (RTI_WINCE) || defined(RTI_INTIME)) && defined(NDDS_USER_DLL_EXPORT)
/* If the code is building on Windows, start exporting symbols.
*/
#undef NDDSUSERDllExport
#define NDDSUSERDllExport __declspec(dllexport)
#endif

#ifndef NDDS_STANDALONE_TYPE
NDDSUSERDllExport DDS_TypeCode* ProximityData_get_typecode(void); /* Type code */
NDDSUSERDllExport RTIXCdrTypePlugin *ProximityData_get_type_plugin_info(void);
NDDSUSERDllExport RTIXCdrSampleAccessInfo *ProximityData_get_sample_access_info(void);
#endif

#define REDA_SEQUENCE_USER_API
#define T ProximityData
#define TSeq ProximityDataSeq
#define REDA_SEQUENCE_EXCLUDE_C_METHODS
#define REDA_SEQUENCE_USER_CPP
#include <reda/reda_sequence_decl.h>
#define REDA_SEQUENCE_USER_API
#define T ProximityData
#define TSeq ProximityDataSeq
#define REDA_SEQUENCE_EXCLUDE_STRUCT
#define REDA_SEQUENCE_USER_CPP
#include <reda/reda_sequence_decl.h>

NDDSUSERDllExport extern ProximityData*
ProximityData_create();

NDDSUSERDllExport extern void
ProximityData_delete(ProximityData* sample);

NDDSUSERDllExport
RTIBool ProximityData_initialize(
    ProximityData* self);

NDDSUSERDllExport
RTIBool ProximityData_initialize_ex(
    ProximityData* self,
    RTIBool allocatePointers,
    RTIBool allocateMemory);

NDDSUSERDllExport
RTIBool ProximityData_initialize_w_params(
    ProximityData* self,
    const struct DDS_TypeAllocationParams_t * allocParams);  

NDDSUSERDllExport
RTIBool ProximityData_finalize(
    ProximityData* self);

NDDSUSERDllExport
RTIBool ProximityData_finalize_w_return(
    ProximityData* self);

NDDSUSERDllExport
void ProximityData_finalize_ex(
    ProximityData* self,RTIBool deletePointers);

NDDSUSERDllExport
void ProximityData_finalize_w_params(
    ProximityData* self,
    const struct DDS_TypeDeallocationParams_t * deallocParams);

NDDSUSERDllExport
void ProximityData_finalize_optional_members(
    ProximityData* self, RTIBool deletePointers);  

NDDSUSERDllExport
RTIBool ProximityData_copy(
    ProximityData* dst,
    const ProximityData* src);

#if (defined(RTI_WIN32) || defined (RTI_WINCE) || defined(RTI_INTIME)) && defined(NDDS_USER_DLL_EXPORT)
/* If the code is building on Windows, stop exporting symbols.
*/
#undef NDDSUSERDllExport
#define NDDSUSERDllExport
#endif

extern "C" {

    extern const char *BrakeCommandTYPENAME;

}

struct BrakeCommandSeq;
#ifndef NDDS_STANDALONE_TYPE
class BrakeCommandTypeSupport;
class BrakeCommandDataWriter;
class BrakeCommandDataReader;
#endif
class BrakeCommand 
{
  public:
    typedef struct BrakeCommandSeq Seq;
    #ifndef NDDS_STANDALONE_TYPE
    typedef BrakeCommandTypeSupport TypeSupport;
    typedef BrakeCommandDataWriter DataWriter;
    typedef BrakeCommandDataReader DataReader;
    #endif

    DDS_String   device_id ;
    DDS_Float   brake_intensity ;

};
#if (defined(RTI_WIN32) || defined (RTI_WINCE) || defined(RTI_INTIME)) && defined(NDDS_USER_DLL_EXPORT)
/* If the code is building on Windows, start exporting symbols.
*/
#undef NDDSUSERDllExport
#define NDDSUSERDllExport __declspec(dllexport)
#endif

#ifndef NDDS_STANDALONE_TYPE
NDDSUSERDllExport DDS_TypeCode* BrakeCommand_get_typecode(void); /* Type code */
NDDSUSERDllExport RTIXCdrTypePlugin *BrakeCommand_get_type_plugin_info(void);
NDDSUSERDllExport RTIXCdrSampleAccessInfo *BrakeCommand_get_sample_access_info(void);
#endif

#define REDA_SEQUENCE_USER_API
#define T BrakeCommand
#define TSeq BrakeCommandSeq
#define REDA_SEQUENCE_EXCLUDE_C_METHODS
#define REDA_SEQUENCE_USER_CPP
#include <reda/reda_sequence_decl.h>
#define REDA_SEQUENCE_USER_API
#define T BrakeCommand
#define TSeq BrakeCommandSeq
#define REDA_SEQUENCE_EXCLUDE_STRUCT
#define REDA_SEQUENCE_USER_CPP
#include <reda/reda_sequence_decl.h>

NDDSUSERDllExport extern BrakeCommand*
BrakeCommand_create();

NDDSUSERDllExport extern void
BrakeCommand_delete(BrakeCommand* sample);

NDDSUSERDllExport
RTIBool BrakeCommand_initialize(
    BrakeCommand* self);

NDDSUSERDllExport
RTIBool BrakeCommand_initialize_ex(
    BrakeCommand* self,
    RTIBool allocatePointers,
    RTIBool allocateMemory);

NDDSUSERDllExport
RTIBool BrakeCommand_initialize_w_params(
    BrakeCommand* self,
    const struct DDS_TypeAllocationParams_t * allocParams);  

NDDSUSERDllExport
RTIBool BrakeCommand_finalize(
    BrakeCommand* self);

NDDSUSERDllExport
RTIBool BrakeCommand_finalize_w_return(
    BrakeCommand* self);

NDDSUSERDllExport
void BrakeCommand_finalize_ex(
    BrakeCommand* self,RTIBool deletePointers);

NDDSUSERDllExport
void BrakeCommand_finalize_w_params(
    BrakeCommand* self,
    const struct DDS_TypeDeallocationParams_t * deallocParams);

NDDSUSERDllExport
void BrakeCommand_finalize_optional_members(
    BrakeCommand* self, RTIBool deletePointers);  

NDDSUSERDllExport
RTIBool BrakeCommand_copy(
    BrakeCommand* dst,
    const BrakeCommand* src);

#if (defined(RTI_WIN32) || defined (RTI_WINCE) || defined(RTI_INTIME)) && defined(NDDS_USER_DLL_EXPORT)
/* If the code is building on Windows, stop exporting symbols.
*/
#undef NDDSUSERDllExport
#define NDDSUSERDllExport
#endif

#ifndef NDDS_STANDALONE_TYPE
#endif

#if (defined(RTI_WIN32) || defined(RTI_WINCE)) && defined(NDDS_USER_DLL_EXPORT)
/* If the code is building on Windows, stop exporting symbols. */
#undef NDDSUSERDllExport
#define NDDSUSERDllExport
#endif

#endif /* Datatypes */


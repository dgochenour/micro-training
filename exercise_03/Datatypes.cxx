/*
WARNING: THIS FILE IS AUTO-GENERATED. DO NOT MODIFY.

This file was generated from Datatypes.idl using "rtiddsgen".
The rtiddsgen tool is part of the RTI Data Distribution Service distribution.
For more information, type 'rtiddsgen -help' at a command shell
or consult the RTI Data Distribution Service manual.
*/

#include "Datatypes.h"

#ifndef UNUSED_ARG
#define UNUSED_ARG(x) (void)(x)
#endif

#include "dds_c/dds_c_typecode.h"
/*** SOURCE_BEGIN ***/

ProximityData *
ProximityData_create(void)
{
    ProximityData* sample;
    OSAPI_Heap_allocate_struct(&sample, ProximityData);
    if (sample != NULL)
    {
        if (!ProximityData_initialize(sample))
        {
            OSAPI_Heap_free_struct(sample);
            sample = NULL;
        }
    }
    return sample;
}
void
ProximityData_delete(ProximityData*sample)
{
    if (sample != NULL)
    {
        /* ProximityData_finalize() always 
        returns RTI_TRUE when called with sample != NULL */
        ProximityData_finalize(sample);
        OSAPI_Heap_free_struct(sample);
    }
}

/* ========================================================================= */
const char *ProximityDataTYPENAME = "ProximityData";

#ifndef NDDS_STANDALONE_TYPE
DDS_TypeCode* ProximityData_get_typecode()
{
    static RTIBool is_initialized = RTI_FALSE;

    static DDS_TypeCode ProximityData_g_tc_sensor_id_string = DDS_INITIALIZE_STRING_TYPECODE(((SENSOR_ID_MAX_LENGTH)));

    static DDS_TypeCode_Member ProximityData_g_tc_members[2]=
    {

        {
            (char *)"sensor_id",/* Member name */
            {
                0,/* Representation ID */
                DDS_BOOLEAN_FALSE,/* Is a pointer? */
                -1, /* Bitfield bits */
                NULL/* Member type code is assigned later */
            },
            0, /* Ignored */
            0, /* Ignored */
            0, /* Ignored */
            NULL, /* Ignored */
            RTI_CDR_KEY_MEMBER , /* Is a key? */
            DDS_PUBLIC_MEMBER,/* Member visibility */
            1,
            NULL, /* Ignored */
            RTICdrTypeCodeAnnotations_INITIALIZER
        }, 
        {
            (char *)"proximity",/* Member name */
            {
                1,/* Representation ID */
                DDS_BOOLEAN_FALSE,/* Is a pointer? */
                -1, /* Bitfield bits */
                NULL/* Member type code is assigned later */
            },
            0, /* Ignored */
            0, /* Ignored */
            0, /* Ignored */
            NULL, /* Ignored */
            RTI_CDR_REQUIRED_MEMBER, /* Is a key? */
            DDS_PUBLIC_MEMBER,/* Member visibility */
            1,
            NULL, /* Ignored */
            RTICdrTypeCodeAnnotations_INITIALIZER
        }
    };

    static DDS_TypeCode ProximityData_g_tc =
    {{
            DDS_TK_STRUCT, /* Kind */
            DDS_BOOLEAN_FALSE, /* Ignored */
            -1, /*Ignored*/
            (char *)"ProximityData", /* Name */
            NULL, /* Ignored */      
            0, /* Ignored */
            0, /* Ignored */
            NULL, /* Ignored */
            2, /* Number of members */
            ProximityData_g_tc_members, /* Members */
            DDS_VM_NONE, /* Ignored */
            RTICdrTypeCodeAnnotations_INITIALIZER,
            DDS_BOOLEAN_TRUE, /* _isCopyable */
            NULL, /* _sampleAccessInfo: assigned later */
            NULL /* _typePlugin: assigned later */
        }}; /* Type code for ProximityData*/

    if (is_initialized)
    {
        return &ProximityData_g_tc;
    }

    ProximityData_g_tc._data._annotations._allowedDataRepresentationMask = 5;

    ProximityData_g_tc_members[0]._representation._typeCode = (RTICdrTypeCode *)&ProximityData_g_tc_sensor_id_string;
    ProximityData_g_tc_members[1]._representation._typeCode = (RTICdrTypeCode *)&DDS_g_tc_float;

    /* Initialize the values for member annotations. */
    ProximityData_g_tc_members[0]._annotations._defaultValue._d = RTI_XCDR_TK_STRING;
    ProximityData_g_tc_members[0]._annotations._defaultValue._u.string_value = (DDS_Char *) "";

    ProximityData_g_tc_members[1]._annotations._defaultValue._d = RTI_XCDR_TK_FLOAT;
    ProximityData_g_tc_members[1]._annotations._defaultValue._u.float_value = 0.0f;
    ProximityData_g_tc_members[1]._annotations._minValue._d = RTI_XCDR_TK_FLOAT;
    ProximityData_g_tc_members[1]._annotations._minValue._u.float_value = RTIXCdrFloat_MIN;
    ProximityData_g_tc_members[1]._annotations._maxValue._d = RTI_XCDR_TK_FLOAT;
    ProximityData_g_tc_members[1]._annotations._maxValue._u.float_value = RTIXCdrFloat_MAX;

    ProximityData_g_tc._data._sampleAccessInfo =
    ProximityData_get_sample_access_info();
    ProximityData_g_tc._data._typePlugin =
    ProximityData_get_type_plugin_info();    

    is_initialized = RTI_TRUE;

    return &ProximityData_g_tc;
}

RTIXCdrSampleAccessInfo *ProximityData_get_sample_access_info()
{
    static RTIBool is_initialized = RTI_FALSE;

    ProximityData *sample;

    static RTIXCdrMemberAccessInfo ProximityData_g_memberAccessInfos[2] =
    {RTIXCdrMemberAccessInfo_INITIALIZER};

    static RTIXCdrSampleAccessInfo ProximityData_g_sampleAccessInfo = 
    RTIXCdrSampleAccessInfo_INITIALIZER;

    if (is_initialized)
    {
        return (RTIXCdrSampleAccessInfo*) &ProximityData_g_sampleAccessInfo;
    }

    RTIXCdrHeap_allocateStruct(
        &sample, 
        ProximityData);
    if (sample == NULL) {
        return NULL;
    }

    ProximityData_g_memberAccessInfos[0].bindingMemberValueOffset[0] = 
    (RTIXCdrUnsignedLong) ((char *)&sample->sensor_id - (char *)sample);

    ProximityData_g_memberAccessInfos[1].bindingMemberValueOffset[0] = 
    (RTIXCdrUnsignedLong) ((char *)&sample->proximity - (char *)sample);

    ProximityData_g_sampleAccessInfo.memberAccessInfos = 
    ProximityData_g_memberAccessInfos;

    {
        size_t candidateTypeSize = sizeof(ProximityData);

        if (candidateTypeSize > RTIXCdrUnsignedLong_MAX)
        {
            ProximityData_g_sampleAccessInfo.typeSize[0] =
            RTIXCdrUnsignedLong_MAX;
        } 
        else
        {
            ProximityData_g_sampleAccessInfo.typeSize[0] =
            (RTIXCdrUnsignedLong) candidateTypeSize;
        }
    }

    ProximityData_g_sampleAccessInfo.languageBinding = 
    RTI_XCDR_TYPE_BINDING_CPP ;

    RTIXCdrHeap_freeStruct(sample);

    is_initialized = RTI_TRUE;

    return (RTIXCdrSampleAccessInfo*) &ProximityData_g_sampleAccessInfo;
}

RTIXCdrTypePlugin *ProximityData_get_type_plugin_info()
{
    static RTIXCdrTypePlugin ProximityData_g_typePlugin = 
    {
        NULL, /* serialize */
        NULL, /* serialize_key */
        NULL, /* deserialize_sample */
        NULL, /* deserialize_key_sample */
        NULL, /* skip */
        NULL, /* get_serialized_sample_size */
        NULL, /* get_serialized_sample_max_size_ex */
        NULL, /* get_serialized_key_max_size_ex */
        NULL, /* get_serialized_sample_min_size */
        NULL, /* serialized_sample_to_key */
        (RTIXCdrTypePluginInitializeSampleFunction) 
        ProximityData_initialize_ex,
        NULL,
        (RTIXCdrTypePluginFinalizeSampleFunction)
        ProximityData_finalize_w_return,
        NULL
    };

    return &ProximityData_g_typePlugin;
}
#endif

RTIBool ProximityData_initialize(
    ProximityData* sample)
{
    return ProximityData_initialize_ex(sample, RTI_TRUE, RTI_TRUE);
}

RTIBool ProximityData_initialize_ex(
    ProximityData* sample,
    RTIBool allocatePointers,
    RTIBool allocateMemory)
{
    struct DDS_TypeAllocationParams_t allocParams =
    DDS_TYPE_ALLOCATION_PARAMS_DEFAULT;

    allocParams.allocate_pointers =  (DDS_Boolean)allocatePointers;
    allocParams.allocate_memory = (DDS_Boolean)allocateMemory;

    return ProximityData_initialize_w_params(
        sample,
        &allocParams);

}

RTIBool ProximityData_initialize_w_params(
    ProximityData* sample,
    const struct DDS_TypeAllocationParams_t * allocParams)
{

    if (sample == NULL)
    {
        return RTI_FALSE;
    }

    if (allocParams == NULL)
    {
        return RTI_FALSE;
    }

    if (allocParams->allocate_memory) {
        const DDS_Char stringValue[] = "";
        const DDS_String temp = (const DDS_String)stringValue;  
        sample->sensor_id = DDS_String_alloc(((SENSOR_ID_MAX_LENGTH)));  
        if (!CDR_String_copy(&sample->sensor_id, &temp, ((SENSOR_ID_MAX_LENGTH))))
        {
            return RTI_FALSE;
        }
        if (sample->sensor_id == NULL) {
            return RTI_FALSE;
        }
    } else {
        if (sample->sensor_id != NULL) {
            const DDS_Char stringValue[] = "";
            const DDS_String temp = (const DDS_String)stringValue;  
            if (!CDR_String_copy(&sample->sensor_id, &temp, ((SENSOR_ID_MAX_LENGTH))))
            {
                return RTI_FALSE;
            }
            if (sample->sensor_id == NULL) {
                return RTI_FALSE;
            }
        }
    }

    sample->proximity = 0.0f;

    return RTI_TRUE;
}

RTIBool ProximityData_finalize(
    ProximityData* sample)
{

    ProximityData_finalize_ex(sample,RTI_TRUE);
    return RTI_TRUE;
}

RTIBool ProximityData_finalize_w_return(
    ProximityData* sample)
{

    ProximityData_finalize_ex(sample,RTI_TRUE);
    return RTI_TRUE;
}

void ProximityData_finalize_ex(
    ProximityData* sample,RTIBool deletePointers)
{
    struct DDS_TypeDeallocationParams_t deallocParams =
    DDS_TYPE_DEALLOCATION_PARAMS_DEFAULT;

    if (sample == NULL)
    {
        return;
    } 

    deallocParams.delete_pointers = (DDS_Boolean)deletePointers;

    ProximityData_finalize_w_params(
        sample,&deallocParams);
}

void ProximityData_finalize_w_params(
    ProximityData* sample,
    const struct DDS_TypeDeallocationParams_t * deallocParams)
{

    if (sample == NULL)
    {
        return;
    }

    if (deallocParams == NULL)
    {
        return;
    }

    if (sample->sensor_id != NULL) {
        DDS_String_free(sample->sensor_id);
        sample->sensor_id=NULL;

    }

}

void ProximityData_finalize_optional_members(
    ProximityData* sample,
    RTIBool deletePointers)
{
    struct DDS_TypeDeallocationParams_t deallocParamsTmp =
    DDS_TYPE_DEALLOCATION_PARAMS_DEFAULT;
    struct DDS_TypeDeallocationParams_t * deallocParams =
    &deallocParamsTmp;

    if (sample == NULL)
    {
        return;
    }

    if (deallocParams) {} /* To avoid warnings */

    deallocParamsTmp.delete_pointers = (DDS_Boolean)deletePointers;
    deallocParamsTmp.delete_optional_members = DDS_BOOLEAN_TRUE;

}

RTIBool ProximityData_copy(
    ProximityData* dst,
    const ProximityData* src)
{

    if (dst == NULL || src == NULL)
    {
        return RTI_FALSE;
    }

    if (!DDS_String_copy (
        &dst->sensor_id, &src->sensor_id, 
        ((SENSOR_ID_MAX_LENGTH)) + 1)){
        return RTI_FALSE;
    }
    DDS_Primitive_copy (&dst->proximity, &src->proximity);

    return RTI_TRUE;

}

/**
* <<IMPLEMENTATION>>
*
* Defines:  TSeq, T
*
* Configure and implement 'ProximityData' sequence class.
*/
#define REDA_SEQUENCE_USER_API
#define T ProximityData
#define TSeq ProximityDataSeq

#define T_initialize ProximityData_initialize

#define T_finalize   ProximityData_finalize
#define T_copy       ProximityData_copy

#include "reda/reda_sequence_defn.h"
#undef T_copy
#undef T_finalize
#undef T_initialize

#define REDA_SEQUENCE_USER_API
#define T ProximityData
#define TSeq ProximityDataSeq
#include "dds_cpp/dds_cpp_sequence_defn.hxx"

#undef T_copy
#undef T_finalize

#undef T_initialize

#undef TSeq
#undef T


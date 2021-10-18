
CMAKE_MINIMUM_REQUIRED(VERSION 2.8.4)

ADD_DEFINITIONS()
LINK_DIRECTORIES()
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})
PROJECT(ProximityDatatype CXX C)
SET(SOURCE_EXTENSION cxx)

SET(RTIDDSGEN $ENV{RTIMEHOME}/rtiddsgen/scripts/rtiddsgen)
SET(RTIDDSMAG $ENV{RTIMEHOME}/rtiddsmag/scripts/rtiddsmag)

SET_PROPERTY(GLOBAL PROPERTY RULE_MESSAGES OFF)

SET(IDL_SOURCE_C ProximityDatatype.idl)

SET(IDL_PATH ${CMAKE_CURRENT_SOURCE_DIR})
SET(XML_PATH ${CMAKE_CURRENT_SOURCE_DIR})

SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/objs/${RTIME_TARGET_NAME})
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/objs/${RTIME_TARGET_NAME})
SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/objs/${RTIME_TARGET_NAME})

IF (CMAKE_GENERATOR MATCHES "Visual Studio")
    #Visual studio needs to run commands using 'call'
    #if there is more than one dependency
    SET(COMMAND_PREFIX call)
    SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /NODEFAULTLIB:MSVCRT.lib" CACHE INTERNAL "CMAKE_EXE_LINKER_FLAGS" FORCE)
    ADD_DEFINITIONS(-D_CRT_SECURE_NO_WARNINGS)
ELSE()
    SET(COMMAND_PREFIX)
ENDIF()

SET(RTI_LIB_PREFIX rti_me)
SET(RTI_LIB_SUFFIX)

IF (NOT (DEFINED RTI_SHARED_LIB))
    IF (${CMAKE_BUILD_TYPE} MATCHES "[Rr]elease")
        SET(RTI_LIB_SUFFIX "z")
        SET(RTI_DEBUG_BUILD FALSE)
        ADD_DEFINITIONS(-DNDEBUG)
    ELSE()
        SET(RTI_LIB_SUFFIX "zd")
        SET(RTI_DEBUG_BUILD TRUE)
    ENDIF()
ELSE()
    IF (${CMAKE_BUILD_TYPE} MATCHES "[Rr]elease")
        SET(RTI_LIB_SUFFIX "")
        SET(RTI_DEBUG_BUILD FALSE)
        ADD_DEFINITIONS(-DNDEBUG)
    ELSE()
        SET(RTI_LIB_SUFFIX "d")
        SET(RTI_DEBUG_BUILD TRUE)
    ENDIF()
ENDIF()

IF (DEFINED RTIME_MAG_FILES)
    SET(MICRO_APPGEN_LIB rti_me_appgen${RTI_LIB_SUFFIX})
ENDIF()

SET(MICRO_C_LIBS ${MICRO_APPGEN_LIB}
                 rti_me_cpp${RTI_LIB_SUFFIX} 
                 rti_me_rhsm${RTI_LIB_SUFFIX} 
                 rti_me_whsm${RTI_LIB_SUFFIX} 
                 rti_me_discdpse${RTI_LIB_SUFFIX} 
                 rti_me_discdpde${RTI_LIB_SUFFIX} 
                 rti_me${RTI_LIB_SUFFIX}
                                  )

MESSAGE("RTIME_TARGET_NAME = ${RTIME_TARGET_NAME}")
INCLUDE_DIRECTORIES(gen/ $ENV{RTIMEHOME}/include $ENV{RTIMEHOME}/include/rti_me)
LINK_DIRECTORIES($ENV{RTIMEHOME}/lib/${RTIME_TARGET_NAME})

FOREACH(idl ${IDL_SOURCE_C})
    IF (NOT EXISTS ${idl})
        MESSAGE(FATAL_ERROR "${idl} does not exist.")
    ELSEIF(IS_DIRECTORY ${idl})
        MESSAGE(FATAL_ERROR "${idl} is not a file.")
    ENDIF()

    GET_FILENAME_COMPONENT(filename ${idl} NAME)
    STRING(REGEX REPLACE "\\.idl" "" basename ${filename})
    LIST(APPEND IDL_GEN_C ${CMAKE_CURRENT_SOURCE_DIR}/${basename}.${SOURCE_EXTENSION}
                          ${CMAKE_CURRENT_SOURCE_DIR}/${basename}Plugin.${SOURCE_EXTENSION}
                          ${CMAKE_CURRENT_SOURCE_DIR}/${basename}Support.${SOURCE_EXTENSION})
                          
    LIST(APPEND IDL_GEN_H ${CMAKE_CURRENT_SOURCE_DIR}/${basename}.h
                          ${CMAKE_CURRENT_SOURCE_DIR}/${basename}Plugin.h  
                          ${CMAKE_CURRENT_SOURCE_DIR}/${basename}Support.h)

    IF (RTIME_IDL_ADD_REGENERATE_TYPESUPPORT_RULE)
        ADD_CUSTOM_COMMAND(OUTPUT ${IDL_GEN_C} ${IDL_GEN_H}
                           COMMAND ${COMMAND_PREFIX} ${RTIDDSGEN} -language C++ 
                                   -micro -verbosity 1 -update typefiles
                                   -d ${CMAKE_CURRENT_SOURCE_DIR}
                                   ${IDL_PATH}/${idl}
                           DEPENDS ${IDL_PATH}/${idl}
                           COMMENT "Regeneration type support plugin from ${idl}"
                          )
        SET_SOURCE_FILES_PROPERTIES(${IDL_GEN_C} ${IDL_GEN_H} PROPERTIES GENERATED 1)
    ENDIF()
ENDFOREACH()

IF (DEFINED RTIME_MAG_FILES)
    FOREACH(xml ${RTIME_MAG_FILES})
        IF (NOT EXISTS ${xml})
            MESSAGE(FATAL_ERROR "${xml} does not exist.")
        ELSEIF(IS_DIRECTORY ${xml})
            MESSAGE(FATAL_ERROR "${xml} is not a file.")
        ENDIF()

        GET_FILENAME_COMPONENT(filename ${xml} NAME)
        STRING(REGEX REPLACE "\\.xml" "" basename ${filename})
        GET_FILENAME_COMPONENT(basename ${xml} NAME_WLE)

        STRING(REGEX REPLACE "\\.xml" "Qos.xml" xml_qos ${xml})
        IF (EXISTS ${xml_qos})
            IF(NOT IS_DIRECTORY ${xml_qos})
                SET(REFERENCED_OPT "YES")
            ENDIF()
        ENDIF()

        LIST(APPEND APP_GEN_C ${CMAKE_CURRENT_SOURCE_DIR}/${basename}Appgen.c)

        LIST(APPEND APP_GEN_H ${CMAKE_CURRENT_SOURCE_DIR}/${basename}Appgen.h)

        LIST(APPEND APP_GEN_C ${CMAKE_CURRENT_SOURCE_DIR}/${basename}AppgenPlugin.cxx)

        LIST(APPEND APP_GEN_H ${CMAKE_CURRENT_SOURCE_DIR}/${basename}AppgenPlugin.h)


        IF (DEFINED REFERENCED_OPT)
            ADD_CUSTOM_COMMAND(OUTPUT ${APP_GEN_C} ${APP_GEN_H} 
                               COMMAND ${COMMAND_PREFIX} ${RTIDDSMAG} -verbosity 1 
                                       -language C++ 
                                       -replace  
                                       -d ${CMAKE_CURRENT_SOURCE_DIR} 
                                       -idlFile ${IDL_PATH}/${IDL_SOURCE_C} 
                                       -referencedFile ${XML_PATH}/${basename}Qos.xml
                                       ${XML_PATH}/${xml}
                               DEPENDS ${XML_PATH}/${xml} ${XML_PATH}/${basename}Qos.xml
                               COMMENT "Regeneration of application support from ${xml}"
            )
        ELSE()
            ADD_CUSTOM_COMMAND(OUTPUT ${APP_GEN_C} ${APP_GEN_H} 
                               COMMAND ${COMMAND_PREFIX} ${RTIDDSMAG} -verbosity 1 
                                       -language C++ 
                                       -replace  
                                       -d ${CMAKE_CURRENT_SOURCE_DIR} 
                                       -idlFile ${IDL_PATH}/${IDL_SOURCE_C} 
                                       ${XML_PATH}/${xml}
                               DEPENDS ${XML_PATH}/${xml}
                               COMMENT "Regeneration of application support from ${xml}"
            )
        ENDIF()
    ENDFOREACH()
ENDIF()

ADD_EXECUTABLE(ProximityDatatype_publisher
               ${CMAKE_CURRENT_SOURCE_DIR}/ProximityDatatype_publisher.${SOURCE_EXTENSION}
               ${CMAKE_CURRENT_SOURCE_DIR}/ProximityDatatypeApplication.${SOURCE_EXTENSION} 
               ${CMAKE_CURRENT_SOURCE_DIR}/ProximityDatatypeApplication.h 
                                             ${IDL_GEN_C} ${IDL_GEN_H} ${APP_GEN_C} ${APP_GEN_H})

ADD_EXECUTABLE(ProximityDatatype_subscriber 
               ${CMAKE_CURRENT_SOURCE_DIR}/ProximityDatatype_subscriber.${SOURCE_EXTENSION}
               ${CMAKE_CURRENT_SOURCE_DIR}/ProximityDatatypeApplication.${SOURCE_EXTENSION}
               ${CMAKE_CURRENT_SOURCE_DIR}/ProximityDatatypeApplication.h 
                                             ${IDL_GEN_C} ${IDL_GEN_H} ${APP_GEN_C} ${APP_GEN_H})

TARGET_LINK_LIBRARIES(ProximityDatatype_publisher  ${MICRO_C_LIBS} ${PLATFORM_LIBS})
TARGET_LINK_LIBRARIES(ProximityDatatype_subscriber ${MICRO_C_LIBS} ${PLATFORM_LIBS})

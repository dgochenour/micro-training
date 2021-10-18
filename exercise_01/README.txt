
=====================
C++ ProximityDatatype w/ Dynamic Discovery Example
=====================

An example publication and subscription pair to send and receive simple strings.

An example publication and subscription pair to send and receive simple strings.
Discovery of endpoints is done with the dynamic-endpoint discovery.

Purpose
=======

This example shows how to perform basic publish-subscribe communication.

This example differs from the HelloWorld example in that endpoint discovery is 
done dynamically: state of remote endpoints are propagated automatically by 
built-in discovery endpoints, and the user does not need to manually configure 
remote endpoint state.

For convenience, complete source code, a sample makefile for Linux and VxWorks, 
and a sample project for Windows has been provided.


Source Overview
===============

A simple "ProximityDatatype" type, containing a message string, is defined in 
ProximityDatatype.idl.

For the type to be useable by Connext Micro, type-support files must be 
generated that implement a type-plugin interface.  The example Makefile 
will generate these support files, by invoking rtiddsgen.  Note that rtiddsgen
can be invoked manually, with an example command like this:

    ${RTIMEHOME}/rtiddsgen/scripts/rtiddsgen -micro -language C++ ProximityDatatype.idl

The generated source files are ProximityDatatype.cxx, ProximityDatatypeSupport.cxx, and 
ProximityDatatypePlugin.cxx. Associated header files are also generated.
 
The DataWriter and DataReader of the type are managed in ProximityDatatype_publisher.cxx
and ProximityDatatype_subscriber.cxx, respectively. The DomainParticipant of each is 
managed in ProximityDatatypeApplication.cxx
  


Example Files Overview
======================

ProximityDatatypeApplication.cxx:
This file contains the logic for creating an application.  This includes steps 
for configuring discovery and creating a DomainParticipant.  This file also 
includes code for registering a type with the DomainParticipant.

ProximityDatatype_publisher.cxx:
This file contains the logic for creating a Publisher and a DataWriter, and 
sending data.  

ProximityDatatype_subscriber.cxx:
This file contains the logic for creating a Subscriber and a DataReader, a 
DataReaderListener, and listening for data.

ProximityDatatypePlugin.cxx:
This file creates the plugin for the ProximityDatatype data type.  This file contains 
the code for serializing and deserializing the ProximityDatatype type, creating, 
copying, printing and deleting the ProximityDatatype type, determining the size of the 
serialized type, and handling hashing a key, and creating the plug-in.

ProximityDatatypeSupport.cxx
This file defines the ProximityDatatype type and its typed DataWriter, DataReader, and 
Sequence.

ProximityDatatype.cxx
This file contains the APIs for managing the ProximityDatatype type.


How to Compile and Run
======================


--------------------
Compiling with CMake
--------------------
Before compiling, set environment variable RTIMEHOME to the Connext Micro 
installation directory. 

Depending on the number of interfaces in the local machine, you might need
to limit what interfaces are actually used by RTI Connext DDS Micro. You can find
that setting in file HelloWorldQos.xml, XML element <allow_interfaces_list>.

The RTI Connext DDS Micro source bundle includes a bash (Unix) and BAT (Windows)
script to simplify the invocation of CMake. These scripts is a convenient way 
to invoke CMake with the correct options. E.g.

Linux
-----
cd "<ProximityDatatypeApplication directory>"
rtime-make --config <Debug|Release> --build --name x64Linux3gcc4.8.2 --target Linux --source-dir . -G "Unix Makefiles" --delete [-DRTIME_IDL_ADD_REGENERATE_TYPESUPPORT_RULE=true] [-DRTIME_MAG_FILES=HelloWorld.xml]

Windows
-------
cd "<ProximityDatatypeApplication directory>"
rtime-make.bat --config <Debug|Release> --build --name i86Win32VS2010 --target Windows --source-dir . -G "Visual Studio 10 2010" --delete [-DRTIME_IDL_ADD_REGENERATE_TYPESUPPORT_RULE_eq_true]  [-DRTIME_MAG_FILES_eq_HelloWorld.xml]

Darwin
------
cd "<ProximityDatatypeApplication directory>"
rtime-make --config <Debug|Release> --build --name x64Darwin17.3.0Clang9.0.0 --target Darwin --source-dir . -G "Unix Makefiles" --delete [-DRTIME_IDL_ADD_REGENERATE_TYPESUPPORT_RULE=true]  [-DRTIME_MAG_FILES=HelloWorld.xml]

The executable can be found on directory "objs"



It is also possible to compile using CMake, e.g. in case the RTI Connext DDS 
Micro source bundle is not installed.  


Linux
-----
cmake [-DRTIME_IDL_ADD_REGENERATE_TYPESUPPORT_RULE=true] [-DRTIME_MAG_FILES=HelloWorld.xml] [-DCMAKE_BUILD_TYPE=<Debug|Release>] -G "Unix Makefiles" -B./<your build directory> -H. -DRTIME_TARGET_NAME=x64Linux3gcc4.8.2 -DPLATFORM_LIBS="dl;nsl;m;pthread;rt"
cmake --build ./<your build directory> [--config <Debug|Release>]

Windows
-------
cmake [-DRTIME_IDL_ADD_REGENERATE_TYPESUPPORT_RULE=true] [-DRTIME_MAG_FILES=HelloWorld.xml] [-DCMAKE_BUILD_TYPE=<Debug|Release>] -G "Visual Studio 10 2010" -B./<your build directory> -H. -DRTIME_TARGET_NAME=i86Win32VS2010 -DPLATFORM_LIBS="netapi32.lib;advapi32.lib;user32.lib;winmm.lib;WS2_32.lib;"
cmake --build ./<your build directory> [--config <Debug|Release>]

Darwin
------
cmake [-DRTIME_IDL_ADD_REGENERATE_TYPESUPPORT_RULE=true]  [-DRTIME_MAG_FILES=HelloWorld.xml] [-DCMAKE_BUILD_TYPE=<Debug|Release>] -G "Unix Makefiles" -B./<your build directory> -H. -DRTIME_TARGET_NAME=x64Darwin17.3.0Clang9.0.0 -DPLATFORM_LIBS="dl;m;pthread"
cmake --build ./<your build directory> [--config <Debug|Release>]

The executable can be found on ./objs

Option -DRTIME_IDL_ADD_REGENERATE_TYPESUPPORT_RULE=1 adds a rule to regenerate type support plugin source files if the input IDL/XML file changes. Default value is 'false'.

Option -DRTIME_MAG_FILES=<XML file with Application Generation definitions> adds a rule to generate application support files from XML.

------------------------------------------------------
Running ProximityDatatype_publisher and ProximityDatatype_subscriber
------------------------------------------------------

E.g. in case the ProximityDatatype has been compiled for Linux i86Linux2.6gcc4.4.5 run the subscriber by typing:

objs/i86Linux2.6gcc4.4.5/ProximityDatatype_subscriber -domain <Domain_ID> -peer <address> -sleep <sleep_time> -count <seconds_to_run>

and run the publisher by typing:

objs/i86Linux2.6gcc4.4.5/ProximityDatatype_publisher -domain <Domain_ID> -peer <address> -sleep <sleep_time> -count <seconds_to_run>

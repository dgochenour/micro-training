# RTI Connext Micro "hands-on" exercises

The instructions below reference the `RTIMEHOME` and `RTIMEARCH` environment variables-- these hold the values of the Micro installation directory and the platform architecture, respectively. In the example below, `home/don/rti_connext_dds_micro-3.0.3` is the installation directory, and the platform architecture is `x64Linux5gcc9.3.0`:
```
export RTIMEHOME=home/don/rti_connext_dds_micro-3.0.3
export RTIMEARCH=x64Linux5gcc9.3.0
```

## Exercise 00

Because Micro is delivered as source, we need to build the libraries.

```
$ cd $RTIMEHOME

$ $RTIMEHOME/resource/scripts/rtime-make --target Linux --name ${RTIMEARCH} -G "Unix Makefiles" --build --config Release

$ $RTIMEHOME/resource/scripts/rtime-make --target Linux --name ${RTIMEARCH} -G "Unix Makefiles" --build --config Debug
```
## Exercise 01

1) Design IDL file to contain one type called "ProximityData"
2) Generate type support code with `rtiddsgen`
```
$ cd exercise_01
$ $RTIMEHOME/rtiddsgen/scripts/rtiddsgen -micro -create typefiles -create makefiles -create examplefiles -language C++ ProximityDatatype.idl
```

3) Build the example code
``` 
$ $RTIMEHOME/resource/scripts/rtime-make --config Release --build --name $RTIMEARCH --target Linux --source-dir . -G "Unix Makefiles" --delete
```

4) Run the as-generated example code, just as a sanity check
- In one terminal 
```
$ ./objs/x64Linux5gcc9.3.0/ProximityDatatype_publisher
```
- In a second terminal 
```
$ ./objs/x64Linux5gcc9.3.0/ProximityDatatype_subscriber
```

(Note that the as-generated sample code formatting has been manually cleaned up and declarations moved in-line. Because of this you may choose to reference the code in this repo instead of "stock" generated code)

## Exercise 02

### Add Deadline QoS

1) Add a finite value to the Deadline QoS by modifying the `dw_qos` struct in `ProximityDatatype_publisher.cxx` and `dr_qos` in `ProximityDatatype_subscriber.cxx`
```
        dw_qos.deadline.period.sec = 0;
        dw_qos.deadline.period.nanosec = 500000000;
```
and
```
        dr_qos.deadline.period.sec = 0;
        dr_qos.deadline.period.nanosec = 500000000;
```

Note that the publisher only writes once every 1s, so the deadline will be missed during every period. Add a listener callback to the DataReader's listener to handle this event. 
- In the call to `subscriber->create_datareader()` change the status mask from `DDS_DATA_AVAILABLE_STATUS` to `DDS_DATA_AVAILABLE_STATUS | DDS_REQUESTED_DEADLINE_MISSED_STATUS`
- In the `ProximityDataReaderListener` Class, implement the `on_requested_deadline_missed()` method.

### Create XML Type Representation

We can use Admin Console to help debug events like a QoS mismatch, and even subscribe to Topics. Because Connext Micro does not propagate the TypeObject as part of discovery, we need to load this type information into Admin console via an XML file. 

1) First, increase the resource limits of the DomainParticipant in `ProximityDataTypeApplication.cxx` to allow for the extra entities that Admin Console introduces to the system. 
```
        dp_qos.resource_limits.remote_participant_allocation = 10; //was 8
        dp_qos.resource_limits.remote_reader_allocation = 32; //was 8
        dp_qos.resource_limits.remote_writer_allocation = 32; //was 8
```
2) Next use `rtiddsgen` to create an XML version of our IDL file
```
$ $RTIMEHOME/rtiddsgen/scripts/rtiddsgen -convertToXml ./ProximityDatatype.idl
```
This XML file can now be used to allow Admin Console to subscribe to data from this publisher.

## Exercise 03

Make modifications to the structure of the generated example to allow it to grow into the system we need to develop.

1) Rename the existing applications, and add a stub for a third:
    - `ProximityDatatype_publisher` --> `proximity_sensor`
    - `ProximityDatatype_subscriber` --> `controller`
    - new application, `brake`

2) Rename `ProximityDatatypeApplication.*` to `application_common.*`

3) We will be adding other data types to the system, and we could do that in the same IDL file we have been using. To that end, rename `ProximityDatatype.idl` to the more generic `DataTypes.idl`. when we use `rtime-make` to build, the type support files will be regenerated (with appropriate names) from the new IDL.

3) Refactor `CMakeLists.txt` to allow for these changes. 
- new application names
- extra application
- new IDL name

4) Additionally, move the type registration and Topic creation out of the application Class and into the individual applications. This will facilitate us having more than more Topic and type per application.

5) We can now build the new system as we did earlier.
``` 
$ $RTIMEHOME/resource/scripts/rtime-make --config Release --build --name $RTIMEARCH --target Linux --source-dir . -G "Unix Makefiles" --delete
```
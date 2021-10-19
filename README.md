# RTI Connext Micro "hands-on" exercises

The instructions below reference the `RTIMEHOME` and `RTIMEARCH` environment variables-- these hold the values of the Micro installation directory and the platform architecture, respectively. In the example below, `home/don/rti_connext_dds_micro-3.0.3` is the installation directory, and the platform architecture is `x64Linux5gcc9.3.0`:
```
export RTIMEHOME=home/don/rti_connext_dds_micro-3.0.3
export RTIMEARCH=x64Linux5gcc9.3.0
```

## Exercise 00

Because Micro is delivered as source, we need to build the libraries.

```
cd $RTIMEHOME

$RTIMEHOME/resource/scripts/rtime-make --target Linux --name ${RTIMEARCH} -G "Unix Makefiles" --build --config Release

$RTIMEHOME/resource/scripts/rtime-make --target Linux --name ${RTIMEARCH} -G "Unix Makefiles" --build --config Debug
```
## Exercise 01

1) Design IDL file to contain one type called "ProximityData"
2) Generate type support code with `rtiddsgen`
```
cd exercise_01
$RTIMEHOME/rtiddsgen/scripts/rtiddsgen -micro -create typefiles -create makefiles -create examplefiles -language C++ ProximityDatatype.idl
```

3) Build the example code
``` 
$RTIMEHOME/resource/scripts/rtime-make --config Release --build --name $RTIMEARCH --target Linux --source-dir . -G "Unix Makefiles" --delete
```

4) Run the as-generated example code, just as a sanity check
- In one terminal 
```
./objs/x64Linux5gcc9.3.0/ProximityDatatype_publisher
```
- In a second terminal 
```
./objs/x64Linux5gcc9.3.0/ProximityDatatype_subscriber
```

(Note that the as-generated sample code formatting has been manually cleaned up and declarations moved in-line. Because of this you may choose to reference the code in this repo instead of "stock" generated code)
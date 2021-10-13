# Remote "hands-on" exercises

The instructions below reference the `NDDSHOME` environment variable. The easiest way to set this on your host system is with the supplied bash script. In the example below, `home/don/rti_connext_dds-6.1.0` is the installation directory that will be captured in `NDDSHOME`, and the platform architecture is `x64Linux4gcc7.3.0`:
```
/home/don/rti_connext_dds-6.1.0/resource/scripts/rtisetenv_x64Linux4gcc7.3.0.bash
```


## Exercise 01

1) Design IDL file to contain one type called "ProximityData"
2) Generate type support code with `rtiddsgen`
```
cd exercise_01
$NDDSHOME/bin/rtiddsgen -language C++11 -create typefiles -create examplefiles -create makefiles -platform x64Linux4gcc7.3.0 ProximityDatatype.idl
```

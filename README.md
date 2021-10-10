# Remote "hands-on" exercises


## Exercise 01

1) Inspect IDL file
2) Generate type support code with `rtiddsgen`
```
cd exercise_01
$NDDSHOME/bin/rtiddsgen -language C++11 -create typefiles -create examplefiles -create makefiles -platform x64Linux4gcc7.3.0 ProximityDatatype.idl
```

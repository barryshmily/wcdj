
http://www.swig.org/Doc3.0/SWIGDocumentation.pdf

As a rule of thumb, SWIG should not be used on raw C++ source files, use header files only.


``` bash
$swig -version

SWIG Version 3.0.12

Compiled with g++ [x86_64-pc-linux-gnu]

Configured options: +pcre

Please see http://www.swig.org for reporting bugs and further information
```

## SWIG interface file

The interface file contains ANSI C function prototypes and variable declarations. The `%module` directive defines the name of the module that will be created by
SWIG. The `%{ %}` block provides a location for inserting additional code, such as C header files or additional C declarations, into the generated C wrapper code.

``` bash
swig -c++ -java module.i

gcc -fPIC -shared -o libmodule.so module_wrap.cxx -I$JAVA_HOME/include -I$JAVA_HOME/include/linux

java -Djava.library.path=. Run

```


error:
undefined symbol: __cxa_pure_virtual


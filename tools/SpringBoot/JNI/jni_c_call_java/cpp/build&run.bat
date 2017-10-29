del Sample2.obj
del Sample2.exe

cl -I%JAVA_HOME%\include -I%JAVA_HOME%\include\win32 Sample2.cpp %JAVA_HOME%\lib\jvm.lib

set PATH=%JAVA_HOME%\jre\bin\client\;%PATH%
Sample2

pause
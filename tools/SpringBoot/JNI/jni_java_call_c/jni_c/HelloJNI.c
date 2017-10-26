/*
The header declares a C function Java_HelloJNI_sayHello as follows:
JNIEXPORT void JNICALL Java_HelloJNI_sayHello(JNIEnv *, jobject);
The naming convention for C function is Java_{package_and_classname}_{function_name}(JNI arguments). The dot in package name shall be replaced by underscore.
The arguments:
JNIEnv*: reference to JNI environment, which lets you access all the JNI fucntions.
jobject: reference to "this" Java object.
We are not using these arguments in the hello-world example, but will be using them later. Ignore the macros JNIEXPORT and JNICALL for the time being.
The extern "C" is recognized by C++ compiler only. It notifies the C++ compiler that these functions are to be compiled using C's function naming protocol (instead of C++ naming protocol). C and C++ have different function naming protocols as C++ support function overloading and uses a name mangling scheme to differentiate the overloaded functions. Read "Name Mangling".

Save the C program as "HelloJNI.c".
The header "jni.h" is available under the "<JAVA_HOME>\include" and "<JAVA_HOME>\include\win32" directories, where <JAVA_HOME> is your JDK installed directory (e.g., "c:\program files\java\jdk1.7.0").
The C function simply prints the message "Hello world!" to the console.
Compile the C program - this depends on the C compiler you used.

*/
#include <jni.h>
#include <stdio.h>
#include "HelloJNI.h"
 
// Implementation of native method sayHello() of HelloJNI class
JNIEXPORT void JNICALL Java_HelloJNI_sayHello(JNIEnv *env, jobject thisObj) {
   printf("Hello World!\n");
   return;
}
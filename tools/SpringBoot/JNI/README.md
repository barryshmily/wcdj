
Calling C/C++ code from Java programs
===

JNI allows you to use native code when an application cannot be written entirely in the Java language. The following are typical situations where you might decide to use native code:

* You want to implement time-critical code in a lower-level, faster programming language.
* You have legacy code or code libraries that you want to access from Java programs.
* You need platform-dependent features not supported in the standard Java class library.

## Six steps to call C/C++ from Java code

The process of calling C or C ++ from Java programs consists of six steps. We'll go over each step in depth in the sections that follow, but let's start with a quick look at each one.

1. Write the Java code. We'll start by writing Java classes to perform three tasks: declare the native method we'll be calling; load the shared library containing the native code; and call the native method.
2. Compile the Java code. We must successfully compile the Java class or classes to bytecode before we can use them.
3. Create the C/C++ header file. The C/C++ header file will declare the native function signature that we want to call. This header will then be used with the C/C++ function implementation (see Step 4) to create the shared library (see Step 5).
4. Write the C/C++ code. This step consists of implementing the function in a C or C++ source code file. The C/C++ source file must include the header file we created in Step 3.
5. Create the shared library file. We'll create a shared library file from the C source code file we created in Step 4.
6. Run the Java program. We'll run the code and see if it works. We'll also go over some tips for dealing with the more commonly occurring errors.

## Write the C/C++ code

When it comes to writing the C/C++ function implementation, the important thing to keep in mind is that our signatures must be exactly like the function declarations from Sample1.h. We'll look at the complete code for both a C implementation and a C++ implementation, then discuss the differences between the two.

## C and C++ function implementations compared

The C and C++ code is nearly identical; the only difference is the method used to access JNI functions. In C, JNI function calls are prefixed with "(*env)->" in order to de-reference the function pointer. In C++, the JNIEnv class has inline member functions that handle the function pointer lookup. This slight difference is illustrated below, where the two lines of code access the same function but the syntax is specialized for each language.

C syntax:	jsize len = (*env)->GetArrayLength(env,array);
C++ syntax:	jsize len =env->GetArrayLength(array);


## Troubleshooting

You can run into many problems when using JNI to access native code from Java programs. The three most common errors you'll encounter are:
A dynamic link cannot be found. This results in the error message: java.lang.UnsatisfiedLinkError. This usually means that either the shared library cannot be found, or a specific native method inside the shared library cannot be found.
The shared library file cannot be found. When you load the library file using the file name with the System.loadLibrary(String libname) method, make sure that the file name is spelled correctly and that you do not specify the extension. Also, make sure that the library file is accessible to the JVM by ensuring that the library file's location is in the classpath.
A method with the specified signature cannot be found. Make sure that your C/C++ function implementation has a signature that is identical to the function signature in the header file.

## Conclusion

Calling C or C++ native code from Java, while not trivial, is a well-integrated function in the Java platform. Although JNI supports both C and C++, the C++ interface is somewhat cleaner and is generally preferred over the C interface.
As you have seen, calling C or C++ native code requires that you give your functions special names and create a shared library file. When taking advantage of existing code libraries, it is generally not advisable to change the code. To avoid this, it is common to create proxy code, or a proxy class in the case of C++, that has the specially named functions required by JNI. These functions, then, can call the underlying library functions, whose signatures and implementations remain unchanged.


Calling Java code from C/C++ programs
===

JNI allows you to invoke Java class methods from within native code. Often, to do this, you must create and initialize a JVM within the native code using the Invocation API. The following are typical situations where you might decide to call Java code from C/C++ code:

* You want to implement platform-independent portions of code for functionality that will be used across multiple platforms.
* You have code or code libraries written in the Java language that you need to access in native applications.
* You want to take advantage of the standard Java class library from native code.

## Four steps to call Java code from a C/C++ program

The four steps in the process of calling Java methods from C/C++ are as follows:

1. Write the Java code. This step consists of writing the Java class or classes that implement (or call other methods that implement) the functionality you want to access.
2. Compile the Java code. The Java class or classes must be successfully compiled to bytecode before they can be used.
3. Write the C/C++ code. This code will create and instantiate a JVM and call the correct Java methods.
4. Run the native C/C++ application. We'll run the application to see if it works. We'll also go over some tips for dealing with common errors.


---
## Some Errors

1. JNI_CreateJavaVM  error code 5

jni.h defines the possible return values for JNI functions:

``` cpp
#define JNI_OK           0                 /* success */
#define JNI_ERR          (-1)              /* unknown error */
#define JNI_EDETACHED    (-2)              /* thread detached from the VM */
#define JNI_EVERSION     (-3)              /* JNI version error */
#define JNI_ENOMEM       (-4)              /* not enough memory */
#define JNI_EEXIST       (-5)              /* VM already created */
#define JNI_EINVAL       (-6)              /* invalid arguments */
```

https://stackoverflow.com/questions/43810827/return-code-of-jni-createjavavm


2. Exception in thread "main" java.lang.NoClassDefFoundError: xxx

https://stackoverflow.com/questions/17973970/how-to-solve-java-lang-noclassdeffounderror

3. get classpath

``` java
String path = System.getProperty("java.class.path");
```

4. Problematic frame libstdc++.so JNI




---
Refer

https://www.ibm.com/developerworks/java/tutorials/j-jni/j-jni.html

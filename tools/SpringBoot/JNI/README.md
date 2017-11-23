
# 1 Calling C/C++ code from Java programs

> Java -> C/C++

JNI allows you to use native code when an application cannot be written entirely in the Java language. The following are typical situations where you might decide to use native code:

* You want to implement time-critical code in a lower-level, faster programming language.
* You have legacy code or code libraries that you want to access from Java programs.
* You need platform-dependent features not supported in the standard Java class library.

## 1.1 Six steps to call C/C++ from Java code

The process of calling C or C ++ from Java programs consists of six steps. We'll go over each step in depth in the sections that follow, but let's start with a quick look at each one.

1. Write the Java code. We'll start by writing Java classes to perform three tasks: declare the native method we'll be calling; load the shared library containing the native code; and call the native method.
2. Compile the Java code. We must successfully compile the Java class or classes to bytecode before we can use them.
3. Create the C/C++ header file. The C/C++ header file will declare the native function signature that we want to call. This header will then be used with the C/C++ function implementation (see Step 4) to create the shared library (see Step 5).
4. Write the C/C++ code. This step consists of implementing the function in a C or C++ source code file. The C/C++ source file must include the header file we created in Step 3.
5. Create the shared library file. We'll create a shared library file from the C source code file we created in Step 4.
6. Run the Java program. We'll run the code and see if it works. We'll also go over some tips for dealing with the more commonly occurring errors.

## 1.2 Write the C/C++ code

When it comes to writing the C/C++ function implementation, the important thing to keep in mind is that our signatures must be exactly like the function declarations from Sample1.h. We'll look at the complete code for both a C implementation and a C++ implementation, then discuss the differences between the two.

## 1.3 C and C++ function implementations compared

The C and C++ code is nearly identical; the only difference is the method used to access JNI functions. In C, JNI function calls are prefixed with "(*env)->" in order to de-reference the function pointer. In C++, the JNIEnv class has inline member functions that handle the function pointer lookup. This slight difference is illustrated below, where the two lines of code access the same function but the syntax is specialized for each language.

C syntax:	jsize len = (*env)->GetArrayLength(env,array);
C++ syntax:	jsize len =env->GetArrayLength(array);

## 1.4 Troubleshooting

You can run into many problems when using JNI to access native code from Java programs. The three most common errors you'll encounter are:
A dynamic link cannot be found. This results in the error message: java.lang.UnsatisfiedLinkError. This usually means that either the shared library cannot be found, or a specific native method inside the shared library cannot be found.
The shared library file cannot be found. When you load the library file using the file name with the System.loadLibrary(String libname) method, make sure that the file name is spelled correctly and that you do not specify the extension. Also, make sure that the library file is accessible to the JVM by ensuring that the library file's location is in the classpath.
A method with the specified signature cannot be found. Make sure that your C/C++ function implementation has a signature that is identical to the function signature in the header file.

## 1.5 Conclusion

Calling C or C++ native code from Java, while not trivial, is a well-integrated function in the Java platform. Although JNI supports both C and C++, the C++ interface is somewhat cleaner and is generally preferred over the C interface.
As you have seen, calling C or C++ native code requires that you give your functions special names and create a shared library file. When taking advantage of existing code libraries, it is generally not advisable to change the code. To avoid this, it is common to create proxy code, or a proxy class in the case of C++, that has the specially named functions required by JNI. These functions, then, can call the underlying library functions, whose signatures and implementations remain unchanged.


# 2 Calling Java code from C/C++ programs

JNI allows you to invoke Java class methods from within native code. Often, to do this, you must create and initialize a JVM within the native code using the Invocation API. The following are typical situations where you might decide to call Java code from C/C++ code:

* You want to implement platform-independent portions of code for functionality that will be used across multiple platforms.
* You have code or code libraries written in the Java language that you need to access in native applications.
* You want to take advantage of the standard Java class library from native code.

## 2.1 Four steps to call Java code from a C/C++ program

The four steps in the process of calling Java methods from C/C++ are as follows:

1. Write the Java code. This step consists of writing the Java class or classes that implement (or call other methods that implement) the functionality you want to access.
2. Compile the Java code. The Java class or classes must be successfully compiled to bytecode before they can be used.
3. Write the C/C++ code. This code will create and instantiate a JVM and call the correct Java methods.
4. Run the native C/C++ application. We'll run the application to see if it works. We'll also go over some tips for dealing with common errors.


# 3 Advanced topics

Calling native code from within a Java program compromises the Java program's portability and security. Although the compiled Java bytecode remains highly portable, the native code must be recompiled for each platform on which you intend to run the application. The native code also executes outside of the JVM, so it is not necessarily constrained by the same security protocols as Java code.

Calling Java code from within a native program is also complicated. Because the Java language is object-oriented, calling Java code from a native application typically involves object-oriented techniques. In native languages that have no support or limited support for object-oriented programming, such as C, calling Java methods can be problematic. In this section, we'll explore some of the complexities that arise when working with JNI, and look at ways to work around them.

## 3.1 Java strings versus C strings

``` cpp
/* Convert a C string to a Java String. */
char[]  str  = "To be or not to be.\n";
jstring jstr = (*env)->NewStringUTF(env, str);

/* Convert a Java String into a C string. */
char buf[128]; 
const char *newString = (*env)->GetStringUTFChars(env, jstr, 0);
// ...
(*env)->ReleaseStringUTFChars(env, jstr, newString);
```

## 3.2 Java arrays versus C arrays

``` cpp
/* Looping through the elements in an array. */
int* elem = (*env)->GetIntArrayElements(env, intArray, 0);
for (i=0; I < (*env)->GetIntArrayLength(env, intArray); i++)
    sum += elem[i]
(*env)->ReleaseIntArrayElements(env, intArray, elem, 0);
```

## 3.3 Local versus global references

When programming with JNI you will be required to use references to Java objects. By default, JNI creates local references to ensure that they are liable for garbage collection. Because of this, you may unintentionally write illegal code by trying to store away a local reference so that you can reuse it later, as shown in the code sample below:

``` cpp
/* This code is invalid! */
static jmethodID mid;

JNIEXPORT jstring JNICALL
Java_Sample1_accessMethod(JNIEnv *env, jobject obj)
{
    // ...
    cls = (*env)->GetObjectClass(env, obj);
       if (cls != 0)
          mid = (*env)->GetStaticMethodID(env, cls, "addInt", "(I)I");  // error !
    // ...
}
```

Creating a global reference
To correct this problem, you need to create and use a global reference. A global reference will remain valid until you explicitly free it, which you must remember to do. Failure to free the reference could cause a memory leak.
Create a global reference with NewGlobalRef() and delete it with DeleteGlobalRef(), as shown in the code sample below:

``` cpp
/* This code is valid! */
static jmethodID mid;

JNIEXPORT jstring JNICALL
Java_Sample1_accessMethod(JNIEnv *env, jobject obj)
{
   // ...
   cls = (*env)->GetObjectClass(env, obj);
   if (cls != 0)
   {
      mid1 = (*env)->GetStaticMethodID(env, cls, "addInt", "(I)I");
      mid = (*env)->NewGlobalRef(env, mid1); // OK 
      // ...
   }
}
```

这首先涉及到 Java 和 Native 代码之间函数调用时，参数如何传递的问题。简单类型，也就是内置类型，比如 int, char 等是值传递（pass by value），而其它 Java 对象都是引用传递（pass by reference），这些对象引用由 JVM 传给 Native 代码，每个都有其生命周期。
其次，Java 对象的生命周期是由它的引用类型决定的，这里的引用分两种：local reference 和 global reference。Native 函数参数中 jobject 或者它的子类，其参数都是 local reference。Local reference 只在这个 Native 函数中有效，Native 函数返回后，引用的对象就被释放，它的生命周期就结束了。若要留着日后使用，则需根据这个 local reference 创建 global reference。Global reference 不会被系统自动释放，它仅当被程序明确调用 DeleteGlobalReference 时才被回收。

## 3.4 Error handling

Using native methods in Java programs breaks the Java security model in some fundamental ways. Because Java programs run in a controlled runtime system (the JVM), the designers of the Java platform decided to help the programmer by checking common runtime errors like array indices, out-of-bounds errors, and null pointer errors. C and C++, on the other hand, use no such runtime error checking, so native method programmers must handle all error conditions that would otherwise be caught in the JVM at runtime.

For example, it is common and correct practice in Java programs to report errors to the JVM by throwing an exception. C has no exceptions, so instead you must use the exception handling functions of JNI.

JNI's exception handling functions

There are two ways to throw an exception in the native code: you can call the Throw() function or the ThrowNew() function. Before calling Throw(), you first need to create an object of type Throwable. By calling ThrowNew() you can skip this step because this function creates the object for you. In the example code snippet below, we throw an IOException using both functions:

``` cpp
/* Create the Throwable object. */
jclass cls = (*env)->FindClass(env, "java/io/IOException");
jmethodID mid = (*env)->GetMethodID(env, cls, "<init>", "()V");
jthrowable e = (*env)->NewObject(env, cls, mid);

/* Now throw the exception */
(*env)->Throw(env, e);
...

/* Here we do it all in one step and provide a message*/
(*env)->ThrowNew(env,
                 (*env)->FindClass("java/io/IOException"),
                 "An IOException occurred!");
```

The Throw() and ThrowNew() functions do not interrupt the flow of control in the native method. The exception will not actually be thrown in the JVM until the native method returns. In C you cannot use the Throw() and ThrowNew() functions to immediately exit a method on error conditions, as you can in Java programs by using the throw statement. Instead, you need to use a return statement right after the Throw() and ThrowNew() functions to exit the native method at a point of error.

JNI's exception catching functions

You may also need to catch exceptions when calling Java from C or C++. Many JNI functions throw exceptions that you may want to catch. The ExceptionCheck() function returns a jboolean indicating whether or not an exception was thrown, while the ExceptionOccured() method returns a jthrowable reference to the current exception (or NULL if no exception was thrown).

If you're catching exceptions, you may be handling exceptions, in which case you need to clear out the exception in the JVM. You can do this using the ExceptionClear() function. The ExceptionDescribed() function is used to display a debugging message for an exception.

## 3.5 Multithreading in native methods

One of the more advanced issues you'll face when working with JNI is multithreading with native methods. The Java platform is implemented as a multithreaded system, even when running on platforms that don't necessarily support multithreading; so the onus is on you to ensure that your native functions are thread safe.

In Java programs, you can implement thread-safe code by using synchronized statements. The syntax of the synchronized statements allows you to obtain a lock on an object. As long as you're in the synchronized block, you can perform whatever data manipulation you like without fear that another thread may sneak in and access the object for which you have the lock.

JNI provides a similar structure using the MonitorEnter() and MonitorExit() functions. You obtain a monitor (lock) on the object you pass into the MonitorEnter() function and you keep this lock until you release it with the MonitorExit() function. All of the code between the MonitorEnter() and MonitorExit() functions is guaranteed to be thread safe for the object you locked.

Synchronization in native methods

The table below shows how to synchronize a block of code in Java, C, and C++. As you can see, the C and C++ functions are similar to the synchronized statement in the Java code.

``` java
// java code
synchronized(obj) {
	// synchronized block
}
```

``` c
// c code
(*env)->MonitorEnter(env, obj);

// synchronized block

(*env)->MonitorExit(env, obj);
```

``` cpp
// c++ code
env->MonitorEnter(obj);

// synchronized block

env->MonitorExit(obj);
```

# 4 Summary

The Java Native Interface is a well-designed and well-integrated API in the Java platform. It is designed to allow you to incorporate native code into Java programs as well as providing you a way to use Java code in programs written in other programming languages.

Using JNI almost always breaks the portability of your Java code. When calling native methods from Java programs, you will need to distribute native shared library files for every platform on which you intend to run your program. On the other hand, calling Java code from native programs can actually improve the portability of your application.

Appendices

## 4.1 JNI types

JNI uses several natively defined C types that map to Java types. These types can be divided into two categories: primitive types and pseudo-classes. The pseudo-classes are implemented as structures in C, but they are real classes in C++.

## 4.2 JNI method signature encoding

Native Java method parameter types are rendered, or mangled, into native code using the encoding specified in the table below.

Method Signature Encoding

| Java Type | Code |
| -- | -- |
| boolean | Z
| byte | B
| char | C
| short | S
| int | I
| long | J
| float | F
| double | D
| void | V
| class | Lclassname;


Notes:
* The semicolon at the end of the class type L expression is the terminator of the type expression, not a separator between expressions.

* You must use a forward slash (/) instead of a dot (.) to separate the package and class name. To specify an array type use an open bracket ([). For example, the Java method:

``` cpp
boolean print(String[] parms, int n)
```

has the following mangled signature:

``` cpp
([Ljava/lang/Sting;I)Z
```


# 5 Some Errors

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

[Java Native Interface](https://en.wikipedia.org/wiki/Java_Native_Interface)
[Java Native Interface Specification Contents](https://docs.oracle.com/javase/8/docs/technotes/guides/jni/spec/jniTOC.html)
[JNI Types and Data Structures](https://docs.oracle.com/javase/8/docs/technotes/guides/jni/spec/types.html)
[ORACLE JNI Functions](https://docs.oracle.com/javase/7/docs/technotes/guides/jni/spec/functions.html)
[Java programming with JNI](https://www.ibm.com/developerworks/java/tutorials/j-jni/j-jni.html)
[JNI 对象在函数调用中的生命周期](https://www.ibm.com/developerworks/cn/java/j-lo-jni/index.html)
[JNI/NDK 开发指南](http://wiki.jikexueyuan.com/project/jni-ndk-developer-guide/)
[Java Programming Tutorial Java Native Interface](https://www3.ntu.edu.sg/home/ehchua/programming/java/JavaNativeInterface.html)
[Calling Java from C++ with JNI](https://www.codeproject.com/Articles/993067/Calling-Java-from-Cplusplus-with-JNI)
[Best practices for using the Java Native Interface](https://www.ibm.com/developerworks/library/j-jni/index.html)
[在 JNI 编程中避免内存泄漏](https://www.ibm.com/developerworks/cn/java/j-lo-jnileak/index.html)
[Wrapping a C++ library with JNI – introduction](https://thebreakfastpost.com/2012/01/21/wrapping-a-c-library-with-jni-introduction/)
[Java Native Interface Specification—Contents](https://docs.oracle.com/javase/1.5.0/docs/guide/jni/spec/jniTOC.html)
[How to pass a complex structure between C and Java with JNI on Android NDK](https://stackoverflow.com/questions/10300039/how-to-pass-a-complex-structure-between-c-and-java-with-jni-on-android-ndk)
[JNI 调用性能测试及优化](http://wiki.jikexueyuan.com/project/jni-ndk-developer-guide/performance.html)
[Java Tip 54: Returning data in reference arguments via JNI](https://www.javaworld.com/article/2077554/learn-java/java-tip-54--returning-data-in-reference-arguments-via-jni.html)
[How to catch JNI/Java Exception](https://stackoverflow.com/questions/2054598/how-to-catch-jni-java-exception)
[Exception Handling in JNI](https://www.developer.com/java/data/exception-handling-in-jni.html)

https://www.cl.cam.ac.uk/teaching/0910/CandC++/lecture8-6up.pdf
https://docs.oracle.com/javase/1.5.0/docs/guide/jni/spec/types.html#wp276
https://android.googlesource.com/platform/frameworks/opt/net/wifi/+/lollipop-release/service/jni/jni_helper.cpp
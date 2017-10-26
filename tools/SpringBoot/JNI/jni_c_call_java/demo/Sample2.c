
// The jni.h file contains all the type and function definitions we need for JNI on the C side.
#include <jni.h>
#include <stdio.h>
#include <string.h>

#ifdef _WIN32
#define PATH_SEPARATOR ';'
#else
#define PATH_SEPARATOR ':'
#endif

int main()
{
  // The JavaVMOption options[] holds various optional settings for the JVM. 
  // When declaring variables, be sure that you declare the JavaVMOption options[] array large enough to hold all the options you want to use. 
  // In this case, the only option we're using is the classpath option. 
  // We set the classpath to the current directory because in this example all of our files are in the same directory. 
  // You can set the classpath to point to any directory structure you would like to use.
  JavaVMOption options[1];

  // JNIEnv *env represents JNI execution environment.
  JNIEnv *env;   

  // JavaVM jvm is a pointer to the JVM. We use this primarily to create, initialize, and destroy the JVM.        
  JavaVM *jvm;

  // JavaVMInitArgs vm_args represents various JVM arguments that we can use to initialize our JVM.
  JavaVMInitArgs vm_args;

  long status;
  jclass cls;
  jmethodID mid;
  jint square;
  jboolean not;

  options[0].optionString = "-Djava.class.path=.";
  memset(&vm_args, 0, sizeof(vm_args));
  vm_args.version = JNI_VERSION_1_8;// 1.8
  vm_args.nOptions = 1;
  vm_args.options = options;

  // This method returns zero if successful or JNI_ERR if the JVM could not be created.
  status = JNI_CreateJavaVM(&jvm, (void**)&env, &vm_args);

  if (status != JNI_ERR)
  {
    // The cls variable stores the result of the FindClass() function. 
    // If the class is found, the cls variable represents a handle to the Java class. 
    // If the class cannot be found, cls will be zero.
    cls = (*env)->FindClass(env, "Sample2");
    if (cls != 0)
    { 
      // The mid variable stores the result of the GetStaticMethodID() function. 
      // If the method is found, the mid variable represents a handle to the method. 
      // If the method cannot be found, mid will be zero.

      // Remember that in this example, we are calling static Java methods. 
      // That is why we're using the GetStaticMethodID() function. 
      // The GetMethodID() function does the same thing, but it is used to find instance methods.
      mid = (*env)->GetStaticMethodID(env, cls, "intMethod", "(I)I");
      if (mid != 0)
      { 
        // The CallStaticIntMethod() method takes cls (representing our class), mid (representing our method), 
        // and the parameter or parameters for the method. In this case the parameter is int 5.

        // You will also run across methods of the types CallStaticXXXMethod() and CallXXXMethod(). 
        // These call static methods and member methods, respectively, 
        // replacing the variable (XXX) with the return type for the method (for example, Object, Boolean, Byte, Char, Int, Long, and so on).
        square = (*env)->CallStaticIntMethod(env, cls, mid, 5);
        printf("Result of intMethod: %d\n", square);
      }

      mid = (*env)->GetStaticMethodID(env, cls, "booleanMethod", "(Z)Z");
      if (mid != 0)
      { 
        not = (*env)->CallStaticBooleanMethod(env, cls, mid, 1);
        printf("Result of booleanMethod: %d\n", not);
      }
    }

    (*jvm)->DestroyJavaVM(jvm);
    return 0;
  }
  else
    return -1;
}

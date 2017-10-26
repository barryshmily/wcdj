/*
The static initializer invokes System.loadLibrary() to load the native library "Hello" (which contains the native method sayHello()) during the class loading. It will be mapped to "hello.dll" in Windows; or "libhello.so" in Unixes. This library shall be included in Java's library path (kept in Java system variable java.library.path); otherwise, the program will throw a UnsatisfiedLinkError. You could include the library into Java Library's path via VM argument -Djava.library.path=path_to_lib.
Next, we declare the method sayHello() as a native instance method, via keyword native, which denotes that this method is implemented in another language. A native method does not contain a body. The sayHello() is contained in the native library loaded.
The main() method allocate an instance of HelloJNI and invoke the native method sayHello().
*/

public class HelloJNI {
   static {
      System.loadLibrary("hello"); // Load native library at runtime
                                   // hello.dll (Windows) or libhello.so (Unixes)
   }

   // Declare a native method sayHello() that receives nothing and returns void
   private native void sayHello();

   // Test Driver
   public static void main(String[] args) {
      new HelloJNI().sayHello();  // invoke the native method
   }
}


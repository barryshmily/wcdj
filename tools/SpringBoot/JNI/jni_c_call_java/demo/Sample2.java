// Note that Sample2.java implements two static Java methods, intMethod(int n) and booleanMethod(boolean bool) (lines 3 and 7 respectively). 
// static methods are class methods that are not associated with object instances. 
// It is easier to call static methods because we do not have to instantiate an object to invoke them.

public class Sample2
{
  public static int intMethod(int n) {
      return n*n;
  }

  public static boolean booleanMethod(boolean bool) {
       return !bool;
  }
}
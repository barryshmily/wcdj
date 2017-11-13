
# Thinking in Java


``` java
//: object/HelloDate.java
import java.util.*;

/** The first Thinking in Java example program.
 * Displays a string and today's date.
 * @author Bruce Eckel
 * @author www.MindView.net
 * @version 4.0
*/
public class HelloDate {
  /** Entry point to class & application.
   * @param args array of string arguments
   * @throws exceptions No exceptions thrown
  */
  public static void main(String[] args) {
    System.out.println("Hello, it's: ");
    System.out.println(new Date());
  }
} /* Output: (55% match)
Hello, it's:
Wed Oct 05 14:39:36 MDT 2005
*///:~
```

# Q&A

1. What is a NullPointerException, and how do I fix it?

``` java
public class Example {

    public static void main(String[] args) {
        Object obj = null;
        obj.hashCode();
    }
}
```

https://stackoverflow.com/questions/218384/what-is-a-nullpointerexception-and-how-do-i-fix-it

2. Map遍历

``` java
// java 5
Map<Integer, Integer> map = new HashMap<Integer, Integer>(); 
  
for (Map.Entry<Integer, Integer> entry : map.entrySet()) { 
  
    System.out.println("Key = " + entry.getKey() + ", Value = " + entry.getValue());  
} 

// 遍历map中的键 
  
for (Integer key : map.keySet()) { 
    System.out.println("Key = " + key); 
} 
  
// 遍历map中的值 
for (Integer value : map.values()) { 
    System.out.println("Value = " + value); 
} 

// 使用Iterator遍历
Iterator<Map.Entry<Integer, Integer>> entries = map.entrySet().iterator(); 
  
while (entries.hasNext()) { 
    Map.Entry<Integer, Integer> entry = entries.next(); 
    System.out.println("Key = " + entry.getKey() + ", Value = " + entry.getValue()); 
} 

// 效率低
for (Integer key : map.keySet()) { 
    Integer value = map.get(key); 
    System.out.println("Key = " + key + ", Value = " + value); 
} 


```

http://blog.csdn.net/tjcyjd/article/details/11111401


# Tools

1. FindBugs

http://findbugs.sourceforge.net/

===

Refer:

http://www.mindviewinc.com/TIJ4/CodeInstructions.html

https://docs.oracle.com/javase/9/docs/api/overview-summary.html
https://docs.oracle.com/javase/8/docs/api/


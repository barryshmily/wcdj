
class Cleanser {

	public String toString() {
		return s;
	}

	private String s = "Cleanser";
	public void foo() {
		System.out.println("Cleanser foo");
	}


	public static void main(String[] args) {
		System.out.println("Cleanser main");
		Cleanser x = new Cleanser();
		x.foo();
	}
}

public class Detergent extends Cleanser {
	 
	public void foo2() {
		System.out.println("Cleanser foo2");
	}

	public static void main(String[] args) {
		System.out.println("Detergent main");
		Detergent x = new Detergent();
		x.foo();
		x.foo2();

		Cleanser.main(args);

	}
}
/*
$javac Detergent.java
$java Detergent
Detergent main
Cleanser foo
Cleanser foo2
Cleanser main
Cleanser foo
$java Cleanser 
Cleanser main
Cleanser foo
*/

class A {
	public void doNothing() {
		System.out.println("doNothing");
	}
}

public class Demo {
	public static void main(String[] args) {
		System.out.println("main");

		new A().doNothing();
	}
}
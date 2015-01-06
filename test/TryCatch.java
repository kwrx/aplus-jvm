

public class TryCatch {
	public static void Test3() {
		try {
			throw new Exception("Hello World");
		} catch(Exception e) {
			
		}
	}

	public static void Test2() {
		Test3();
	}

	public static void Test1() {
		Test2();
	}

	public static void main(String[] args) {
		Test1();
	}
}

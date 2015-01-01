

public class Overloading {
	public static int sum(int a, int b) {
		return a + b;
	}

	public static long sum(long a, long b) {
		return a + b;
	}

	public static void main(String[] args) {
		int a = sum(10, 10);
		long b = sum(10, 10);

		return;
	}
}

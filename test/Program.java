
public class Program {

	public static long fib(long x, long y) {
		if(x > 1)
			return fib(x - 1, y) * x;

		return y;
	}

	public static long main(String[] args) {
		return fib(3, 1);
	}
}

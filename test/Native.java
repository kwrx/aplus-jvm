

public class Native {
	public static native int Print(String message);

	public static void main(String[] args) {
		Print("Hello World");
	}
}

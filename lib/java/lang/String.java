
package java.lang;

public class String {

	private char chars[];

	public String intern() {
		return (String) ((Object) chars);
	}

	public String() {

	}

	public String(byte[] bytes) {

	}

	public String(String S) {

	}

	public String(char[] C) {

	}

	char charAt(int index) {
		return chars[index];
	}

	int codePointAt(int index) {
		return (int) chars[index];
	}

	int codePointBefore(int index) throws IndexOutOfBoundsException {
		if(index > 0)
			return chars[index - 1];

		throw new IndexOutOfBoundsException();
	}
}

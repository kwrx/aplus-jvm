

public class Switch {
	public static void main(String[] args) {
		int x = 10;
		switch(x) {
			case 0:
				x -= 1000;
				break;
			case 2:
				x *= 1000;
				break;
			case 5:
			case 6:
			case 8:
			case 10:
				x += 10000;
				break;
			default:
				x /= 2;
				break;
		}
	}
}

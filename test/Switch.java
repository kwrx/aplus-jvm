

public class Switch {
	public static void main(String[] args) {
		int x = 10;
		switch(x) {
			case 0:
				Native.Print("case 0\n");
				break;
			case 2:
				Native.Print("case 2\n");
				break;
			case 5:
			case 6:
			case 8:
			case 10:
				Native.Print("case 10\n");
				break;
			default:
				Native.Print("case default\n");
				break;
		}
	}
}

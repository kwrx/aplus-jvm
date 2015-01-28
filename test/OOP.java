

public class OOP {
	public static void main(String[] args) {
		OOP P1 = new OOP(10);
		OOP P2 = new OOP(5);

		P1.Print();
		P2.Print();
	}


	public int X;
	final int Y = 0x1000;

	public OOP(int x) {
		this.X = x;
	}

	public void Print() {
		//Native.Print("Program: %d\n", this.X);
	}
}

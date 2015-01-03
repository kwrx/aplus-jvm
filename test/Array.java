public class Array {

	public static native int Print(String Format, int... args);

	public static void main(String[] args) {
		byte[] arr = new byte[10];

		for(int i = 0; i < 10; i++)
			arr[i] = (byte) i;
		
		for(int i = 0; i < 10; i++)
			Print("arr[%d] => %d\n", i, (int) arr[i]); 

		if(arr[5] != (byte)5)
			arr[50] = (byte) 100;

		arr[12] = (byte)10;
	}
}

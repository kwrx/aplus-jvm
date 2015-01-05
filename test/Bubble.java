
class Bubble {
	public static void main(String[] args) {

		int[] b = new int[4096];
	
		for(int i = 0; i < 4096; i++)
			b[i] = 4096 - i;


		int check = 0;
		do {
			check = 0;

			for(int i = 1; i < 4096; i++) {
				if(b[i - 1] > b[i]) {
					int t = b[i];
					b[i] = b[i - 1];
					b[i - 1] = t;
			
					check++;
				}
			}
	
		} while(check > 0);

		
	}
}


/*
	Result:
		jvm: 3.284s		(x2,2 slower)
		gij: 1.484s
*/

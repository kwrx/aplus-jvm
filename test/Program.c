#include <stdio.h>
#include <time.h>
#include <sys/times.h>

int main() {
	long x = 1;
	long n = 30;

	do {
		x *= n--;
	} while(n > 0);
}


/* TEST: Fibonacci 30
 C 
Risultato 0.001133 secondi.

  JVM 
Risultato 0.001879 secondi.

*/

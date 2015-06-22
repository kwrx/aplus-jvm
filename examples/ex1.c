/* A+ Java Virtual Machine */
/* Load a Class File and return sum of two parameters from method */

#include <avm.h>
#include <stdlib.h>


/* Calc.java 							*/
/*								*/
/* public class Calc {						*/
/*	public static int Sum(int a, int b) {			*/
/*		return a + b;					*/
/*	}							*/
/* }								*/


int main(int argc, char** argv) {

	/* Open Calc.class */
	if(avm_open("Calc.class") == J_ERR)
		{ perror("Calc.class"); abort(); }


	/* Initialize context & resolve assemblies */
	avm_begin();


	j_int a = 10;
	j_int b = 15;

	/* c = Calc.Sum(a, b); */
	j_int c = avm_call("Calc", "Sum", 2, a, b).i32;

	/* Destroy context & all resources */
	avm_end();

}

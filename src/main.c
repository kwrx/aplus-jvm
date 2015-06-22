#include <avm.h>
#include "ops.h"

#include <stdio.h>
#include <string.h>


static void die(char* e) {
	perror(e);
	exit(1);
}


#define DEFARG(x)	\
	static void x (char** argv, int* idx)



DEFARG(help);
DEFARG(version);

DEFARG(nostdlib);
static int __stdlib = 1;




struct {
	char* longopt;
	char* shortopt;
	char* docs;
	void (*handler) (char** argv, int* idx);
} args[] = {
	{ "--version", "-v", "Display AVM version information", version },
	{ "--help", "-h", "Display this information", help },
	{ "-nostdlib", "", "Use only library's directories defined", nostdlib },
	{ NULL, NULL, NULL, NULL }
};


DEFARG(help) {
	fprintf(stderr, APP_VERSION_FORMAT, APP_VERSION_ARGS);
	

	fprintf(stderr,
			"\nUsage:\n\tavm CLASS [ARGS] ...\n"
			"\t\tto invoke CLASS.main, or\n"
			"\tavm JPKFILE [ARGS] ...\n"
			"\t\tto execute a jpk file\n"
		);


	fprintf(stderr, "\nOptions:\n");

	int j;
	for(j = 0; args[j].handler; j++)
		fprintf(stderr, "%15s, %2s\t%s\n", args[j].longopt, args[j].shortopt, args[j].docs);


	exit(0);
}

DEFARG(version) {
	fprintf(stderr, APP_VERSION_FORMAT, APP_VERSION_ARGS);
	exit(0);
}


DEFARG(nostdlib) {
	__stdlib = 0;
}


int main(int argc, char** argv) {

	if(argc < 2) {
		fprintf(stderr,
			"Usage:\tavm CLASS [ARGS] ...\n"
			"\t\tto invoke CLASS.main, or\n"
			"\tavm JPKFILE [ARGS] ...\n"
			"\t\tto execute a jpk file\n"
			"try 'avm --help' for more information.\n"
		);

		exit(1);
	}


	int i, j;
	for(i = 1; i < argc; i++) {
		for(j = 0; args[j].handler; j++) {
			if(
				!((strcmp(argv[i], args[j].longopt) == 0) ||
				(strcmp(argv[i], args[j].shortopt) == 0))
			) continue;

			args[j].handler(argv, &i);
			break;
		}

		if(args[j].handler == NULL) {
			if(avm_open(argv[i]) == J_ERR) {
				fprintf(stderr, "avm: Invalid argument '%s'\n", argv[i]);
				exit(1);
			}
		}
	}	
	

	if(__stdlib)
		if(avm_open(__LIBDIR__ "/rt.jpk") == J_ERR)
			die("rt.jpk");

	avm_begin();
	avm_main(argc, argv);
	avm_end();

 
	return 0;
}

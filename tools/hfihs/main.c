#include <stdio.h>
#include <stdlib.h>

#define P_START 1902
#define P_END 10029

int main(int argc, char* argv[]) {
	
	if (argc < 2) {
		printf("Not enough arguments! Usage:\n");
		printf("\thfihs <p=? number>\n");
	} else {
		int page = atoi(argv[1]);
		int current = page-P_START;
		int last = P_END-P_START;	
		float percent = (float)current/last*100;
		printf("%4.2f%% complete! (%d/%d)\n", percent, current, last);
	}

	return 0;
}
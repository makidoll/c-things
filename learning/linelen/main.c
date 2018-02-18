#include <stdio.h>
#include <string.h>

#define MAXLINE 1000

void copyArray(char* src, char* dest) {
	for (int i; i<MAXLINE; ++i) {
		dest[i] = src[i];
	}
}

int longestLine(char* filename) {
	FILE* file;
	file = fopen(filename, "r");

	if (!file) return -1;

	int l_longest_num = 0;
	int l_longest_len = 0;
	char l_longest_str[MAXLINE];
	memset(l_longest_str, 0, MAXLINE); // empty it!

	int l_num = 0;
	int l_len = 0;
	char l_str[MAXLINE];
	memset(l_str, 0, MAXLINE); // empty it!

	int chr;
	while ((chr = getc(file)) != EOF) {
		l_str[l_len] = chr;
		++l_len;

		if (chr == 10) { // new line
			++l_num; --l_len; 
			l_str[l_len] = 0; // remove new line
			printf("%d (%d): %s\n", l_num, l_len, l_str);

			if (l_len > l_longest_len) {
				l_longest_num = l_num;
				l_longest_len = l_len;
				copyArray(l_str, l_longest_str);
			} 

			memset(l_str, 0, MAXLINE);
			l_len = 0;
		}
	}

	++l_num; 
	printf("%d (%d): %s\n\n", l_num, l_len, l_str);
	fclose(file);

	printf("Longest:\n  %d (%d): %s\n", l_longest_num, l_longest_len, l_longest_str);
	return 0;
}

int main(int argc, char *argv[]) {
	if (argc <= 1) {
		printf("Please specify a file path.\n");
		return 0;
	}

	if (longestLine(argv[1])!=0) {
		printf("An error has occurred!\n");
	}

	return 0;
}
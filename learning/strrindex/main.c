#include <stdio.h>

// returns the rightmost occurrence of t in s, or -1 if there is none
int strrindex(char* s, char* t) {
	int index = -1;
	char schr = t[0];

	int i = 0;
	int chr;
	int eof = 0; 
	while (!eof) {
		chr = s[i];
		if (chr == schr) index = i;
		if (chr == 0) eof = 1;
		++i;
	}

	return index;
}

int main() {

	printf("%d\n", strrindex("test hello what", "h"));

}
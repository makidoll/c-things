#include <stdio.h>
#include "game.h"
#include "text.h"

int split(int num) {
	int len = 0;
	for (int n=num; n>0; n/=10) len++;

	int arr[len]; int index = 0;
	for (int n=num; n>0; n/=10) {
		arr[len-index] = n%10;
		index++;
	}

	return *arr;
}

void printNum(Game* game, int x, int y, int num) {

	printf("%d\n", num);

}
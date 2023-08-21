// a text based version of paint, user can change a board with certain commands
// usage for a 10 x 10 canvas: ./paint.out
// usage for r x c canvas: ./paint.out [r] [c]
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "setup.h"
#include "paint.h"

int main(int argc, char** argv) {

	int num_rows = 0;
	int num_cols = 0;
	bool big_canvas = false;

	// check command line arguments
	if (argc == 1) {
		num_rows = 10;
		num_cols = 10;
	} else if (argc != 3) {
		printf("Wrong number of command line arguments entered.\nUsage: ./paint.out [num_rows num_cols]\nMaking default board of 10 X 10.\n");
		num_rows = 10;
		num_cols = 10;
		big_canvas = true;
	} else {
		int num = 0;
		int i = 1;
		while (i < argc) {
			sscanf(argv[i], " %d", &num);
			i++;
			if (num_rows == 0) num_rows = num;
			else num_cols = num;
		}
		if (num_rows < 1) {
			printf("The number of rows is less than 1.\nMaking default board of 10 X 10.\n");
			num_rows = 10;
			num_cols = 10;
		} else if (num_cols < 1) {
			printf("The number of columns is less than 1.\nMaking default board of 10 X 10.\n");
			num_rows = 10;
			num_cols = 10;
		}
	}

	Canvas canvas = make_canvas(num_rows, num_cols, big_canvas);
	paint(&canvas, big_canvas);

	return 0;
}

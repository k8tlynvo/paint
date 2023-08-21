// setup and print the canvas 
#include "setup.h"
#include <ctype.h>

Canvas make_canvas(int num_rows, int num_cols, bool big_canvas) {
	/*
	make the canvas, a 2d array of blank characters '*'
	@num_rows: the number of rows on the canvas
	@num_cols: the number of cols on the canvas
	@big_canvas: whether the canvas has extra spaceing between characters
	@return: the canvas
	*/
	Canvas canvas;
	canvas.rows = num_rows;
	canvas.cols = num_cols;
	canvas.big_canvas = big_canvas;
	canvas.space = (char**)malloc(num_rows * sizeof(char*));
	for (int row = 0; row < num_rows; row++) {
		canvas.space[row] = (char*)malloc(num_cols * sizeof(char));
		for (int col = 0; col < num_cols; col++) {
			canvas.space[row][col] = '*';
		}
	}
	return canvas;
}

void print_canvas(Canvas canvas) {
	/*
	print the canvas to the screen
	@canvas: the canvas
	*/
	for(int i = canvas.rows - 1; i >= 0; i--) {
		// label the row
		if (canvas.big_canvas) printf(" %d ", i);
		else printf("%d ", i);
		// print the stars
		for(int j = 0; j < canvas.cols; j++) {
			if (canvas.big_canvas) printf(" %c ", canvas.space[i][j]);
			else printf("%c ", canvas.space[i][j]);
		}
		printf("\n");
	}
	// bottom numbers
	if (canvas.big_canvas) printf("   ");
	else printf("  ");
	for (int i = 0; i < canvas.cols; i++) {
		if (canvas.big_canvas) printf(" %d ", i);
		else printf("%d ", i);
	}
	printf("\n");
}

void delete_canvas(Canvas *canvas) {
	/*
	delete the canvas space at the end of paint
	@*canvas: a pointer to the canvas
	*/

	for(int i = 0; i < canvas->rows; i++) {
		free(canvas->space[i]);
	}
	free(canvas->space);
}

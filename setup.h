#ifndef SETUP_H
	#define SETUP_H

	#include <string.h>
	#include <stdbool.h>
	#include <stdlib.h>
	#include <stdio.h>

	typedef struct Canvas_struct{
		int rows;
		int cols;
		char** space;
		bool big_canvas;
	} Canvas;

	Canvas make_canvas(int num_rows, int num_cols, bool big_canvas);
	void print_canvas(Canvas canvas);
	void delete_canvas(Canvas *canvas);

#endif

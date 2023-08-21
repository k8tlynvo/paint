#ifndef PAINT_H
	#define PAINT_H

	#include <stdlib.h>
	#include "setup.h"

	typedef struct Line_struct{
		int start_row;
		int start_col;
		int end_row;
		int end_col;
		bool is_horizontal;
		bool is_vertical;
		bool is_diagonal;
		bool is_one_cell;
	} Line;

	typedef struct Point_struct{
		int row;
		int col;
	} Point;

	void paint(Canvas *canvas, bool big_canvas);
	void get_valid_command(Canvas *canvas, char* *command);
	void do_command(char* command, Canvas *canvas);
	void print_help();
	void write(Canvas *canvas, Line line);
	void write_horizontal_line(Canvas *canvas, Line line);
	void write_vertical_line(Canvas *canvas, Line line);
	void write_diagonal_line(Canvas *canvas, Line line);
	void write_one_cell_line(Canvas *canvas, Line line);
	void erase(Canvas *canvas, Point point);
	void resize(Canvas *canvas, int *num_rows, int *num_cols);
	void add(Canvas *canvas, char r_or_c, int pos);
	void delete(Canvas *canvas, char r_or_c, int pos);
	void save(Canvas canvas, char* file_path);
	void load(Canvas *canvas, char* file_path);

#endif

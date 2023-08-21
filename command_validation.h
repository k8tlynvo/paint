#ifndef COMMAND_VALIDATION_H
	#define COMMAND_VALIDATION_H

	#include "setup.h"
	#include "paint.h"

	bool is_valid_command(char* command, Canvas *canvas, int num_char);
	bool is_valid_quit(char* command);
	bool is_valid_help(char* command);
	bool is_valid_write(char* command, Canvas *canvas, Line *line);
	bool in_row_bounds(int row, Canvas canvas);
	bool in_col_bounds(int col, Canvas canvas);
	bool is_horizontal_line(Line *line);
	bool is_vertical_line(Line *line);
	bool is_diagonal_line(Line *line);
	bool is_one_cell_line(Line *line);
	bool is_valid_erase(char* command, Canvas canvas, Point *point);
	bool is_valid_resize(char* command, int* num_rows, int* num_cols);
	bool is_valid_add(char* command, Canvas *canvas, char* r_or_c, int* pos);
	bool is_valid_delete(char* command, Canvas *canvas, char* r_or_c, int* pos);
	bool is_valid_save(char* command, int num_char, char* *file_path);
	bool is_valid_load(char* command, int num_char, char* *file_path);

#endif

// validate paint commands and calls to execute the commands if they're valid
#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include "command_validation.h"

bool is_valid_command(char* command, Canvas *canvas, int num_char) {
	/*
	check whether a command is valid and execute the command if it is
	@command: the command as a string
	@*canvas: a pointer to the canvas
	@num_char: the number of characters in command
	@return: whether a command is valid
	*/
	char c;
	sscanf(command, " %c", &c);

	if (is_valid_quit(command)) {
		return true;
	} else if (is_valid_help(command)) {
		print_help();
		return true;
	}
	else if (c == 'w') {
		Line line;
		if (!is_valid_write(command, canvas, &line)) {
			printf("Improper draw command.\n");
			print_canvas(*canvas);
			return false;
		} else {
			write(canvas, line);
			return true;
		}
	} else if (c == 'e') {
		Point point;
		if (!is_valid_erase(command, *canvas, &point)) {
			printf("Improper erase command.\n");
			print_canvas(*canvas);
			return false;
		} else {
			erase(canvas, point);
			return true;
		}
	} else if (c == 'r') {
		int num_rows, num_cols;
		if (!is_valid_resize(command, &num_rows, &num_cols)) {
			printf("Improper resize command.\n");
			print_canvas(*canvas);
			return false;
		} else {
			resize(canvas, &num_rows, &num_cols);
			return true;
		}
	} else if (c == 'a') {
		char row_or_col;
		int pos;
		if (!is_valid_add(command, canvas, &row_or_col, &pos)) {
			printf("Improper add command.\n");
			print_canvas(*canvas);
			return false;
		} else {
			add(canvas, row_or_col, pos);
			return true;
		}
	} else if (c == 'd') {
		char row_or_col;
		int pos;
		if (!is_valid_delete(command, canvas, &row_or_col, &pos)) {
			printf("Improper delete command.\n");
			print_canvas(*canvas);
			return false;
		} else {
			delete(canvas, row_or_col, pos);
			return true;
		}
	} else if (c == 's') {
		char* file_path;
		if (!is_valid_save(command, num_char, &file_path)) {
			printf("Improper save command or file could not be created.\n");
			free(file_path);
			return false;
		} else {
			save(*canvas, file_path);
			free(file_path);
		} return true;
	} else if (c == 'l') {
		char* file_path;
		if (!is_valid_load(command, num_char, &file_path)) {
			printf("Improper load command or file could not be opened.\n");
			free(file_path);
			return false;
		} else {
			load(canvas, file_path);
			free(file_path);
			return true;
		}
	} else {
		printf("Unrecognized command. Type h for help.\n");
		print_canvas(*canvas);
		return false;
	}

}

bool is_valid_quit(char* command) {
	/*
	check whether a quit command is valid
	@command: the command as a string
	@return: whether the command is valid
	*/
	char q[10];
	sscanf(command, " %s", q);
	if (strcmp(q, "q") != 0) return false;
	else return true;
}

bool is_valid_help(char* command) {
	/*
	check whether a help command is valid
	@command: the command as a string
	@return: whether the command is valid
	*/
	char h[10];
	sscanf(command, " %s", h);
	if (strcmp(h, "h") != 0) return false;
	else return true;
}

bool is_valid_write(char* command, Canvas *canvas, Line *line) {
	/*
	check whether a write command is valid
	@command: the command as a string
	@*canvas: a pointer to the canvas
	@*line: a pointer to the line that will be written
	@return: whether the command is valid
	*/
	int num_args_needed = 5;
	int num_args_read;
	char w;
	char space[20];
	num_args_read = sscanf(command, " %c %d %d %d %d %s", &w, &line->start_row, &line->start_col, &line->end_row, &line->end_col, space);
	if (num_args_read != num_args_needed) {
		return false;
	} else if (!in_row_bounds(line->start_row, *canvas) || !in_col_bounds(line->start_col, *canvas) ||
					 !in_row_bounds(line->end_row, *canvas) || !in_col_bounds(line->end_col, *canvas)) {
		// line not in bounds
		return false;
	} else if(!is_horizontal_line(line) && !is_vertical_line(line) && !is_diagonal_line(line) && !is_one_cell_line(line)) {
		// not a straight line
		return false;
	}
	else { // it's a valid line
		return true;
	}
}

bool in_row_bounds(int row, Canvas canvas) {
	/*
	check whether a row is in the bounds of the canvas
	@row: the row number
	@canvas: the canvas
	@return: whether the row is in bounds
	*/
	return row >= 0 && row < canvas.rows;
}

bool in_col_bounds(int col, Canvas canvas) {
	/*
	check whether a column is in the bounds of the canvas
	@col: the column number
	@canvas: the canvas
	@return: whether the column is in bounds
	*/
	return col >= 0 && col < canvas.cols;
}

bool is_horizontal_line(Line *line) {
	/*
	check whether a line is horizontal and update it's information
	@*line: a pointer to the line
	@return: whether the line is horizontal
	*/
	if (line->start_row == line->end_row) {
		line->is_horizontal = true;
		return true;
	} else {
		line->is_horizontal = false;
		return false;
	}
}

bool is_vertical_line(Line *line) {
	/*
	check whether a line is vertical and update it's information
	@*line: a pointer to the line
	@return: whether the line is vertical
	*/
	if (line->start_col == line->end_col) {
		line->is_vertical = true;
		return true;
	} else {
		line->is_vertical = false;
		return false;
	}
}

bool is_diagonal_line(Line *line) {
	/*
	check whether a line is diagonal and update it's information
	@*line: a pointer to the line
	@return: whether the line is diagonal
	*/
	int row_difference = line->start_row - line->end_row;
	if (row_difference < 0) row_difference *= -1;
	int col_difference = line->start_col - line->end_col;
	if (col_difference < 0) col_difference *= -1;
	if (row_difference == col_difference) {
		line->is_diagonal = true;
		return true;
	} else {
		line->is_diagonal = false;
		return false;
	}
}

bool is_one_cell_line(Line *line) {
	/*
	check whether a line is one cell big and update it's information
	@*line: a pointer to the line
	@return: whether the line is one cell big
	*/
	int row_difference = line->start_row - line->end_row;
	if (row_difference < 0) row_difference *= -1;
	int col_difference = line->start_col - line->end_col;
	if (col_difference < 0) col_difference *= -1;
	if (row_difference == 1 && line->start_col == line->end_col) {
		line->is_one_cell = true;
		return true;
	} else if (col_difference == 1 && line->start_row == line->end_row) {
		line->is_one_cell = true;
		return true;
	} else {
		line->is_one_cell = false;
		return false;
	}
}

bool is_valid_erase(char* command, Canvas canvas, Point *point) {
	/*
	check whether an erase command is valid
	@command: the command as a string
	@canvas: the canvas
	@*point: a pointer to the point that's being erased
	@return whether the command is valid
	*/
	int num_args_needed = 3;
	int num_args_read;
	char e;
	char space[20];
	num_args_read = sscanf(command, " %c %d %d %s", &e, &point->row, &point->col, space);
	if (num_args_read == num_args_needed) {
		// if the row and col are in bounds
		return in_row_bounds(point->row, canvas) && in_col_bounds(point->col, canvas);
	} else return false;
}

bool is_valid_resize(char* command, int* num_rows, int* num_cols) {
	/*
	check whether a resize command is valid
	@command: the command as a string
	@num_rows: the number of rows the new sized canvas will have
	@num_cols: the number of columns the new sized canvas will have
	@return: whether the commadn is valid
	*/
	int num_args_needed = 3;
	int num_args_read;
	char r;
	char space[20];
	num_args_read = sscanf(command, " %c %d %d %s", &r, num_rows, num_cols, space);
	if (num_args_read == num_args_needed) {
		return *num_rows >= 1 && *num_cols >= 1;
	} else return false;
}

bool is_valid_add(char* command, Canvas *canvas, char* r_or_c, int* pos) {
	/*
	check whether an add command is valid
	@command: the command as a string
	@*canvas: a pointer to the canvas
	@*r_or_c: a pointer to the character indicating if we're adding a row or column
	@*pos: a pointer to the row or column number that we're adding to
	@return: whether the command is valid
	*/
	int num_ints_needed = 3;
	int num_ints_read;
	char a;
	char space[20];
	num_ints_read = sscanf(command, " %c %c %d %s", &a, r_or_c, pos, space);
	if (*r_or_c != 'r' && *r_or_c != 'c') {
		return false;
	} else if (num_ints_read != num_ints_needed) {
		return false;
	} else {
		if (*r_or_c == 'r') {
			return in_row_bounds(*pos, *canvas) || *pos == canvas->rows;
		}
		else {
			return in_col_bounds(*pos, *canvas) || *pos == canvas->cols;
		}
	}
}

bool is_valid_delete(char* command, Canvas *canvas, char* r_or_c, int* pos) {
	/*
	check whether a delete command is valid
	@command: the command as a string
	@*canvas: a pointer to the canvas
	@*r_or_c: a pointer to the character indicating if we're deleting a row or column
	@*pos: a pointer to the row or column number that we're deleting to
	@return: whether the command is valid
	*/
	int num_ints_needed = 3;
	int num_ints_read;
	char d;
	char space[20];
	num_ints_read = sscanf(command, " %c %c %d %s",&d, r_or_c, pos, space);
	if (*r_or_c != 'r' && *r_or_c != 'c') {
		return false;
	} else if (num_ints_read != num_ints_needed){
		return false;
	} else {
		if (*r_or_c == 'r') {
			return in_row_bounds(*pos, *canvas);
		} else {
			return in_col_bounds(*pos, *canvas);
		}
	}
}

bool is_valid_save(char* command, int num_char, char* *file_path) {
	/*
	check whether a save command is valid
	@command: the command as a string
	@num_char: the number of characters in the command
	@*file_path: a pointer to the file path
	@return: whether the command is valid
	*/
	int num_args_needed = 2;
	int num_args_read;
	char s;
	*file_path = (char*)malloc((num_char - 2) * sizeof(char));
	num_args_read = sscanf(command, " %c %s", &s, *file_path);
	FILE* f = fopen(*file_path, "w");
	if (num_args_read != num_args_needed) {
		return false;
	} else if (f == NULL) {
		return false;
	} else {
		fclose(f);
		return true;
	}
}

bool is_valid_load(char* command, int num_char, char* *file_path) {
	/*
	check whether a load command is valid
	@command: the command as a string
	@num_char: the number of characters in the command
	@*file_path: a pointer to the file path
	@return: whether the command is valid
	*/
	int num_args_needed = 2;
	int num_args_read;
	char l;
	*file_path = (char*)malloc((num_char - 2) * sizeof(char));
	num_args_read = sscanf(command, " %c %s", &l, *file_path);
	FILE* f = fopen(*file_path, "r");
	if (num_args_read != num_args_needed) {
		return false;
	} else if (f == NULL) {
		return false;
	} else {
		fclose(f);
		return true;
	}
}

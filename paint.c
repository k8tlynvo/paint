// functions to change the canvas according to user commands
#include "paint.h"
#include <ctype.h>
#include "command_validation.h"
#include <stdio.h>

void paint(Canvas *canvas, bool big_canvas) {
	/*
	paint the canvas
	@*canvas: a pointer to the canvas
	@big_canvas: whether the canvas has extra spacing
	*/
	char* command = (char*)malloc(sizeof(char));
	do {
		print_canvas(*canvas);
		get_valid_command(canvas, &command);
	} while (!is_valid_quit(command));
	free(command);
	delete_canvas(canvas);
	exit(0);
}

void get_valid_command(Canvas *canvas, char* *command) {
	/*
	get a valid command from the user, execute it if it is
	@*canvas: a pointer to the canvas
	@command: pointer to the command
	*/
	int num_char;
	do{
		printf("Enter your command: ");
		char c;
		scanf("%c", &c);
		num_char = 1;
		while (c != '\n') {
			*command = (char*)realloc(*command, num_char * sizeof(char));
			(*command)[num_char - 1] = c;
			num_char++;
			scanf("%c", &c);
		}
		*command = (char*)realloc(*command, num_char * sizeof(char));
		(*command)[num_char - 1] = '\0';
	} while(!is_valid_command(*command, canvas, num_char));
}

void print_help() {
	/* print the help information */
  printf("Commands:\n");
  printf("Help: h\n");
  printf("Quit: q\n");
  printf("Draw line: w row_start col_start row_end col_end\n");
  printf("Resize: r num_rows num_cols\n");
  printf("Add row or column: a [r | c] pos\n");
  printf("Delete row or column: d [r | c] pos\n");
  printf("Erase: e row col\n");
  printf("Save: s file_name\n");
  printf("Load: l file_name\n");
}

void write(Canvas *canvas, Line line) {
	/*
	write a line on the canvas
	@*canvas: pointer to the canvas
	@line: the line to be drawn
	*/
	if (line.is_horizontal) {
		write_horizontal_line(canvas, line);
	} else if (line.is_vertical) {
		write_vertical_line(canvas, line);
	} else if (line.is_diagonal){
		write_diagonal_line(canvas, line);
	} else { // it's one cell big
		write_one_cell_line(canvas, line);
	}
}

void write_horizontal_line(Canvas *canvas, Line line) {
	/*
	write a horizontal line on the canvas
	@*canvas: pointer to the canvas
	@line: the line to be drawn
	*/
	// left to right
	if (line.start_col < line.end_col) {
		for (int col = line.start_col; col <= line.end_col; col++) {
			// if the space is empty
			if ((*canvas).space[line.start_row][col] == '*') {
				(*canvas).space[line.start_row][col] = '-';
			} else if ((*canvas).space[line.start_row][col] == '-' ){ // it's a also a hor line
				(*canvas).space[line.start_row][col] = '-';
			} else { // the char is something else
				(*canvas).space[line.start_row][col] = '+';
			}
		}
	} else { // right to left
		for (int col = line.start_col; col >= line.end_col; col--) {
			// if the space is empty
			if ((*canvas).space[line.start_row][col] == '*') {
				(*canvas).space[line.start_row][col] = '-';
			} else if ((*canvas).space[line.start_row][col] == '-' ){ // it's a also a hor line
				(*canvas).space[line.start_row][col] = '-';
			} else { // the char is something else
				(*canvas).space[line.start_row][col] = '+';
			}
		}
	}
}

void write_vertical_line(Canvas *canvas, Line line) {
	/*
	write a vertical line on the canvas
	@*canvas: pointer to the canvas
	@line: the line to be drawn
	*/
	// down to up
	if (line.start_row < line.end_row) {
		for (int row = line.start_row; row <= line.end_row; row++) {
			// if the space is empty
			if ((*canvas).space[row][line.start_col] == '*') {
				(*canvas).space[row][line.start_col] = '|';
			} else if ((*canvas).space[row][line.start_col] == '|' ){ // it's a also a ver line
				(*canvas).space[row][line.start_col] = '|';
			} else { // the char is something else
				(*canvas).space[row][line.start_col] = '+';
			}
		}
	} else { // up to down
		for (int row = line.start_row; row >= line.end_row; row--) {
			if ((*canvas).space[row][line.start_col] == '*') {
				(*canvas).space[row][line.start_col] = '|';
			} else if ((*canvas).space[row][line.start_col] == '|' ){ // it's a also a ver line
				(*canvas).space[row][line.start_col] = '|';
			} else { // the char is something else
				(*canvas).space[row][line.start_col] = '+';
			}
		}
	}
}

void write_diagonal_line(Canvas *canvas, Line line) {
	/*
	write a diagonal line on the canvas
	@*canvas: pointer to the canvas
	@line: the line to be drawn
	*/
	int length = line.start_row - line.end_row;
	if (length < 0) length *= -1;
	int cur_row = line.start_row;
	int cur_col = line.start_col;
	// right diagonal down to up
	if (line.start_row < line.end_row && line.start_col < line.end_col) {
		for (int i = 0; i <= length; i++) {
			if ((*canvas).space[cur_row][cur_col] == '*') {
				(*canvas).space[cur_row][cur_col] = '/';
			} else if ((*canvas).space[cur_row][cur_col] == '/' ){ // it's a also a ver line
				(*canvas).space[cur_row][cur_col] = '/';
			} else { // the char is something else
				(*canvas).space[cur_row][cur_col] = '+';
			}
			cur_row++;
			cur_col++;
		}
	} else if (line.start_row > line.end_row && line.start_col > line.end_col) { // right diagonal up to down
		for (int i = 0; i <= length; i++) {
			if ((*canvas).space[cur_row][cur_col] == '*') {
				(*canvas).space[cur_row][cur_col] = '/';
			} else if ((*canvas).space[cur_row][cur_col] == '/' ){ // it's a also a ver line
				(*canvas).space[cur_row][cur_col] = '/';
			} else { // the char is something else
				(*canvas).space[cur_row][cur_col] = '+';
			}
			cur_row--;
			cur_col--;
		}
	} else if (line.start_row < line.end_row && line.start_col > line.end_col) { // left diagonal down to up
		for (int i = 0; i <= length; i++) {
			if ((*canvas).space[cur_row][cur_col] == '*') {
				(*canvas).space[cur_row][cur_col] = '\\';
			} else if ((*canvas).space[cur_row][cur_col] == '\\' ){ // it's a also a ver line
				(*canvas).space[cur_row][cur_col] = '\\';
			} else { // the char is something else
				(*canvas).space[cur_row][cur_col] = '+';
			}
			cur_row++;
			cur_col--;
		}
	} else { // left diagonal up to down
		for (int i = 0; i <= length; i++) {
			if ((*canvas).space[cur_row][cur_col] == '*') {
				(*canvas).space[cur_row][cur_col] = '\\';
			} else if ((*canvas).space[cur_row][cur_col] == '\\' ){ // it's a also a ver line
				(*canvas).space[cur_row][cur_col] = '\\';
			} else { // the char is something else
				(*canvas).space[cur_row][cur_col] = '+';
			}
			cur_row--;
			cur_col++;
		}
	}
}

void write_one_cell_line(Canvas *canvas, Line line) {
	/*
	write a one-cell-ed line on the canvas
	@*canvas: pointer to the canvas
	@line: the line to be drawn
	*/
	// if the space is empty
	if ((*canvas).space[line.start_row][line.start_col] == '*') {
		(*canvas).space[line.start_row][line.start_col] = '-';
	} else if ((*canvas).space[line.start_row][line.start_col] == '-'){ // if ther's a hor line
		(*canvas).space[line.start_row][line.start_col] = '-';
	} else { // there's something else there
		(*canvas).space[line.start_row][line.start_col] = '+';
	}
}

void erase(Canvas *canvas, Point point) {
	/*
	erase a point on the canvas
	@*canvas: pointer to the canvas
	@point: the point to be erased
	*/
	canvas->space[point.row][point.col] = '*';
}

void resize(Canvas *canvas, int *num_rows, int *num_cols) {
	/*
	resize the canvas to certain dimensions
	@*canvas: pointer to the canvas
	@num_rows: the number of rows of the new canvas size
	@num_cols: the number of columns of the new canvas size
	*/
	int og_rows = canvas->rows;
	int og_cols = canvas->cols;
	canvas->rows = *num_rows;
	canvas->cols = *num_cols;

	if (canvas->rows < og_rows) { // if delete rows
		for (int i = canvas->rows; i < og_rows; i++) {
			free(canvas->space[i]);
		}
	}
	canvas->space = (char**)realloc(canvas->space, canvas->rows * sizeof(char*));

	if (canvas->cols < og_cols) { // if delete cols
		for (int i = 0; i < canvas->rows; i++) {

			canvas->space[i] = (char*)realloc(canvas->space[i], canvas->cols * sizeof(char));
		}
	}

	if (canvas->rows > og_rows && canvas->cols == og_cols) { // only added more rows
		for (int i = og_rows; i < canvas->rows; i++) {
			canvas->space[i] = (char*)malloc(canvas->cols * sizeof(char));
			for(int j = 0; j < canvas->cols; j++) {
				canvas->space[i][j] = '*';
			}
		}
	} else if (canvas->rows == og_rows && canvas->cols > og_cols) { // only added more cols
		for (int i = 0; i  < canvas->rows; i++) {
			// make more space for the extra col
			canvas->space[i] = (char*)realloc(canvas->space[i], canvas->cols * sizeof(char));
			for (int j = og_cols; j < canvas->cols; j++) {
				canvas->space[i][j] = '*';
			}
		}
	} else { // added both more rows and cols
		// add rows
		for (int i = og_rows; i < canvas->rows; i++) {
			canvas->space[i] = (char*)malloc(og_cols * sizeof(char));
			for(int j = 0; j < og_cols; j++) {
				canvas->space[i][j] = '*';
			}
		}
		for (int i = 0; i  < canvas->rows; i++) {
			// make more space for the extra col
			canvas->space[i] = (char*)realloc(canvas->space[i], canvas->cols * sizeof(char));
			for (int j = og_cols; j < canvas->cols; j++) {
				canvas->space[i][j] = '*';
			}
		}
	}
}

void add(Canvas *canvas, char r_or_c, int pos) {
	/*
	add a row or column to the canvas
	@*canvas: pointer to the canvas
	@r_or_c: the character r for row or c for column
	@pos: the specified row or column number
	*/
	// adding row
	if (r_or_c == 'r') {
		canvas->rows++;
		canvas->space = (char**)realloc(canvas->space, canvas->rows * sizeof(char*));
		canvas->space[canvas->rows - 1] = (char*)malloc(canvas->cols * sizeof(char));
		// if not adding outermost row, move the values
		if (pos != canvas->rows - 1) {
			for (int row = canvas->rows - 2; row >= pos; row--) {
				for (int col = 0; col < canvas->cols; col++) {
					canvas->space[row + 1][col] = canvas->space[row][col];
				}
			}
		}

		for (int i = 0; i < canvas->cols; i++) {
			canvas->space[pos][i] = '*';
		}

	} else { // if adding a col
		canvas->cols++;
		for (int row = 0; row < canvas->rows; row++) {
			canvas->space[row] = (char*)realloc(canvas->space[row], canvas->cols * sizeof(char));
		}
		if (pos != canvas->cols - 1) {
			for (int row = 0; row < canvas->rows; row++) {
				for (int col = canvas->cols - 2; col >= pos; col--) {
					canvas->space[row][col+1] = canvas->space[row][col];
				}
			}
		}

		for (int i = 0 ; i < canvas->rows; i++) {
			canvas->space[i][pos] = '*';
		}
	}
}

void delete(Canvas *canvas, char r_or_c, int pos) {
	/*
	delete a specified row or column
	@*canvas: pointer to the canvas
	@r_or_c: the character r for row or c for column
	@pos: the specified row or column number
	*/
	if (r_or_c == 'r') {
		canvas->rows--;
		if (pos == canvas->rows) { // deleting the outermost row
			canvas->space = (char**)realloc(canvas->space, canvas->rows * sizeof(char*));
		} else {
			// move the values from the outer rows to that row
			// then realloc the canvas space
			for (int row = pos + 1; row <= canvas->rows; row++) {
				for (int col = 0; col < canvas->cols; col++) {
					canvas->space[row-1][col] = canvas->space[row][col];
				}
			}
			canvas->space = (char**)realloc(canvas->space, canvas->rows * sizeof(char*));
		}
	} else { // deleting a col
		canvas->cols--;
		if (pos == canvas->cols) { // delete outermost col
			for (int row = 0; row < canvas->rows; row++) {
				canvas->space[row] = (char*)realloc(canvas->space[row], canvas->cols * sizeof(char));
			}
		} else {
			for (int row = 0; row < canvas->rows; row++) {
				for (int col = pos + 1; col <= canvas->cols; col++) {
					canvas->space[row][col - 1] = canvas->space[row][col];
				}
				canvas->space[row] = (char*)realloc(canvas->space[row], canvas->cols * sizeof(char));
			}
		}
	}
}

void save(Canvas canvas, char* file_path) {
	/*
	save the canvas to a file, the first two values in the file are the number of rows and columns
	@canvas: the canvas
	@file_path: the path to the file
	*/
	FILE* f = fopen(file_path, "w");
	// say rows, cols, then canvas chars wo the row and col labels
	fprintf(f, "%d %d", canvas.rows, canvas.cols);
	for (int row = 0; row < canvas.rows; row++) {
		for (int col = 0; col < canvas.cols; col++) {
			fprintf(f, "%c", canvas.space[row][col]);
		}
	}
	fclose(f);
}

void load(Canvas *canvas, char* file_path) {
	/*
	load the a canvas from a file into the program
	@*canvas: a pointer to the canvas
	@file_path: the path to the file
	*/
	FILE* f = fopen(file_path, "r");
	fscanf(f, "%d %d", &canvas->rows, &canvas->cols);
	canvas->space = (char**)malloc(canvas->rows * sizeof(char*));
	for (int row = 0; row < canvas->rows; row++) {
		canvas->space[row] = (char*)malloc(canvas->cols * sizeof(char));
	}
	for (int row = 0; row < canvas->rows; row++) {
		for (int col = 0; col < canvas->cols; col++) {
			fscanf(f, "%c", &canvas->space[row][col]);
		}
	}
	fclose(f);
}

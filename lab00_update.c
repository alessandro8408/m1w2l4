#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>


// grid width equals grid height
const char grid_size			= 10;

void print_intro();
void create_grid(char grid[grid_size][grid_size]);
void print_grid(char grid[grid_size][grid_size]);
void update_grid(char grid[grid_size][grid_size], char current_position[2]);
bool move_check(char new_value);
void main_loop(char grid[grid_size][grid_size]);
void find_max(char grid[grid_size][grid_size], char dir, char y, char x, short curr_score);

// save score as global variable
short score			= 0;
short max_score				= 0;

// check if you want to play another game
char new_game = 'y';

void check_new_game() {
	printf("Do you want to play another game? (y/n) ");
	scanf(" %c", &new_game);
}

int main() {
	while (new_game == 'y' || new_game == 'Y' || new_game == 's' || new_game == 'S') {
		print_intro();

		char grid[grid_size][grid_size];
		create_grid(grid);
		find_max(grid, 'd', 0, 0, 0);
		find_max(grid, 'r', 0, 0, 0);

		main_loop(grid);
		printf("The maximum score is: %d.\n", max_score);
		if (max_score == score)
			printf("Congrats, you reached the maximum possible score.\n");

		check_new_game();
	}

	return EXIT_SUCCESS;
}

// explain the rules of the game 
void print_intro() {
	printf("You start the game at the upper left of a 10x10 grid.\n");
	printf("You can only move right or down.\n");
	printf("Every time you move to a cell with value N, you add N to your score.\n");
	printf("N is an integer ranging from -16 and 15.\n");
	printf("The game ends as you reach the lower right corner of the grid, the value of which will be added to your total score, but you are free to end the game earlier.\n");
}

// create the grid, with the starting point being at the upper left corner
void create_grid(char grid[grid_size][grid_size]) {
	// the seed varies every game
	srand(time(NULL));
	for (int y = 0; y < grid_size; y++) {
		for (int x = 0; x < grid_size; x++) {
			if (y < x) {
				// set the upper right triangle of the grid to 0
				grid[y][x]	= 0;
				continue;
			}
			// set the grid at coordinates to a "random" value within given range
			grid[y][x]	= rand() % 32 - 16;
		}
	}
	// print the current position as 'X' 
	grid[0][0]	= 'X';
}

// print out the current grid, given the player's current position
void print_grid(char grid[grid_size][grid_size]) {
	for (int y = 0; y < grid_size; y++) {
		for (int x = 0; x < grid_size; x++) {
			// higihlight the player's current position
			if (grid[y][x] == 'X') 
				printf("x\t");
			else
				printf("%d\t", grid[y][x]);
		}
		printf("\n");
	}
}

void update_grid(char grid[grid_size][grid_size], char current_position[2]) {
	// update the score with the value you moved onto
	score	+= grid[current_position[0]][current_position[1]];
	printf("Your score increased by %d points.\n", grid[current_position[0]][current_position[1]]);
	// update the grid (the value inside the cell was already used)
	grid[current_position[0]][current_position[1]]	= 'X';
}

bool move_check(char new_value) {
	// check boundaries
	if (new_value >= 0 && new_value < 10)
		return true;
	return false;
}

void main_loop(char grid[grid_size][grid_size]) {
	// current_position[0] corresponds to y
	// current_position[1] corresponds to x
	char current_position[2]	= {0, 0};
	char choice;

	do {
		print_grid(grid);
		printf("You are currently at (%d, %d).\n\n", current_position[1], current_position[0]);
		printf("SCORE: %d\n", score);
		printf("Enter 'd' to move down, 'r' to move right, 'q' to quit the game (only the first character will be read).\n");
		scanf(" %c", &choice);

		bool valid_move	= true;

		// move down = increase y
		if (choice == 'd') 
		{
			if (!move_check(++current_position[0])) {
				current_position[0]--;
				valid_move	= false;
			}
		}

		// move right = increase x
		else if (choice == 'r') {
			if (!move_check(++current_position[1])) {
				current_position[1]--;
				valid_move	= false;
			}
		}

		// jump to while check
		else {
			if (choice != 'q')
				printf("Invalid input.\n");
			continue;
		}
		
		// update the grid only if you played a legal move
		if (valid_move)
			update_grid(grid, current_position);
		else
			printf("Please enter a valid move.\n");

	} while (choice != 'q' && !(current_position[0] == 9 && current_position[1] == 9));

	printf("You have finished the game with: %d POINTS.\n", score);
}

void find_max(char grid[grid_size][grid_size], char dir, char y, char x, short curr_score) {
	if (dir == 'd' && move_check(y+1)) {
		curr_score += grid[y+1][x];
	} else if (dir == 'r' && move_check(x+1)) {
		curr_score += grid[y][x+1];
	}
	if (move_check(y+1))
		find_max(grid, 'd', y+1, x, curr_score);
	if (move_check(x+1))
		find_max(grid, 'r', y, x+1, curr_score);

	if (curr_score > max_score) {
		max_score	= curr_score;
	}
}

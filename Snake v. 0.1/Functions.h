#pragma once

#include <iostream>
#include <array>
#include <vector>
#include <cstdlib>
#include <ctime>


// WYMIARY PLANSZY - MOŻLIWA EDYCJA
constexpr int WIDTH{ 30 };  //SZEROKOŚĆ
constexpr int HEIGHT{ 15 };  //WYSOKOŚĆ

// WARTOŚCI PÓL NA PLANSZY - ZNACZENIE
// 0 - puste
// 1 - ciało węża
// 2 - głowa węża
// 3 - jabłko

std::array<std::array<int, HEIGHT>, WIDTH> board;
std::array<int, 2> apple;
std::array<std::vector<int>, 2> snake;


// POCZĄTKOWA DŁUGOŚĆ WĘŻA - ZMIANY NIEZALECANE - MINIMUM 2
int snake_len = 4;

//OSTATNI RUCH
char last_move = 'w';

// WYPEŁNIENIE PLANSZY ZERAMI
void InitBoard() {

	for (int i = 0; i < WIDTH; i++) {
		board[i].fill(0);
	}
}

// WYPRINTOWANIE PLANSZY DO TERMINALU W CZYTELNEJ FORMIE
void PrintBoard() {

	int tmp = 0;

	for (int a = 0; a < (WIDTH + 2); a++) { std::cout << "*"; }
	std::cout << std::endl;

	for (int i = 0; i < HEIGHT; i++) {
		std::cout << "*";
		for (int j = 0; j < WIDTH; j++) {

			tmp = board[j][i];
			switch (tmp) {
			case 1:
				std::cout << ".";  //OGON
				break;
			case 2:
				std::cout << "o";  //GŁOWA
				break;
			case 3:
				std::cout << "@";  //JABŁKO
				break;
			default:
				std::cout << " ";  //PUSTE POLE
				break;
			}
		}
		std::cout << "*" << std::endl;
	}
	for (int a = 0; a < (WIDTH + 2); a++) { std::cout << "*"; }
	std::cout << std::endl;
}


void InitSnake() {
	//POZYCJA POCZĄTKOWA
	int width_setting = WIDTH / 2;  //SZEROKOŚĆ
	int height_setting = HEIGHT / 2;  //WYSOKOŚĆ

	//INICJALIZACJA ZMIENNEJ PAMIĘTAJĄCEJ POZYCJĘ WĘŻA
	snake[0].push_back(width_setting);
	snake[1].push_back(height_setting);
	for (int i = 1; i < snake_len; i++) {
		snake[0].push_back(width_setting);
		snake[1].push_back((height_setting) + i);
	}

	//DODANIE WĘŻA DO PLANSZY
	board[snake[0][0]][snake[1][0]] = 2;
	for (int i = 1; i < snake_len; i++) {
		board[snake[0][i]][snake[1][i]] = 1;
	}
}

//GENEROWANIE POZYCJI JABŁKA
void SpawnApple() {
	//JABŁKO JEST GENEROWANE LOSOWO
	std::srand((unsigned)std::time(NULL));

	int x = 0;
	int y = 0;

	do {
		x = std::rand() % WIDTH;
		y = std::rand() % HEIGHT;
	} while (board[x][y]!=0);

	apple[0] = x;
	apple[1] = y;

	board[x][y] = 3;
}

int MoveSnake(char move) {

	std::array<std::vector<int>, 2> snake_copy = snake;
	int x = 0;
	int y = 0;

	switch (move) {
	case 'w':
		y = -1;
		break;
	case 'a':
		x = -1;
		break;
	case 's':
		y = 1;
		break;
	case 'd':
		x = 1;
	default:
		break;
	}

	// jeśli próbujemy zrobić 180 stopni - błąd
	if (((snake[0][0] + x) == (snake[0][1])) && ((snake[1][0] + y) == (snake[1][1]))) { 
		MoveSnake(last_move);
		return 1;
	}

	// jeśli trafimy w ciało lub ścianę
	if (((snake[0][0] + x) < 0) || ((snake[0][0] + x) >= WIDTH) || ((snake[1][0] + y) < 0) || ((snake[1][0] + y) >= HEIGHT) || (board[snake[0][0] + x][snake[1][0] + y] == 1)) {
		return 2; 
	}

	// jeśli trafimy w jabłko
	if (board[snake[0][0] + x][snake[1][0] + y] == 3) {

		snake[0][0] += x;
		snake[1][0] += y;

		for(int i = 1; i < snake_len; i++) {
			snake[0][i] = snake_copy[0][i - 1];
			snake[1][i] = snake_copy[1][i - 1];
		}

		//wydłużamy węża		
		snake[0].push_back(snake_copy[0].back());
		snake[1].push_back(snake_copy[1].back());
		snake_len += 1;
		
		InitBoard();

		board[snake[0][0]][snake[1][0]] = 2;
		for (int i = 1; i < snake_len; i++) {
			board[snake[0][i]][snake[1][i]] = 1;
		}

		SpawnApple();
		last_move = move;

		return 0;
	}

	//jeśli w nic nie trafimy - jedziemy dalej
	snake[0][0] += x;
	snake[1][0] += y;

	for (int i = 1; i < snake_len; i++) {
		snake[0][i] = snake_copy[0][i - 1];
		snake[1][i] = snake_copy[1][i - 1];
	}

	board[snake[0][0]][snake[1][0]] = 2;
	for (int i = 1; i < snake_len; i++) {
		board[snake[0][i]][snake[1][i]] = 1;
	}

	board[snake_copy[0].back()][snake_copy[1].back()] = 0;

	last_move = move;
	return 0;

}

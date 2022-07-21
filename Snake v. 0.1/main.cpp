#include "Functions.h"
#include <conio.h>
#include <Windows.h>

int main() {

	int tmp = 0;
	char move = 'w';

	InitBoard();
	InitSnake();
	SpawnApple();
	PrintBoard();



	while (1) {
		Sleep(100);

		if (_kbhit() != 0) {
			move = _getch();
			FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
		}
		tmp = MoveSnake(move);
		
		if (tmp == 2) {
			std::cout << "Game over!";
			return 0;
		}
		system("cls");
		PrintBoard();
		
	}

	return 0;
}
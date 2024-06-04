#include <SFML\Graphics.hpp>
const int HEIGHT_MAP = 25;//размер карты высота
const int WIDTH_MAP = 40;//размер карты ширина 


sf::String TileMap[HEIGHT_MAP] = {
	"0000000000000000000000000000000000000000",
	"0dddd1     1fdhdddd1    ddddd          0",
	"0dddd1  f  1ddddfdd1   hddddd          0",
	"0dddd1     1ddfdddd1      1            0",
	"0dddd1     1sdsdddd1      1        h   0",
	"0dddd1     1ddddddd1 f    1            0",
	"0sddss            ddd     1  f         0",
	"0ddddddddddddddddddddddddd1            0",
	"0   dd            ddd     1            0",
	"0111111111111111111111111111111dddd11110",
	"0         ddd     ddd     1            0",
	"0         dddh    ddd     1            0",
	"0         ddd     ddd     1        h   0",
	"0          1       1dddddd1dd          0",
	"0          1    h  1ddddddddd      f   0",
	"0  dddd    1f    f 1dddddddd           0",
	"011dddd111111111111111111111111111111110",
	"0  ddddddf            1                0",
	"0      ddd           ddd  dddddddddh   0",
	"0      ddd      f    ddd  d++++++++    0",
	"0       1             1   d++++++++    0",
	"0   h   1             1   d++++++++    0",
	"0       1        h    1   d++++++++    0",
	"0       1             1   d++++++++    0",
	"0000000000000000000000000000000000000000",
}; 



void randomMapGenerate() {//рандомно расставляем камни

	int randomElementX = 0;//случайный элемент по горизонтали
	int randomElementY = 0;//случ эл-т по вертикали
	srand(time(0));//рандом	
	int countStone = 15;//количество камней


	while (countStone > 0) {
		randomElementX = 1 + rand() % (WIDTH_MAP - 1);//рандомное по иксу от 1 до ширина карты-1, чтобы не получать числа бордюра карты
		randomElementY = 1 + rand() % (HEIGHT_MAP - 1);//по игреку так же

		if (TileMap[randomElementY][randomElementX] == ' ') {//если встретили символ пробел, 
			TileMap[randomElementY][randomElementX] = 's'; //то ставим туда камень.
			countStone--;
		}
	}

}

void randomGenerateScores() {
	int randomElementX = 0;
	int randomElementY = 0;
	srand(time(0));
	int countScores = 1;
	while (countScores > 0) {
		randomElementX = 1 + rand() % (WIDTH_MAP - 1);
		randomElementY = 1 + rand() % (HEIGHT_MAP - 1);

		if (TileMap[randomElementY][randomElementX] == ' ') {
			TileMap[randomElementY][randomElementX] = 'f'; 
			countScores--;
		}
	}
}


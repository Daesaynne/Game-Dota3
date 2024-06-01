#include <SFML\Graphics.hpp>
const int HEIGHT_MAP = 25;//������ ����� ������
const int WIDTH_MAP = 40;//������ ����� ������ 


sf::String TileMap[HEIGHT_MAP] = {
	"0000000000000000000000000000000000000000",
	"0      fff                             0",
	"0   s                                  0",
	"0             f           r            0",
	"0                         r            0",
	"0                    f    r            0",
	"0        s                r  s         0",
	"0                                      0",
	"0                  1                   0",
	"0       h          1                   0",
	"0                  1                   0",
	"0            h     1         ss        0",
	"0                                      0",
	"0                                      0",
	"0             11111111                 0",
	"0                                      0",
	"0                      h               0",
	"0        f                             0",
	"0                                      0",
	"0                          h           0",
	"0                                      0",
	"0                                      0",
	"0                                      0",
	"0                                      0",
	"0000000000000000000000000000000000000000",
}; 


void randomMapGenerate() {//�������� ����������� �����

	int randomElementX = 0;//��������� ������� �� �����������
	int randomElementY = 0;//���� ��-� �� ���������
	srand(time(0));//������	
	int countStone = 15;//���������� ������


	while (countStone > 0) {
		randomElementX = 1 + rand() % (WIDTH_MAP - 1);//��������� �� ���� �� 1 �� ������ �����-1, ����� �� �������� ����� ������� �����
		randomElementY = 1 + rand() % (HEIGHT_MAP - 1);//�� ������ ��� ��

		if (TileMap[randomElementY][randomElementX] == ' ') {//���� ��������� ������ ������, 
			TileMap[randomElementY][randomElementX] = 's'; //�� ������ ���� ������.
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


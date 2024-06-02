
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <clocale>
#include <sstream>
#include <vector>

#include "map.h"
#include "view.h"
#include "menu.h"

using namespace std;
using namespace sf;

class Player { // ����� ������
private:
    float x, y = 0;
public:
    float w, h, dx, dy, speed = 0; //���������� ������ � � �, ������ ������, ��������� (�� � � �� �), ���� ��������
    int dir = 0; //����������� (direction) �������� ������
    int PlayerScore;
    int health;
    bool life;
    String File; //���� � �����������
    Image image;
    Texture texture;
    Sprite sprite;

    Player(String F, float X, float Y, float W, float H) { 
        health = 100; life = true;
        File = F;//��� �����+����������
        w = W; h = H;//������ � ������
        PlayerScore = 0;
        image.loadFromFile("images/" + File);//���������� � image ���� �����������
        image.createMaskFromColor(Color(63, 63, 63));
        texture.loadFromImage(image);//���������� ���� ����������� � ��������
        sprite.setTexture(texture);//�������� ������ ���������
        x = X; y = Y;
        sprite.setTextureRect(IntRect(0,0,w,h));  //������ ������� ���� �������������
    }



    void update(float time) //������� "���������" ������� ������. update - ����������. ��������� � ���� ����� SFML , ���������� ���� �������� ����������, ����� ��������� ��������.
    {
        switch (dir)//��������� ��������� � ����������� �� �����������. (������ ����� ������������� �����������)
        {
        case 0: dx = speed; dy = 0;   break;//�� ���� ������ ������������� ��������, �� ������ ��������. ��������, ��� �������� ���� ������ ������
        case 1: dx = -speed; dy = 0;   break;//�� ���� ������ ������������� ��������, �� ������ ��������. ����������, ��� �������� ���� ������ �����
        case 2: dx = 0; dy = speed;   break;//�� ���� ������ ������� ��������, �� ������ �������������. ����������, ��� �������� ���� ������ ����
        case 3: dx = 0; dy = -speed;   break;//�� ���� ������ ������� ��������, �� ������ �������������. ����������, ��� �������� ���� ������ �����
        }

        x += dx * time;//�� �������� �� �������� �����. ���� ��������� �� ����� �������� �������� ��������� � ��� ��������� ��������
        y += dy * time;//���������� �� ������

        speed = 0;//�������� ��������, ����� �������� �����������.
        sprite.setPosition(x, y); //������� ������ � ������� x y , ����������. ���������� ������� � ���� �������, ����� �� ��� ������ ����� �� �����.
        interactionWithMap();//�������� �������, ���������� �� �������������� � ������

        if (health <= 0) { life = false; }
    }



    float getplayercoordinateX() {	//���� ������� ����� �������� ���������� �	
        return x;
    }
    float getplayercoordinateY() {	//���� ������� ����� �������� ���������� Y 	
        return y;
    }


    void interactionWithMap()//�-��� �������������� � ������
    {

        for (int i = y / 32; i < (y + h) / 32; i++)//���������� �� ��������, �������������� � �������,, �� ���� �� ���� ����������� ������� 32*32, ������� �� ���������� � 9 �����. ��� ������� ������� ����.
            for (int j = x / 32; j < (x + w) / 32; j++)//��� ����� �� 32, ��� ����� �������� ����� ���������, � ������� �������� �������������. (�� ���� ������ ������� 32*32, ������� ����� ������������ ������ �� ���������� ���������). � j<(x + w) / 32 - ������� ����������� ��������� �� ����. �� ���� ���������� ������ ������� ��������, ������� ������������� � ����������. ����� ������� ���� � ����� ����� ������� �� ����, ������� �� �� ������ �������� (���������������� � ������), �� ������� �������� (���������������� � ������)
            {
                if (TileMap[i][j] == '0' || TileMap[i][j] == '1')//���� ��� ��������� ������������� ������� 0 (�����), �� ��������� "����������� ��������" ���������:
                {
                    if (dy > 0)//���� �� ��� ����,
                    {
                        y = i * 32 - h;//�� �������� ���������� ����� ���������. ������� �������� ���������� ������ ���������� �� �����(�����) � ����� �������� �� ������ ������� ���������.
                    }
                    if (dy < 0)
                    {
                        y = i * 32 + 32;//���������� � ������� �����. dy<0, ������ �� ���� ����� (���������� ���������� ������)
                    }
                    if (dx > 0)
                    {
                        x = j * 32 - w;//���� ���� ������, �� ���������� � ����� ����� (������ 0) ����� ������ ���������
                    }
                    if (dx < 0)
                    {
                        x = j * 32 + 32;//���������� ���� �����
                    }
                }

                // �����
                if (TileMap[i][j] == 'f') { 
                    PlayerScore++;
                    TileMap[i][j] = ' ';
                    randomGenerateScores();
                }

                // ����
                if (TileMap[i][j] == 's') { 
                    health -= 50; 
                    TileMap[i][j] = 'b';
                }

                //�����
                if (TileMap[i][j] == 'h') { health += 15; TileMap[i][j] = ' '; }
            }
    }
};

int main()
{
    setlocale(LC_ALL, "Russian");
    randomMapGenerate();




    RenderWindow window(VideoMode(1366, 768), "Dota 3", Style::Fullscreen);
    view.reset(FloatRect(0, 0, 640, 480));//������ ���� ������
    menu(window);//����� ����



    Music music;
    music.openFromFile("ost1.ogg");
    music.setVolume(30);
    music.play();


    //�����
    Image map_image;
    map_image.loadFromFile("images/map_texture2_3.png");
    Texture map;
    map.loadFromImage(map_image);
    Sprite s_map;
    s_map.setTexture(map);

    //�����
    Font font;

    //��������
    font.loadFromFile("barlow-semibold.ttf");
    Text text_health(" ", font, 20);
    text_health.setFillColor(Color::Black);
    text_health.setStyle(Text::Italic);

    //����
    Text text_score(" ", font, 20);
    text_score.setFillColor(Color::Black);
    text_score.setStyle(Text::Italic);

    //������
    Text text_die(" ", font, 50);
    text_die.setFillColor(Color::Black);
    text_die.setStyle(Text::Bold);

    //������
    Player hero("runinghero3.png", 50, 50, 30, 33);
    hero.sprite.setPosition(300, 74);

    Player pudge("pudge5.png", 300, 300, 75, 75);
    pudge.sprite.setPosition(300, 74);

    float CurrentFrame = 0; //������ ������� ����

    Clock clock;

    int createScoreTimer = 0;

    while (window.isOpen())
    {
        float time = clock.getElapsedTime().asMicroseconds(); // ���� ��������� �����
        clock.restart(); // ������������� �����
        time = time / 800; // �������� ����

        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
        }



        ///////////////////////////////////////////���������� ���������� � ���������////////////////////////////////////////////////////////////////////////
        if(hero.life == true)
        {
            if (Keyboard::isKeyPressed(Keyboard::A)) {
                hero.dir = 1; hero.speed = 0.15;//dir =1 - ����������� �����, speed =0.1 - �������� ��������. �������� - ����� �� ��� ����� �� �� ��� �� �������� � ����� �� ���������� ������ ���
                CurrentFrame += 0.005 * time;
                if (CurrentFrame > 3) CurrentFrame -= 3;
                hero.sprite.setTextureRect(IntRect(50 * int(CurrentFrame) + 30, 74, -30, 33)); //����� ������ p ������ player ������ ������, ����� �������� (��������� �������� �����)
                getplayercoordinateforview(hero.getplayercoordinateX(), hero.getplayercoordinateY());//�������� ���������� ������ � ������� ���������� �������
            }

            if (Keyboard::isKeyPressed(Keyboard::D)) {
                hero.dir = 0; hero.speed = 0.15;//����������� ������, �� ����
                CurrentFrame += 0.005 * time;
                if (CurrentFrame > 3) CurrentFrame -= 3;
                hero.sprite.setTextureRect(IntRect(IntRect(50 * int(CurrentFrame), 74, 30, 33)));  //����� ������ p ������ player ������ ������, ����� �������� (��������� �������� �����)
                getplayercoordinateforview(hero.getplayercoordinateX(), hero.getplayercoordinateY());//�������� ���������� ������ � ������� ���������� �������
            }

            if (Keyboard::isKeyPressed(Keyboard::W)) {
                hero.dir = 3; hero.speed = 0.15;//����������� ����, �� ����
                CurrentFrame += 0.005 * time;
                if (CurrentFrame > 3) CurrentFrame -= 3;
                hero.sprite.setTextureRect(IntRect(0, 0, 30, 33));  //����� ������ p ������ player ������ ������, ����� �������� (��������� �������� �����)
                getplayercoordinateforview(hero.getplayercoordinateX(), hero.getplayercoordinateY());//�������� ���������� ������ � ������� ���������� �������

            }

            if (Keyboard::isKeyPressed(Keyboard::S)) { //���� ������ ������� ������� ����� ��� ���� ����� �
                hero.dir = 2; hero.speed = 0.15;//����������� �����, �� ����
                CurrentFrame += 0.005 * time; //������ ��� ����������� �� "������". ���������� ������� �� ���� �������� ������������ ������� � ��������. ������� 0.005 ����� �������� �������� ��������
                if (CurrentFrame > 3) CurrentFrame -= 3; //���������� �� ������ � ������� �� ������ ������������. ���� ������ � �������� ����� - ������������ �����.
                hero.sprite.setTextureRect(IntRect(220, 73, 20, 40)); //���������� �� ����������� �. ���������� 96,96*2,96*3 � ����� 96
                getplayercoordinateforview(hero.getplayercoordinateX(), hero.getplayercoordinateY());//�������� ���������� ������ � ������� ���������� �������

            }
        }

       //������� ������ � ������� �����
        sf::Vector2i LocalPosition = Mouse::getPosition(window);//���������� ���� ������������ x � y


        if (LocalPosition.x < 3) { view.move(-0.2 * time, 0); } //���� � ����� ���� ������ -> �����
        if (LocalPosition.x > window.getSize().x - 3) { view.move(0.2 * time, 0); } //�����
        if (LocalPosition.y > window.getSize().y - 3) { view.move(0, 0.2*time); } //���
        if (LocalPosition.y < 3) { view.move(0, -0.2*time); }
        hero.update(time);//�������� ������ p ������ Player � ������� ������� sfml, ��������� ����� � �������� ��������� ������� update. ��������� ����� �������� ����� ���������


        //createScoreTimer += time;
        //if (createScoreTimer > 10000)
        //{
        //    randomGenerateScores();
        //    createScoreTimer = 0;
        //}
        hero.update(time);

        window.setView(view); //�������� ������

        //���������� �������� ��������� �������� � ������
        window.clear(Color(159,185,93));


        /////////////////////////////������ �����/////////////////////
        for (int i = 0; i < HEIGHT_MAP; i++)
            for (int j = 0; j < WIDTH_MAP; j++)
            {
                if (TileMap[i][j] == ' ')  s_map.setTextureRect(IntRect(0, 0, 32, 32)); //���� ��������� ������ ������, �� ������ 1� ���������
                if (TileMap[i][j] == 'r')  s_map.setTextureRect(IntRect(32, 0, 32, 32));//���� ��������� ������ s, �� ������ 2� ���������
                if ((TileMap[i][j] == '0')) s_map.setTextureRect(IntRect(96, 0, 32, 32));//���� ��������� ������ 0, �� ������ 3� ���������
                if ((TileMap[i][j] == '1')) s_map.setTextureRect(IntRect(64, 0, 32, 32));//���� ��������� ������ 0, �� ������ 3� ���������
                if ((TileMap[i][j] == 'f')) s_map.setTextureRect(IntRect(128, 0, 32, 32));
                if ((TileMap[i][j] == 's')) s_map.setTextureRect(IntRect(160, 0, 32, 32));
                if ((TileMap[i][j] == 'h')) s_map.setTextureRect(IntRect(192, 0, 32, 32));
                if ((TileMap[i][j] == 'b')) s_map.setTextureRect(IntRect(224, 0, 32, 32));
                s_map.setPosition(j * 32, i * 32);//�� ���� ����������� ����������, ��������� � �����. �� ���� ������ ������� �� ��� �������. ���� ������, �� ��� ����� ���������� � ����� �������� 32*32 � �� ������ ���� �������
                window.draw(s_map);//������ ���������� �� �����
            }

        //������� ��������
        ostringstream playerScoreString;
        playerScoreString << hero.PlayerScore;
        text_health.setString("Score: " + playerScoreString.str());
        text_health.setPosition(view.getCenter().x - 285, view.getCenter().y + 200);// ������� ������� ����� ������

        //���������� � ������� ����� � ����
        ostringstream playerHealthString;
        playerHealthString << hero.health;
        text_score.setString("Helth: " + playerHealthString.str());
        text_score.setPosition(view.getCenter().x - 185, view.getCenter().y + 200);
        

        if (hero.life == false)
        {
            text_die.setString("Wasted");
            text_die.setPosition(view.getCenter().x, view.getCenter().y);
            window.draw(text_die);
        }

        window.draw(hero.sprite);
        window.draw(text_health);
        window.draw(text_score);
        window.draw(pudge.sprite);
        window.display();
    }

    return 0;
}
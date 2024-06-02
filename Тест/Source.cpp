
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

class Player { // класс Игрока
private:
    float x, y = 0;
public:
    float w, h, dx, dy, speed = 0; //координаты игрока х и у, высота ширина, ускорение (по х и по у), сама скорость
    int dir = 0; //направление (direction) движения игрока
    int PlayerScore;
    int health;
    bool life;
    String File; //файл с расширением
    Image image;
    Texture texture;
    Sprite sprite;

    Player(String F, float X, float Y, float W, float H) { 
        health = 100; life = true;
        File = F;//имя файла+расширение
        w = W; h = H;//высота и ширина
        PlayerScore = 0;
        image.loadFromFile("images/" + File);//запихиваем в image наше изображение
        image.createMaskFromColor(Color(63, 63, 63));
        texture.loadFromImage(image);//закидываем наше изображение в текстуру
        sprite.setTexture(texture);//заливаем спрайт текстурой
        x = X; y = Y;
        sprite.setTextureRect(IntRect(0,0,w,h));  //Задаем спрайту один прямоугольник
    }



    void update(float time) //функция "оживления" объекта класса. update - обновление. принимает в себя время SFML , вследствие чего работает бесконечно, давая персонажу движение.
    {
        switch (dir)//реализуем поведение в зависимости от направления. (каждая цифра соответствует направлению)
        {
        case 0: dx = speed; dy = 0;   break;//по иксу задаем положительную скорость, по игреку зануляем. получаем, что персонаж идет только вправо
        case 1: dx = -speed; dy = 0;   break;//по иксу задаем отрицательную скорость, по игреку зануляем. получается, что персонаж идет только влево
        case 2: dx = 0; dy = speed;   break;//по иксу задаем нулевое значение, по игреку положительное. получается, что персонаж идет только вниз
        case 3: dx = 0; dy = -speed;   break;//по иксу задаем нулевое значение, по игреку отрицательное. получается, что персонаж идет только вверх
        }

        x += dx * time;//то движение из прошлого урока. наше ускорение на время получаем смещение координат и как следствие движение
        y += dy * time;//аналогично по игреку

        speed = 0;//зануляем скорость, чтобы персонаж остановился.
        sprite.setPosition(x, y); //выводим спрайт в позицию x y , посередине. бесконечно выводим в этой функции, иначе бы наш спрайт стоял на месте.
        interactionWithMap();//вызываем функцию, отвечающую за взаимодействие с картой

        if (health <= 0) { life = false; }
    }



    float getplayercoordinateX() {	//этим методом будем забирать координату Х	
        return x;
    }
    float getplayercoordinateY() {	//этим методом будем забирать координату Y 	
        return y;
    }


    void interactionWithMap()//ф-ция взаимодействия с картой
    {

        for (int i = y / 32; i < (y + h) / 32; i++)//проходимся по тайликам, контактирующим с игроком,, то есть по всем квадратикам размера 32*32, которые мы окрашивали в 9 уроке. про условия читайте ниже.
            for (int j = x / 32; j < (x + w) / 32; j++)//икс делим на 32, тем самым получаем левый квадратик, с которым персонаж соприкасается. (он ведь больше размера 32*32, поэтому может одновременно стоять на нескольких квадратах). А j<(x + w) / 32 - условие ограничения координат по иксу. то есть координата самого правого квадрата, который соприкасается с персонажем. таким образом идем в цикле слева направо по иксу, проходя по от левого квадрата (соприкасающегося с героем), до правого квадрата (соприкасающегося с героем)
            {
                if (TileMap[i][j] == '0' || TileMap[i][j] == '1')//если наш квадратик соответствует символу 0 (стена), то проверяем "направление скорости" персонажа:
                {
                    if (dy > 0)//если мы шли вниз,
                    {
                        y = i * 32 - h;//то стопорим координату игрек персонажа. сначала получаем координату нашего квадратика на карте(стены) и затем вычитаем из высоты спрайта персонажа.
                    }
                    if (dy < 0)
                    {
                        y = i * 32 + 32;//аналогично с ходьбой вверх. dy<0, значит мы идем вверх (вспоминаем координаты паинта)
                    }
                    if (dx > 0)
                    {
                        x = j * 32 - w;//если идем вправо, то координата Х равна стена (символ 0) минус ширина персонажа
                    }
                    if (dx < 0)
                    {
                        x = j * 32 + 32;//аналогично идем влево
                    }
                }

                // бонус
                if (TileMap[i][j] == 'f') { 
                    PlayerScore++;
                    TileMap[i][j] = ' ';
                    randomGenerateScores();
                }

                // шипы
                if (TileMap[i][j] == 's') { 
                    health -= 50; 
                    TileMap[i][j] = 'b';
                }

                //хилка
                if (TileMap[i][j] == 'h') { health += 15; TileMap[i][j] = ' '; }
            }
    }
};

int main()
{
    setlocale(LC_ALL, "Russian");
    randomMapGenerate();




    RenderWindow window(VideoMode(1366, 768), "Dota 3", Style::Fullscreen);
    view.reset(FloatRect(0, 0, 640, 480));//размер вида камеры
    menu(window);//вызов меню



    Music music;
    music.openFromFile("ost1.ogg");
    music.setVolume(30);
    music.play();


    //карта
    Image map_image;
    map_image.loadFromFile("images/map_texture2_3.png");
    Texture map;
    map.loadFromImage(map_image);
    Sprite s_map;
    s_map.setTexture(map);

    //текст
    Font font;

    //здоровье
    font.loadFromFile("barlow-semibold.ttf");
    Text text_health(" ", font, 20);
    text_health.setFillColor(Color::Black);
    text_health.setStyle(Text::Italic);

    //счет
    Text text_score(" ", font, 20);
    text_score.setFillColor(Color::Black);
    text_score.setStyle(Text::Italic);

    //смерть
    Text text_die(" ", font, 50);
    text_die.setFillColor(Color::Black);
    text_die.setStyle(Text::Bold);

    //игроки
    Player hero("runinghero3.png", 50, 50, 30, 33);
    hero.sprite.setPosition(300, 74);

    Player pudge("pudge5.png", 300, 300, 75, 75);
    pudge.sprite.setPosition(300, 74);

    float CurrentFrame = 0; //хранит текущий кадр

    Clock clock;

    int createScoreTimer = 0;

    while (window.isOpen())
    {
        float time = clock.getElapsedTime().asMicroseconds(); // даем прошедшее время
        clock.restart(); // Перезагружает время
        time = time / 800; // скорость игры

        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
        }



        ///////////////////////////////////////////Управление персонажем с анимацией////////////////////////////////////////////////////////////////////////
        if(hero.life == true)
        {
            if (Keyboard::isKeyPressed(Keyboard::A)) {
                hero.dir = 1; hero.speed = 0.15;//dir =1 - направление вверх, speed =0.1 - скорость движения. Заметьте - время мы уже здесь ни на что не умножаем и нигде не используем каждый раз
                CurrentFrame += 0.005 * time;
                if (CurrentFrame > 3) CurrentFrame -= 3;
                hero.sprite.setTextureRect(IntRect(50 * int(CurrentFrame) + 30, 74, -30, 33)); //через объект p класса player меняем спрайт, делая анимацию (используя оператор точку)
                getplayercoordinateforview(hero.getplayercoordinateX(), hero.getplayercoordinateY());//передаем координаты игрока в функцию управления камерой
            }

            if (Keyboard::isKeyPressed(Keyboard::D)) {
                hero.dir = 0; hero.speed = 0.15;//направление вправо, см выше
                CurrentFrame += 0.005 * time;
                if (CurrentFrame > 3) CurrentFrame -= 3;
                hero.sprite.setTextureRect(IntRect(IntRect(50 * int(CurrentFrame), 74, 30, 33)));  //через объект p класса player меняем спрайт, делая анимацию (используя оператор точку)
                getplayercoordinateforview(hero.getplayercoordinateX(), hero.getplayercoordinateY());//передаем координаты игрока в функцию управления камерой
            }

            if (Keyboard::isKeyPressed(Keyboard::W)) {
                hero.dir = 3; hero.speed = 0.15;//направление вниз, см выше
                CurrentFrame += 0.005 * time;
                if (CurrentFrame > 3) CurrentFrame -= 3;
                hero.sprite.setTextureRect(IntRect(0, 0, 30, 33));  //через объект p класса player меняем спрайт, делая анимацию (используя оператор точку)
                getplayercoordinateforview(hero.getplayercoordinateX(), hero.getplayercoordinateY());//передаем координаты игрока в функцию управления камерой

            }

            if (Keyboard::isKeyPressed(Keyboard::S)) { //если нажата клавиша стрелка влево или англ буква А
                hero.dir = 2; hero.speed = 0.15;//направление вверх, см выше
                CurrentFrame += 0.005 * time; //служит для прохождения по "кадрам". переменная доходит до трех суммируя произведение времени и скорости. изменив 0.005 можно изменить скорость анимации
                if (CurrentFrame > 3) CurrentFrame -= 3; //проходимся по кадрам с первого по третий включительно. если пришли к третьему кадру - откидываемся назад.
                hero.sprite.setTextureRect(IntRect(220, 73, 20, 40)); //проходимся по координатам Х. получается 96,96*2,96*3 и опять 96
                getplayercoordinateforview(hero.getplayercoordinateX(), hero.getplayercoordinateY());//передаем координаты игрока в функцию управления камерой

            }
        }

       //двигаем камеру с помощью мышки
        sf::Vector2i LocalPosition = Mouse::getPosition(window);//координаты мыши относительно x и y


        if (LocalPosition.x < 3) { view.move(-0.2 * time, 0); } //если в левом краю экрана -> влево
        if (LocalPosition.x > window.getSize().x - 3) { view.move(0.2 * time, 0); } //право
        if (LocalPosition.y > window.getSize().y - 3) { view.move(0, 0.2*time); } //низ
        if (LocalPosition.y < 3) { view.move(0, -0.2*time); }
        hero.update(time);//оживляем объект p класса Player с помощью времени sfml, передавая время в качестве параметра функции update. благодаря этому персонаж может двигаться


        //createScoreTimer += time;
        //if (createScoreTimer > 10000)
        //{
        //    randomGenerateScores();
        //    createScoreTimer = 0;
        //}
        hero.update(time);

        window.setView(view); //оживляем камеру

        //производим анимацию постоянно отчищаем и рисуем
        window.clear(Color(159,185,93));


        /////////////////////////////Рисуем карту/////////////////////
        for (int i = 0; i < HEIGHT_MAP; i++)
            for (int j = 0; j < WIDTH_MAP; j++)
            {
                if (TileMap[i][j] == ' ')  s_map.setTextureRect(IntRect(0, 0, 32, 32)); //если встретили символ пробел, то рисуем 1й квадратик
                if (TileMap[i][j] == 'r')  s_map.setTextureRect(IntRect(32, 0, 32, 32));//если встретили символ s, то рисуем 2й квадратик
                if ((TileMap[i][j] == '0')) s_map.setTextureRect(IntRect(96, 0, 32, 32));//если встретили символ 0, то рисуем 3й квадратик
                if ((TileMap[i][j] == '1')) s_map.setTextureRect(IntRect(64, 0, 32, 32));//если встретили символ 0, то рисуем 3й квадратик
                if ((TileMap[i][j] == 'f')) s_map.setTextureRect(IntRect(128, 0, 32, 32));
                if ((TileMap[i][j] == 's')) s_map.setTextureRect(IntRect(160, 0, 32, 32));
                if ((TileMap[i][j] == 'h')) s_map.setTextureRect(IntRect(192, 0, 32, 32));
                if ((TileMap[i][j] == 'b')) s_map.setTextureRect(IntRect(224, 0, 32, 32));
                s_map.setPosition(j * 32, i * 32);//по сути раскидывает квадратики, превращая в карту. то есть задает каждому из них позицию. если убрать, то вся карта нарисуется в одном квадрате 32*32 и мы увидим один квадрат
                window.draw(s_map);//рисуем квадратики на экран
            }

        //выводим здоровье
        ostringstream playerScoreString;
        playerScoreString << hero.PlayerScore;
        text_health.setString("Score: " + playerScoreString.str());
        text_health.setPosition(view.getCenter().x - 285, view.getCenter().y + 200);// позиция тнекста центр камеры

        //записываем и выводим текст и счет
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
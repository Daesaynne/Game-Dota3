
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <clocale>
#include <sstream>
#include <vector>
#include <windows.h>

#include "map.h"
#include "view.h"
#include "menu.h"

using namespace std;
using namespace sf;

bool restart = false;
bool tron = false;
bool win = false;


HWND gameWindow;





class Player { // класс Игрока
private:
    float x, y = 0;
public:
    float w, h, dx, dy, speed = 0; //координаты игрока х и у, высота ширина, ускорение (по х и по у), сама скорость
    float xx, yy;
    int dir = 0; //направление (direction) движения игрока
    int PlayerScore;
    float health;
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
        xx = X; yy = Y;
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

                //трон
                if (TileMap[i][j] == '+')
                {
                    tron = true;
                }
                if (TileMap[i][j] != '+')
                {
                    tron = false;
                }
            }
    }
};



class Enemy {
private:
    float x, y;
public:
    float w, h, dx = 0, dy = 0.0, speed = 0;
    float xx, yy;
    int dir = 0; //направление (direction) движения врага

    String File;
    Image image;
    Texture texture;
    Sprite sprite;


    Enemy(String F, float X, float Y, float W, float H)
    {
        File = F;
        w = W, h = H;
        image.loadFromFile("images/" + File);
        texture.loadFromImage(image);
        sprite.setTexture(texture);
        x = X; y = Y;
        xx = X; yy = Y;
        sprite.setTextureRect(IntRect(0, 0, w, h));
    }

    void update(float time)
    {
        checkCollisionWithMap(dx, dy); // обрабатываем столкновение по X
        if (dx == 0)
            dx=dx-0.1;
        x += dx * time;
        y += dy * time;
        sprite.setPosition(x, y); // задаем позицию спрайта в место его центра
    }

    void checkCollisionWithMap(float Dx, float Dy)
    {
        for (int i = y / 32; i < (y + h) / 32; i++)
        {
            for (int j = x / 32; j < (x + w) / 32; j++)
            {
                if (TileMap[i][j] == '0' || TileMap[i][j] == '1')
                {
                    if (dy > 0)
                    {
                        y = i * 32 - h;
                        dy = -dy;
                    }
                    if (dy < 0)
                    {
                        y = i * 32 + 32;
                        dy = -dy;
                    }
                    if (dx > 0)
                    {
                        
                        x = j * 32 - w;
                        dx = 0;
                        //sprite.scale(-1, 1);
                    }
                    if (dx < 0)
                    {
                        x = j * 32 + 32;
                        dx = -dx;

                    }
                }
            }
        }
    }

    float getEnemycoordinateX() {	//этим методом будем забирать координату Х	
        return x;
    }
    float getEnemycoordinateY() {	//этим методом будем забирать координату Y 	
        return y;
    }

    



    

};



void game_run()
{
    RenderWindow window(VideoMode(1366, 768), "Dota 3"/*, Style::Fullscreen*/);
    view.reset(FloatRect(0, 0, 640, 480));//размер вида камеры

    Texture button;
    button.loadFromFile("images/control_music.png");
    Sprite buttonSet(button);
    
    buttonSet.scale(0.25f, 0.25f);

    gameWindow = GetForegroundWindow(); // Получаем дескриптор активного окна

    Music music;
    music.openFromFile("ost1.ogg");
    music.setVolume(30);
    music.play();

    ////бонус звук
    //SoundBuffer bonusBufer;
    //bonusBufer.loadFromFile("bonus.ogg");
    //Sound bonus(bonusBufer);

    

    //карта
    Image map_image;
    map_image.loadFromFile("images/map_texture2_5.png");
    Texture map;
    map.loadFromImage(map_image);
    Sprite s_map;
    s_map.setTexture(map);

    //картинка победы
    Image win_image;
    win_image.loadFromFile("images//black_font1.png");
    Texture win_im;
    win_im.loadFromImage(win_image);
    Sprite win_sprite;
    win_sprite.setTexture(win_im);


    //текст
    Font font;

    //здоровье
    font.loadFromFile("barlow-semibold.ttf");
    Text text_health(" ", font, 20);
    text_health.setFillColor(Color::Black);
    text_health.setStyle(Text::Italic);
    
    //трон время
    Text text_timer(" ", font, 20);
    text_timer.setFillColor(Color::Red);
    text_timer.setStyle(Text::Bold);


    
    //счет
    Text text_score(" ", font, 20);
    text_score.setFillColor(Color::Black);
    text_score.setStyle(Text::Italic);

    //смерть
    Text text_die(" ", font, 50);
    text_die.setFillColor(Color::Black);
    text_die.setStyle(Text::Bold);

    //win
    Text text_win(" ", font, 50);
    text_win.setFillColor(Color::Red);
    text_win.setStyle(Text::Bold);

    Text text_win_1(" ", font, 25);
    text_win_1.setFillColor(Color::Red);
    text_win_1.setStyle(Text::Bold);

    //игроки
    Player hero("runinghero3.png", 50, 50, 30, 33);
    hero.sprite.setPosition(300, 74);

    //Враги
    Enemy pudge("pudge5.png", 500, 100, 72, 72);
    Enemy pudge_1("pudge5.png", 500, 300, 72, 72);


    float CurrentFrame = 0; //хранит текущий кадр

    Clock clock;

    Clock gameTimeClock;//переменная игрового времени, будем здесь хранить время игры 
    int gameTime = 0;//объявили игровое время, инициализировали.

    int createScoreTimer = 0;



    while (window.isOpen() && restart == false)
    {
        
        float time = clock.getElapsedTime().asMicroseconds(); // даем прошедшее время

        if (tron == false)
        {
            gameTimeClock.restart();
        }

        else if (tron == true)
        {
            gameTime = gameTimeClock.getElapsedTime().asSeconds();

            if (gameTime == 20 && hero.PlayerScore == 10)
            {
                win = true;
                gameTimeClock.restart();
            }
        }
        
        
        clock.restart(); // Перезагружает время
        time = time / 800; // скорость игры

        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
        }

        //закрывашка на esc
        if (Keyboard::isKeyPressed(Keyboard::Escape)) {
            window.close();
        }

        ///////////////////////////////////////////Управление персонажем с анимацией////////////////////////////////////////////////////////////////////////
        if (hero.life == true && win == false)
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
        if (LocalPosition.y > window.getSize().y - 3) { view.move(0, 0.2 * time); } //низ
        if (LocalPosition.y < 3) { view.move(0, -0.2 * time); }
        hero.update(time);//оживляем объект p класса Player с помощью времени sfml, передавая время в качестве параметра функции update. благодаря этому персонаж может двигаться

        pudge.update(time);
        pudge_1.update(time);


       

        if (abs(hero.getplayercoordinateX() - pudge.getEnemycoordinateX()) < hero.w && abs(hero.getplayercoordinateY() - pudge.getEnemycoordinateY()) < hero.h) {
            hero.health = hero.health - 0.08;
        }

        if (abs(hero.getplayercoordinateX() - pudge_1.getEnemycoordinateX()) < hero.w && abs(hero.getplayercoordinateY() - pudge_1.getEnemycoordinateY()) < hero.h) {
            hero.health = hero.health - 0.08;
        }

        

        

        window.setView(view); //оживляем камеру

        //производим анимацию постоянно отчищаем и рисуем
        window.clear(Color(159, 185, 93));


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
                if ((TileMap[i][j] == 'd')) s_map.setTextureRect(IntRect(256, 0, 32, 32));
                if ((TileMap[i][j] == '+')) s_map.setTextureRect(IntRect(288, 0, 32, 32));
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

        ostringstream playerOnTron;
        playerOnTron << gameTime;
        text_timer.setString("Time on tron: " + playerOnTron.str());
        text_timer.setPosition(view.getCenter().x + 160, view.getCenter().y + 200);

        
        

        if (hero.life == false)
        {

            text_die.setString("Wasted");
            text_die.setPosition(view.getCenter().x, view.getCenter().y);
            window.draw(text_die);

            Text text_die_info(" ", font, 20);
            text_die_info.setFillColor(Color::Black);
            text_die_info.setStyle(Text::Bold);
            text_die_info.setString("1)To resurrect, press TAB");
            text_die_info.setPosition(view.getCenter().x - 200, view.getCenter().y - 200);

            Text text_die_info1(" ", font, 20);
            text_die_info1.setFillColor(Color::Black);
            text_die_info1.setStyle(Text::Bold);
            text_die_info1.setString("2)To turn off the game, press ESC");
            text_die_info1.setPosition(view.getCenter().x - 200, view.getCenter().y - 150);
            window.draw(text_die_info);
            window.draw(text_die_info1);

            
            window.draw(text_die);
            if (Keyboard::isKeyPressed(Keyboard::Tab))
            {
                window.clear();
                window.close();
                restart = true;
            }
        }

        int menuNum;

        

        buttonSet.setPosition(view.getCenter().x - 50, view.getCenter().y + 190);
        if (IntRect(600, 700, 100, 100).contains(Mouse::getPosition(window))) {
            {
                buttonSet.setColor(Color::Yellow); menuNum = 1;
            }
        }
        else
        {
            menuNum = 0;
        }

        Text chat(" ", font, 10);
        chat.setFillColor(Color::Black);
        chat.setStyle(Text::Italic);
        chat.setPosition(view.getCenter().x, view.getCenter().y + 100);

        Text chat_1(" ", font, 10);
        chat_1.setFillColor(Color::Black);
        chat_1.setStyle(Text::Italic);
        chat_1.setPosition(view.getCenter().x, view.getCenter().y + 115);

        Text chat_2(" ", font, 10);
        chat_2.setFillColor(Color::Black);
        chat_2.setStyle(Text::Italic);
        chat_2.setPosition(view.getCenter().x, view.getCenter().y + 130);

        Text chat_3(" ", font, 10);
        chat_3.setFillColor(Color::Black);
        chat_3.setStyle(Text::Italic);
        chat_3.setPosition(view.getCenter().x, view.getCenter().y + 145);

        bool mousePressed = false; // флаг для отслеживания состояния нажатия кнопки мыши
        int lastRandomNumber = 0; // переменная для хранения последнего сгенерированного числа
        int generate = 0;

        if (Mouse::isButtonPressed(Mouse::Left) && !mousePressed)
        {
            mousePressed = true; // установка флага нажатия кнопки

            if (menuNum == 1) {
                
                lastRandomNumber = rand() % 10 + 1; // генерация случайного числа

                chat.setString("-First Player: I **** your mother!");
                window.draw(chat);

                chat_1.setString("-Second Player: You son of a *****, shut up!");
                window.draw(chat_1);

                chat_2.setString("-First Player: **** you forest monster");
                window.draw(chat_2);

                chat_3.setString("-Second Player: You just ****** creep");
                window.draw(chat_3);

            }
            
        }
        else if (!Mouse::isButtonPressed(Mouse::Left))
        {
            mousePressed = false; // сброс флага нажатия кнопки при отпускании кнопки
            generate++;
        }

        
        window.draw(buttonSet);
        window.draw(hero.sprite);
        window.draw(pudge.sprite);
        window.draw(pudge_1.sprite);
        window.draw(text_health);
        window.draw(text_score);

        if (win == false)
        {
            window.draw(text_timer);
        }

        if (win == true)
        {
            
            window.draw(win_sprite);

            text_win.setString("YOU WIN");
            text_win.setPosition(view.getCenter().x, view.getCenter().y);
            window.draw(text_win);

            text_win_1.setString("To exit the game, press ESC");
            text_win_1.setPosition(view.getCenter().x-100, view.getCenter().y-100);
            window.draw(text_win_1);
            

        }

        window.display();


    }
}

void menu(/*RenderWindow& window*/) {
    RenderWindow window(VideoMode(1366, 768), "menu"/*, Style::Fullscreen*/);


    Texture menuTexture1, menuTexture2, menuTexture3, menuTexture4, menuTexture5, aboutTexture, menuBackground, menuBackground_r;
    Texture music_on_off, music_control, menu_info;
    menuTexture1.loadFromFile("images/dota_3.png");
    menuTexture2.loadFromFile("images/start_game.png");
    menuTexture3.loadFromFile("images/settings.png");
    menuTexture4.loadFromFile("images/exit.png");
    menuTexture5.loadFromFile("images/about.png");
    menuBackground.loadFromFile("images/menu_pudge.jpeg");
    menuBackground_r.loadFromFile("images/menu_pudge1.png");
    music_on_off.loadFromFile("images/music_on_off.png");
    music_control.loadFromFile("images/control_music.png");
    menu_info.loadFromFile("images/menu_info1.png");
    Sprite menu1(menuTexture1), menu2(menuTexture2), menu3(menuTexture3), menu4(menuTexture4), menu5(menuTexture5), menuBg(menuBackground), menuBg_r(menuBackground_r);
    Sprite set1(music_on_off), set2(music_control), menu_inf(menu_info);
    int menuNum = 0, control = 1;
    menu1.setPosition(105, 250);
    menu2.setPosition(80, 450);
    menu4.setPosition(80, 625);
    menu5.setPosition(900, 650);

    menuBg.setPosition(0, 0);

    int checker_music = 0;

    Music music;
    music.openFromFile("ost2.ogg");
    music.setVolume(50);
    music.play();

    ////////////////////////////МЕНЮ///////////////////
    while (window.isOpen())
    {

        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
        }

        menu2.setColor(Color::White);
        menu4.setColor(Color::White);
        menu5.setColor(Color::White);
        menuNum = 0;


        if (IntRect(100, 470, 420, 100).contains(Mouse::getPosition(window))) { menu2.setColor(Color::Yellow); menuNum = 1; }
        if (IntRect(100, 650, 200, 100).contains(Mouse::getPosition(window))) { menu4.setColor(Color::Yellow); menuNum = 3; }
        if (IntRect(900, 650, 450, 60).contains(Mouse::getPosition(window))) { menu5.setColor(Color::Red); menuNum = 4; }


        if (Mouse::isButtonPressed(Mouse::Left))
        {
            //начать игру
            if (menuNum == 1) {
                music.pause();
                window.close();
                game_run();
                //если нажали первую кнопку, то выходим из меню 

            }

            //выход
            if (menuNum == 3) {
                break;
            }

            //информация
            if (menuNum == 4) {
                menu_inf.setPosition(-100, 0);
                window.draw(menu_inf);
                window.display();
                while (!Keyboard::isKeyPressed(Keyboard::Escape));
            }

        }

        window.draw(menuBg);
        window.draw(menu1);
        window.draw(menu2);
        window.draw(menu4);
        window.draw(menu5);
        window.display();
    }
    ////////////////////////////////////////////////////

}



int main()
{
    setlocale(LC_ALL, "Russian");
    randomMapGenerate();
    
    menu();//вызов меню
    while (restart)
    {
        restart = false;
        game_run();
    }

    return 0;
}
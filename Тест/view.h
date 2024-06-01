#include <SFML/Graphics.hpp>
using namespace sf;

sf::View view;//объ€вили sfml объект "вид", который и €вл€етс€ камерой

View getplayercoordinateforview(float x, float y) { //функци€ дл€ считывани€ координат игрока

	float tempX = x; float tempY = y;
	if (x < 320) tempX = 320; // убираю из вида камеру слева
	if (y < 240) tempY = 240; // убираю из вида камеру сверхуй
	if (y > 554) tempY = 554; // убираю из вида камеру слева

	view.setCenter(tempX, tempY); //следим за игроком, передава€ его координаты камере. +100 - сместили камеру по иксу вправо. эксперементируйте

	return view; // обновл€ем камеру
}



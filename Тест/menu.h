void menu(RenderWindow& window) {
	Texture menuTexture1, menuTexture2, menuTexture3, menuTexture4, menuTexture5, aboutTexture, menuBackground, menuBackground_r;
	Texture music_on_off, music_control;
	menuTexture1.loadFromFile("images/dota_3.png");
	menuTexture2.loadFromFile("images/start_game.png");
	menuTexture3.loadFromFile("images/settings.png");
	menuTexture4.loadFromFile("images/exit.png");
	menuTexture5.loadFromFile("images/about.png");
	menuBackground.loadFromFile("images/menu_pudge.jpeg");
	menuBackground_r.loadFromFile("images/menu_pudge1.png");
	music_on_off.loadFromFile("images/music_on_off.png");
	music_control.loadFromFile("images/control_music.png");
	Sprite menu1(menuTexture1), menu2(menuTexture2), menu3(menuTexture3), menu4(menuTexture4), menu5(menuTexture5), menuBg(menuBackground), menuBg_r(menuBackground_r);
	Sprite set1(music_on_off), set2(music_control);
	bool isMenu = 1;
	int menuNum = 0, control = 1;
	menu1.setPosition(105, 100);
	menu2.setPosition(80, 300);
	menu3.setPosition(80, 450);
	menu4.setPosition(80, 625);
	menu5.setPosition(900, 650);

	menuBg.setPosition(0, 0);
	

	Music music;
	music.openFromFile("ost1.ogg");
	music.setVolume(30);
	music.play();
	
	////////////////////////////МЕНЮ///////////////////
	while (isMenu)
	{



		menu2.setColor(Color::White);
		menu3.setColor(Color::White);
		menu4.setColor(Color::White);
		menu5.setColor(Color::White);
		menuNum = 0;


		if (IntRect(100, 300, 420, 100).contains(Mouse::getPosition(window))) { menu2.setColor(Color::Yellow); menuNum = 1; }
		if (IntRect(100, 470, 370, 100).contains(Mouse::getPosition(window))) { menu3.setColor(Color::Yellow); menuNum = 2; }
		if (IntRect(100, 650, 200, 100).contains(Mouse::getPosition(window))) { menu4.setColor(Color::Yellow); menuNum = 3; }
		if (IntRect(900, 650, 450, 60).contains(Mouse::getPosition(window))) { menu5.setColor(Color::Red); menuNum = 4; }


		if (Mouse::isButtonPressed(Mouse::Left))
		{
			if (menuNum == 1) {
				music.pause();
				isMenu = false;//если нажали первую кнопку, то выходим из меню 

			}
			if (menuNum == 2) { 
				menuBg_r.setPosition(0, 0);
				menu2.setColor(Color::White);
				set1.setPosition(300, 300);
				set2.setPosition(600, 400);

				window.draw(menuBg_r); 
				window.draw(set1);
				window.draw(set2);



				window.display(); 
				while (!Keyboard::isKeyPressed(Keyboard::Escape));
			}
			if (menuNum == 3) { window.close(); isMenu = false; }
			//if (menuNum == 4) { window.draw(about); window.display(); while (!Keyboard::isKeyPressed(Keyboard::Escape)); }

		}

		window.draw(menuBg);
		window.draw(menu1);
		window.draw(menu2);
		window.draw(menu3);
		window.draw(menu4);
		window.draw(menu5);
		window.display();
	}
	////////////////////////////////////////////////////
}
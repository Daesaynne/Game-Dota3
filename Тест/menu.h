//void menu(/*RenderWindow& window*/) {
//	RenderWindow window(VideoMode(1366, 768), "menu"/*, Style::Fullscreen*/);
//
//
//	Texture menuTexture1, menuTexture2, menuTexture3, menuTexture4, menuTexture5, aboutTexture, menuBackground, menuBackground_r;
//	Texture music_on_off, music_control, menu_info;
//	menuTexture1.loadFromFile("images/dota_3.png");
//	menuTexture2.loadFromFile("images/start_game.png");
//	menuTexture3.loadFromFile("images/settings.png");
//	menuTexture4.loadFromFile("images/exit.png");
//	menuTexture5.loadFromFile("images/about.png");
//	menuBackground.loadFromFile("images/menu_pudge.jpeg");
//	menuBackground_r.loadFromFile("images/menu_pudge1.png");
//	music_on_off.loadFromFile("images/music_on_off.png");
//	music_control.loadFromFile("images/control_music.png");
//	menu_info.loadFromFile("images/menu_info1.png");
//	Sprite menu1(menuTexture1), menu2(menuTexture2), menu3(menuTexture3), menu4(menuTexture4), menu5(menuTexture5), menuBg(menuBackground), menuBg_r(menuBackground_r);
//	Sprite set1(music_on_off), set2(music_control), menu_inf(menu_info);
//	int menuNum = 0, control = 1;
//	menu1.setPosition(105, 250);
//	menu2.setPosition(80, 450);
//	menu4.setPosition(80, 625);
//	menu5.setPosition(900, 650);
//
//	menuBg.setPosition(0, 0);
//	
//	int checker_music = 0;
//
//	Music music;
//	music.openFromFile("ost2.ogg");
//	music.setVolume(50);
//	music.play();
//	
//	////////////////////////////МЕНЮ///////////////////
//	while (window.isOpen())
//	{
//
//		Event event;
//		while (window.pollEvent(event))
//		{
//			if (event.type == Event::Closed)
//				window.close();
//		}
//
//		menu2.setColor(Color::White);
//		menu4.setColor(Color::White);
//		menu5.setColor(Color::White);
//		menuNum = 0;
//
//
//		if (IntRect(100, 470, 420, 100).contains(Mouse::getPosition(window))) { menu2.setColor(Color::Yellow); menuNum = 1; }
//		if (IntRect(100, 650, 200, 100).contains(Mouse::getPosition(window))) { menu4.setColor(Color::Yellow); menuNum = 3; }
//		if (IntRect(900, 650, 450, 60).contains(Mouse::getPosition(window))) { menu5.setColor(Color::Red); menuNum = 4; }
//
//
//		if (Mouse::isButtonPressed(Mouse::Left))
//		{
//			//начать игру
//			if (menuNum == 1) {
//				music.pause();
//				window.close();
//				game_run();
//				//если нажали первую кнопку, то выходим из меню 
//
//			}
//
//			//выход
//			if (menuNum == 3) { 
//				break;
//			}
//
//			//информация
//			if (menuNum == 4) { 
//				menu_inf.setPosition(-100, 0);
//				window.draw(menu_inf); 
//				window.display(); 
//				while (!Keyboard::isKeyPressed(Keyboard::Escape)); 
//			}
//
//		}
//
//		window.draw(menuBg);
//		window.draw(menu1);
//		window.draw(menu2);
//		window.draw(menu4);
//		window.draw(menu5);
//		window.display();
//	}
//	////////////////////////////////////////////////////
//
//}

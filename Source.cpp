#include "Graphics.hpp"
#include <string>
#include <ctime>
#include <fstream> // работа с файлами

void Menu(sf::RenderWindow& window, sf::Font font, bool* isPlay, bool* isSettings, bool* typeOfGame); // выводится меню
void Tick(int* dir, int* sizeOfSnake); // змейка двигается
void Settings(sf::RenderWindow& window, sf::Font font, bool* isPlay, bool* isSettings, bool* typeOfGame);

struct Snake
{
	int x;
	int y;
} gridS[250]; // тут будет храниться 50 точек, 50 координат, 49 тел и 1 голова

struct Fruct
{
	int x;
	int y;
} gridF[250]; // тут будут храниться координаты фруктов

int main()
{
	srand(time(NULL));
	int weightWindow = 800;
	int heightWindow = 895;
	int gridX = 25;
	int gridY = 25;
	int size = 32; // размер одной клетки

	sf::RenderWindow window(sf::VideoMode(weightWindow, heightWindow), "Snake", sf::Style::Close);
										// создаем окно 800х800, называем "Змейка" и 
																	// закрепляем его( не даем изменять размер окна)

	bool isPlay = false; // Проверка игра запущена или нет
	int dir; // узнаем куда змейке двигаться
	unsigned int score = 0;
	unsigned int highscore;
	// ------ Прямоугольничек сверху для счета ----------
	sf::RectangleShape header(sf::Vector2f(weightWindow, 97)); //тут будет счет, время в игре
	header.setFillColor(sf::Color(83, 0, 144));
	// ------------ Шрифт -----------------
	sf::Font font;//шрифт 
	font.loadFromFile("natural_mono.ttf");//передаем нашему шрифту файл шрифта	
	// -------------- Задний фон клетка ------------
	sf::RectangleShape bg(sf::Vector2f(size, size));
	sf::Texture bgT;
	bgT.loadFromFile("sprites/white.png");
	bg.setTexture(&bgT);
	// -------------- Змейка клетка ------------
	sf::RectangleShape snake(sf::Vector2f(size, size));
	sf::Texture snakeT;
	snakeT.loadFromFile("sprites/green.png");
	snake.setTexture(&snakeT);
	int sizeOfSnake = 1;
	// -------------- Яблочко клетка ------------
	sf::RectangleShape apple(sf::Vector2f(size, size));
	sf::Texture appleT;
	appleT.loadFromFile("sprites/red.png");
	apple.setTexture(&appleT);

	sf::Clock clock;
	float timer = 0;
	float delay = 0.1;

	(*(gridS)).x = 11;
	(*(gridS)).y = 15;

	while ((*(gridF)).x != (*(gridS)).x && (*(gridF)).y != (*(gridS)).y) //будем искать место появление фрукта пока не найдем подходящее
	{
		(*(gridF)).x = rand() % 25;
		(*(gridF)).y = rand() % 25 + 3;
	}

	bool left_right = false; //если она тру, значит мы не можем кликать влево-вправо
	bool up_down = false;// если она тру, то мы не можем нажимать кнопки вверх-вниз
	bool isSettings = false; // если она тру, то функция Settings вызывается постоянно(из функции Menu) 
	bool typeOfGame = false; // если typeOfGame = false то змейка при столкновении с границами игры умирает,
							 // а если typeOfGame = true, то змейка проходит границы и телепортируется в другую часть карты
	
	std::string filePath = "highscore.bin"; // Путь к файлу с записями о лучшем результате игры
	std::ifstream in(filePath, std::ios::binary || std::ios::app); // Ставим режим "бинарный файл" и перезаписывание в файл

	if (in.peek() != 0)
	{
		in >> highscore;
	}
	in.close();
	std::ofstream out(filePath, std::ios::binary || std::ios::app); // Ставим что записываем в бинарный файл и каждый раз очищаем предыдущие записи

	while (window.isOpen())
	{
		Menu(window, font, &isPlay, &isSettings, &typeOfGame);
		float time = clock.getElapsedTime().asSeconds();
		clock.restart();
		timer += time;
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
		}
		
		// ---------------- draw -------------------
		if (isPlay == true)
		{
			// ---------------- Score Text -----------------
			sf::Text scoreT("", font, 50);
			std::string scoreString = std::to_string(score);
			scoreT.setString("Score: " + scoreString);
			scoreT.setFillColor(sf::Color::White);
			scoreT.setPosition(sf::Vector2f(10, 10));

			// ---------------- Highscore Text ---------------
			sf::Text highscoreT("", font, 50);
			std::string highscoreString = std::to_string(highscore); // переводим int highscore в строку
			highscoreT.setString("Highscore: " + highscoreString);
			highscoreT.setFillColor(sf::Color::White);
			highscoreT.setPosition(sf::Vector2f(window.getSize().x - highscoreT.getGlobalBounds().width - 10, 10));

			for (int i = 0;i < gridX;i++)
			{
				for (int j = 3;j < gridY + 3;j++)
				{
					bg.setPosition(i * size, j * size);
					window.draw(bg); // рисуем задний фон( сетку)
				}
			}
			window.draw(header);
			window.draw(scoreT);
			window.draw(highscoreT);
			apple.setPosition((*(gridF)).x * size, (*(gridF)).y * size); //первый фрукт спавнится в рандомной месте на карте
			window.draw(apple); // выводим на экран фрукт

			// -------------------- Движения змейки -----------------
			if (left_right == false && sf::Keyboard::isKeyPressed(sf::Keyboard::A))
			{
				dir = 0;
				left_right = true;
				up_down = false;
			}
			if (left_right == false && sf::Keyboard::isKeyPressed(sf::Keyboard::D))
			{
				dir = 1;
				left_right = true;
				up_down = false;
			}
			if (up_down == false && sf::Keyboard::isKeyPressed(sf::Keyboard::W))
			{
				dir = 2;
				left_right = false;
				up_down = true;
			}
			if (up_down == false && sf::Keyboard::isKeyPressed(sf::Keyboard::S))
			{
				dir = 3;
				left_right = false;
				up_down = true;
			}
				
			// Змейка проходит сквозь стены и телепортируется на другую часть карты
			if (typeOfGame)
			{
				// Если змейка врезается в верхнюю границу, то она спавнится в той же клетке( по Ох) с нижней границы
				if ((*gridS).y < 3)
				{
					(*gridS).y = 27;
					snake.setPosition((*gridS).x, (*gridS).y);
				}
				// Если змейка врезается в нижнюю границу, то она спавнится в той же клетке( по Ох) с верхней границы
				if ((*gridS).y > 27)
				{
					(*gridS).y = 3;
					snake.setPosition((*gridS).x, (*gridS).y);
				}
				// Если змейка врезается в правую границу - спавнится в той же клетке( по Оу) с левой границы
				if ((*gridS).x > 24)
				{
					(*gridS).x = 0;
					snake.setPosition((*gridS).x, (*gridS).y);
				}
				// Если змейка врезается в левую границу - спавнится в той же клетке( по Оу) с правой границы
				if ((*gridS).x < 0)
				{
					(*gridS).x = 24;
					snake.setPosition((*gridS).x, (*gridS).y);
				}
			}
			else //иначе при столкновении с границами карты змейка умирает
			{
				if ((*gridS).y < 3 || (*gridS).y > 27 || (*gridS).x > 24 || (*gridS).x < 0)
				{
					sizeOfSnake = 1;
					score = 0;
					(*(gridS)).x = 11;
					(*(gridS)).y = 15;
					isPlay = false;
				}
			}


			// Если голова змеи столкнулась с фруктом - убираем фрукт, спавним его в другом месте, увеличиваем змейку и 
																										// увеличиваем счет
			if ((*(gridS)).x == (*(gridF)).x && (*(gridS)).y == (*(gridF)).y)
			{
				(*(gridF)).x = rand() % 25;
				(*(gridF)).y = rand() % 25 + 3;
				
										// Проверяем нет ли на месте спавна змейки
				for (int i = 0;i < sizeOfSnake;i++)
				{
					// Сравнимваем рандомное место появления фрукта со змейкой и если есть совпадение -
																	// обнулсяем цикл, рандомим другие значения и сравниваем опять
					if((*(gridF)).x == (*(gridS + i)).x && (*(gridF)).y == (*(gridS + i)).y)
					{
						i = 0; // обнуляем начало цикла, чтобы сравнивать со всей змеей, а не только с оставшейся частью змейки
						(*(gridF)).x = rand() % 25;
						(*(gridF)).y = rand() % 25 + 3;
					}
				}
				apple.setPosition((*(gridF)).x* size, (*(gridF)).y* size); // спавним фрукт в другом рандомном месте
				sizeOfSnake++; //увеличиваем размер змейки
				score++;
			}

			// Проверка на столкновение головы змеи с телом
			for (int i = 1;i < sizeOfSnake;i++)
			{
				if ((*(gridS)).x == (*(gridS + i)).x && (*(gridS)).y == (*(gridS + i)).y)
				{
					
					sizeOfSnake = 1;
					score = 0;
					(*(gridS)).x = 11;
					(*(gridS)).y = 15;
					isPlay = false;
				}
			}

			// Постоянно перезаписываем highscore
			if (sizeOfSnake - 1 > highscore)
			{
				highscore = sizeOfSnake - 1;
			}

			// шаг змеи
			if (timer > delay)
			{
				timer = 0;
				Tick(&dir, &sizeOfSnake);
			}
			for (int i = 0;i < sizeOfSnake;i++)
			{
				snake.setPosition((*(gridS +i)).x * size, (*(gridS + i)).y * size);
				window.draw(snake);
			}
			
		}

		window.display(); // выводим на экран окно
	}
	out << highscore;
	out.close();//закрываем файлы
	return 0;
}

void Tick(int* dir, int* sizeOfSnake)
{
	for (int i = *sizeOfSnake;i > 0;i--)
	{
		(*(gridS + i)).x = (*(gridS + i - 1)).x;
		(*(gridS + i)).y = (*(gridS + i - 1)).y;
	}

	if (*dir == 0) // Если змейка двигается влево
	{
		(*gridS).x -= 1;
	}
	if (*dir == 1) // Если змейка двигается вправо
	{
		(*gridS).x += 1;
	}
	if (*dir == 2) // Если змейка двигается вверх
	{
		(*gridS).y -= 1;
	}
	if (*dir == 3) // Если змейка двигается вниз
	{
		(*gridS).y += 1;
	}
}

void Menu(sf::RenderWindow& window, sf::Font font, bool* isPlay, bool* isSettings, bool* typeOfGame)
{
	window.clear(sf::Color(83, 0, 144)); // заливаем окно сплошным цветом
	sf::Vector2f centerPos = sf::Vector2f(window.getSize().x / 2, window.getSize().y / 2); // находим центр экрана
	// ------------ Рисуем кнопку Play ----------------
	sf::RectangleShape buttonPlay(sf::Vector2f(0,0));
	buttonPlay.setSize(sf::Vector2f(150, 50)); // кнопка размером 150х50
	buttonPlay.setPosition(centerPos.x - buttonPlay.getGlobalBounds().width / 2, centerPos.y - 100 - buttonPlay.getGlobalBounds().height / 2);
														// рисуем кнопку по центру экрана( по оси Ох) и чуть выше центра по оси Оу
	buttonPlay.setFillColor(sf::Color::Magenta);
	buttonPlay.setOutlineThickness(3);
	buttonPlay.setOutlineColor(sf::Color::Black); // делаем черную обводку толщиной в 3 пикселя
	// ----------------- Текст Play ---------------
	sf::Text textPlay("Play", font, 30);
	textPlay.setPosition(sf::Vector2f(centerPos.x - textPlay.getGlobalBounds().width / 2, centerPos.y - 105 - textPlay.getGlobalBounds().height / 2));
	textPlay.setFillColor(sf::Color::White);

	// ------------ Рисуем кнопку Settings ----------------
	sf::RectangleShape buttonSettings(sf::Vector2f(0, 0));
	buttonSettings.setSize(sf::Vector2f(150, 50)); // кнопка размером 150х50
	buttonSettings.setPosition(centerPos.x - buttonSettings.getGlobalBounds().width / 2, centerPos.y - buttonSettings.getGlobalBounds().height / 2);
								// рисуем кнопку по центру экрана( по оси Ох) и чуть выше центра по оси Оу
	buttonSettings.setFillColor(sf::Color::Magenta);
	buttonSettings.setOutlineThickness(3);
	buttonSettings.setOutlineColor(sf::Color::Black); // делаем черную обводку толщиной в 3 пикселя

	// ----------------- Текст Settings ---------------
	sf::Text textSettings("Settings", font, 30);
	textSettings.setPosition(sf::Vector2f(centerPos.x - textSettings.getGlobalBounds().width / 2, centerPos.y  - textSettings.getGlobalBounds().height / 2));
	textSettings.setFillColor(sf::Color::White);

	// ----------------- Кнопка выхода из игры --------
	sf::RectangleShape exitButton(sf::Vector2f(0, 0));
	exitButton.setSize(sf::Vector2f(150, 50)); // кнопка размером 150х50
	exitButton.setPosition(centerPos.x - exitButton.getGlobalBounds().width / 2, centerPos.y + 100 - exitButton.getGlobalBounds().height / 2);
	exitButton.setFillColor(sf::Color::Magenta);
	exitButton.setOutlineThickness(3);
	exitButton.setOutlineColor(sf::Color::Black);

	// --------------- Текст нокпки выхода --------------
	sf::Text exitText("Exit", font, 30);
	exitText.setPosition(sf::Vector2f(centerPos.x - exitText.getGlobalBounds().width / 2, centerPos.y + 95 - exitText.getGlobalBounds().height / 2));
	exitText.setFillColor(sf::Color::White);

	// ----------- draw -------------
	window.draw(buttonPlay);
	window.draw(buttonSettings);
	window.draw(exitButton);
	window.draw(textPlay);
	window.draw(textSettings);
	window.draw(exitText);

	// ----------- Ждемс пока нажмется кнопка Play -----------------
	sf::Event buttonPress;
	while (window.pollEvent(buttonPress))
	{
		if (buttonPress.type == buttonPress.MouseButtonReleased && buttonPress.mouseButton.button == sf::Mouse::Left && (sf::Mouse::getPosition(window).x >= buttonPlay.getPosition().x && sf::Mouse::getPosition(window).x <= buttonPlay.getPosition().x + 150) && (sf::Mouse::getPosition(window).y >= buttonPlay.getPosition().y && sf::Mouse::getPosition(window).y <= buttonPlay.getPosition().y + 50))
		{
			*isPlay = true;
		}
		if (buttonPress.type == buttonPress.MouseButtonReleased && buttonPress.mouseButton.button == sf::Mouse::Left && (sf::Mouse::getPosition(window).x >= buttonSettings.getPosition().x && sf::Mouse::getPosition(window).x <= buttonSettings.getPosition().x + 150) && (sf::Mouse::getPosition(window).y >= buttonSettings.getPosition().y && sf::Mouse::getPosition(window).y <= buttonSettings.getPosition().y + 50))
		{
			*isSettings = true;
		}
		if (buttonPress.type == buttonPress.MouseButtonReleased && buttonPress.mouseButton.button == sf::Mouse::Left && (sf::Mouse::getPosition(window).x >= exitButton.getPosition().x && sf::Mouse::getPosition(window).x <= exitButton.getPosition().x + 150) && (sf::Mouse::getPosition(window).y >= exitButton.getPosition().y && sf::Mouse::getPosition(window).y <= exitButton.getPosition().y + 50))
		{
			window.close();
		}
	}
	if (*isSettings)
	{
		Settings(window, font, isPlay, isSettings, typeOfGame);
	}
}

void Settings(sf::RenderWindow& window, sf::Font font, bool* isPlay, bool* isSettings, bool* typeOfGame)
{
	window.clear(sf::Color(83, 0, 144)); // заливаем окно сплошным цветом

	// ---------- Кнопка какие границы выбрать( телепорты или убийственные)---------
	sf::Text textAboveButtons("Select the type of game:", font, 30);
	textAboveButtons.setPosition(sf::Vector2f(window.getSize().x / 2 - textAboveButtons.getGlobalBounds().width / 2, 50));
	textAboveButtons.setFillColor(sf::Color::Black);

	// ------------ Первая кнопка( умирает от стенки) -------------------
	sf::RectangleShape button1(sf::Vector2f(0, 0));
	button1.setSize(sf::Vector2f(300, 300)); // кнопка размером 300 x 300
	button1.setPosition(window.getSize().x / 3 - 50 - button1.getGlobalBounds().width / 2, window.getSize().y / 3 - 50);
	button1.setFillColor(sf::Color::Magenta);
	button1.setOutlineThickness(3);
	button1.setOutlineColor(sf::Color::Black); // делаем черную обводку толщиной в 3 пикселя

	// ----------- Текст в первой кнопке -------------------
	sf::Text text1("When colliding with the\n walls, the snake dies", font, 17);
	text1.setPosition(sf::Vector2f(window.getSize().x / 3 - 20 - button1.getGlobalBounds().width / 2, window.getSize().y / 3 + text1.getGlobalBounds().height + 30));
	text1.setFillColor(sf::Color::Black);
	text1.setStyle(sf::Text::Bold);

	// ------------ Вторая кнопка( телепортируется сквозь стенки) -------------------
	sf::RectangleShape button2(sf::Vector2f(0, 0));
	button2.setSize(sf::Vector2f(300, 300)); // кнопка размером 300 x 300
	button2.setPosition(2 *( window.getSize().x / 3) + 50 - button2.getGlobalBounds().width / 2, window.getSize().y / 3 - 50);
	button2.setFillColor(sf::Color::Magenta);
	button2.setOutlineThickness(3);
	button2.setOutlineColor(sf::Color::Black); // делаем черную обводку толщиной в 3 пикселя

	// ----------- Текст во второй кнопке -------------------
	sf::Text text2("When colliding with a\nwall, the snake moves\nto the opposite side\n   of the screen", font, 17);
	text2.setPosition(sf::Vector2f(2 * (window.getSize().x / 3) + 80 - button2.getGlobalBounds().width / 2, window.getSize().y / 3 + text2.getGlobalBounds().height - 20));
	text2.setFillColor(sf::Color::Black);
	text2.setStyle(sf::Text::Bold);

	// --------------- Выход обратно в меню -----------
	sf::RectangleShape buttonEscape(sf::Vector2f(0, 0));
	buttonEscape.setSize(sf::Vector2f(150, 75));
	buttonEscape.setPosition(2 * (window.getSize().x / 3) + 50 - button2.getGlobalBounds().width / 2, 2 * (window.getSize().y / 3) + 50);
	buttonEscape.setFillColor(sf::Color::Magenta);
	buttonEscape.setOutlineThickness(3);
	buttonEscape.setOutlineColor(sf::Color::Black);

	// --------------- Текст в кнопке віхода в меню -------------
	sf::Text textEscape("Return to the\n\t menu", font, 15);
	textEscape.setPosition(sf::Vector2f(2 * (window.getSize().x / 3) - button2.getGlobalBounds().width / 2 + textEscape.getGlobalBounds().width / 2, 2 * (window.getSize().y / 3) + 50 + 25));
	textEscape.setFillColor(sf::Color::Black);
	textEscape.setStyle(sf::Text::Bold);
	
	// ------------------ draw ------------------
	window.draw(textAboveButtons);
	window.draw(button1);
	window.draw(button2);
	window.draw(buttonEscape);
	window.draw(text1);
	window.draw(text2);
	window.draw(textEscape);

	sf::Event buttonPress;
	while (window.pollEvent(buttonPress))
	{
		if (buttonPress.type == buttonPress.MouseButtonReleased && buttonPress.mouseButton.button == sf::Mouse::Left && (sf::Mouse::getPosition(window).x >= button1.getPosition().x && sf::Mouse::getPosition(window).x <= button1.getPosition().x + 300) && (sf::Mouse::getPosition(window).y >= button1.getPosition().y && sf::Mouse::getPosition(window).y <= button1.getPosition().y + 300))
		{
			*typeOfGame = false;
		}
		if (buttonPress.type == buttonPress.MouseButtonReleased && buttonPress.mouseButton.button == sf::Mouse::Left && (sf::Mouse::getPosition(window).x >= button2.getPosition().x && sf::Mouse::getPosition(window).x <= button2.getPosition().x + 300) && (sf::Mouse::getPosition(window).y >= button2.getPosition().y && sf::Mouse::getPosition(window).y <= button2.getPosition().y + 300))
		{
			*typeOfGame = true;
		}
		if (buttonPress.type == buttonPress.MouseButtonReleased && buttonPress.mouseButton.button == sf::Mouse::Left &&  (sf::Mouse::getPosition(window).x >= buttonEscape.getPosition().x && sf::Mouse::getPosition(window).x <= buttonEscape.getPosition().x + 150) && (sf::Mouse::getPosition(window).y >= buttonEscape.getPosition().y && sf::Mouse::getPosition(window).y <= buttonEscape.getPosition().y + 75))
		{
			*isSettings = false;
			Menu(window, font, isPlay, isSettings, typeOfGame);
		}
	}
}
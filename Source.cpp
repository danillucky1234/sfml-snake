#include <SFML/Graphics.hpp> // Graphic library
#include <string> 
#include <ctime> // Library for srand
#include <fstream> // Work with files

void Menu(sf::RenderWindow& window, sf::Font font, bool* isPlay, bool* isSettings, bool* typeOfGame); // The menu is displayed
void Tick(int* dir, int* sizeOfSnake); // Snake movement
void Settings(sf::RenderWindow& window, sf::Font font, bool* isPlay, bool* isSettings, bool* typeOfGame);

struct Snake
{
	int x;
	int y;
} gridS[625]; // The snake can be with a maximum length of 250 cells 

struct Fruct
{
	int x;
	int y;
} gridF; 

int main()
{
	srand(time(NULL));
	int weightWindow = 800;
	int heightWindow = 895;
	int gridX = 25; // The amount of cells in window by Ox
	int gridY = 25; // The amount of cells in window by Oy
	int size = 32; // The size of cell

	sf::RenderWindow window(sf::VideoMode(weightWindow, heightWindow), "Snake", sf::Style::Close);
										// Create a window 800 x 800, the title "Snake" and 
																	// fix the window size

	bool isPlay = false; // Find out where we are: menu/settings or play game?
	int dir; // The direction of the snake
	unsigned int score = 0; // How much fruits were eaten
	unsigned int highscore; // The best score
	// ------ The rectangle on top( header) ----------
	sf::RectangleShape header(sf::Vector2f(weightWindow, 97)); 
	header.setFillColor(sf::Color(83, 0, 144)); // Fill rectangle
	// ------------ Font -----------------
	sf::Font font;
	font.loadFromFile("natural_mono.ttf"); // Load the font from the file
	// -------------- Background( cells) ------------
	sf::RectangleShape bg(sf::Vector2f(size, size)); // Cell
	sf::Texture bgT; // Cell texture
	bgT.loadFromFile("sprites/white.png"); // Load the cell texture from the file
	bg.setTexture(&bgT); // Set the texture to the cell
	// -------------- Snake ------------
	sf::RectangleShape snake(sf::Vector2f(size, size)); // Snake cell
	sf::Texture snakeT; // Snake texture
	snakeT.loadFromFile("sprites/green.png"); // Load the snake cell texture from the file
	snake.setTexture(&snakeT); // Set the texture to the snake cell
	int sizeOfSnake = 1; // The size of snake
	// -------------- Fruit cell ------------
	sf::RectangleShape apple(sf::Vector2f(size, size)); // Fruit cell
	sf::Texture appleT; // Fruit texture
	appleT.loadFromFile("sprites/red.png"); // Load the fruit cell texture from the file
	apple.setTexture(&appleT); // Set the texture to the fruit cell

	sf::Clock clock;
	float timer = 0;
	float delay = 0.1; // Delay between the transition of the snake between the cells

	(*(gridS)).x = 11; // Start position of snake by Ox
	(*(gridS)).y = 15; // Start position of snake by Oy

	// We will look for the place where the fruit appears until we find a suitable one
	while (gridF.x != (*(gridS)).x && gridF.y != (*(gridS)).y)
	{
		gridF.x = rand() % 25;
		gridF.y = rand() % 25 + 3;
	}

	bool left_right = false; // If it is true, then we cannot press the buttons left/right
	bool up_down = false; // If it is true, then we cannot press the buttons up/down
	bool isSettings = false; // If its true, then function Settings called constantly( from the function Menu) 
	bool typeOfGame = false; // If typeOfGame = false the snake dies when it collides with borders,
							 // but if typeOfGame = true, then the snake passes the borders and teleports to 
																							// another part of the map
	
	std::string filePath = "highscore.bin"; // The path to the file with the records of the best game result
	std::ifstream in(filePath, std::ios::binary || std::ios::app); // We set the "binary file" mode and rewrite to a file

	if (in.peek() != 0) // If this check works, then we calculate the best score from the file
	{
		in >> highscore;
	}
	in.close(); // Close the file 
	std::ofstream out(filePath, std::ios::binary || std::ios::app);

	while (window.isOpen())
	{
		Menu(window, font, &isPlay, &isSettings, &typeOfGame); // Call the menu function
												// The menu opens before the game
		float time = clock.getElapsedTime().asSeconds();
		clock.restart();
		timer += time;

		sf::Event event;
		while (window.pollEvent(event))
		{
			// We can close the snake window
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
		}
		
		
		// If we pressed the play button, then a check is performed and the gameplay starts
		if (isPlay)
		{
			// ---------------- Score Text -----------------
			sf::Text scoreT("", font, 50); 
			std::string scoreString = std::to_string(score);// Converting int score to string
			scoreT.setString("Score: " + scoreString); // We update the game score every time
			scoreT.setFillColor(sf::Color::White); // Setting the color for score text
			scoreT.setPosition(sf::Vector2f(10, 10)); // Set score text position

			// ---------------- Highscore Text ---------------
			sf::Text highscoreT("", font, 50);
			std::string highscoreString = std::to_string(highscore); // Converting int highscore to string
			highscoreT.setString("Highscore: " + highscoreString); // We update the highscore every time
			highscoreT.setFillColor(sf::Color::White); // Setting the color for highscore text
			highscoreT.setPosition(sf::Vector2f(window.getSize().x - highscoreT.getGlobalBounds().width - 10, 10));
																							// Set highscore text position
			for (int i = 0;i < gridX;i++)
			{
				for (int j = 3;j < gridY + 3;j++)
				{
					bg.setPosition(i * size, j * size); 
					window.draw(bg); // Draw the background cells
				}
			}
			apple.setPosition(gridF.x * size, gridF.y * size); // The first fruit spawns in a random place on the map
			
			window.draw(header); // Draw rectangle on top
			window.draw(scoreT);
			window.draw(highscoreT);
			window.draw(apple);
			
			// -------------------- The snake movement -----------------
			if (left_right == false && sf::Keyboard::isKeyPressed(sf::Keyboard::A))
				{
					dir = 0; // Set direction
					left_right = true; // If we pressed the A key, we can`t press left/right button
					up_down = false; // And we can press the up/down button 
				}
			if (left_right == false && sf::Keyboard::isKeyPressed(sf::Keyboard::D))
				{
					dir = 1; // Set direction
					left_right = true; // If we pressed the D key, we can`t press left/right button
					up_down = false; // And we can press the up/down button 
				}
			if (up_down == false && sf::Keyboard::isKeyPressed(sf::Keyboard::W))
				{
					dir = 2; // Set direction
					left_right = false; // We can press the left/right button 
					up_down = true; // We cannot press the up/down button
				}
			if (up_down == false && sf::Keyboard::isKeyPressed(sf::Keyboard::S))
				{
					dir = 3; // Set direction
					left_right = false; // We can press the left/right button 
					up_down = true; // We cannot press the up/down button
				}
			
			// If typeOfGame = false the snake dies when it collides with borders
			if (typeOfGame)
			{
				// If the snake crashes into the upper border, then it will spawn in the same cell (by Ox) from the lower border
				if ((*gridS).y < 3)
				{
					(*gridS).y = 27; // The snake's head moves to square 27 in Oy
					snake.setPosition((*gridS).x, (*gridS).y); // Set the snake head position 
				}
				//If the snake crashes into the lower border, then it will spawn in the same cell (by Ox) from the upper border
				if ((*gridS).y > 27)
				{
					(*gridS).y = 3; // The snake's head moves to square 3 in Oy
					snake.setPosition((*gridS).x, (*gridS).y); // Set the snake head position  
				}
				// If the snake crashes into the right border, it will spawn in the same cell (according to Oy) from the left border
				if ((*gridS).x > 24)
				{
					(*gridS).x = 0; // The snake's head moves to square 0 in Ox
					snake.setPosition((*gridS).x, (*gridS).y); // Set the snake head position  
				}
				// If the snake crashes into the left border, it will spawn in the same cell (according to Oy) from the right border
				if ((*gridS).x < 0)
				{
					(*gridS).x = 24;  // The snake's head moves to square 24 in Ox
					snake.setPosition((*gridS).x, (*gridS).y); // Set the snake head position  
				}
			}
			else // Otherwise, when colliding with the borders of the map, the snake dies
			{
				if ((*gridS).y < 3 || (*gridS).y > 27 || (*gridS).x > 24 || (*gridS).x < 0)
				{
					sizeOfSnake = 1; // Set size of snake to 1
					score = 0; // Set score to zero
					(*(gridS)).x = 11; // Set the start position to the snake in Ox
					(*(gridS)).y = 15; // Set the start position to the snake in Oy
					isPlay = false; // Go to the menu
				}
			}

			// If the snake's head collides with a fruit, remove the fruit, spawn it in another place,
																			// increase the snake and increase the score
			if ((*(gridS)).x == gridF.x && (*(gridS)).y == gridF.y)
			{
				gridF.x = rand() % 25; // Choosing a random fruit spawn location on the map in Ox
				gridF.y = rand() % 25 + 3; // Choosing a random fruit spawn location on the map in Oy
				
				// We check if there is a snake in place of the spawn
				for (int i = 0;i < sizeOfSnake;i++)
				{
					// Compare the random place where the fruit appears with the snake, and if there is a match,
												// we will turn the cycle around, randomize other values ​​and compare again
					if(gridF.x == (*(gridS + i)).x && gridF.y == (*(gridS + i)).y)
					{
						i = 0; // We reset the beginning of the cycle to compare with the whole snake, 
																		// and not just with the rest of the snake
						gridF.x = rand() % 25; // Choosing a random fruit spawn location on the map in Ox
						gridF.y = rand() % 25 + 3; // Choosing a random fruit spawn location on the map in Oy
					}
				}
				apple.setPosition(gridF.x* size, gridF.y* size); // Spawn the fruit in a different random location
				sizeOfSnake++; // Increasing the size of the snake
				score++; //Increasing the score
			}

			// Snake Head Collision Test
			for (int i = 1;i < sizeOfSnake;i++)
			{
				// If a collision occurs, the game ends, the snake dies and the score is reset
				if ((*(gridS)).x == (*(gridS + i)).x && (*(gridS)).y == (*(gridS + i)).y)
				{
					sizeOfSnake = 1; // Set size of snake to 1
					score = 0; // Set score to zero
					(*(gridS)).x = 11; // Set the start position to the snake in Ox
					(*(gridS)).y = 15; // Set the start position to the snake in Oy
					isPlay = false; // Go to the menu
				}
			}

			// We constantly overwrite highscore
			if (sizeOfSnake - 1 > highscore)
			{
				highscore = sizeOfSnake - 1;
			}

			// Each movement of the snake through the cells
			if (timer > delay)
			{
				timer = 0.0;
				Tick(&dir, &sizeOfSnake); // Snake movement
			}
			
			for (int i = 0;i < sizeOfSnake;i++)
			{
				snake.setPosition((*(gridS + i)).x * size, (*(gridS + i)).y * size); // Move each cell of the snake's 
																								// body to another cell
				window.draw(snake);
			}			
		}

		window.display(); // выводим на экран окно
	}
	out << highscore; // We write highscore to a file
	out.close(); // Close the file
	return 0;
}

void Tick(int* dir, int* sizeOfSnake)
{
	// From tail to head, repeat the movement of the head
	for (int i = *sizeOfSnake;i > 0;i--)
	{
		// Each cell follows the one in front of it
		(*(gridS + i)).x = (*(gridS + i - 1)).x;
		(*(gridS + i)).y = (*(gridS + i - 1)).y;
	}

	if (*dir == 0) // If the snake moves to the left, move the snake's head one square to the left
	{
		(*gridS).x -= 1;
	}
	if (*dir == 1) // If the snake moves to the right, move the snake's head one square to the right
	{
		(*gridS).x += 1;
	}
	if (*dir == 2) // If the snake moves to the up, move the snake's head one square to the up
	{
		(*gridS).y -= 1;
	}
	if (*dir == 3) // If the snake moves to the down, move the snake's head one square to the down
	{
		(*gridS).y += 1;
	}
}

void Menu(sf::RenderWindow& window, sf::Font font, bool* isPlay, bool* isSettings, bool* typeOfGame)
{
	window.clear(sf::Color(83, 0, 144)); // Fill the window with a solid color
	sf::Vector2f centerPos = sf::Vector2f(window.getSize().x / 2, window.getSize().y / 2); // Find the center of the window
	// ------------ Drawing the Play button ----------------
	sf::RectangleShape buttonPlay(sf::Vector2f(0,0));
	buttonPlay.setSize(sf::Vector2f(150, 50)); // Set size for Play button
	buttonPlay.setPosition(centerPos.x - buttonPlay.getGlobalBounds().width / 2, centerPos.y - 100 - buttonPlay.getGlobalBounds().height / 2);
												// Draw a button in the center of the screen (along the Ox axis) and slightly 
																		// above the center along the Oy axis
	buttonPlay.setFillColor(sf::Color::Magenta); // Setting the color to the Play button
	buttonPlay.setOutlineThickness(3);  // Setting the outline thickness to the Play button and make it 3px 
	buttonPlay.setOutlineColor(sf::Color::Black); // Setting the color to the outline thikness
	// ----------------- Text Play ---------------
	sf::Text textPlay("Play", font, 30);
	textPlay.setPosition(sf::Vector2f(centerPos.x - textPlay.getGlobalBounds().width / 2, centerPos.y - 105 - textPlay.getGlobalBounds().height / 2));
														// Set text Play position
	textPlay.setFillColor(sf::Color::White); // Setting the color to the text Play

	// ------------ Draw the Settings button ----------------
	sf::RectangleShape buttonSettings(sf::Vector2f(0, 0));
	buttonSettings.setSize(sf::Vector2f(150, 50)); // Set the size to the settings button
	buttonSettings.setPosition(centerPos.x - buttonSettings.getGlobalBounds().width / 2, centerPos.y - buttonSettings.getGlobalBounds().height / 2);
								// Draw a button in the center of the screen (along the Ox axis) and slightly above the center along the Oy axis
	buttonSettings.setFillColor(sf::Color::Magenta); // Setting the color to the settings button
	buttonSettings.setOutlineThickness(3); // Setting the outline thickness to the settings button and make it 3px 
	buttonSettings.setOutlineColor(sf::Color::Black); // Setting the color to the outline thikness

	// ----------------- Text Settings ---------------
	sf::Text textSettings("Settings", font, 30);
	textSettings.setPosition(sf::Vector2f(centerPos.x - textSettings.getGlobalBounds().width / 2, centerPos.y  - textSettings.getGlobalBounds().height / 2));
															// Set text settings position
	textSettings.setFillColor(sf::Color::White); // Setting the color to the text settings

	// ----------------- Exit button --------
	sf::RectangleShape exitButton(sf::Vector2f(0, 0));
	exitButton.setSize(sf::Vector2f(150, 50)); // Set size to the exit button
	exitButton.setPosition(centerPos.x - exitButton.getGlobalBounds().width / 2, centerPos.y + 100 - exitButton.getGlobalBounds().height / 2);
																	// Set exit button position
	exitButton.setFillColor(sf::Color::Magenta); // Setting the color to the exit button
	exitButton.setOutlineThickness(3); // Setting the outline thickness to the exit button and make it 3px 
	exitButton.setOutlineColor(sf::Color::Black); // Setting the color to the outline thikness

	// --------------- Text exit --------------
	sf::Text exitText("Exit", font, 30);
	exitText.setPosition(sf::Vector2f(centerPos.x - exitText.getGlobalBounds().width / 2, centerPos.y + 95 - exitText.getGlobalBounds().height / 2));
																			//  Set text exit position
	exitText.setFillColor(sf::Color::White); // Setting the color to the exit test

	// ----------- draw -------------
	window.draw(buttonPlay);
	window.draw(buttonSettings);
	window.draw(exitButton);
	window.draw(textPlay);
	window.draw(textSettings);
	window.draw(exitText);

	// ----------- We are waiting for the Play button to be pressed -----------------
	sf::Event buttonPress;
	while (window.pollEvent(buttonPress))
	{
		// If we press the Play button, the game will start
		if (buttonPress.type == buttonPress.MouseButtonReleased && buttonPress.mouseButton.button == sf::Mouse::Left && (sf::Mouse::getPosition(window).x >= buttonPlay.getPosition().x && sf::Mouse::getPosition(window).x <= buttonPlay.getPosition().x + 150) && (sf::Mouse::getPosition(window).y >= buttonPlay.getPosition().y && sf::Mouse::getPosition(window).y <= buttonPlay.getPosition().y + 50))
		{
			*isPlay = true;
		}
		// If we press the Settings button, the Settings function will start and it will continue as long as isSettings is true
		if (buttonPress.type == buttonPress.MouseButtonReleased && buttonPress.mouseButton.button == sf::Mouse::Left && (sf::Mouse::getPosition(window).x >= buttonSettings.getPosition().x && sf::Mouse::getPosition(window).x <= buttonSettings.getPosition().x + 150) && (sf::Mouse::getPosition(window).y >= buttonSettings.getPosition().y && sf::Mouse::getPosition(window).y <= buttonSettings.getPosition().y + 50))
		{
			*isSettings = true;
		}
		// Pressing the Exit button will end the program
		if (buttonPress.type == buttonPress.MouseButtonReleased && buttonPress.mouseButton.button == sf::Mouse::Left && (sf::Mouse::getPosition(window).x >= exitButton.getPosition().x && sf::Mouse::getPosition(window).x <= exitButton.getPosition().x + 150) && (sf::Mouse::getPosition(window).y >= exitButton.getPosition().y && sf::Mouse::getPosition(window).y <= exitButton.getPosition().y + 50))
		{
			window.close();
		}
	}

	if (*isSettings)
	{
		// We pass to the function a link to a window, a font, whether the game is running, 
													// whether the settings are running, the type of game
		Settings(window, font, isPlay, isSettings, typeOfGame);
	}
}

void Settings(sf::RenderWindow& window, sf::Font font, bool* isPlay, bool* isSettings, bool* typeOfGame)
{
	window.clear(sf::Color(83, 0, 144)); // Fill the window with a solid color

	// ---------- Button what borders to choose (teleports or murderous) ---------
	sf::Text textAboveButtons("Select the type of game:", font, 30);
	textAboveButtons.setPosition(sf::Vector2f(window.getSize().x / 2 - textAboveButtons.getGlobalBounds().width / 2, 50));
																					// Set text above the buttons position
	textAboveButtons.setFillColor(sf::Color::Black); // Setting the color to the text above the buttons

	// ------------ First button (dies from wall) -------------------
	sf::RectangleShape button1(sf::Vector2f(0, 0));
	button1.setSize(sf::Vector2f(300, 300)); // Set button size 300 x 300
	button1.setPosition(window.getSize().x / 3 - 50 - button1.getGlobalBounds().width / 2, window.getSize().y / 3 - 50); 
																								// Set button1 position
	button1.setFillColor(sf::Color::Magenta); // Setting the color of the button
	button1.setOutlineThickness(3); // Setting the outline thickness to the buttin1 and make it 3px 
	button1.setOutlineColor(sf::Color::Black); // Setting the color to the outline thikness

	// ----------- Text in button1 -------------------
	sf::Text text1("When colliding with the\n walls, the snake dies", font, 17); // Text
	text1.setPosition(sf::Vector2f(window.getSize().x / 3 - 20 - button1.getGlobalBounds().width / 2, window.getSize().y / 3 + text1.getGlobalBounds().height + 30));
																				// Setting position to the text1
	text1.setFillColor(sf::Color::Black); // Setting the color of the button
	text1.setStyle(sf::Text::Bold); // Settint the style of the text1

	// ------------ Вторая кнопка( телепортируется сквозь стенки) -------------------
	sf::RectangleShape button2(sf::Vector2f(0, 0));
	button2.setSize(sf::Vector2f(300, 300)); // кнопка размером 300 x 300
	button2.setPosition(2 *( window.getSize().x / 3) + 50 - button2.getGlobalBounds().width / 2, window.getSize().y / 3 - 50);
	button2.setFillColor(sf::Color::Magenta); // Setting the color to the button2
	button2.setOutlineThickness(3); // Setting the outline thickness to the button2 and make it 3px 
	button2.setOutlineColor(sf::Color::Black); // Setting the color to the outline thikness to the button2

	// ----------- Text in the button2 -------------------
	sf::Text text2("When colliding with a\nwall, the snake moves\nto the opposite side\n   of the screen", font, 17); // Text
	text2.setPosition(sf::Vector2f(2 * (window.getSize().x / 3) + 80 - button2.getGlobalBounds().width / 2, window.getSize().y / 3 + text2.getGlobalBounds().height - 20));
																								// Setting text2 position
	text2.setFillColor(sf::Color::Black); // Setting the color to the text2
	text2.setStyle(sf::Text::Bold); // Setting the style to the text2 

	// --------------- Exit back to the menu -----------
	sf::RectangleShape buttonEscape(sf::Vector2f(0, 0)); // escape button to the menu
	buttonEscape.setSize(sf::Vector2f(150, 75)); // Settings size to the button escape
	buttonEscape.setPosition(window.getSize().x - buttonEscape.getGlobalBounds().width - 50, window.getSize().y - buttonEscape.getGlobalBounds().height - 50);
																				// Set escape button position
	buttonEscape.setFillColor(sf::Color::Magenta); // Setting the color to the escape button
	buttonEscape.setOutlineThickness(3); //Setting the outline thickness to the escape button and make it 3px 
	buttonEscape.setOutlineColor(sf::Color::Black); // Setting the color to the outline thikness to the escape button

	// ---------------- Text in the escape button -------------
	sf::Text textEscape("Return to the\n\t menu", font, 15); 
	textEscape.setPosition(window.getSize().x - buttonEscape.getGlobalBounds().width - 50 + 16, window.getSize().y - buttonEscape.getGlobalBounds().height - 50 + 10 + textEscape.getGlobalBounds().height / 1.5);
																// Set text in the escape button position 
	textEscape.setFillColor(sf::Color::Black); // Setting the color to the text in the escape button
	textEscape.setStyle(sf::Text::Bold); // Setting the style to the ttextEscape
	
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
		// When colliding with the borders of the map, the snake dies
		if (buttonPress.type == buttonPress.MouseButtonReleased && buttonPress.mouseButton.button == sf::Mouse::Left && (sf::Mouse::getPosition(window).x >= button1.getPosition().x && sf::Mouse::getPosition(window).x <= button1.getPosition().x + 300) && (sf::Mouse::getPosition(window).y >= button1.getPosition().y && sf::Mouse::getPosition(window).y <= button1.getPosition().y + 300))
		{
			*typeOfGame = false;
		}
		// If typeOfGame = false the snake dies when it collides with borders
		if (buttonPress.type == buttonPress.MouseButtonReleased && buttonPress.mouseButton.button == sf::Mouse::Left && (sf::Mouse::getPosition(window).x >= button2.getPosition().x && sf::Mouse::getPosition(window).x <= button2.getPosition().x + 300) && (sf::Mouse::getPosition(window).y >= button2.getPosition().y && sf::Mouse::getPosition(window).y <= button2.getPosition().y + 300))
		{
			*typeOfGame = true;
		}
		// If we pressed the exit button, then we exit back to the menu
		if (buttonPress.type == buttonPress.MouseButtonReleased && buttonPress.mouseButton.button == sf::Mouse::Left &&  (sf::Mouse::getPosition(window).x >= buttonEscape.getPosition().x && sf::Mouse::getPosition(window).x <= buttonEscape.getPosition().x + 150) && (sf::Mouse::getPosition(window).y >= buttonEscape.getPosition().y && sf::Mouse::getPosition(window).y <= buttonEscape.getPosition().y + 75))
		{
			*isSettings = false;
			Menu(window, font, isPlay, isSettings, typeOfGame);
		}
	}
}
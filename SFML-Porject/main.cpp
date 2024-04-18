#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <time.h>
#include <vector>
#include <cmath>
using namespace std;
using namespace sf;

int status=1;
/*
	1	----->	NOT STARTED
	2	----->	PLAYING
	3	----->	Died
*/
float spdd = -0.65f;
float tt = 0.19f;
RectangleShape upp(Vector2f(200, 10));
RectangleShape downn(Vector2f(200, 10));

void playagain();
void beforeStart();
void updateScore();

bool hasCollided = false;  // for collision	
const int SCREEN_W =512 ;
const int SCREEN_H = 460;
Clock wingTimer;
bool gameOverBool = false;
struct BirdStruct
{
	Sprite sprite; Texture birds[3]; Sound jumpSound; SoundBuffer jBuffer; Sound collSound; SoundBuffer cBuffer;
	int currBird = 0;
	float maxAngle = 45.0f;

	
	
	



	// for bird moving
	float jumpSpeed = -1.26f;
	float gravity = 0.16f;
	bool hasMoved = false;

	void spawn(){
	
		sprite.setPosition(512 / 2, 460 / 2);
	birds[0].loadFromFile("bird-01.png");
	birds[1].loadFromFile("bird-02.png");
	birds[2].loadFromFile("bird-03.png");

	sprite.setTexture(birds[0]);
	sprite.setScale(0.5, 0.5);
	sprite.setOrigin(sprite.getGlobalBounds().width / 2, sprite.getGlobalBounds().width / 2);

	jBuffer.loadFromFile("jump.wav");
	jumpSound.setBuffer(jBuffer);

	cBuffer.loadFromFile("collided.wav");
	collSound.setBuffer(cBuffer);
	}
	void wingMove() {

		
		if (wingTimer.getElapsedTime().asMilliseconds() >= 100) {
			currBird = (currBird+1) % 3;
			sprite.setTexture(birds[currBird]);
			
			wingTimer.restart();
		
			
		}
		
		
	}
		

	void Jump(Event event, RenderWindow& window) {
		
		

			if (event.type == Event::KeyPressed) {
				if (event.key.code == Keyboard::Space && status == 3) {
					
					playagain();
				}
				if (event.key.code == Keyboard::Space && !hasMoved) {
					status = 2;
					//sprite.rotate(-maxAngle);
					jumpSpeed = -2.51f;
					hasMoved = true;
					if(!hasCollided)
					jumpSound.play();
				}
				
			}
			if (event.type = Event::KeyReleased) {
				if (event.key.code == Keyboard::Space) {
					hasMoved = false;
				}
			}


		
		

	}

	void Fall() {
		jumpSpeed += gravity;
		sprite.move(Vector2f(0, 2*jumpSpeed));
		
		
	}


	void draw(RenderWindow& window) {
		window.draw(sprite);
	}

	float rotationAngle=0;

	void rotation() {
		// Rotate Down
		if (jumpSpeed > 0) {
			if (rotationAngle <= 30) {
				rotationAngle += 3;
				sprite.setRotation(rotationAngle);
			}
		}

		// Rotate UP
		if (jumpSpeed < 0) {
			if (rotationAngle >= -30) {
				rotationAngle = -30;
				sprite.setRotation(rotationAngle);
			}
		}
	}
	bool f = false; Clock tt;
	
	

	

}bird;
struct pipeStruct
{
	

	Sprite up[2], down[2];
	Texture tx_up, tx_down;
	const int GAP_H = 110;
	const int PIPE_H= 360;
	void spawn() {
		srand(time(NULL));
		tx_up.loadFromFile("PipeUp.png");
		tx_down.loadFromFile("PipeDown.png");

		for (int i = 0; i < 2; i++) {
			up[i].setTexture(tx_up);
			down[i].setTexture(tx_down);

			up[i].setScale(0.9, 1);
			down[i].setScale(0.9, 1);
			up[i].setPosition(SCREEN_W, 0);                
			down[i].setPosition(SCREEN_W, 0);

			

		}

		

		down[0].setPosition(550 + 400, -140);
		up[0].setPosition(550 + 400, 330);
		down[1].setPosition(800 + 400, -220);
		up[1].setPosition(800 + 400, 250);
		
	}

	void move(float speed) {

		

		for (int i = 0; i < 2; i++) {

			up[i].move(Vector2f(-speed, 0));
			down[i].move(Vector2f(-speed, 0));
			if (up[i].getPosition().x <= -50) {

				int yPos = -(140 + rand() % 141);
				down[i].setPosition(SCREEN_W, yPos);
				up[i].setPosition(SCREEN_W, yPos + GAP_H + PIPE_H );

			}
		}
	}

	void draw(RenderWindow& window) {
		for (int i = 0; i < 2; i++) {
			window.draw(pipes.up[i]);  window.draw(pipes.down[i]);
		}
	}

}pipes;
struct collisionStruct {
	

	Sound collision; SoundBuffer cBuffer;
	bool soundCPlayed=false;
	void spawnCollision() {

		cBuffer.loadFromFile("collided.wav");
		collision.setBuffer(cBuffer);
		
		
	}
	void checkCollision(Sprite& player, Sprite& pipeup, Sprite& pipedown , Sprite& land) {

		if (player.getGlobalBounds().intersects(pipeup.getGlobalBounds())) {

			hasCollided = true;

		}
		else if (player.getGlobalBounds().intersects(pipedown.getGlobalBounds())) {
			hasCollided=  true;
		}
		else if (player.getGlobalBounds().intersects(land.getGlobalBounds())) {
			hasCollided=  true;
		}
		else
		hasCollided = false;
	}


}collision;
struct gameStruct
{	
	bool hasIncreased;
	bool hasIncreased2;
	int score=0;
	int highestScore=0;
	Sound scoreSound; SoundBuffer ssBuffer;
	Sprite invisiblePipe; Texture tx;
	Font font;
	Text scoreText;
	Text startingText;

	void spawnText() {
		font.loadFromFile("FlappyFont.ttf");
		scoreText.setFont(font);
		scoreText.setString("SCORE: ");
		ssBuffer.loadFromFile("Code_STATE CREATION_8 - Main Menu State_Resources_audio_Point.wav");
		scoreSound.setBuffer(ssBuffer);
		startingText.setFont(font);
		//startingText.setColor(Color::Black);
		startingText.setString("TAP TO START");
		startingText.setPosition(165, 150);
		startingText.setScale(1.2, 1.2);
	}
	void spawnInvisible() {
		hasIncreased = false;
		hasIncreased2 = false;
		
		invisiblePipe.setScale(0.9, 1);
		tx.loadFromFile("PipeUp.png");
		invisiblePipe.setTexture(tx);
		invisiblePipe.setPosition(203, 300);
		invisiblePipe.setColor(Color::Red);
		
	}
	void scoreCount() {
		if (pipes.up[0].getGlobalBounds().intersects(invisiblePipe.getGlobalBounds()) && !hasIncreased) {
			score++;
			
			hasIncreased = true;
			scoreSound.play();
		}

		if (!(pipes.up[0].getGlobalBounds().intersects(invisiblePipe.getGlobalBounds()))) {
			hasIncreased = false;
		}

		if (pipes.up[1].getGlobalBounds().intersects(invisiblePipe.getGlobalBounds()) && !hasIncreased2) {
			score++;
			
			hasIncreased2 = true;
			scoreSound.play();

		}

		if (!(pipes.up[1].getGlobalBounds().intersects(invisiblePipe.getGlobalBounds()))) {
			hasIncreased2 = false;
		}
		if (score > highestScore)
			highestScore = score;
		
		
	}

	
	void drawScore(RenderWindow& window) {
		window.draw(scoreText);
	}
	void drawStartText(RenderWindow& window) {
		window.draw(startingText);
	}

}game;
struct backgroundStruct {

	RectangleShape sky; Texture skyT, landT; Sprite lands[2]; Sprite gameOverTitle,gameOverBody; Texture gt,gtt;
	int currLand = 0; // TO MOVE THE LAND 
	Text score;
	void spawn() {
		RectangleShape skyy(Vector2f(512, 460)); sky = skyy;
		skyT.loadFromFile("sky.png");
		landT.loadFromFile("land.png");
		sky.setTexture(&skyT);

		
		for (int i = 0; i < 2; i++) {
			lands[i].setTexture(landT);
			lands[i].setScale(0.7, 0.5);
		}
			lands[1].setPosition(Vector2f(506, 400.0f));
			lands[0].setPosition(Vector2f(0, 400.0f));
			// diffrecne between them = 36
		
		

	}
	
	
	void drawSky(RenderWindow& window) {
		window.draw(sky);
	}
	void drawLand(RenderWindow& window) {
		for (int i = 0; i < 2; i++) {
			window.draw(lands[i]);
		}
	}
	void moveLands(float speed) {

		lands[currLand].move(Vector2f(-speed, 0));
		currLand++;
		currLand %= 2;

		if (lands[0].getPosition().x <= -506) {
			lands[0].setPosition(Vector2f(506, 400.0f));
		}
		if (lands[1].getPosition().x <= -506) {
			lands[1].setPosition(Vector2f(506, 400.0f));
		}
	}
}background;
struct died
{
	Sound sound; SoundBuffer sBuffer;
	bool doneit = false;
	float speed = 7.2f;

	
	Sprite gameOverTitle, gameOverBody; Texture gt, gtt; 
	Text score; Text high;
	

	void spawn() {
		sBuffer.loadFromFile("die-101soundboards.wav");
		sound.setBuffer(sBuffer);
	}
	void done() {

		if (!doneit) {
			bird.sprite.setRotation(90);
			sound.play();
			doneit = true;
		}

		bird.sprite.move(0, speed);
		
		for (int i = 0; i < 2; i++) {
			if (bird.sprite.getPosition().y>390) {
			bird.sprite.move(0, -speed);
			bird.sprite.setRotation(135);
			
				
			}

		}
	}
	void spawnGameOver() {
		if (!gameOverBool) {
			gameOverBool = true;
			gt.loadFromFile("Game-Over-Title.png");
			gameOverTitle.setTexture(gt);
			gameOverTitle.setOrigin(gameOverTitle.getGlobalBounds().width / 2, gameOverTitle.getGlobalBounds().height / 2);
			
			gameOverTitle.setScale(0.4, 0.4);

			gtt.loadFromFile("Game-Over-Body.png");
			gameOverBody.setTexture(gtt);
			gameOverBody.setOrigin(gameOverBody.getGlobalBounds().width / 2, gameOverBody.getGlobalBounds().height / 2);
			
			gameOverBody.setScale(0.4, 0.4);

			score.setFillColor(Color::White);
			score.setFont(game.font);

			high.setFillColor(Color::White);
			high.setFont(game.font);
			high.setScale(0.8, 0.8);

			score.setOutlineColor(Color::Black);
			score.setScale(0.8, 0.8);

			gameOverBody.setPosition(512 / 2, 400);  // 75
			high.setPosition(315, 410);
			score.setPosition(315, 371);
			gameOverTitle.setPosition(512 / 2, 60);
		}

		// gameoveBody  256 , 245   title  256 , 170
	}
	void drawGameOver(RenderWindow& window) {
		window.draw(gameOverTitle);
		window.draw(gameOverBody);
		window.draw(score);
		window.draw(high);
	}
	void moveGameOver() {
		if (gameOverBody.getPosition().y >= 245) {
			gameOverBody.move(0, -5);
		}
		if (gameOverTitle.getPosition().y <= 160) {
			gameOverTitle.move(0, 5);
		}
		if (score.getPosition().y >= 216) {
			score.move(0, -5);
		}
		if (high.getPosition().y >= 255) {
			high.move(0, -5);
		}

		// gameoveBody  256 , 245   title  256 , 170
	}
	// gameoveBody  256 , 245   title  256 , 170 , score 216

}die;

	int main() {
		
		srand(time(NULL));
		RenderWindow window(VideoMode(512, 460), "Game", Style::Close | Style::Resize);
		window.setFramerateLimit(60);
		window.setKeyRepeatEnabled(false);
		bird.spawn();
		pipes.spawn();
		game.spawnText();
		die.spawnGameOver();
		background.spawn();
		game.spawnInvisible();
		die.spawn();
		collision.spawnCollision();

		cout << background.lands[0].getGlobalBounds().width;
		


		

		
		while (window.isOpen()) {
			Event _event;
			while (window.pollEvent(_event)) {
				if (_event.type == Event::Closed)
				{
					window.close();
					break;
				}
				
					bird.Jump(_event,window);
				
				

			}
			

			// waiting to start
			if (status == 1) {
				background.moveLands(5);
				bird.wingMove();
				beforeStart();
			}


			// playing
			if (status == 2) {
				
				bird.rotation();
				// for check collision
				for (int cc =0 ; cc < 2; cc++) {
				collision.checkCollision(bird.sprite, pipes.up[cc], pipes.down[cc] , background.lands[cc]);
				if (hasCollided) {
					if (!collision.soundCPlayed) {
						collision.collision.play();
						collision.soundCPlayed = true;
					}
					status = 3;
					break;
				}
			}

				// while the bird is alive
				if (!hasCollided)
				{
					bird.Fall();
					pipes.move(6);
					bird.wingMove();
					background.moveLands(7);
					game.scoreCount();

				}

				// if the bird died
				if (hasCollided){
					if (!collision.soundCPlayed) {
						bird.collSound.play();
						collision.soundCPlayed = true;
					}
					status = 3;	
					
				}

			}

			 // die
			if (status == 3)
			{
				die.done();
				die.moveGameOver();							
			}
			
			
			
			updateScore();

			//window.clear();
			background.drawSky(window);
			pipes.draw(window);
			background.drawLand(window);
			//window.draw(game.invisiblePipe);
			bird.draw(window);
			game.drawScore(window);
			if (status == 1) {
				game.drawStartText(window);
			}
			if (status == 3)
			{
				die.drawGameOver(window);
			}
			

			// gameoveBody  256 , 245   title  256 , 170 , score 315 ,216
			window.display();

		}
		return 0;
	}

void beforeStart() {
	upp.setFillColor(Color::Red);
	downn.setFillColor(Color::Red);
	upp.setPosition(200, 202);
	downn.setPosition(200, 249);

		bird.sprite.move(0, spdd);
		if (bird.sprite.getGlobalBounds().intersects(upp.getGlobalBounds())) {
			spdd +=tt;
		}
		if (bird.sprite.getGlobalBounds().intersects(downn.getGlobalBounds())) {
			spdd -=tt;
		}


	}
void playagain() {
	pipes.down[0].setPosition(550 + 400, -140);
	pipes.up[0].setPosition(550 + 400, 330);
	pipes.down[1].setPosition(800 + 400, -220);
	pipes.up[1].setPosition(800 + 400, 250);
	bird.sprite.setPosition(512 / 2, 460 / 2);
	collision.soundCPlayed = false;
	die.doneit = false;
	game.score = 0;
	gameOverBool = false;

	die.gameOverBody.setPosition(512 / 2, 400);  // 75
	die.high.setPosition(315, 410);
	die.score.setPosition(315, 371);
	die.gameOverTitle.setPosition(512 / 2, 60);
}
void updateScore() {
	game.scoreText.setString("SCORE: " + to_string(game.score));
	die.score.setString(to_string(game.score));
	die.high.setString(to_string(game.highestScore));
}





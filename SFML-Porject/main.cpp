#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <time.h>
#include <vector>
#include <cmath>
using namespace std;
using namespace sf;


/******************************
*	1	----->	NOT STARTED	  *
*	2	----->	PLAYING		  *
*	3	----->	Died		  *							  
******************************/
int status = 1;


RenderWindow window(VideoMode().getDesktopMode(), "Game", Style::Close | Style::Fullscreen);
Event _event;
const float SCREEN_W = window.getSize().x;
const float SCREEN_H = window.getSize().y;

void playagain();
void beforeStart();
void updateScore();
void setAssests();
void draw();
void updateStatus();
void gamePlay();
void closeWindow();


struct BirdStruct
{
	Sprite sprite;		Texture birds[3];
	Sound jumpSound;	SoundBuffer jBuffer;
	Clock wingTimer;
	RectangleShape upp;
	RectangleShape downn;
	int currBird = 0;
	float maxAngle = 45.0f;
	float jumpSpeed = -2.26f;
	float gravity = 0.22f;
	bool hasMoved = false;
	float rotationAngle = 0;
	float yVelocity = -0.15f;
	float yGravity = 0.07f;
	
	void set() {
		RectangleShape _up(Vector2f(200, 10));
		RectangleShape _down(Vector2f(200, 10));
		upp = _up;
		downn = _down;
		sprite.setPosition(SCREEN_W / 2, SCREEN_H/ 2);
		birds[0].loadFromFile("flappy1.png");
		birds[1].loadFromFile("flappy2.png");
		birds[2].loadFromFile("flappy3.png");
		sprite.setTexture(birds[0]);
		sprite.setScale(1.5, 1.5);
		sprite.setOrigin(sprite.getGlobalBounds().width / 2, sprite.getGlobalBounds().width / 2);
		jBuffer.loadFromFile("jump.wav");
		jumpSound.setBuffer(jBuffer);
	}
	void wingMove() {


		if (wingTimer.getElapsedTime().asMilliseconds() >= 150) {
			currBird = (currBird + 1) % 3;
			sprite.setTexture(birds[currBird]);

			wingTimer.restart();


		}


	}
	void Jump(float speed);
	void Fall() {
		jumpSpeed += gravity;
		sprite.move(Vector2f(0, 2 * jumpSpeed));


	}
	void draw() {
		window.draw(sprite);
	}
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
}bird;
struct pipeStruct
{
	Sprite up[3], down[3];
	void set() {
		tx_up.loadFromFile("PipeUp.png");
		tx_down.loadFromFile("PipeDown.png");
		for (int i = 0; i < 3; i++) {
			up[i].setTexture(tx_up);
			down[i].setTexture(tx_down);
			up[i].setScale(1.4, 1.8);
			down[i].setScale(1.4, 1.8);
			up[i].setPosition(SCREEN_W, 0);
			down[i].setPosition(SCREEN_W, 0);
		}
		int yPos = -(30 + rand() % 431);
		int yPoss = -(30 + rand() % 431);
		int yPosss = -(30 + rand() % 431);
		down[0].setPosition( SCREEN_W+70 , yPos);
		up[0].setPosition( SCREEN_W+70, yPos + GAP_H);
		down[1].setPosition(up[0].getPosition().x + GAP_X, yPoss);
		up[1].setPosition(up[0].getPosition().x + GAP_X, yPoss + GAP_H);
		down[2].setPosition(up[1].getPosition().x + GAP_X, yPosss);
		up[2].setPosition(up[1].getPosition().x + GAP_X, yPosss + GAP_H);
	}
	Texture tx_up, tx_down;
	const int GAP_H = 850;
	const int GAP_X = 460;
	const int PIPE_H = up[0].getGlobalBounds().height;
	void move(float speed) {
		for (int i = 0; i < 3; i++) {
			up[i].move(Vector2f(-speed, 0));
			down[i].move(Vector2f(-speed, 0));
			if (up[i].getPosition().x <= -50) {
				int yPos = -(30 + rand() % 431);
				down[i].setPosition(SCREEN_W, yPos);
				up[i].setPosition(SCREEN_W, yPos + GAP_H);
				up[i].setPosition(SCREEN_W, up[i].getPosition().y);
				down[i].setPosition(SCREEN_W, down[i].getPosition().y);
			}
		}
	}
	void draw() 
	{
		for (int i = 0; i < 3; i++) {
			window.draw(pipes.up[i]);  window.draw(pipes.down[i]);
		}
	}
}pipes;
struct collisionStruct 
{
	bool isCollided = false;  // for collision	
	Sound collision; SoundBuffer cBuffer;
	bool soundCPlayed = false;
	void set() {

		cBuffer.loadFromFile("collided.wav");
		collision.setBuffer(cBuffer);


	}
	void checkCollision(Sprite& player, Sprite& pipeup, Sprite& pipedown, Sprite& land) {
		if (player.getGlobalBounds().intersects(pipeup.getGlobalBounds())) {
			isCollided = true;
		}
		else if (player.getGlobalBounds().intersects(pipedown.getGlobalBounds())) {
			isCollided = true;
		}
		else if (player.getGlobalBounds().intersects(land.getGlobalBounds())) {
			isCollided = true;
		}
		else
			isCollided = false;
	}
}collision;
struct gameStruct
{
	bool hasIncreased;
	bool hasIncreased2;
	bool hasIncreased3;
	int score = 0;
	int highestScore = 0;
	Sound scoreSound; SoundBuffer ssBuffer;
	Sprite invisiblePipe; Texture tx;
	Font font;
	Text scoreText;
	Text startingText;
	void setText() {
		font.loadFromFile("FlappyFont.ttf");
		scoreText.setFont(font);
		scoreText.setString("SCORE: ");
		ssBuffer.loadFromFile("Code_STATE CREATION_8 - Main Menu State_Resources_audio_Point.wav");
		scoreSound.setBuffer(ssBuffer);
		startingText.setFont(font);
		//startingText.setColor(Color::Black);
		startingText.setString("TAP TO START");
		startingText.setOrigin(startingText.getGlobalBounds().width / 2, startingText.getGlobalBounds().height / 2);
		startingText.setPosition(SCREEN_W / 2 , SCREEN_H / 2 - 80);
		startingText.setScale(1.2, 1.2);
	}
	void setInvisible() {
		hasIncreased = false;
		hasIncreased2 = false;
		hasIncreased3 = false;

		invisiblePipe.setScale(0.9, 2);
		tx.loadFromFile("PipeUp.png");
		invisiblePipe.setTexture(tx);
		invisiblePipe.setPosition(SCREEN_W /2 - 50, 400);
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
		if (pipes.up[2].getGlobalBounds().intersects(invisiblePipe.getGlobalBounds()) && !hasIncreased3) {
			score++;

			hasIncreased3 = true;
			scoreSound.play();

		}

		if (!(pipes.up[2].getGlobalBounds().intersects(invisiblePipe.getGlobalBounds()))) {
			hasIncreased3 = false;
		}
		if (score > highestScore)
			highestScore = score;


	}
	void drawScore() {
		window.draw(scoreText);
	}
	void drawStartText() {
		window.draw(startingText);
	}

}game;
struct backgroundStruct {

	Sprite sky; Texture skyT, landT; Sprite lands[3];
	int currLand = 0; // TO MOVE THE LAND 
	Text score;
	void set() {

		skyT.loadFromFile("sky.png");
		landT.loadFromFile("land.png");
		sky.setTexture(skyT);
		sky.setScale(1.7, 1);


		for (int i = 0; i < 3; i++) {
			lands[i].setTexture(landT);
			lands[i].setScale(1.2, 0.6);
		}
		lands[0].setPosition(Vector2f(0, SCREEN_H-60));
		lands[1].setPosition(Vector2f(lands[0].getGlobalBounds().width-7.8, SCREEN_H-60));
		lands[2].setPosition(Vector2f(2*lands[0].getGlobalBounds().width-15.6, SCREEN_H-60));
		// diffrecne between them = 36



	}


	void drawSky() {
		window.draw(sky);
	}
	void drawLand() {
		for (int i = 0; i < 3; i++) {
			window.draw(lands[i]);
		}
	}
	void moveLands(float speed) {

		lands[currLand].move(Vector2f(-speed, 0));
		currLand++;
		currLand %= 3;
		for (int i = 0; i < 3; i++) {
			if (lands[i].getPosition().x <= -SCREEN_W - 7.7) {
				lands[i].setPosition(Vector2f(SCREEN_W - 7.8, SCREEN_H - 60));
			}
			
		}
	}
}background;
struct died
{
	bool gameOverBool = false;
	Sound sound; SoundBuffer sBuffer;
	bool doneit = false;
	float speed = 7.2f;
	Sprite gameOverTitle, gameOverBody; Texture gt, gtt;
	Text score; Text high;
	void set() {
		sBuffer.loadFromFile("die-101soundboards.wav");
		sound.setBuffer(sBuffer);
	}
	float anglee = 90;
	void done() {
		
		if (!doneit) {
			bird.sprite.setRotation(90);
			sound.play();
			doneit = true;
		}

		bird.sprite.move(0, speed);
		
			if (bird.sprite.getPosition().y > SCREEN_H- 100) {
				bird.sprite.move(0, -speed);
				if (anglee >= 135 && anglee <= 200)
				{
					anglee+=2;
					bird.sprite.setRotation(anglee);
				}
			}
				if (anglee <= 135)
					bird.sprite.setRotation(anglee);
		anglee +=1;

		
	}
	void setGameOver() {
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
			high.setScale(1, 1);

			score.setOutlineColor(Color::Black);
			score.setScale(1, 1);

			gameOverBody.setPosition(SCREEN_W / 2, 650);  
			gameOverTitle.setPosition(SCREEN_W/2 , 300);
			score.setPosition(SCREEN_W / 2 + 59 , 619);
			high.setPosition(SCREEN_W / 2 + 59, 655);
		}
	}
	void drawGameOver() {
		window.draw(gameOverTitle);
		window.draw(gameOverBody);
		window.draw(score);
		window.draw(high);
	}
	void moveGameOver() {
		if (gameOverBody.getPosition().y >= SCREEN_H/2) 
		{
			gameOverBody.move(0, -5);
		}
		if (gameOverTitle.getPosition().y <= SCREEN_H / 2 - 75)
		{ 
			gameOverTitle.move(0, 5);
		}
		if (score.getPosition().y >= SCREEN_H / 2.0f - 27.7)
		{ 
			score.move(0, -5);
		}
		if (high.getPosition().y >= SCREEN_H / 2 + 12)
		{
			high.move(0, -5);
		}

		// gameoveBody  256 , 245   title  256 , 170
	}
}die;

int main() {
	srand(time(NULL));
	setAssests();
	while (window.isOpen())
	{
		while (window.pollEvent(_event)) {
			updateStatus();
			bird.Jump(3.51);
			closeWindow();
		}
		gamePlay();
		draw();
	}
	return 0;
}

void beforeStart() {
	
	bird.upp.setFillColor(Color::Red);
	bird.downn.setFillColor(Color::Red);
	bird.upp.setPosition(SCREEN_W / 2 - 100 , SCREEN_H / 2 - 30);
	bird.downn.setPosition(SCREEN_W / 2 - 100 , SCREEN_H /  2 + 20);

	bird.sprite.move(0, bird.yVelocity);
	if (bird.sprite.getGlobalBounds().intersects(bird.upp.getGlobalBounds())) {
		bird.yVelocity += bird.yGravity;
	}
	if (bird.sprite.getGlobalBounds().intersects(bird.downn.getGlobalBounds())) {
		bird.yVelocity -= bird.yGravity;
	}


}
void playagain() {
	pipes.set();
	bird.sprite.setPosition(SCREEN_W / 2, SCREEN_H / 2);
	collision.soundCPlayed = false;
	die.doneit = false;
	game.score = 0;
	die.gameOverBool = false;
	die.gameOverBody.setPosition(SCREEN_W / 2, 650);
	die.gameOverTitle.setPosition(SCREEN_W / 2, 300);
	die.score.setPosition(SCREEN_W / 2 + 59, 619);
	die.high.setPosition(SCREEN_W / 2 + 59, 655);
	die.anglee = 90;
	collision.isCollided = false;
	status = 2;
}
void updateScore() {
	game.scoreText.setString("SCORE: " + to_string(game.score));
	die.score.setString(to_string(game.score));
	die.high.setString(to_string(game.highestScore));
}
void setAssests() {
	window.setFramerateLimit(60);
	bird.set();
	pipes.set();
	game.setText();
	die.setGameOver();
	background.set();
	game.setInvisible();
	die.set();
	collision.set();
}
void draw() {
	window.clear();
	background.drawSky();
	pipes.draw();
	//window.draw(game.invisiblePipe);
	bird.draw();
	
	background.drawLand();
	game.drawScore();
	if (status == 1) {
		game.drawStartText();
	}
	if (status == 3)
	{
		die.drawGameOver();
	}
	//window.draw(upp);
	//window.draw(downn);
	window.display();
}
void updateStatus() 
{
		if (_event.type == Event::KeyPressed) 
		{
			if (_event.key.code == Keyboard::Space && status == 3) {
				playagain();
			}
			if (_event.key.code == Keyboard::Space) {
				status = 2;
			}

		}

	if (status == 2) 
	{
		if (collision.isCollided)
		{
			status = 3;
		}	
	}


}
void gamePlay() 
{
	if (status == 1) {
		background.moveLands(5);
		bird.wingMove();
		beforeStart();
	}
	// playing
	if (status == 2) 
	{

		bird.rotation();
		bird.Jump(3.51);
		// for check collision
		for (int cc = 0; cc < 3; cc++) {
			collision.checkCollision(bird.sprite, pipes.up[cc], pipes.down[cc], background.lands[cc]);
			if (collision.isCollided) {
				if (!collision.soundCPlayed) {
					collision.collision.play();
					collision.soundCPlayed = true;
				}
				status = 3;
				break;
			}
		}

		// while the bird is alive
		if (!collision.isCollided)
		{
			bird.Fall();
			pipes.move(5.3);
			bird.wingMove();
			background.moveLands(9);
			game.scoreCount();
		}

	}
	// die
	if (status == 3)
	{
		die.done();
		die.moveGameOver();
	}
	updateScore();
}
void closeWindow(){
	while (window.pollEvent(_event)) 
	{
		if (_event.type == Event::Closed)
		{
			window.close();
			break;
		}
	}
}

void BirdStruct::Jump(float speed)
{
	if (_event.type == Event::KeyPressed) {
		if (_event.key.code == Keyboard::Space && !hasMoved) {
			jumpSpeed = -speed;
			hasMoved = true;
			if (!collision.isCollided)
				jumpSound.play();
		}
	}
	if (_event.type == Event::KeyReleased) {
		if (_event.key.code == Keyboard::Space) {
			hasMoved = false;
		}
	}
}




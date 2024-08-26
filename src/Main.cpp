// main.cpp

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <time.h>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include "glut.h"
#include "Game.h"

using namespace std;

const int BLOCKSIZE = 40;
const int VPWIDTH = COLS * BLOCKSIZE;
const int VPHEIGHT = ROWS * BLOCKSIZE;
const string HIGHSCORE_FILE = "highscores.txt";
const int NUM_HIGHSCORES = 3;
Game game;
GLvoid* font_style = GLUT_BITMAP_TIMES_ROMAN_24;

// Displaying text on the screen
void BitmapText(char *str, int wcx, int wcy)
{
	glRasterPos2i(wcx, wcy);
	for (int i=0; str[i] != '\0'; i++) {
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, str[i]);
	}
}

// Getting a random number in the interval [0, 1]
double random()
{ 
	int r = rand();
	return double(r) / RAND_MAX;
}
//High score
vector<int> readHighScores() {
	std::ifstream infile(HIGHSCORE_FILE);
	std::vector<int> highScores(NUM_HIGHSCORES, 0);
	if (infile.is_open()) {
		for (int i = 0; i < NUM_HIGHSCORES; ++i) {
			if (!(infile >> highScores[i])) {
				break;
			}
		}
		infile.close();
	}
	return highScores;
}
void writeHighScores(const std::vector<int>& highScores) {
	std::ofstream outfile(HIGHSCORE_FILE);
	if (outfile.is_open()) {
		for (int i = 0; i < NUM_HIGHSCORES; ++i) {
			outfile << highScores[i] << std::endl;
		}
		outfile.close();
	}
}

void displayHighScores() {
	vector<int> highScores = readHighScores();
	const int N = 100;
	char msg[N + 1];
	glColor3f(1, 1, 1); // Màu trắng
	sprintf_s(msg, N, "An phim [E] de quay lai menu");
	BitmapText(msg, VPWIDTH / 2 - 130, VPHEIGHT / 2 + 150);
	sprintf_s(msg, N, "HIGH SCORES");
	BitmapText(msg, VPWIDTH / 2 - 50, VPHEIGHT/2 + 100);

	for (int i = 0; i < NUM_HIGHSCORES; ++i) {
		sprintf_s(msg, N, "%d. %d", i + 1, highScores[i]);
		BitmapText(msg, 170, VPHEIGHT/2 + 50 - (i * 50));
	}
}
// Callback function, timer
void timer(int id)
{
	if (game.killed) {	// If the game is over

		game.paused = true;
		//glutTimerFunc(200, timer, id);
		game.clearMainGrid();
		game.clearNextPieceGrid();
		vector<int> highScores = readHighScores();
		highScores.push_back(game.linesCleared);
		sort(highScores.rbegin(), highScores.rend()); // Sắp xếp giảm dần
		highScores.resize(NUM_HIGHSCORES); // Giữ lại top 3
		writeHighScores(highScores);
		glutPostRedisplay();

	} else if (!game.paused) {	// If the game continues and is not paused
		game.update();		
		if (game.killed) {
			glutTimerFunc(10, timer, 1);
		} else {
			glutPostRedisplay();
			glutTimerFunc(game.timer, timer, 0);
		}
	}
}

// Callback function, handling keyboard actions
void keyboard(unsigned char key, int x, int y)
{
	if (game.paused) {
		if (game.killed)
		{
			if (key == 13) { // 13 == ENTER
				game.killed = false;
				game.restart();
				glutTimerFunc(game.timer, timer, 0);
			}
		}
		else
		{	
			if (game.highScore)
			{
				if (key = 'e' || key == 69)
				{
					game.highScore = false;
					glutTimerFunc(game.timer, timer, 0);
				}
			}
			else {
				if (key == 'p' || key == 80) { // 27 == ESCAPE
					game.paused = !game.paused;
					if (!game.paused)
						glutTimerFunc(game.timer, timer, 0);
				}
				if (key == 'n' || key == 78)
				{
					game.restart();
					glutTimerFunc(game.timer, timer, 0);
				}
				if (key == 'h' || key == 72)
				{
					game.highScore = true;
					glutTimerFunc(game.timer, timer, 0);
				}
				if (key == 'q' || key == 81)
				{
					game.exist();
					glutTimerFunc(game.timer, timer, 0);
				}
			}
			
		}
	}
	else {
		if (key == 'p' || key == 80) { // 27 == ESCAPE
			game.paused = !game.paused;
			if (!game.paused)
				glutTimerFunc(game.timer, timer, 0);
		}
		else if (!game.paused && !game.killed && key == ' ') { // ' ' == SPACE
			game.update();
		}
	}

	glutPostRedisplay();
}

// Callback function, handling arrow actions
void special(int key, int x, int y)
{
	if (!game.paused && !game.killed) {
		if (key == GLUT_KEY_LEFT) {
			game.move(-1);
			glutPostRedisplay();
		} else if (key == GLUT_KEY_RIGHT) {
			game.move(1);
			glutPostRedisplay();
		} else if (key == GLUT_KEY_UP) {
			game.rotateShape(1);
			glutPostRedisplay();
		} else if (key == GLUT_KEY_DOWN) {
			game.rotateShape(-1);
			glutPostRedisplay();
		}
	}
}
// Callback function, displaying the gameplay window
void display(void)
{
	const int N = 100;
	char msg[N + 1];
	glClearColor(0, 0, 0, 0); // Đổi màu phần khối tiếp theo
	glClear(GL_COLOR_BUFFER_BIT);

	// Grid
	glViewport(0, 0, VPWIDTH, VPHEIGHT);
	glMatrixMode(GL_PROJECTION);
	if (!game.paused) {	// If the game is active, the movement of the pieces is carried out

		glLoadIdentity();
		gluOrtho2D(0, COLS, ROWS, 0);

		for (int r = 0; r < ROWS; r++) {
			for (int c = 0; c < COLS; c++) {
				Square& square = game.mainGrid[r][c];
				if (square.isFilled) {
					glColor3f(square.red, square.green, square.blue);
					glRectd(c + .1, r + .1, c + .9, r + .9);
				}
				else {
					glColor3f(0, 0, 0);
					glRectd(c, r, c + 1, r + 1);
				}
			}
		}
	}
	else {

		glLoadIdentity();
		gluOrtho2D(0, VPWIDTH, 0, VPHEIGHT);

		if (game.paused && !game.killed) {		// If the game is paused, the pause menu is displayed
			if (game.highScore)
			{
				displayHighScores();
			}
			else {
				glColor3f(255, 255, 255); // Đổi màu menu
				sprintf_s(msg, N, "An phim [P] de tiep tuc");
				BitmapText(msg, 75, VPHEIGHT / 2 + 100);
				sprintf_s(msg, N, "An phim [N] de choi moi");
				BitmapText(msg, 75, VPHEIGHT / 2 + 50);
				sprintf_s(msg, N, "An phim [H] de xem diem cao");
				BitmapText(msg, 65, VPHEIGHT / 2);
				sprintf_s(msg, N, "An phim [Q] de thoat");
				BitmapText(msg, 85, VPHEIGHT / 2 - 50);
			}
		}
		if (game.paused && game.killed) {		// If the game is over, the restart menu is displayed
			glColor3f(255, 255, 255); // Đổi màu menu
			sprintf_s(msg, N, "BAN THUA ROI");
			BitmapText(msg,  155, VPHEIGHT/2 + 50);
			sprintf_s(msg, N, "Diem cua ban: %d", game.linesCleared);
			BitmapText(msg,  140, VPHEIGHT/2);
			sprintf_s(msg, N, "An phim [ENTER] de choi lai!");
			BitmapText(msg, 75, VPHEIGHT/2 - 100);
		}
	}

	// Vertical dividing strip
	glViewport(VPWIDTH, 0, VPWIDTH, VPHEIGHT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, VPWIDTH, 0, VPHEIGHT);

	glBegin(GL_LINES);
	glColor3f(0, 0, 0);
	glVertex2d(1, 0);
	glVertex2d(1, glutGet(GLUT_WINDOW_HEIGHT));
	glEnd();

	// Messages on the right side of the screen
	glLoadIdentity();
	gluOrtho2D(0, VPWIDTH, 0, VPHEIGHT);

	glColor3f(255, 255, 255);
	sprintf_s(msg, N, "Diem cua ban: %d", game.linesCleared);
	BitmapText(msg, 50, 100);
	sprintf_s(msg, N, "So khoi xuat hien: %d", game.shapesCount);
	BitmapText(msg, 50, 50);
	sprintf_s(msg, N, "Khoi tiep theo:");
	BitmapText(msg, 50, VPHEIGHT - 50);

	// Vertical dividing strip
	glBegin(GL_LINES);
		glColor3f(1, 1 ,1);
		glVertex2d(1,0);
		glVertex2d(1, glutGet(GLUT_WINDOW_HEIGHT));
	glEnd();

	// Grid displaying the next piece
	glViewport(VPWIDTH + 50, -50, VPWIDTH, VPHEIGHT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, COLS, ROWS, 0);

	for (int r = 1; r < 5; r++) {
		for (int c = 0; c < 2; c++) {
			Square& square = game.nextPieceGrid[r][c];
			if (square.isFilled) {
				glColor3f(square.red, square.green, square.blue);
				glRectd(c + .1, r + .1, c + .9, r + .9);
			}
			else {
				glColor3f(0, 0, 0);
				glRectd(c, r, c + 1, r + 1);
			}
		}
	}

	glutSwapBuffers();
}

void main(int argc, char *argv[])
{
	srand(time(0));
	glutInit(&argc, argv);
	double randomValue = static_cast<double>(std::rand()) / RAND_MAX;
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);	

	glutInitWindowPosition(50, 50);
	glutInitWindowSize(VPWIDTH*2, VPHEIGHT);

	glutCreateWindow("Tetris");
	game.paused = true;
	glutDisplayFunc(display);
	glutSpecialFunc(special);
	glutKeyboardFunc(keyboard);
	glutTimerFunc(game.timer, timer, 0);

	glutMainLoop();
}

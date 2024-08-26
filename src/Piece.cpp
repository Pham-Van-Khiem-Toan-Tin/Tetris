// piece.cpp

#include "Piece.h"
#include <cstdlib>
#include <ctime>
#include <random>
Piece::Piece() {
	Piece(0);
}

/*
Tạo khối mới, set màu và rotation mặc định
*/
float colors[3] = {0.0f,0.0f,0.0f};
Piece::Piece(int numType) {
	type = numType;
	switch (type) {
		case 0: color(1.0, 1.0, 0.0); break; // vàng
		case 1: color(0.5, 0.5, 0.5); break; // xám
		case 2: color(0.0, 1.0, 0.0); break; // xanh lá
		case 3: color(0.0, 0.0, 1.0); break; // xanh dương
		case 4: color(1.0, 0.0, 0.0); break; // đỏ
		case 5: color(1.0, 0.0, 1.0); break; // hồng
		case 6: color(0.6, 0.3, 0.1); break; // nâu
	}
	rotation = 0;
}

/*
Mảng chứa thông tin về khối cụ thể dựa trên rotation
*/
const int* Piece::rotations() {
	return gamePieces[type][rotation];
}

/*
Set màu
*/
void Piece::color(float r, float g, float b) {
	redVal = r;
	greenVal = g;
	blueVal = b;
}

/*
Increase or decrease the rotation index of game pieces
*/
void Piece::rotatePiece(int dir) {
	if(dir > 0) {
		if (rotation == 3)
			rotation = 0;
		else
			rotation += dir;
	}
	else {
		if (rotation == 0)
			rotation = 3;
		else
			rotation += dir;
	}
}

// piece.h

// Basic constants
const int numPieces = 7;	// 7 khối khác nhau
const int numRotations = 4; // Số lần xoay khối
const int numSpaces = 8;	// Dung lượng lưu trữ cho mỗi khối

// Lớp Piece
class Piece {

public:

	// Constructors
	Piece();
	Piece(int newPiece);

	// Tọa độ khối trên lưới trò chơi
	int x;
	int y;

	// Giá trị màu
	float redVal, greenVal, blueVal;

	// Biến loại khối và xoay
	int type;
	int rotation;

	// Hàm xoay khối
	void rotatePiece(int dir);

	// Set màu theo bảng RGB
	void color(float r, float g, float b);

	// Trả về con trỏ Giá trị xoay khối hiện tại
	const int* rotations();
};

// All pieces with each option of their rotation in the grid in the format {x0, y0, x1, y1, x2, y2, x3, y3}
const int gamePieces[numPieces][numRotations][numSpaces] =
{
	{
		{0, 0, 1, 0, 0, 1, 1, 1}, // Hình vuông
		{0, 0, 1, 0, 0, 1, 1, 1},
		{0, 0, 1, 0, 0, 1, 1, 1},
		{0, 0, 1, 0, 0, 1, 1, 1},
	},
	{
		{0, 0, 0, 1, 0, 2, 0, 3}, // Đoạn thẳng
		{0, 0, 1, 0, 2, 0, 3, 0},
		{0, 3, 0, 2, 0, 1, 0, 0},
		{0, 0, 1, 0, 2, 0, 3, 0},
	},
	{
		{0, 0, 0, 1, 1, 1, 0, 2}, // Chữ T
		{1, 0, 0, 1, 1, 1, 2, 1},
		{0, 1, 1, 0, 1, 1, 1, 2}, 
		{0, 0, 1, 0, 2, 0, 1, 1}
	},
	{	{0, 0, 1, 0, 0, 1, 0, 2}, // Chữ L
		{0, 0, 0, 1, 1, 1, 2, 1},
		{1, 0, 1, 1, 0, 2, 1, 2},
		{0, 0, 1, 0, 2, 0, 2, 1}
	},
	{	{0, 0, 1, 0, 1, 1, 1, 2}, // Chữ L ngược
		{0, 0, 1, 0, 2, 0, 0, 1},
		{0, 0, 0, 1, 0, 2, 1, 2},
		{2, 0, 0, 1, 1, 1, 2, 1}
	},
	{	{0, 0, 0, 1, 1, 1, 1, 2}, // Chữ Z
		{1, 0, 2, 0, 0, 1, 1, 1},
		{0, 0, 0, 1, 1, 1, 1, 2},
		{1, 0, 2, 0, 0, 1, 1, 1}
	},
	{	{1, 0, 0, 1, 1, 1, 0, 2}, // Chữ Z ngược
		{0, 0, 1, 0, 1, 1, 2, 1},
		{1, 0, 0, 1, 1, 1, 0, 2},
		{0, 0, 1, 0, 1, 1, 2, 1}
	}
};

//Welcome

#include<iostream>
#include<windows.h>
#include<conio.h>
#include<cmath>
#include<string>
#include<thread>
using namespace std;

void gotoxy(short a, short b)
{
	COORD coordinates;
	coordinates.X = a;
	coordinates.Y = b;
	SetConsoleCursorPosition(
		GetStdHandle(STD_OUTPUT_HANDLE), coordinates);
}

char convert(int a) {  // this function converts a number to a piece
	char x;
	if (abs(a) == 1)
		x = 'P';
	else if (abs(a) == 2)
		x = 'R';
	else if (abs(a) == 3)
		x = 'N';
	else if (abs(a) == 4)
		x = 'B';
	else if (abs(a) == 5)
		x = 'Q';
	else if (abs(a) == 6)
		x = 'K';
	else if (abs(a) == 7)
		x = 254;
	else
		x = ' ';
	return x;
}
class PlayerInfo {
	static int enpassant;
public:
	static void findKing(int x, int& whereRow, int& whereCol, int** grid) {
		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 8; j++) {
				if (grid[i][j] / float(x) > 0 && abs(grid[i][j]) == 6) {
					whereRow = i;
					whereCol = j;
				}

			}
		}
	}
	static int getEnpassant() {
		return enpassant;
	}
	static void setEnpassant(int x) {
		enpassant = x;
	}
};
int PlayerInfo::enpassant;
class Rook {
protected:
	bool move(int whereRow, int whereCol, int toRow, int toCol, int x, int** grid) {
		if (toRow == whereRow) {
			int a = (toCol - whereCol) / abs(toCol - whereCol), ai = a;
			for (; grid[whereRow][whereCol + a] == 0 && whereCol + a != toCol; a += ai);
			if (whereCol + a == toCol) {
				return 1;
			}
			else
				return  0;

		} // if Column is same
		if (toCol == whereCol) {
			int a = (toRow - whereRow) / abs(toRow - whereRow), ai = a;
			for (; grid[whereRow + a][whereCol] == 0 && whereRow + a != toRow; a += ai);
			if (whereRow + a == toRow) {
				return 1;
			}
			else
				return  0;
		}
		return 0;
	}
};
class Pawn {
protected:
	bool move(int whereRow, int whereCol, int toRow, int toCol, int x, int** grid) {
		int v = 1, * where = &grid[whereRow][whereCol], * to = &grid[toRow][toCol];// validity of move
		// check if piece is valid for two steps
		if ((whereRow == 6 || whereRow == 1) && (whereCol == toCol && whereRow + x * 2 == toRow)) { // for player 1 move will be in postiive direction 
			if (grid[whereRow + x * 1][whereCol] == 0 && grid[toRow][toCol] == 0) {
				v = 1;
			}
			else
				v = 0;
		} // for one step
		else if (whereCol == toCol && whereRow + x * 1 == toRow && grid[toRow][toCol] == 0) {
			v = 1;
		} // can move through slope
		else if ((whereRow + x == toRow) && (abs(toCol - whereCol) == 1)) {
			if (*to / float(x) < 0) {
				v = 1;
			}
			else if (*to == 0 && grid[toRow - x][toCol] / x == -1) {
				PlayerInfo::setEnpassant(1);
				v = 1;
			}
			else
				v = 0;
		}
		else
			v = 0;
		return v;

	}
};
class Knight {
protected:
	bool move(int whereRow, int whereCol, int toRow, int toCol, int x, int** grid) {
		int a = x / abs(x);
		if ((whereRow + 2 == toRow || whereRow - 2 == toRow) && (whereCol + 1 == toCol || whereCol - 1 == toCol) || (whereCol + 2 == toCol || whereCol - 2 == toCol) && (whereRow + 1 == toRow || whereRow - 1 == toRow)) {
			return 1;
		}
		else
			return 0;
	}
};
class Bishop {
protected:
	bool move(int whereRow, int whereCol, int toRow, int toCol, int x, int** grid) {
		if (abs(whereRow - toRow) == abs(whereCol - toCol)) {
			int a = (toRow - whereRow) / abs(toRow - whereRow), b = (toCol - whereCol) / abs(toCol - whereCol), ai = a, bi = b;
			for (; grid[whereRow + a][whereCol + b] == 0 && whereRow + a != toRow; a += ai, b += bi);

			if ((whereRow + a) == toRow && (whereCol + b) == toCol) {
				return 1;
			}
			else
				return 0;
		}
		else
			return 0;

		return 0;
	}
};
class Queen : public Bishop, public Rook {
protected:
	bool move(int whereRow, int whereCol, int toRow, int toCol, int x, int** grid) {
		if (Bishop::move(whereRow, whereCol, toRow, toCol, x, grid) == 1 || Rook::move(whereRow, whereCol, toRow, toCol, x, grid) == 1)
			return 1;

		return 0;
	}
};
class King {
protected:
	bool move(int whereRow, int whereCol, int toRow, int toCol, int x, int** grid) {
		if ((abs(whereRow - toRow) == 1 && abs(whereCol - toCol) == 1) || (abs(toRow - whereRow) == 1 && toCol == whereCol) || (abs(toCol - whereCol) == 1 && toRow == whereRow)) {
			return 1;
		}
		return 0;
	}
};

class  Moving : public Knight, public King, public Queen, public Pawn {
public:

	int whereRow, whereCol, toRow, toCol, * where, * to, x, ** grid;

	void setMoving(int wRow, int wCol, int tRow, int tCol, int x, int** grid) {
		this->grid = grid;
		whereRow = wRow;
		whereCol = wCol;
		toRow = tRow;
		toCol = tCol;
		where = &grid[whereRow][whereCol];
		to = &grid[toRow][toCol];
		this->x = x;
	}


	int canMoveTo() {
		int valid = 1;
		//all moving pieces must satisfy these all conditions

		if (whereRow < 0 || whereRow>7 || toRow < 0 || toRow>7 || whereCol < 0 || whereCol>7 || toCol < 0 || toCol>7 || (toRow == whereRow && toCol == whereCol) || grid[toRow][toCol] / float(x) > 0 || grid[whereRow][whereCol] / float(x) <= 0) {
			valid = 0;
			goto end;
		}
		if (abs(*where) == 1)
			valid = Pawn::move(whereRow, whereCol, toRow, toCol, x, grid);
		else if (abs(*where) == 2)
			valid = Rook::move(whereRow, whereCol, toRow, toCol, x * 2, grid);
		else if (abs(*where) == 3)
			valid = Knight::move(whereRow, whereCol, toRow, toCol, x * 3, grid);
		else if (abs(*where) == 4)
			valid = Bishop::move(whereRow, whereCol, toRow, toCol, x * 4, grid);
		else if (abs(*where) == 5)
			valid = Queen::move(whereRow, whereCol, toRow, toCol, x * 5, grid);
		else if (abs(*where) == 6)
			valid = King::move(whereRow, whereCol, toRow, toCol, x * 6, grid);
		else
			valid = 0;  // means invalid move  
	end:
		return valid;
	}
};
class Check :public PlayerInfo { // this class will check if king is in check by using the PlayerInfo class
public:
	bool kingChecked(int x, int** grid) {
		Moving mv;
		int whereRow, whereCol;
		findKing(x, whereRow, whereCol, grid);
		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 8; j++) {
				int piece = grid[i][j];
				if (piece / float(x) < 0) {
					mv.setMoving(i, j, whereRow, whereCol, piece / abs(piece), grid);
					if (mv.canMoveTo())
						return 1;
				}
			}
		}
		return 0;
	}
};
class AfterMoving : public Moving, public Check {
public:
	int tempWhere, tempTo;
	AfterMoving(int wRow, int wCol, int tRow, int tCol, int x, int** grid) {
		setMoving(wRow, wCol, tRow, tCol, x, grid);
		tempWhere = 0; tempTo = 0;
	}
	void temporaryMove() {
		if (getEnpassant()) { // if the piece is a pawn and has moved enpassant
			tempWhere = *where;
			tempTo = grid[toRow - x][toCol];
			grid[toRow - x][toCol] = 0;
		}
		else {
			tempWhere = *where;
			tempTo = *to;
		}
		*to = *where;
		*where = 0;
	}
	void temporaryBack() {

		if (getEnpassant()) {
			*where = tempWhere;
			grid[toRow - x][toCol] = tempTo;
			*to = 0;
		}
		else {
			*where = tempWhere;
			*to = tempTo;
		}

	}
	bool presentKingNotChecked() {
		bool v = 0;
		if (v = canMoveTo()) {
			temporaryMove();
			v = !kingChecked(x, grid);
			temporaryBack();
		}
		return v;
	}
	bool presentKingNotChecked(string& dialog) {
		bool v = 0;
		if (v = canMoveTo()) {
			temporaryMove();
			v = !kingChecked(x, grid);
			temporaryBack();
			if (v == 0) {
				if (abs(*where) == 6)
					dialog = "The King cannot move there";
				else
					dialog = "The piece is pinned";
			}
		}
		return v;
	}
	void moveTo(string& player1, string& player2) {
		if (*to / float(x) < 0||getEnpassant()) {
			(x > 0) ? player1 = player1 + convert((getEnpassant())?x*-1:*to) + " " : player2 = player2 + convert((getEnpassant()) ? x * -1 : *to) + " ";
		}
		*to = *where;
		*where = 0;
		if (getEnpassant()) {
			grid[toRow - x][toCol] = 0;
			setEnpassant(0);
		}
			

		if (abs(*to) == 1 && (toRow == 7 || toRow == 0)) {
			char h;
			while (1) {
				cout << "What to become N, Q, B, R: ";
				cin >> h;
				if (h == 'R') {
					*to = x * 2;
					break;
				}
				else if (h == 'N') {
					*to = x * 3;
					break;
				}

				else if (h == 'B') {
					*to = x * 4;
					break;
				}
				else if (h == 'Q') {
					*to = x * 5;
					break;
				}

			}
		}

	}
	bool oppositeKingCheck(string& dialog) {
		if (kingChecked(x * (-1), grid)) {
			dialog += dialog + "King " + to_string((x == 1) ? 2 : 1) + " is Checked";
			return 1;
		}
		return 0;
	}
};
class CheckMate : PlayerInfo {
	int kingRow, kingCol, checkRow, checkCol, x, ** grid;
public:
	CheckMate(int checkRow, int checkCol, int x, int** grid) {
		this->checkRow = checkRow;
		this->checkCol = checkCol;
		this->x = x;
		this->grid = grid;
		findKing(x, kingRow, kingCol, grid);
	}
	bool kingCheckMate() {
		if (canKingGetOutOfCheck() || canAnyPieceCaptureThePiece())
			return 1;
		return 0;
	}
	bool canKingGetOutOfCheck() {
		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 8; j++) {
				AfterMoving am(kingRow, kingCol, i, j, x, grid);
				if (am.presentKingNotChecked())
					return 0;
			}
		}
		return 1;
	}
	bool canAnyPieceCaptureThePiece() {
		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 8; j++) {
				for (int k = 0; k < 8; k++) {
					for (int m = 0; m < 8; m++) {
						AfterMoving am(i, j, m, k, x, grid);
						if (am.presentKingNotChecked())
							return 0;
					}
				}

			}
		}
		return 1;
	}
};

class Highlight {
	int** highlightArray;
public:
	Highlight() {
		highlightArray = new int* [8];
		for (int i = 0; i < 8; i++)
			highlightArray[i] = new int[8];

	}
	void Intitialize() {
		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 8; j++)
				highlightArray[i][j] = 0;
		}
	}
	int** highlight(int whereRow, int whereCol, int x, int** grid) {
		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 8; j++) {
				AfterMoving am(whereRow, whereCol, i, j, x, grid);
				if (am.presentKingNotChecked()) {
					highlightArray[i][j] = x * 7;
				}
			}
		}
		return highlightArray;
	}

	~Highlight() {
		delete[] highlightArray;
	}
};
class Chess {
	void Instructions() {
		cout << "\n\nIn chess there are two players which play against each other\n"
			"\nEach player will be asked the position of the piece which he wants to move and then where to move\n"
			"\nIf the move is valid the then piece will be moved there\n";
		_getch();
		system("cls");
	}
	static int whichPlayer;
	 int** grid;
	 string player1, player2, dialog;
	static int player1Min, player1Sec, player2Min, player2Sec, win;
public:
	Chess() {
		grid = new int* [8];
		for (int i = 0; i < 8; i++)
			grid[i] = new int[8];
	}
	void start() {
		string choice;

		while (1) {
			system("cls");
			gotoxy(33, 10);
			cout << "Welcome to Go Chess";
			gotoxy(35, 12);
			cout << "1. Start";
			gotoxy(35, 14);
			cout << "2. Instructions";
			gotoxy(35, 16);
			cout << "3. exit";
			gotoxy(35, 18);
			cin >> choice;
			if (choice.length() > 1)
				continue;
			
			if (choice == "1") {
				player1Min = 10; player2Min = 10; player1Sec = 0; player2Sec = 0; whichPlayer = 1;
				filler();
				//thread obj(timmer);
				collector();
				
			}
			else if (choice == "2") {
				system("cls");
				Instructions();
			}
			else if (choice == "3")
				exit(0);
		}

	}
	void printMenu(int** grid) {
		int k = 8;
		for (int i = 0; i < 8; i++) {
			cout << "\033[0;37m" << k-- << "  ";
			for (int j = 0; j < 8; j++) {
				int a = grid[i][j];
				char x = convert(a);

				if (a < 0)
					cout << "\033[0;32m" << x << "   ";
				else
					cout << "\033[0;36m" << x << "   ";
			}

			cout << endl << endl;
		}
		cout << "   ";
		for (char j = 'a'; j <= 'h'; j++)
			cout << "\033[0;37m" << j << "   ";
		gotoxy(50, 2);
		cout << "\033[0;36mPlayer1: ";
		gotoxy(50, 4);
		cout << "\033[0;32m" << player1;
		gotoxy(50, 6);
		cout << "\033[0;32mPlayer2: ";
		gotoxy(50, 8);
		cout << "\033[0;36m" << player2 << "\033[0;37m";
		gotoxy(50, 12);
		cout << "\033[0;36m" << dialog;
	}

	bool takeInput(int& whereRow, int& whereCol, int& toRow, int& toCol) {
		string where, to;
		gotoxy(1, 18);
		cout << "\nPlayer: " << whichPlayer << "\nSelect piece: ";
		
		cin >> where;
		if (where.length() > 2)
			return 0;
		whereRow = (8 - (where[1] - '0')), whereCol = int(where[0]) - 97;
		int x = (whichPlayer == 1) ? 1 : -1;
		Highlight hl;
		hl.Intitialize();
		gotoxy(0, 0);
		printMenu(hl.highlight(whereRow, whereCol, x, grid));

		gotoxy(0, 21);
		cout << "Move to: ";
		
		cin >> to;
		if (to.length() > 2)
			return 0;
		toRow = (8 - (to[1] - '0')), toCol = int(to[0]) - 97;

		return 1;
	}
	void filler() { // This function sets pieces on the board
		for (int i = 2; i <= 5; i++) {
			for (int j = 0; j < 8; j++)
				grid[i][j] = 0;
		}
		for (int i = 0; i < 8; i++) {
			grid[1][i] = 1;
			grid[6][i] = -1;
		}
		grid[0][0] = 2;
		grid[0][7] = 2;
		grid[7][0] = -2;
		grid[7][7] = -2;


		grid[0][1] = 3;
		grid[0][6] = 3;
		grid[7][1] = -3;
		grid[7][6] = -3;

		grid[0][2] = 4;
		grid[0][5] = 4;
		grid[7][2] = -4;
		grid[7][5] = -4;

		grid[0][3] = 5;
		grid[7][3] = -5;

		grid[0][4] = 6;
		grid[7][4] = -6;

	}
	~Chess() {
		delete grid;
	}
	  void collector() {
		  while (1) {
			  if (((player1Min <= 0 && player1Sec <= 0) || (player2Min <= 0 && player2Sec <= 0)) && (win == 1)) {
				  system("cls");
				  break;
			  }
			  else {
				  system("cls");
				  printMenu(grid);
				  int whereRow, whereCol, toRow, toCol, v, x = (whichPlayer == 1) ? 1 : -1;;
				  v = takeInput(whereRow, whereCol, toRow, toCol);
				  if (v) {
					  AfterMoving am(whereRow, whereCol, toRow, toCol, x, grid);
					  dialog = "\0";
					  if (am.presentKingNotChecked(dialog)) { // if king is not checked
						  am.moveTo(player1, player2);
						  if (am.oppositeKingCheck(dialog)) {
							  CheckMate check(toRow, toCol, x * -1, grid);
							  if (check.kingCheckMate()) {
								  system("cls");
								  win = 1;
								  cout << "Player " << whichPlayer << " wins the game";
								  Sleep(3000);
								  break;
							  }
						  }
					  }
					  else
						  v = 0;
				  }
				  if (v == 1)
					  whichPlayer = 1 + (whichPlayer % 2);
			  }
		  }
	}
	static void timmer() {
		while ((player1Min > 0 || player1Sec > 0) && (player2Min > 0 || player2Sec > 0)&&(win==0)) {
			int x = (whichPlayer == 1) ? 1 : -1;
			gotoxy(60, 2);
			cout << "\033[0;36m" << " " << player1Min << ":" << ((player1Sec < 10) ? "0" : "") << player1Sec;
			gotoxy(60, 6);
			cout << "\033[0;32m" << " " << player2Min << ":" << ((player2Sec < 10) ? "0" : "") << player2Sec;
			
			if (x == 1) {
				if (player1Sec == 0) {
					player1Min--;
					player1Sec = 60;
				}
				player1Sec--;
			}
			if (x == -1) {
				if (player2Sec == 0) {
					player2Min--;
					player2Sec = 60;
				}
				player2Sec--;
			}
			gotoxy(14, 20);
			Sleep(1000);
		}
			system("cls");
			gotoxy(30, 18);
			if (player1Min == 0)
				cout << "Player 2" << " wins";
			else
				cout << "Player 1" << " wins";
			cout << " by timeout";
			Sleep(2000);
			win = 1;
			system("cls");
		}
};
int Chess::whichPlayer=1;
int Chess::player1Min;
int Chess::player1Sec;
int Chess::player2Min;
int Chess::player2Sec;
int Chess::win = 0;
int main() {
	Chess ch;
	ch.start();
}

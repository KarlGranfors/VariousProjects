
/* n-dimensional chess
 * breaks if n < 2
 * 
 * board length = len
 * len must be 8 for pawns to work properly
 */

#include <iostream>
#include <math.h>
using namespace std;

class Chess {
	public:
        int n, len, size;
		int* state;
        int turn;

        void displayPiece(int piece) {
            switch (piece) {
                case 0:
                    cout << ".";
                    break;
                case 1:
                    cout << "K";
                    break;
                case 2:
                    cout << "Q";
                    break;
                case 3:
                    cout << "B";
                    break;
                case 4:
                    cout << "N";
                    break;
                case 5:
                    cout << "R";
                case 6:
                    break;
                    cout << "P";
                    break;
                case 11:
                    cout << "k";
                    break;
                case 12:
                    cout << "q";
                    break;
                case 13:
                    cout << "b";
                    break;
                case 14:
                    cout << "n";
                    break;
                case 15:
                    cout << "r";
                    break;
                case 16:
                    cout << "p";
                    break;
                default:
                    break;
            }

            cout << " ";
        }

        //Will mirror the board due to bottom line being displayed first
		//Coordinates counted from top left
		void display() {

			//Header
			cout << "Turn: " << turn << endl;

			//Board
			for (int i = 0; i < size; i++) {
				//if (state[i] < 10) {cout << " ";}     //Commented out because of displayPiece
				//cout << state[i] << " ";
                displayPiece(state[i]);

				//Dimension dots
				for (int x = 0; x < n; x++) {
					if ((i + 1) % (int) pow(len, x + 1) == 0) {
						if (x > 0) {
							cout << ".";
							for (int y = 0; y < x; y++) {
								cout << ".";
							}
							cout << endl;
						}
						cout << endl;
					}
				}
			}
		}

        //Will retry strings, will floor floats
        int getInt(int max) {
            while (true) { // Loop until user enters a valid input
                bool printError = true;

                int x;
                cin >> x;
                if (x == 10) {
                    display();
                    printError = false;
                    cout << "From: " << endl;
                }

                // Check for failed extraction or exceeded limits
                if (!cin || x < 0 || x >= max) // has a previous extraction failed?
                {
                    // yep, so let's handle the failure
                    cin.clear(); // put us back in 'normal' operation mode
                    cin.ignore(100, '\n');  // clear up to 100 characters out of the buffer, or until a '\n' character is removed
                    if (printError) {
                        cerr << "Please enter an integer [0-" << max << "]" << endl;
                    }
                }
                else
                {
                    cin.ignore(100, '\n');  // clear up to 100 characters out of the buffer, or until a '\n' character is removed
                    return x;
                }
            }
        }

        int abs(int num) {
            if (num < 0) {num *= -1;}
            return num;
        }
        

		void clear() {
			for (int i = 0; i < size; i++) {
				state[i] = 0;
			}
		}

        Chess(int d, int l) {
            n = d;
            len = l;
            size = pow(len, n);
            state = new int [size]; //Copied from the internet
            turn = 0;
        }

		//2D only
		//Vertical mirroring accounted for, brings white pieces to bottom
		void display2D() { 
			for (int line = len - 1; line >= 0; line--) {
				for (int i = 0; i < len; i++) {
					int v = len*line + i;
					if (state[v] < 10) {cout << " ";}
					cout << state[v] << " ";
				}
				cout << endl;
			}
		}

		

		/*
		 * 0 = empty
		 * 1 = king
		 * 2 = queen
		 * 3 = bishop
		 * 4 = knight
		 * 5 = rook
		 * 6 = pawn
		 *
		 * 11 = black king
		 * ...
		 * 16 = black pawn
		 */

		//Size >= 64
		void set() {

			//Pawns
			for (int i = 8; i < 16; i++) {
				state[i] = 6;
			}
			for (int i = 48; i < 56; i++) {
				state[i] = 16;
			}

			//Black
			state[56] = 15;
			state[57] = 14;
			state[58] = 13;
			state[59] = 12;
			state[60] = 11;
			state[61] = 13;
			state[62] = 14;
			state[63] = 15;

			//White
			state[0] = 5;
			state[1] = 4;
			state[2] = 3;
			state[3] = 2;
			state[4] = 1;
			state[5] = 3;
			state[6] = 4;
			state[7] = 5;
		}


        //Movement
        int* start = new int [n];   //Like below, but without errors
        int* goal = new int [n];    //int start[n], goal[n];
        int iStart, iGoal;

		int  makeIndex(int *coordinates) {
			int index = 0;
			for (int i = 0; i < n; i++) {
				index += coordinates[i]*pow(len, i);
			}
			return index;
		}

        bool white(int piece) {
            if (piece != 0 && piece < 10) {return true;}
            else {return false;}
        }

        bool black(int piece) {
            if (piece > 10) {return true;}
            else {return false;}
        }

        //Checks if number of dimensions used == dims
        bool dimCount(int dims) {
            for (int i = 0; i < n; i++) {
                if (start[i] != goal[i]) {dims--;}
            }
            if (dims == 0) {return true;}
            else {return false;}
        }

        bool getMove() {

            //Who's move
            if (turn % 2 == 0) {
                cout << "White to move" << endl;
            } else {
                cout << "Black to move" << endl;
            }

            //Take input
            cout << "From: " << endl;
            for (int i = 0; i < n; i++) {
                start[i] = getInt(len);
            }
            cout << endl;

            cout << "To: " << endl;
            for (int i = 0; i < n; i++) {
                goal[i] = getInt(len);
            }
            cout << endl;

            //Generate start and goal indexes
			iStart = makeIndex(start);
			iGoal = makeIndex(goal);
            
            return true;
        }

        bool valid() {
            int sPiece = state[iStart], gPiece = state[iGoal];
            int* spaceBetween = new int [n];
            int* movementVector = new int [n];

            //Set movementVector
            for (int i = 0; i < n; i++) {
                movementVector[i] = goal[i] - start[i];
            }

            //Rook
            int rookDir;
            int steps;

            //Pawn
            int sidesteps = 0;
            int direction = 1;
            bool notMoved = false;

            //Queen
            bool isRook, isBishop;

            //Knight
            int singleMoves, doubleMoves;

            //General
            if (sPiece == 0) {
                return false;
            }

            //White or black
            if (turn % 2 == 0) {
                if (black(sPiece)) {
                    return false;
                } else if (white(gPiece)) {
                    return false;
                }
            } else {
                if (white(sPiece)) {
                    return false;
                } else if (black(gPiece)) {
                    return false;
                }
            }

            //Pawn
            if (sPiece % 10 == 6) {

                //Black moves like white but in reverse
                if (black(sPiece)) {
                    direction = -1;
                }

                //Pawns can move 2 steps the first time
                if (len == 8) {
                    if ((white(sPiece) && start[1] == 1) || (black(sPiece) && start[1] == 6)){
                        notMoved = true;
                    } 
                }

                //Movement without capture
                if (gPiece == 0) {
                    for (int i = 0; i < n; i++) {
                        if (i == 1) {
                            if (!notMoved && (goal[1] - start[1] != direction)) {
                                return false;
                            }
                            if (notMoved) {
                                if (goal[1] - start[1] != direction && goal[1] - start[1] != 2*direction) {
                                    return false;
                                }
                            }
                        } else if (goal[i] != start[i]) {
                            return false;
                        }
                    }

                    //Check space between is empty
                    if (notMoved) {
                        for (int i = 0; i < n; i++) {
                            spaceBetween[i] = start[i];
                        }
                        spaceBetween[1] += direction;
                        if (state[makeIndex(spaceBetween)] != 0) {
                            return false;
                        }
                    }
                } 
                
                //Movement with capture
                //Pawns can capture by moving one step forward + one step in at least one other dimension
                else {
                    for (int i = 0; i < n; i++) {
                        if (i == 1) {
                            if (goal[1] - start[1] != direction) {
                                return false;
                            }
                        } else {
                            if (goal[i] - start[i] == 1 || goal[i] - start[i] == -1) {
                                sidesteps++;
                            } else if (goal[i] != start[i]) {
                                return false;
                            }
                        }
                    }
                    if (sidesteps == 0) {
                        return false;
                    }
                }
            }

            //Rooks
            if (sPiece % 10 == 5){
                if (!dimCount(1)) {
                    return false;
                } else {
                    for (int i = 0; i < n; i++) {
                        if (movementVector[i] != 0) {
                            direction = i;
                        }
                    }
                    for (int i = 0; i < n; i++) {
                        spaceBetween[i]=start[i];
                    }
                    steps = movementVector[direction];
                    for (int i = 0; i < (abs(steps) - 1); i++) {
                        spaceBetween[direction] += (steps/abs(steps)); //Adds 1 or -1
                        if (state[makeIndex(spaceBetween)] != 0) {
                            return false;
                        }
                    }
                }
            }

            //Bishops
            if (sPiece % 10 == 3) {
                steps = 0;
                for (int i = 0; i < n; i++) {
                    if (i == 0) {
                        steps = abs(movementVector[i]);
                    } else if (abs(movementVector[i]) != steps) {
                        return false;
                    }
                }

                //Check spaces between
                for (int i = 0; i < n; i++) {
                    spaceBetween[i]=start[i];
                }
                for (int i = 0; i < (abs(steps) - 1); i++) {
                    for (int d = 0; d < n; d++) {
                        spaceBetween[d] += (movementVector[d]/steps);
                    }

                    if (state[makeIndex(spaceBetween)] != 0) {
                        return false;
                    }
                }
            }

            //Queen
            //Can move as either bishop or rook
            if (sPiece % 10 == 2) {

                isRook = isBishop = true;
                //Check if bishop
                for (int i = 0; i < n; i++) {
                    if (i == 0) {
                        steps = abs(movementVector[i]);
                    } else if (abs(movementVector[i]) != steps) {
                        isBishop = false;
                    }
                }

                //Check if rook
                if (!dimCount(1)) {
                    isRook = false;
                }

                //Rules for rook
                if (isRook) {

                    //Set direction
                    for (int i = 0; i < n; i++) {
                        if (movementVector[i] != 0) {
                            direction = i;
                        }
                    }

                    //Check spaces between
                    for (int i = 0; i < n; i++) {
                        spaceBetween[i]=start[i];
                    }
                    steps = movementVector[direction];  //Redefines steps to be + or -
                    for (int i = 0; i < (abs(steps) - 1); i++) {
                        spaceBetween[direction] += (steps/abs(steps)); //Adds 1 or -1

                        if (state[makeIndex(spaceBetween)] != 0) {
                            return false;
                        }
                    }

                //Rules for bishop
                } else if (isBishop) {

                    //Check spaces between
                    for (int i = 0; i < n; i++) {
                        spaceBetween[i]=start[i];
                    }
                    for (int i = 0; i < (abs(steps) - 1); i++) {
                        for (int d = 0; d < n; d++) {
                            spaceBetween[d] += (movementVector[d]/steps);
                        }

                        if (state[makeIndex(spaceBetween)] != 0) {
                            return false;
                        }
                    }

                } else {
                    return false;
                }
            }

            if (sPiece % 10 == 1) {
                for (int i = 0; i < n; i++) {
                    if (abs(movementVector[i]) > 1) {
                        return false;
                    }
                }
            }

            if (sPiece % 10 == 4) {
                singleMoves = doubleMoves = 0;
                for (int i = 0; i < n; i++) {
                    if (abs(movementVector[i]) == 1) {singleMoves++;} 
                    else if (abs(movementVector[i]) == 2) {doubleMoves++;}
                }
                if (doubleMoves == 1 && singleMoves == 1) {return true;}
                else {return false;}
            }

			return true;
        }

        bool move() {
            if (valid()) {
				state[iGoal] = state[iStart];
				state[iStart] = 0;
                return true;
            } else {
                return false;
            }
        }

        void doTurn() {
            bool moveDone = false;
            while (!moveDone) {
                getMove();
                if(move()) {
                    turn++;
                    moveDone = true;
                } else {
                    cout << "Invalid move" << endl;
                }
            }
            
        }

        bool checkIfWon() {
            int kings = 0;
            for(int i = 0; i < size; i++) {
                if (state[i] % 10 == 1) {
                    kings++;
                }
            }
            if (kings < 2) {return true;}
            else {return false;}
        }


};

int main() {;

    cout << "Number of dimensions: ";
    int dimensions;
    cin >> dimensions;

	Chess board(dimensions, 8);
	board.clear();
	board.set();

    while (!board.checkIfWon()) {
        //board.display();
        board.doTurn();
    }

    board.display();

    if (board.turn % 2 == 1) {
        cout << "White wins" << endl;
    } else {
        cout << "Black wins" << endl;
    }

   	return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef uint64_t board;

board movements[31];

void printBoard(board bd){
    for(int i=0; i<33; i++){
        if(i==0||i==3||i==27||i==30) printf("    ");
        printf("%c ", 1&(bd>>i)?'o':'.');
        if(i==2||i==5||i==12||i==19||i==26||i==29||i==32) printf("\n");
    }
    printf("\n");
}

int down(int i){
	if(i<3 || (i>26 && i<30))
		return i+3;
	if((i>2 && i<6)||(i>21 && i<25))
		return i+5;
	if(i>5 && i < 20)
		return i+7;
	return -1;

}

int up(int i){
	if((i>2 && i<6)||(i>29 && i< 33))
		return i-3;
	if(i>12 && i < 27)
		return i-7;
	if((i>7 && i < 11)||(i>26&&i<30))
		return i-5;
	return -1;
}

int right(int i){
	if(i==2||i==5||i==12||i==19||i==26||i==29||i==32)
		return -1;
	return i+1;
}

int left(int i){
	if(i==0||i==3||i==6||i==13||i==20||i==27||i==30)
		return -1;
	return i-1;
}

int boardArray[11][11]={
    {0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,1,1,1,0,0,0,0},
    {0,0,0,0,1,1,1,0,0,0,0},
    {0,0,1,1,1,1,1,1,1,0,0},
    {0,0,1,1,1,1,1,1,1,0,0},
    {0,0,1,1,1,1,1,1,1,0,0},
    {0,0,0,0,1,1,1,0,0,0,0},
    {0,0,0,0,1,1,1,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0}
};

int moves[76][3];

int solved=0;

void setMoves(){
    int currLine=0, boardPosition=0;
    for(int i=2; i<9; i++){
        for(int k=2; k<9; k++){
            if(boardArray[i][k]){
                if(boardArray[i-1][k] && boardArray[i-2][k]){
                    moves[currLine][0]=boardPosition;
                    moves[currLine][1]=up(moves[currLine][0]);
                    moves[currLine][2]=up(moves[currLine][1]);
                    currLine++;
                }
                if(boardArray[i][k+1] && boardArray[i][k+2]){
                    moves[currLine][0]=boardPosition;
                    moves[currLine][1]=right(moves[currLine][0]);
                    moves[currLine][2]=right(moves[currLine][1]);
                    currLine++;
                }if(boardArray[i+1][k] && boardArray[i+2][k]){
                    moves[currLine][0]=boardPosition;
                    moves[currLine][1]=down(moves[currLine][0]);
                    moves[currLine][2]=down(moves[currLine][1]);
                    currLine++;
                }
                if(boardArray[i][k-1] && boardArray[i][k-2]){
                    moves[currLine][0]=boardPosition;
                    moves[currLine][1]=left(moves[currLine][0]);
                    moves[currLine][2]=left(moves[currLine][1]);
                    currLine++;
                }
                boardPosition++;
            }
        }
    }
}

board initialBoard(){
    board bd=~(1ull<<16);
    return bd;
}

board setPosition(board bd, int pos, board setState){
    if(setState) return bd | 1ull<<pos;
    return bd & ~(1ull<<pos);
}

int getPosition(board bd, int pos){
    return 1&(bd>>pos);
}

int isSolved(board b){
    int count_1=0;
    for(int i=0; i<33; i++){
        if(getPosition(b, i)) count_1++;
    }
    return count_1==1;
}

void solve(board b, int depth){
    if(depth>31) return;
    if(isSolved(b)){
        solved=1;
        return;
    }
    board mask, trgt;
    for(int i=0; i<76; i++){
        if(solved) break;
        trgt=(1ull<<moves[i][0]) | (1ull<<moves[i][1]);
        mask=trgt | (1ull<<moves[i][2]);
        if((mask&b)==trgt){
            movements[depth]=mask;
            solve(b^mask, depth+1);
        }
    }
}

int main()
{
    setMoves();
    board b=initialBoard();
    solve(b, 0);
    printBoard(b);
    for(int i=0; i<31; i++){
        b=b^movements[i];
        printBoard(b);
    }

    return 0;
}

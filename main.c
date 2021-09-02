#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef uint64_t board;

int jumps[31];

long long int explored=0;

void printBoard(board bd){
    for(int i=0; i<33; i++){
        if(i==0||i==3||i==27||i==30) printf("    ");
        printf("%c ", 1&(bd>>i)?'o':'.');
        if(i==2||i==5||i==12||i==19||i==26||i==29||i==32) printf("\n");
    }
    printf("\n");
}

board initialBoard(){
    board bd=~(1ull<<16);
    return bd;
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

board masks[76];

board pegs[76];

int moves[76][3];

int solved=0;

void setPegsMasks(){
    int currLine=0, boardPosition=0;
    int next, nNext;
    for(int i=2; i<9; i++){
        for(int k=2; k<9; k++){
            if(boardArray[i][k]){
                if(boardArray[i-1][k] && boardArray[i-2][k]){
                    next=up(boardPosition);
                    nNext=up(next);
                    pegs[currLine]=1ull<<boardPosition | 1ull<<next;
                    masks[currLine]=pegs[currLine] | 1ull<<nNext;
                    currLine++;
                }
                if(boardArray[i][k+1] && boardArray[i][k+2]){
                    next=right(boardPosition);
                    nNext=right(next);
                    pegs[currLine]=1ull<<boardPosition | 1ull<<next;
                    masks[currLine]=pegs[currLine] | 1ull<<nNext;
                    currLine++;
                }if(boardArray[i+1][k] && boardArray[i+2][k]){
                    next=down(boardPosition);
                    nNext=down(next);
                    pegs[currLine]=1ull<<boardPosition | 1ull<<next;
                    masks[currLine]=pegs[currLine] | 1ull<<nNext;
                    currLine++;
                }
                if(boardArray[i][k-1] && boardArray[i][k-2]){
                    next=left(boardPosition);
                    nNext=left(next);
                    pegs[currLine]=1ull<<boardPosition | 1ull<<next;
                    masks[currLine]=pegs[currLine] | 1ull<<nNext;
                    currLine++;
                }
                boardPosition++;
            }
        }
    }
}

board setPosition(board bd, int pos, board setState){
    if(setState) return bd | 1ull<<pos;
    return bd & ~(1ull<<pos);
}

int getPosition(board bd, int pos){
    return 1&(bd>>pos);
}

int solNum=0, maxResults=0, showAllResults=0, showExploredNumber=0;

void showSolution(){
    printf("%d> ", solNum);
    for(int i=0;i<31; i++)
        printf("%d ", jumps[i]);
    printf("\n");
}

void solve(board b, int depth){
    if(showExploredNumber) explored++;
    if(depth>30){
        if(solNum>=maxResults-1)
            solved=1;
        if(showAllResults){
            solNum++;
            showSolution();
        }
        return;
    }

    for(int i=0; i<76; i++){
        if(solved) break;
        if((masks[i]&b)==pegs[i]){
            jumps[depth]=i;
            solve(b^masks[i], depth+1);
        }
    }
}

int main()
{
    setPegsMasks();
    board b=initialBoard();

    maxResults=20000;
    showAllResults=1;
    showExploredNumber=1;
    solve(b, 0);

    printBoard(b);
    for(int i=0; i<31; i++){
        b=b^masks[jumps[i]];
        printBoard(b);
    }
    if(showExploredNumber) printf("%d nodes explored\n", explored);

    return 0;
}

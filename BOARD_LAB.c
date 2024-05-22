#include <stdio.h>
#include <malloc.h>
#include <assert.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include "board.h"

  int visited[WH_BOARD-2][WH_BOARD-2]={0};
// Initialize a new Game for the nQueens problem: an empty board..
Item *initGame()
{
  int i;
  Item *node;

	char *initial = (char*)malloc(MAX_BOARD*sizeof(char));
	for (int i=0; i<MAX_BOARD; i++) initial[i] = -1;

    node = nodeAlloc();
	initBoard(node, initial);
  generer(node,WH_BOARD+1);
  for(int i = 1; i < WH_BOARD-1 ; i++ ){
  	for (int j = 1 ; j<WH_BOARD-1 ; j++) {
  		if(isole(node,i,j) ==1) {
  		if(i==1) node->board[(i+1) *WH_BOARD + j] =0;
  		else if(i==WH_BOARD-2) node->board[(i-1) *WH_BOARD + j] =0;
  		else if(j==1) node->board[i *WH_BOARD + j+1] =0;
  		else if(j==WH_BOARD-2) node->board[i *WH_BOARD + j-1] =0;
  		
  		
  		
  		
  		
  		
  		
  		
  		
  		
  		
  		
  		
  		
  		
  		
  		
  		
  		//(WH_BOARD-2)*WH_BOARD+ WH_BOARD-2
        //node->board[(i-1)*WH_BOARD+j]=0;
        node->board[(i)*WH_BOARD+j]=0;}
  		}
  		}
  
  node->board[(WH_BOARD-2)*WH_BOARD+ WH_BOARD-2]=99; 
 
  node->depth = 0;
  node->blank = (WH_BOARD-2)*WH_BOARD+ WH_BOARD-2; // node->board[(WH_BOARD-2)*WH_BOARD+ WH_BOARD-2]

  return node;
}

// print a board
void printBoard( Item *node )
{
  assert(node);
	printf("\n");
	for (int j=0; j<WH_BOARD; j++) if (j==0) printf(" ___"); else printf("____"); printf("\n");
	for (int i = 0 ; i < MAX_BOARD ; i++) {
    if (i%WH_BOARD == 0) printf("|");
    if (node->board[i] == 0) printf("   |");
    else printf("%2d |", node->board[i]);
    if (((i+1)%WH_BOARD) == 0) {
			printf("\n");
			for (int j=0; j<WH_BOARD; j++) if (j==0) printf(" ___"); else printf("____"); printf("\n");
		}
  }
	printf("\n");
}


// initialize node's state from a given board
void initBoard(Item *node, char *board) {
	assert( node );

	node->size = MAX_BOARD;
    node->board = (char*)calloc(MAX_BOARD, sizeof(char));
	/* Copy board */
    for(int i=0; i<MAX_BOARD; i++) {
    	node->board[i] = board[i];
    	 }

}




// Return 0 if all queens are placed. Positive otherwise
// ie: nb queens that still need to be placed.
double evaluateBoard(Item *node) {
	if(node->board[WH_BOARD+1]==99) return 0;
  	return 1;
}


// Test if position pos is valid with respect to node's state
// nQueens -> not same row ; not same column ; not same diagonal
int isValidPosition( Item *node, int pos )
{
  int i = pos / WH_BOARD;
	int j = pos % WH_BOARD;
  int N = i*WH_BOARD+j;

  int ii = node->blank / WH_BOARD;
  int jj = node->blank % WH_BOARD;

  if(node->board[pos]==-1) return 0;
  if ( abs(pos - node->blank) == 1 && abs(i-ii) == 0  && abs(jj-j) == 1) return 1;      //HAROUN
  else if (abs(node->blank - pos) == WH_BOARD && abs(j-jj) == 0 && abs(ii-i) == 1) return 1;
  else return ((WH_BOARD-2)*WH_BOARD + WH_BOARD -2 )- node->blank;
}

int distance_fin ( Item *node )
{
  int dist;
  int i = node->blank / WH_BOARD;
  int j = node->blank % WH_BOARD;
  int ii = WH_BOARD-2;
  int jj = WH_BOARD-2;

  dist = abs(i-ii)+abs(j-jj);

  return dist;
}

// Return a new item where a new queen is added at position pos if possible. NULL if not valid
Item *getChildBoard( Item *node, int pos )
{
  Item *child_p = NULL;


  if ( isValidPosition(node, pos)==1 ) {

    /* allocate and init child node */
    child_p=nodeAlloc();
    initBoard(child_p,node->board);


		/* Make move */

    child_p->board[pos]=99;
    child_p->board[node->blank]=0;
    child_p->blank = pos;

		/* link child to parent for backtrack */
		child_p->parent=node;

		child_p->depth=node->depth+1;

    /* COST */
      child_p->g=child_p->depth;
      child_p->h = evaluateBoard(child_p);
      child_p->f =child_p->g;

    printBoard(node);



  }

  return child_p;
}

void generer(Item * node,int pos){  // genère un board aléatoire de labyrinthe
	node->board[pos] = 0;
	int x = pos/WH_BOARD;
	int y = pos%WH_BOARD;

	visited[x-1][y-1]=1;

	int moves[4][2] = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};

	for ( int i =0; i<4;i++) {
		int r = rand() %4;
		int tempX= x + moves[r][0]*2;
		int tempY= y + moves[r][1]*2;

		if(tempX > 0 && tempX < WH_BOARD -1 && tempY >0 && tempY < WH_BOARD -1  && !visited[tempX-1][tempY-1]) {
			node->board[(x+moves[r][0])*WH_BOARD + y + moves[r][1] ] = 0;
			generer(node,tempX*WH_BOARD+tempY);
		}
  }
}

bool isole(Item * node,int i , int j ) {
	if(node->board[(i-1)*WH_BOARD+j] == -1 && node->board[(i+1)*WH_BOARD+j] == -1 && node->board[i*WH_BOARD+j-1] == -1 && node->board[i*WH_BOARD+j+1] == -1) return true;
	return false;}

#include "list.h"
#include <stdbool.h>

#define RANDINIT()  srand(time(NULL))
#define RANDOM()  ((float)rand() / (float)RAND_MAX)
#define RANDMAX(x)  (int)((float)(x)*rand()/(RAND_MAX+1.0))


Item *initGame();

void initBoard(Item *node, char *board);


double evaluateBoard( Item *node );

void printBoard( Item *board );
Item *getChildBoard( Item *node, int pos );
void generer(Item * node,int pos);
bool isole(Item * node,int i , int j );

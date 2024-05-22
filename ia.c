#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <time.h>
#include <malloc.h>
#include "list.h"
#include "board.h"
list_t openList_p;
list_t closedList_p;
int positions[100]; // Adjust the size as needed
int position_count = 0;

void collectSolutionPath(Item *goal) {
  while (goal) {
    // Find and record the position of '99' in the board
    for (int pos = 0; pos < MAX_BOARD; pos++) {
      if (goal->board[pos] == 99) {
        positions[position_count++] = pos;
      }
    }
    goal = goal->parent;
  }
}

void showSolution(Item *goal) {
  int i = 0;

  printf("\nSolution:");

  collectSolutionPath(goal);

  // Print boards from the start to the end of the solution
  for (i = position_count - 1; i >= 0; i--) {
    printBoard(goal);
    goal = goal->parent;
  }

  printf("\nLength of the solution = %d\n", position_count - 1);
  printf("Size of open list = %d\n", openList_p.numElements);
  printf("Size of closed list = %d\n", closedList_p.numElements);

  // Print recorded positions in correct order
  printf("Positions of 99:\n");
  for (int j = position_count - 1; j >= 0; j--) {
    printf("%d; ", positions[j]);
  }
  printf("\n");
}


void ucs(void) {
    Item *cur_node, *child_p;
    int i;

    while (listCount(&openList_p) != 0) { // Tant qu'il y a des éléments dans la liste ouverte
        // Obtenir le meilleur élément de la liste ouverte
        cur_node = popBest(&openList_p);

        // Traces de débogage
        printf("Exploring node with f = %f, openList size = %d\n", cur_node->f, listCount(&openList_p));

        // Avons-nous une solution?
        if (evaluateBoard(cur_node) == 0.0) {
            showSolution(cur_node);
            return;
        }

        // Ajouter le nœud actuel à la liste fermée
        addFirst(&closedList_p, cur_node);

        // Énumérer les états adjacents
        for (int i = 0; i < MAX_BOARD; i++) {
            child_p = getChildBoard(cur_node, i);
            if (child_p != NULL) { // C'est un enfant valide !
                // Traces de débogage
                printf("Generated child node with f = %f\n", child_p->f);

                // Ignorer cet enfant s'il a déjà été visité
                if (onList(&closedList_p, child_p->board)==NULL) {
                    // Traces de débogage
                    printf("Adding child node with f = %f to open list\n", child_p->f);

                    // Ajouter le nœud enfant à la liste ouverte
                    addFirst(&openList_p, child_p);
                } else {
                    freeItem(child_p); // Libérer la mémoire si l'enfant a déjà été visité
                }
            }
        }
    }

    printf("No solution found!\n");
}


int main() {
	// init lists
  initList(&openList_p);
  initList(&closedList_p);


  printf("\nInitial:");
  Item *initial_state = initGame();


  addLast( &openList_p, initial_state );

  //bfs();
	//printf("Finished!\n");
  printBoard(initial_state);
  ucs();
  
  printf("Finished!\n");

  //ucs();
  //printf("Finished!\n");

  //dfs();
  //printf("Finished!\n");
	// clean lists
	cleanupList( &openList_p );
  cleanupList( &closedList_p );


  return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#ifndef GRAPH_H
#define GRAPH_H

#define BOARD_DIMENSION 9

int directions[8][2] = {
        {-1, 0}, {1, 0}, {0, -1}, {0, 1},  // Left, right, Up, Down
        {-1, -1}, {-1, 1}, {1, -1}, {1, 1} // Diagonal
};

void Graph_init(int board[BOARD_DIMENSION][BOARD_DIMENSION]);
void updateNeighbors(int board[BOARD_DIMENSION][BOARD_DIMENSION], int x, int y);

void Graph_init(int board[BOARD_DIMENSION][BOARD_DIMENSION]){
    int bombs = 0;
    
    while (bombs < 9) {
        //random bomb placement
        int x = rand() % 9;
        int y = rand() % 9;
        
        //place bomb (-1) if space is open
        if (board[x][y] != -1) {
            board[x][y] = -1;
            bombs++;
            //update neighbors
            updateNeighbors(board, x, y);
        }
    }
}

void updateNeighbors(int board[BOARD_DIMENSION][BOARD_DIMENSION], int x, int y) {
    for (int i = 0; i < 8; i++) {
        int nx = x + directions[i][0];
        int ny = y + directions[i][1];

        // Ensure that the neighbor is within bounds
        if (nx >= 0 && nx < BOARD_DIMENSION && ny >= 0 && ny < BOARD_DIMENSION) {
            // Only increment if the neighbor is not a bomb
            if (board[nx][ny] != -1) {
                board[nx][ny]++;
            }
        }
    }
}


#endif /* GRAPH_H */
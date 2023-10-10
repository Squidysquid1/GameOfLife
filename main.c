#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <raylib.h>

#define SQUARESIZE 2
#define HEIGHT 540
#define WIDTH 960

typedef struct GameBoards {
    int (*current)[HEIGHT][WIDTH];
    int (*previous)[HEIGHT][WIDTH];
} GameBoard;

GameBoard initGameBoard(){
    GameBoard MainBoard;
    
    MainBoard.current = malloc((HEIGHT * WIDTH) * sizeof(int));
    MainBoard.previous = malloc((HEIGHT * WIDTH) * sizeof(int));

    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            (*MainBoard.current)[i][j] = rand() % 2;
            (*MainBoard.previous)[i][j] = 0;
        }
    }
    
    return MainBoard;
}

void printBoard(GameBoard *board, int genNumber){
    printf("Generation %d:\n", genNumber+1);
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if((*board->current)[i][j] == 1)
                printf("█");
            else
                printf(" ");
        }
        printf("\n");
    }

}

void swapBoard(GameBoard *board){
    int (*tmp)[HEIGHT][WIDTH] = board->current;
    board->current = board->previous;
    board->previous = tmp;
}

int countNeighbors(GameBoard *board, int x, int y){
    int sum = 0;
    int col, row;
    for (int i = -1; i < 2; i++) {
        for (int j = -1; j < 2; j++) {
            col = (x + i + HEIGHT) % HEIGHT;
            row = (y + j + WIDTH) % WIDTH;
            sum += (*board->previous)[col][row];

        }
    }
    sum -= (*board->previous)[x][y];
    return sum;
}
void playGeneration(GameBoard *board){
    int state, neighbors = 0;

    swapBoard(board);

    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            state = (*board->previous)[i][j];
            neighbors = countNeighbors(board, i, j);
            if(state == 0 && neighbors == 3){
                (*board->current)[i][j] = 1;
            }else if(state == 1 && (neighbors < 2 || neighbors > 3)){
                (*board->current)[i][j] = 0;
            }else{
                (*board->current)[i][j] = state;
            }
        }
    }
}

void freeGameBoard (GameBoard *board){
    free(board->current);
    free(board->previous);
}

int main(){
    const int screenWidth = WIDTH*SQUARESIZE;
    const int screenHeight = HEIGHT*SQUARESIZE;
 
    InitWindow(screenWidth, screenHeight, "Conway's game of life");
    SetTargetFPS(4);

    GameBoard gameBoard = initGameBoard();
    Color color =  WHITE;
    int generation = 1;

    while (!WindowShouldClose())    // Detect window close button or ESC key
    {

        BeginDrawing();

            ClearBackground(RAYWHITE);

            for (int y = 0; y < HEIGHT; y++) {
                for (int x = 0; x < WIDTH; x++) {
                    if((*gameBoard.current)[y][x] == 1)
                        color = BLACK;
                    else
                        color = WHITE;
                    DrawRectangle(x * SQUARESIZE, y * SQUARESIZE, SQUARESIZE, SQUARESIZE, color);
                }
            }
            
            DrawRectangle(0, 0, 270, 30, (Color){ 255, 161, 0, 190 });
            DrawText(TextFormat("Generation: %d", generation), 0, 0, 30, BLUE);
        
        EndDrawing();
        //printBoard(&gameBoard, 0);
        generation++;
        playGeneration(&gameBoard);
    }

    CloseWindow();

    freeGameBoard(&gameBoard);
    return 69;
}

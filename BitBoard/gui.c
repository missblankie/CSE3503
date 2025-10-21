//
// Created by saraw on 2025-10-20.
//
#include <stdio.h>
#include <string.h>

#include "raylib.h"
#include "game.h"

#define TILE_SIZE 80
#define BOARD_SIZE 8
#define WINDOW_WIDTH (TILE_SIZE * BOARD_SIZE)
#define WINDOW_HEIGHT (TILE_SIZE * BOARD_SIZE)

// Draw a single checker piece at a specific position
void DrawPieceAtPixel(float x, float y, Color color, bool isKing) {
    DrawCircle(x, y, TILE_SIZE * 0.4f, color);
    if (isKing) DrawCircleLines(x, y, TILE_SIZE * 0.2f, GOLD);
}

// Draws checker piece on given board square
void DrawPiece(int col, int row, Color color, bool isKing) {
    int centerX = col * TILE_SIZE + TILE_SIZE / 2;
    int centerY = row * TILE_SIZE + TILE_SIZE / 2;
    DrawPieceAtPixel(centerX, centerY, color, isKing);
}

// Draws the entire board grid and all pieces from GameState
void DrawBoard(GameState *g) {

    //Draw alternating Dark/Light squares
    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            bool dark = (row + col) % 2 == 1;
            Color tileColor = dark ? BEIGE : DARKBROWN;
            DrawRectangle(col * TILE_SIZE, row * TILE_SIZE, TILE_SIZE, TILE_SIZE, tileColor);
        }
    }

    //Draw checker pieces
    for (int i = 0; i < 64; i++) {
        int row = i / 8;
        int col = i % 8;
        uint64_t mask = 1ULL << i;


        if (g->white & mask)
            DrawPiece(col, 7 - row, RAYWHITE, (g->white_kings & mask));
        else if (g->black & mask)
            DrawPiece(col, 7 - row, BLACK, (g->black_kings & mask));
    }
}





bool can_capture_again(GameState * game_state, int target, bool is_white_piece);

// Main GUI game loop using Raylib GUI
void run_gui(GameState *g) {
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "BitBoard Checkers");
    SetTargetFPS(60);

    // Interaction tracking
    int selected = -1;
    bool dragging = false;
    Vector2 dragPos = {0};

    // Multi-capture tracking
    bool chainCapture = false;
    int chainPos = -1;

    // Endgame tracking
    bool gameOver = false;
    char winnerText[64] = "";

    // Main loop
    while (!WindowShouldClose()) {
        //Check game end state
        if (!gameOver && check_winner(g)) {
            gameOver = true;
            if (g->white == 0 && g->white_kings == 0)
                strcpy(winnerText, "BLACK WINS!");
            else if (g->black == 0 && g->black_kings == 0)
                strcpy(winnerText, "WHITE WINS!");
            else strcpy(winnerText, "DRAW!");

        }
        //Draw Frame
        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawBoard(g);

        // Highlight selected square
        if (selected != -1) {
            int selRow = selected / 8;
            int selCol = selected % 8;
            DrawRectangleLines(selCol * TILE_SIZE + 2, (7 - selRow) * TILE_SIZE + 2,
                               TILE_SIZE - 4, TILE_SIZE - 4, GOLD);
        }

        // Highlight chain piece in red if mid-jump
        if (chainCapture && chainPos != -1) {
            int r = chainPos / 8;
            int c = chainPos % 8;
            DrawRectangleLines(c * TILE_SIZE + 4, (7 - r) * TILE_SIZE + 4,
                               TILE_SIZE - 8, TILE_SIZE - 8, RED);
        }

        // Handle piece selection
        if (!gameOver && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            Vector2 mouse = GetMousePosition();
            int col = (int)(mouse.x / TILE_SIZE);
            int row = (int)(mouse.y / TILE_SIZE);
            int clickedSquare = (7 - row) * 8 + col;

            if (selected == -1) {
                // select only current player's piece
                if (is_square_occupied(g, clickedSquare)) {
                    bool correctColor = (g->white_turn && is_white(g, clickedSquare)) ||
                                        (!g->white_turn && is_black(g, clickedSquare));

                    if (correctColor && (!chainCapture || clickedSquare == chainPos)) {
                        selected = clickedSquare;
                        dragging = true;
                    }
                }
            }
        }

        // Draw floating dragged piece
        if (dragging && selected != -1 && IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
            dragPos = GetMousePosition();
            bool isWhitePiece = is_white(g, selected);
            bool isKing = (g->white_kings & (1ULL << selected)) || (g->black_kings & (1ULL << selected));
            DrawPieceAtPixel(dragPos.x, dragPos.y, isWhitePiece ? RAYWHITE : BLACK, isKing);
        }

        // Handle when piece is dropped
        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON) && dragging) {
            dragging = false;
            Vector2 mouse = GetMousePosition();
            int col = (int)(mouse.x / TILE_SIZE);
            int row = 7 - (int)(mouse.y / TILE_SIZE);
            int target = row * 8 + col;

            // Ignore invalid drops
            if (target < 0 || target >= 64 || ((row + col) % 2 == 0)) {
                selected = -1;
                chainCapture = false;
                chainPos = -1;
                continue;
            }

            if (target >= 0 && target < 64) {
                int oldTurn = g->white_turn;
                if (target == selected) {
                    // cancel drag if dropped on same square
                    selected = -1;
                    chainCapture = false;
                    chainPos = -1;
                    continue;
                }
                //Attempt move
                bool success = apply_move(g, selected, target);
                if (!success) {
                    // Invalid move
                    selected = -1;
                    chainCapture = false;
                    chainPos = -1;
                    continue;
                }
                // TCheck if another capture is required
                if (g->white_turn == oldTurn) {
                    chainCapture = true;
                    chainPos = target;
                    selected = -1;
                } else {
                    chainCapture = false;
                    chainPos = -1;
                    selected = -1;
                }
            }
        }

        // Draw winner overlay
        if (gameOver) {
            DrawRectangle(0, WINDOW_HEIGHT / 2 - 40, WINDOW_WIDTH, 80, Fade(BLACK, 0.6f));
            DrawText(winnerText,
                     WINDOW_WIDTH / 2 - MeasureText(winnerText, 40) / 2,
                     WINDOW_HEIGHT / 2 - 20, 40, GOLD);
        }

        EndDrawing();
    }

    CloseWindow();
}


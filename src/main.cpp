#define SDL_MAIN_HANDLED

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <string>
#include <vector>

#include "Button.hpp"


int is_valid(int row, int col, int num, const short sudoku[9][9]){
    for(int i = 0; i < 9; ++i){
        if(sudoku[row][i] == num)
            return false;
    }

    for(int i = 0; i < 9; ++i){
        if(sudoku[i][col] == num)
            return false;
    }

    col = (col / 3) * 3;
    row = (row / 3) * 3;

    for(int i = 0; i < 3; ++i){
        for(int j = 0; j < 3; ++j){
            if(sudoku[row + i][col + j] == num)
                return false;
        }
    }

    return true;
}

void find_empty(const short sudoku[9][9], int &row_o, int &col_o){
    for(int row = 0; row < 9; ++row){
        for(int col = 0; col < 9; ++col){
            if(sudoku[row][col] == 0){
                row_o = row;
                col_o = col;

                return;
            }
        }
    }

    row_o = col_o = -1;

    return;
}

void drawBoard(SDL_Renderer *renderer, int screen_width, int screen_height, int margin_bottom, int box_space){
    int board_width = box_space * 9;
    int left_x = (screen_width - board_width) / 2;
    int right_x = board_width + left_x;
    int bottom_y = screen_height - margin_bottom;

    // It makes the horizontal lines
    for(int i = 0; i <= 9; ++i){
        SDL_RenderDrawLine(renderer, left_x, bottom_y - box_space * i, right_x, bottom_y - box_space * i);

        // It makes the line thicker
        if(i % 3 == 0){
            SDL_RenderDrawLine(renderer, left_x, bottom_y - box_space * i - 1, right_x, bottom_y - box_space * i - 1);
            SDL_RenderDrawLine(renderer, left_x, bottom_y - box_space * i + 1, right_x, bottom_y - box_space * i + 1);
        }
    }
    
    // It makes the vertical lines
    for(int i = 0; i <= 9; ++i){
        SDL_RenderDrawLine(renderer, left_x + box_space * i, bottom_y - board_width, left_x + box_space * i, bottom_y);

        // It makes the line thicker
        if(i % 3 == 0){
            SDL_RenderDrawLine(renderer, left_x + box_space * i - 1, bottom_y - board_width, left_x + box_space * i - 1, bottom_y);
            SDL_RenderDrawLine(renderer, left_x + box_space * i + 1, bottom_y - board_width, left_x + box_space * i + 1, bottom_y);
        }
    }
}

void display_text_in_box(const char *text, int start_point_x, int start_point_y, int box_space, int row, int col, TTF_Font *font, SDL_Color color, SDL_Renderer *renderer){
    SDL_Surface *text_surface = TTF_RenderText_Blended(font, text, color);
    SDL_Texture *text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);

    SDL_FreeSurface(text_surface);

    SDL_Rect source_texture;
    SDL_Rect dest_texture;

    source_texture.x = source_texture.y = 0;

    SDL_QueryTexture(text_texture, NULL, NULL, &source_texture.w, &source_texture.h);

    dest_texture.x = start_point_x + (col * box_space) + (box_space - source_texture.w) / 2;
    dest_texture.y = start_point_y + (row * box_space) + (box_space - source_texture.h) / 2;

    dest_texture.w = source_texture.w;
    dest_texture.h = source_texture.h;

    SDL_RenderCopy(renderer, text_texture, &source_texture, &dest_texture);

    SDL_DestroyTexture(text_texture);
}

void display_sudoku(const short sudoku[9][9], int start_point_x, int start_point_y, int box_space, TTF_Font *font, SDL_Color color, SDL_Renderer *renderer, int row_f = -1, int col_f = -1, SDL_Color color_f = {255, 10, 10}){
    for(int row = 0; row < 9; ++row){
        for(int col = 0; col < 9; ++col){
            if(sudoku[row][col] != 0){
                char num[] = {sudoku[row][col] + 48, 0};
                display_text_in_box(&num[0], start_point_x, start_point_y, box_space, row, col, font, color, renderer);
            }
        }
    }
}


int main(int argc, char *args[]){
    // Seeds the srand function with the number of seconds of the machine
    srand(time(0));

    const int WIDTH = 440;
    const int HEIGHT = 500;

    const int FPS = 60;
    const int FRAME_DELAY = 1000 / FPS;

    const int BOARD_BOX_SPACE = 40;
    const int BOARD_MARGIN_BOTTOM = 40;
    const int BOARD_WIDTH = BOARD_BOX_SPACE * 9;
    const int BOARD_START_POINT_X = (WIDTH - BOARD_WIDTH) / 2;
    const int BOARD_START_POINT_Y = HEIGHT - BOARD_MARGIN_BOTTOM - BOARD_WIDTH;

    const int SPACE_BETWEEN_BT = 20;
    const int BT_WIDTH = (BOARD_WIDTH - SPACE_BETWEEN_BT) / 2;
    const int BT_HEIGHT = 40;
    const int BT_START_POINT_Y = (BOARD_START_POINT_Y - BT_HEIGHT) / 2;

    const SDL_Color BLACK = {0, 0, 0};
    const SDL_Color RED_ERROR = {255, 10, 10};

    short sudoku[9][9] {
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0}
    };

    SDL_Window *win = SDL_CreateWindow("Sudoku solver", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);

    if(win == NULL){
        std::cout << "Window could not be created. SDL_Error: " << SDL_GetError() << std::endl;
        return -1;
    }

    if(TTF_Init()){
        std::cout << "TTF could not be initialized. TTF_Error" << SDL_GetError() << std::endl;
        return -1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);

    SDL_Event event;


    TTF_Font *font = TTF_OpenFont("res/font/font.ttf", 28);
    TTF_Font *font_bt = TTF_OpenFont("res/font/font.ttf", 18);
    if(font == NULL || font_bt == NULL){
        std::cout << "Failed to load font. SDL_ttf Error: " << TTF_GetError() << std::endl;
    }

    const Uint8 *keyboard_state = SDL_GetKeyboardState(NULL);

    Uint32 frame_start;
    int frame_time;

    const char *clear_bt_text = "Clear";
    Button clear_bt(BOARD_START_POINT_X, BT_START_POINT_Y, BT_WIDTH, BT_HEIGHT, clear_bt_text, font_bt, BLACK);

    const char *solve_bt_text = "Solve";
    Button solve_bt(BOARD_START_POINT_X + BT_WIDTH + SPACE_BETWEEN_BT, BT_START_POINT_Y, BT_WIDTH, BT_HEIGHT, solve_bt_text, font_bt, BLACK);

    bool box_clicked = false;
    bool show_num = false;

    int num = -1;
    int num_f = 0;
    int num_reg = 0;

    int mouse_x, mouse_y;
    int x_index, y_index;

    int row_f = -1;
    int col_f = -1;

    int row_clear = 0;
    int col_clear = 0;

    int row_solve;
    int col_solve;

    std::vector <std::vector <int>> empty_num_reg;

    int num_val_solve;

    bool clear = false;
    bool solve = false;
    bool break_clear = false;

    bool running = true;
    while(running){
        frame_start = SDL_GetTicks();

        SDL_GetMouseState(&mouse_x, &mouse_y);

        // Set the color used for drawing operations
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

        // Clear the current rendering target with the drawing color
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

        drawBoard(renderer, WIDTH, HEIGHT, BOARD_MARGIN_BOTTOM, BOARD_BOX_SPACE);

        if(SDL_PollEvent(&event)){
            if(event.type == SDL_QUIT)
                running = false;
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        
        solve_bt.create(renderer);
        clear_bt.create(renderer);
        if(event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT){
            if(solve_bt.clicked(mouse_x, mouse_y)){
                solve = true;

                find_empty(sudoku, row_solve, col_solve);

                num_val_solve = 0;

                empty_num_reg.push_back(std::vector <int> {row_solve, col_solve});
            }
            
            if(clear_bt.clicked(mouse_x, mouse_y)){
                if(!solve){
                    clear = true;
                }
            }
        }

        if(clear){
            do{
                if(sudoku[row_clear][col_clear] != 0){
                    sudoku[row_clear][col_clear] = 0;
                    break_clear = true;
                }

                else{
                    break_clear = false;
                }

                if(++col_clear == 9){
                    col_clear = 0;
                        
                    if(++row_clear == 9){
                        row_clear = 0;
                        break_clear = true;
                        clear = false;
                    }
                }
            }while(!break_clear);
        }

        if(solve){
            for(int n = num_val_solve; n <= 9; ++n){
                if(is_valid(row_solve, col_solve, n, sudoku)){
                    sudoku[row_solve][col_solve] = n;
                    num_val_solve = 0;

                    find_empty(sudoku, row_solve, col_solve);

                    empty_num_reg.push_back(std::vector <int> {row_solve, col_solve});
                    break;
                }

                if(n == 9){
                    do{
                        sudoku[row_solve][col_solve] = 0;

                        empty_num_reg.pop_back();

                        if(empty_num_reg.size() <= 0)
                            return false;

                        row_solve = empty_num_reg.back()[0];
                        col_solve = empty_num_reg.back()[1];

                    }while(sudoku[row_solve][col_solve] + 1 > 9);

                    num_val_solve = sudoku[row_solve][col_solve] + 1;
                    break;
                }
            }

            if(row_solve == -1 && col_solve == -1){
                solve = false;
            }
        }

        if(mouse_x >= BOARD_START_POINT_X && mouse_x < WIDTH - BOARD_START_POINT_X){
            if(mouse_y >= BOARD_START_POINT_Y && mouse_y < HEIGHT - BOARD_MARGIN_BOTTOM){
                SDL_SetRenderDrawColor(renderer, 100, 100, 200, 255);

                if(event.button.button == SDL_BUTTON_LEFT || box_clicked == true){
                    SDL_SetRenderDrawColor(renderer, 255, 10, 10, 255);

                    box_clicked = true;
                }

                for(int i = 1, j = 0; j <= 1; i -= 2, ++j){
                    SDL_Rect hover_rect = {((mouse_x - BOARD_START_POINT_X) / BOARD_BOX_SPACE) * BOARD_BOX_SPACE + BOARD_START_POINT_X + j,
                                           ((mouse_y - BOARD_START_POINT_Y) / BOARD_BOX_SPACE) * BOARD_BOX_SPACE + BOARD_START_POINT_Y + j,
                                           BOARD_BOX_SPACE, BOARD_BOX_SPACE};

                    SDL_RenderDrawRect(renderer, &hover_rect);
                }
            }
        }

        if((mouse_x - BOARD_START_POINT_X) % BOARD_BOX_SPACE == 0 || (mouse_y - BOARD_START_POINT_Y) % BOARD_BOX_SPACE == 0){

            if(show_num){
                sudoku[row_f][col_f] = num_reg;
            }

            box_clicked = false;
            show_num = false;

            row_f = col_f = -1;
        }

        if(box_clicked){
            num = (keyboard_state[SDL_SCANCODE_0] == true) ? 0 : num;
            num = (keyboard_state[SDL_SCANCODE_1] == true) ? 1 : num;
            num = (keyboard_state[SDL_SCANCODE_2] == true) ? 2 : num;
            num = (keyboard_state[SDL_SCANCODE_3] == true) ? 3 : num;
            num = (keyboard_state[SDL_SCANCODE_4] == true) ? 4 : num;
            num = (keyboard_state[SDL_SCANCODE_5] == true) ? 5 : num;
            num = (keyboard_state[SDL_SCANCODE_6] == true) ? 6 : num;
            num = (keyboard_state[SDL_SCANCODE_7] == true) ? 7 : num;
            num = (keyboard_state[SDL_SCANCODE_8] == true) ? 8 : num;
            num = (keyboard_state[SDL_SCANCODE_9] == true) ? 9 : num;

            x_index = (mouse_x - BOARD_START_POINT_X) / BOARD_BOX_SPACE;
            y_index = (mouse_y - BOARD_START_POINT_Y) / BOARD_BOX_SPACE;

            if(num != -1 && x_index >= 0 && x_index <= 8 && y_index >= 0 && y_index <= 8){
                box_clicked = false;

                if(is_valid(y_index, x_index, num, sudoku) || num == 0 || sudoku[y_index][x_index] == num){
                    if(event.type == SDL_KEYDOWN){
                        sudoku[y_index][x_index] = num;
                    }

                    row_f = -1;
                    col_f = -1;

                    show_num = false;
                }

                else{
                    row_f = y_index;
                    col_f = x_index;

                    num_f = num;

                    box_clicked = true;
                    show_num = true;
                }
            }
        }

        if(show_num){
            if(sudoku[row_f][col_f] != 0){
                num_reg = sudoku[row_f][col_f];
                sudoku[row_f][col_f] = 0;
            }

            if(num_f && row_f != -1 && col_f != -1){
                char num[] = {num_f + 48, 0};
                display_text_in_box(&num[0], BOARD_START_POINT_X, BOARD_START_POINT_Y, BOARD_BOX_SPACE, row_f, col_f, font, RED_ERROR, renderer);
            }
        }

        else{
            num_reg = 0;
        }

        display_sudoku(sudoku, BOARD_START_POINT_X, BOARD_START_POINT_Y, BOARD_BOX_SPACE, font, BLACK, renderer, row_f, col_f, RED_ERROR);

        num = -1;

        // Shows the new content that we have been creating
        SDL_RenderPresent(renderer);

        frame_time = SDL_GetTicks() - frame_start;
        if(FRAME_DELAY > frame_time){
            SDL_Delay(FRAME_DELAY - frame_time);
        }
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(win);

    SDL_Quit();

    return 0;
}

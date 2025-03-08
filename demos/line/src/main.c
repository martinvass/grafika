#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdbool.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define MAX_LINE_COUNT 100

typedef struct {
    Uint8 r, g, b;
} Color;

typedef struct {
    int x1, y1, x2, y2;
    Color color;
} Line;

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
Line lines[MAX_LINE_COUNT];
int line_count = 0;
Color current_color = {255, 0, 0};
bool drawing_rectangles = false;

void draw_palette() {
    Color colors[] = {{255, 0, 0}, {0, 255, 0}, {0, 0, 255}, {255, 255, 0}, {255, 165, 0}, {128, 0, 128}};
    int color_count = sizeof(colors) / sizeof(colors[0]);
    int size = 40;
    for (int i = 0; i < color_count; i++) {
        SDL_SetRenderDrawColor(renderer, colors[i].r, colors[i].g, colors[i].b, 255);
        SDL_Rect rect = {10 + i * (size + 5), 10, size, size};
        SDL_RenderFillRect(renderer, &rect);
    }
}

void add_line(int x1, int y1, int x2, int y2) {
    if (line_count < MAX_LINE_COUNT) {
        lines[line_count++] = (Line){x1, y1, x2, y2, current_color};
    }
}

void render_lines() {
    for (int i = 0; i < line_count; i++) {
        SDL_SetRenderDrawColor(renderer, lines[i].color.r, lines[i].color.g, lines[i].color.b, 255);
        if (drawing_rectangles) {
            SDL_Rect rect = {lines[i].x1, lines[i].y1, lines[i].x2 - lines[i].x1, lines[i].y2 - lines[i].y1};
            SDL_RenderFillRect(renderer, &rect);
        } else {
            SDL_RenderDrawLine(renderer, lines[i].x1, lines[i].y1, lines[i].x2, lines[i].y2);
        }
    }
}

void handle_mouse_click(int x, int y) {
    static int first_x = -1, first_y = -1;
    if (y < 50) {
        int color_index = (x - 10) / 45;
        Color colors[] = {{255, 0, 0}, {0, 255, 0}, {0, 0, 255}, {255, 255, 0}, {255, 165, 0}, {128, 0, 128}};
        if (color_index >= 0 && color_index < 6) {
            current_color = colors[color_index];
        }
    } else {
        if (first_x == -1) {
            first_x = x;
            first_y = y;
        } else {
            add_line(first_x, first_y, x, y);
            first_x = -1;
        }
    }
}

int main() {
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("Szakaszrajzolo", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    
    bool running = true;
    SDL_Event event;
    
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            } else if (event.type == SDL_MOUSEBUTTONDOWN) {
                int x, y;
                SDL_GetMouseState(&x, &y);
                handle_mouse_click(x, y);
                printf("Eger pozicio: %d, %d\n", x, y);
            } else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE) {
                drawing_rectangles = !drawing_rectangles;
            }
        }
        
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);
        draw_palette();
        render_lines();
        SDL_RenderPresent(renderer);
    }
    
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
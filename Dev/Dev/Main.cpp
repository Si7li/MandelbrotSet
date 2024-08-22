#include "SDL.h"
#include <iostream>
#include <numeric>
#include <cmath>

int RES = 250;
int MAX_ITERATIONS  = 1000;

double count = 1.0;
double kx = 0.0;
double ky = 0.0;

double map(double x, double in_max, double in_min, double out_max, double out_min)
{
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

int isInSet(double x, double y)
{
    double x0 = x;
    double y0 = y;

    for (int i = 0; i < MAX_ITERATIONS; i++)
    {
        double x1 = x * x -y * y;
        double y1 = 2.0 * x * y;

        x = x0 + x1;
        y = y0 + y1;

        if (std::sqrt(x*x + y*y) > 10.0)
        {
            return i;
        }
    }
    return 0;
}

void drawMandelbrotSet(SDL_Renderer* renderer)
{
    for (int x = 0; x < RES; x++)
    {
        for (int y = 0; y < RES; y++)
        {
            double a = map(x, RES, 0, 2.0, -2.0);
            double b = map(y, RES, 0, 2.0, -2.0);

            double dx = a / count + kx;
            double dy = b / count + ky;

            Uint8 iters = isInSet(dx, dy);
            if (iters == 0)
            {
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                SDL_RenderDrawPoint(renderer, x, y);
            }
            else
            {
                SDL_SetRenderDrawColor(renderer, 1* iters % 255,2 * iters % 255,3 * iters % 255, 4 * iters % 255);
                SDL_RenderDrawPoint(renderer, x, y);
            }
        }
    }
    count++;
}

int main(int argc, char* argv[])
{
    std::cout << "Iterations = ";
    std::cin >> MAX_ITERATIONS;
    std::cout << "Resolution = ";
    std::cin >> RES;
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_CreateWindowAndRenderer(RES, RES, 0, &window, &renderer);
    const Uint8* state = SDL_GetKeyboardState(NULL);
    while (true)
    {
        SDL_Event e;
        while (SDL_PollEvent(&e))
        {
            switch (e.type)
            {
            case SDL_QUIT:
                SDL_Quit();
                break;
            case SDL_KEYDOWN:
                switch (e.key.keysym.sym)
                {
                case SDLK_q:
                    SDL_Quit();
                    break;
                case SDLK_SPACE:
                    count *= 2;
                    break;
                case SDLK_LSHIFT:
                    count *= -2;
                    break;
                case SDLK_LEFT:
                    kx -= (0.1 / count);
                    break;
                case SDLK_RIGHT:
                    kx += (0.1 / count);
                    break;
                case SDLK_UP:
                    ky -= (0.1 / count);
                    break;
                case SDLK_DOWN:
                    ky += (0.1 / count);
                    break;
                default:
                    break;
                }
            }
        }
        SDL_RenderClear(renderer);
        drawMandelbrotSet(renderer);
        SDL_RenderPresent(renderer);
    }

    return 0;
}
/*
 * simGenericDisplay.cpp
 *
 *  Created on: May 9, 2019
 *      Author: pokas
 */

#include "simGenericDisplay.h"
#include <SDL2/SDL.h>
#include <iostream>
#include "diagnostic/diagnostic.h"

using namespace std::literals;
using namespace microhal::diagnostic;

namespace microhal {

GenericDisplay::~GenericDisplay() {
    if (texture) SDL_DestroyTexture(texture);
    if (renderer) SDL_DestroyRenderer(renderer);
    if (window) SDL_DestroyWindow(window);
    quit = true;
    eventLoop.join();
    SDL_Quit();
}

void GenericDisplay::eventChecker() {
    SDL_Event e;
    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                std::terminate();
            }
            if (e.type == SDL_KEYDOWN) {
            }
            if (e.type == SDL_MOUSEBUTTONDOWN) {
            }
        }
        std::this_thread::sleep_for(100ms);
    }
}

bool GenericDisplay::init() {
    eventLoop = std::thread(&GenericDisplay::eventChecker, this);
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        diagChannel << lock << MICROHAL_ERROR << "SDL_Init Error: " << SDL_GetError() << endl << unlock;
        return false;
    }
    // Opening a Window
    window =
        SDL_CreateWindow("Display Simulator", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, scalling * width, scalling * height, SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        diagChannel << lock << MICROHAL_ERROR << "SDL_CreateWindow Error: " << SDL_GetError() << endl << unlock;
        return false;
    }
    // Creating a Renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);
    if (renderer == nullptr) {
        diagChannel << lock << MICROHAL_ERROR << "SDL_CreateRenderer Error: " << SDL_GetError() << endl << unlock;
        return 1;
    }
    texture = SDL_CreateTexture(renderer, SDL_GetWindowPixelFormat(window), SDL_TEXTUREACCESS_TARGET, width, height);
    if (texture == nullptr) {
        diagChannel << lock << MICROHAL_ERROR << "SDL_CreateTexture Error: " << SDL_GetError() << endl << unlock;
    }
    SDL_SetRenderTarget(renderer, texture);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);
    return true;
}

bool GenericDisplay::drawBitmap(Point begin, size_t width, size_t height, Color bitmap[]) {
    for (size_t h = 0; h < height; h++) {
        for (size_t w = 0; w < width; w++) {
            auto color = bitmap[w + (h * width)];
            auto point = Point{begin.x + w, begin.y + h};
            SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);
            SDL_RenderDrawPoint(renderer, point.x, point.y);
        }
    }
    redraw();
    return true;
}

}  // namespace microhal

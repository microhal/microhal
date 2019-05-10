#ifndef _MICROHAL_SIMGENERICDISPLAY_H_
#define _MICROHAL_SIMGENERICDISPLAY_H_

#include <SDL2/SDL.h>
#include <thread>
#include "color.h"
#include "display.h"

namespace microhal {

class GenericDisplay : public graphics::Display {
 public:
    using Point = graphics::Point;
    using Color = graphics::Color;
    enum class Flip { None = SDL_FLIP_NONE, Horizontal = SDL_FLIP_HORIZONTAL, Vertical = SDL_FLIP_VERTICAL };

    const uint32_t width;
    const uint32_t height;

    GenericDisplay(uint32_t width, uint32_t height, float scalling = 1.0, Flip flip = Flip::None)
        : width(width), height(height), flip(flip), scalling(scalling) {}
    ~GenericDisplay();

    bool init();

    bool setPixel(Point point, Color color) final {
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);
        SDL_RenderDrawPoint(renderer, point.x, point.y);
        redraw();
        return true;
    }

    bool drawLine(Point begin, Point end, Color color) {
        if (begin.x > end.x) std::swap(begin.x, end.x);
        if (begin.y > end.y) std::swap(begin.y, end.y);
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);
        SDL_RenderDrawLine(renderer, begin.x, begin.y, end.x, end.y);
        redraw();
        return true;
    }

    bool drawFilledRectangle(Point begin, Point end, Color color) final {
        if (begin.x > end.x) std::swap(begin.x, end.x);
        if (begin.y > end.y) std::swap(begin.y, end.y);
        SDL_Rect rect;
        rect.x = begin.x;
        rect.y = begin.y;
        rect.h = end.y - begin.y;
        rect.w = end.x - begin.x;
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);
        SDL_RenderFillRect(renderer, &rect);
        redraw();
        return true;
    }

    bool drawBitmap(Point begin, size_t width, size_t height, Color bitmap[]) final;

    void redraw() {
        SDL_SetRenderTarget(renderer, NULL);
        SDL_RenderSetScale(renderer, scalling, scalling);
        SDL_RenderClear(renderer);
        SDL_RenderCopyEx(renderer, texture, NULL, NULL, 0, NULL, static_cast<SDL_RendererFlip>(flip));
        SDL_RenderPresent(renderer);
        SDL_SetRenderTarget(renderer, texture);
        SDL_RenderSetScale(renderer, 1, 1);
    }

 private:
    SDL_Window *window = nullptr;
    SDL_Renderer *renderer = nullptr;
    SDL_Texture *texture = nullptr;
    Flip flip;
    float scalling;
    std::thread eventLoop;
    volatile bool quit = false;

    void eventChecker();
};

}  // namespace microhal

#endif /* _MICROHAL_SIMGENERICDISPLAY_H_ */

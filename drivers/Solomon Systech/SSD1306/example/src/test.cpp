/*
 * test.cpp
 *
 *  Created on: Apr 6, 2019
 *      Author: pokas
 */

#include "bsp.h"
#include "doctest.h"
#include "fonts/defaultFont.h"
#include "ssd1306.h"
#include "widgets/button.h"

using namespace microhal;
using namespace graphics;

SSD1306 display(bsp::i2c, SSD1306::Address_0x78);

TEST_CASE("ILI9325 ID check") {
    display.reset();
    CHECK(display.init());

    display.setPixel(Point{1, 1}, Color(255, 255, 255));
    display.setPixel(Point{1, display.height - 1}, Color(0, 0, 255));
    display.setPixel(Point{display.width - 1, 1}, Color(0, 0, 255));
    display.setPixel(Point{display.width - 1, display.height - 1}, Color(255, 255, 255));
    display.drawLine(Point{1, 5}, Point{1, display.height - 5}, Color(255, 255, 255));
    display.drawLine(Point{5, 1}, Point{display.width - 5, 1}, Color(255, 0, 0));
    display.drawLine(Point{display.width - 1, display.height - 5}, Point{display.width - 1, 5}, Color(0, 255, 0));   // backward line
    display.drawLine(Point{display.width - 5, display.height - 1}, Point{5, display.height - 1}, Color(0, 0, 255));  // backward line
    // display.drawLineWidth(Position{0, 20}, Position{200, 300}, 5);
    display.drawRectangle(Point{5, 5}, Point{30, 10}, Color{255, 255, 0});
    display.drawRectangle(Point{display.width - 5, display.height - 5}, Point{display.width - 30, display.height - 10},
                          Color{255, 0, 255});  // backward rectangle
    display.drawFilledRectangle(Point{7, 7}, Point{28, 8}, Color{255, 255, 0});
    display.drawFilledRectangle(Point{display.width - 7, display.height - 7}, Point{display.width - 28, display.height - 8},
                                Color{255, 255, 0});  // backward rectangle

    Point circleMiddle(display.width / 2, display.height / 2);
    display.drawCircle(circleMiddle, 10, Color{255, 0, 255});
    display.drawFilledCircle(circleMiddle, 8, Color{0, 255, 255});

    display.drawTriangle(Point{display.width / 2 + 15, display.height / 2}, Point{display.width / 2 + 30, display.height / 2 + 10},
                         Point{display.width / 2 + 30, display.height / 2 - 10}, Color{255, 0, 255});
    display.drawRoundedRectangle(Point{5, display.height - 15}, Point{30, display.height - 5}, 4, Color{255, 255, 15});

    display.drawFilledRoundedRectangle(Point{7, display.height - 13}, Point{28, display.height - 7}, 4, Color{255, 255, 15});

    //  DefaultFont font;
    //  font.color = Color{55, 0, 0};
    //  font.draw(display, Point{display.width - 33, 5}, 0, "Hello");
    display.redraw();
}

TEST_CASE("Create button") {
    Button button(Point{display.width - 30, 5}, Point{display.width - 5, 13}, "OK");
    button.setMode(Button::Mode::SquareNoBorder);
    button.font()->color = Color{0, 0, 0};
    button.draw(display);
    display.redraw();
}

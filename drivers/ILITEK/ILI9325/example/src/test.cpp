/*
 * test.cpp
 *
 *  Created on: Apr 6, 2019
 *      Author: pokas
 */

#include "bsp.h"
#include "doctest.h"
#include "fonts/defaultFont.h"
#include "ili9325.h"
#include "widgets/button.h"

using namespace microhal;
using namespace graphics;

GenericDataBus dataBus(bsp::lcd_wr, bsp::lcd_rd, bsp::lcd_d0, bsp::lcd_d1, bsp::lcd_d2, bsp::lcd_d3, bsp::lcd_d4, bsp::lcd_d5, bsp::lcd_d6,
                       bsp::lcd_d7);

ILI9325 display(bsp::lcd_rst, bsp::lcd_cs, bsp::lcd_rs, dataBus);

TEST_CASE("ILI9325 ID check") {
    display.reset();
    CHECK(display.readID() == ILI9325::id);
    CHECK(display.init());

    display.setPixel(Point{10, 10}, Color(255, 255, 255));
    display.setPixel(Point{10, 310}, Color(0, 0, 0));
    display.setPixel(Point{230, 10}, Color(0, 0, 255));
    display.setPixel(Point{230, 310}, Color(255, 255, 255));
    display.drawLine(Point{10, 15}, Point{10, 305}, Color(255, 255, 255));
    display.drawLine(Point{15, 10}, Point{225, 10}, Color(255, 0, 0));
    display.drawLine(Point{230, 305}, Point{230, 15}, Color(0, 255, 0));  // backward line
    display.drawLine(Point{225, 310}, Point{15, 310}, Color(0, 0, 255));  // backward line
    // display.drawLineWidth(Position{0, 20}, Position{200, 300}, 5);
    display.drawRectangle(Point{15, 15}, Point{30, 100}, Color{255, 255, 0});
    display.drawRectangle(Point{225, 305}, Point{210, 220}, Color{255, 0, 255});  // backward rectangle
    display.drawFilledRectangle(Point{20, 20}, Point{25, 95}, Color{255, 255, 0});
    display.drawFilledRectangle(Point{220, 300}, Point{215, 225}, Color{255, 255, 0});  // backward rectangle

    display.drawCircle(Point{240 / 2, 320 / 2}, 25, Color{255, 0, 255});
    display.drawFilledCircle(Point{240 / 2, 320 / 2}, 20, Color{0, 255, 255});

    display.drawTriangle(Point{240 / 2, 320 / 2 + 30}, Point{240 / 2 + 15, 320 / 2 + 60}, Point{240 / 2 - 15, 320 / 2 + 60}, Color{255, 0, 255});
    display.drawRoundedRectangle(Point{50, 15}, Point{70, 100}, 5, Color{255, 255, 15});

    display.drawFilledRoundedRectangle(Point{100, 15}, Point{130, 100}, 5, Color{255, 255, 15});
    //
    //    DefaultFont font;
    //    font.draw(display, Point{200, 100}, 90, "Hello world");
}

TEST_CASE("Create button") {
    Button button(Point{100, 50}, Point{130, 150}, "OK");
    button.draw(display);
}

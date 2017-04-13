/* NeatGUI Library
 * Copyright (c) 2013 Neil Thiessen
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "Canvas.h"
#include "font_57.h"

Canvas::Canvas(int w, int h)
{
    m_Width = w;
    m_Height = h;
}

void Canvas::clear(unsigned int c)
{
    fillRect(0, 0, m_Width, m_Height, c);
}

void Canvas::drawLine(int x0, int y0, int x1, int y1, unsigned int c)
{
    int steep = abs(y1 - y0) > abs(x1 - x0);
    if (steep) {
        int temp = x0;
        x0 = y0;
        y0 = temp;

        temp = x1;
        x1 = y1;
        y1 = temp;
    }

    if (x0 > x1) {
        int temp = x0;
        x0 = x1;
        x1 = temp;

        temp = y0;
        y0 = y1;
        y1 = temp;
    }

    int dx, dy;
    dx = x1 - x0;
    dy = abs(y1 - y0);

    int err = dx / 2;
    int ystep;

    if (y0 < y1) {
        ystep = 1;
    } else {
        ystep = -1;
    }

    for (; x0 <= x1; x0++) {
        if (steep) {
            drawPixel(y0, x0, c);
        } else {
            drawPixel(x0, y0, c);
        }
        err -= dy;
        if (err < 0) {
            y0 += ystep;
            err += dx;
        }
    }
}

void Canvas::drawHLine(int x, int y, int w, unsigned int c)
{
    drawLine(x, y, x + w - 1, y, c);
}

void Canvas::drawVLine(int x, int y, int h, unsigned int c)
{
    drawLine(x, y, x, y + h - 1, c);
}

void Canvas::drawRect(int x, int y, int w, int h, unsigned int c)
{
    drawHLine(x, y, w, c);
    drawHLine(x, y + h - 1, w, c);
    drawVLine(x, y, h, c);
    drawVLine(x + w - 1, y, h, c);
}

void Canvas::fillRect(int x, int y, int w, int h, unsigned int c)
{
    for (int i = x; i < x + w; i++) {
        drawVLine(i, y, h, c);
    }
}

void Canvas::drawTriangle(int x0, int y0, int x1, int y1, int x2, int y2, unsigned int c)
{
    drawLine(x0, y0, x1, y1, c);
    drawLine(x1, y1, x2, y2, c);
    drawLine(x2, y2, x0, y0, c);
}

void Canvas::fillTriangle(int x0, int y0, int x1, int y1, int x2, int y2, unsigned int c)
{
    int a, b, y, last;

    //Sort coordinates by Y order (y2 >= y1 >= y0)
    if (y0 > y1) {
        int temp = y0;
        y0 = y1;
        y1 = temp;

        temp = x0;
        x0 = x1;
        x1 = temp;
    }
    if (y1 > y2) {
        int temp = y1;
        x1 = y2;
        y2 = temp;

        temp = x1;
        x1 = x2;
        x2 = temp;
    }
    if (y0 > y1) {
        int temp = y0;
        y0 = y1;
        y1 = temp;

        temp = x0;
        x0 = x1;
        x1 = temp;
    }

    //Handle awkward all-on-same-line case as its own thing
    if(y0 == y2) {
        a = b = x0;
        if(x1 < a)
            a = x1;
        else if(x1 > b)
            b = x1;
        if(x2 < a)
            a = x2;
        else if(x2 > b)
            b = x2;

        drawHLine(a, y0, b - a + 1, c);

        return;
    }

    int dx01 = x1 - x0;
    int dy01 = y1 - y0;
    int dx02 = x2 - x0;
    int dy02 = y2 - y0;
    int dx12 = x2 - x1;
    int dy12 = y2 - y1;
    int sa = 0;
    int sb = 0;

    //For upper part of triangle, find scanline crossings for segments
    //0-1 and 0-2.  If y1=y2 (flat-bottomed triangle), the scanline y1
    //is included here (and second loop will be skipped, avoiding a /0
    //error there), otherwise scanline y1 is skipped here and handled
    //in the second loop...which also avoids a /0 error here if y0=y1
    //(flat-topped triangle).
    if(y1 == y2)
        last = y1;      //Include y1 scanline
    else
        last = y1 - 1;  //Skip it

    for(y = y0; y <= last; y++) {
        a = x0 + sa / dy01;
        b = x0 + sb / dy02;
        sa += dx01;
        sb += dx02;

        if(a > b) {
            int temp = a;
            a = b;
            b = temp;
        }
        drawHLine(a, y, b - a + 1, c);
    }

    //For lower part of triangle, find scanline crossings for segments
    //0-2 and 1-2.  This loop is skipped if y1=y2.
    sa = dx12 * (y - y1);
    sb = dx02 * (y - y0);
    for(; y <= y2; y++) {
        a = x1 + sa / dy12;
        b = x0 + sb / dy02;
        sa += dx12;
        sb += dx02;

        if(a > b) {
            int temp = a;
            a = b;
            b = temp;
        }
        drawHLine(a, y, b - a + 1, c);
    }
}

void Canvas::drawCircle(int x, int y, int r, unsigned int c)
{
    int f = 1 - r;
    int ddF_x = 1;
    int ddF_y = -2 * r;
    int i = 0;
    int j = r;

    drawPixel(x, y + r, c);
    drawPixel(x, y - r, c);
    drawPixel(x + r, y, c);
    drawPixel(x - r, y, c);

    while(i < j) {
        if(f >= 0) {
            j--;
            ddF_y += 2;
            f += ddF_y;
        }
        i++;
        ddF_x += 2;
        f += ddF_x;
        drawPixel(x + i, y + j, c);
        drawPixel(x - i, y + j, c);
        drawPixel(x + i, y - j, c);
        drawPixel(x - i, y - j, c);
        drawPixel(x + j, y + i, c);
        drawPixel(x - j, y + i, c);
        drawPixel(x + j, y - i, c);
        drawPixel(x - j, y - i, c);
    }
}

void Canvas::fillCircle(int x, int y, int r, unsigned int c)
{
    drawVLine(x, y - r, 2 * r + 1, c);
    fillCircleHelper(x, y, r, 3, 0, c);
}

void Canvas::drawRoundRect(int x, int y, int w, int h, int r, unsigned int c)
{
    //Draw the four lines
    drawHLine(x + r, y, w - 2 * r, c);            //Top
    drawHLine(x + r, y + h - 1, w - 2 * r, c);    //Bottom
    drawVLine(x, y + r, h - 2 * r, c);            //Left
    drawVLine(x + w - 1, y + r, h - 2 * r, c);    //Right

    //Draw the four corners
    drawCircleHelper(x + r, y + r, r, 1, c);
    drawCircleHelper(x + w - r - 1, y + r, r, 2, c);
    drawCircleHelper(x + w - r - 1, y + h - r - 1, r, 4, c);
    drawCircleHelper(x + r, y + h - r - 1, r, 8, c);
}

void Canvas::fillRoundRect(int x, int y, int w, int h, int r, unsigned int c)
{
    //Draw the body
    fillRect(x + r, y, w - 2 * r, h, c);

    //Draw the four corners
    fillCircleHelper(x + w - r - 1, y + r, r, 1, h - 2 * r - 1, c);
    fillCircleHelper(x+r, y + r, r, 2, h - 2 * r - 1, c);
}

void Canvas::drawImage(Image* img, int x, int y)
{
    for (int j = 0; j < img->height(); j++) {
        for (int i = 0; i < img->width(); i++) {
            drawPixel(i + x, j + y, img->pixel(i, j));
        }
    }
}

int Canvas::drawChar(char c, Font* fnt, int x, int y)
{
    //Get the character glyph
    BitmapImage glyph = fnt->glyph(c);

    //Draw the character glyph
    drawImage(&glyph, x, y);

    //Return the width of the glyph
    return glyph.width();
}

void Canvas::drawString(const char* str, Font* fnt, int x, int y)
{
    int cursX = 0;
    int cursY = 0;

    while(*str != NULL) {
        //Check for a new line character
        if (*str == '\n') {
            //Go to a new line
            cursX = 0;
            cursY += fnt->height();

            //We're done for this character
            str++;
            continue;
        }

        //Check for a carriage return character
        if (*str == '\r') {
            //Ignore it, we're done for this character
            str++;
            continue;
        }

        //Draw the character
        cursX += drawChar(*str++, fnt, x + cursX, y + cursY);
    }
}

void Canvas::drawString(const char* str, Font* fnt, int x, int y, int w, int h)
{
    int cursX = 0;
    int cursY = 0;

    while(*str != NULL) {
        //Check for a new line character
        if (*str == '\n') {
            //Check if we can fit another line
            if ((cursY + 2 * fnt->height()) < h) {
                //Yes we can
                cursX = 0;
                cursY += fnt->height();

                //We're done for this character
                str++;
                continue;
            } else {
                //Nope, we can't print any more so return
                return;
            }
        }

        //Check for a carriage return character
        if (*str == '\r') {
            //Ignore it, we're done for this character
            str++;
            continue;
        }

        //Check for entire words first
        if ((*str > ' ') && (*str <= 0x7E)) {
            //Draw entire word on canvas with correct wrapping
            //int i = 0;
            int wlen;

            //Determine the length of the next word
            wlen = fnt->measureWord(str);

            //Will the length of the next word exceed the margins?
            if ((wlen + cursX) > w) {
                //Only do a newline if the word will fit on it
                if (wlen <= w) {
                    //Check if we can fit another line
                    if ((cursY + 2 * fnt->height()) < h) {
                        //Yes we can
                        cursX = 0;
                        cursY += fnt->height();
                    } else {
                        //Nope, we can't print any more so return
                        return;
                    }
                }
            }

            //Put just the word characters on the display up to the next non-whitespace character or the end of the string
            while ((*str > ' ') && (*str <= 0x7E)) {
                //Check if the character will fit on the screen
                if ((fnt->glyph(*str).width() + cursX) > w) {
                    //Check if we can fit another line
                    if ((cursY + 2 * fnt->height()) < h) {
                        //Yes we can
                        cursX = 0;
                        cursY += fnt->height();
                    } else {
                        //Nope, we can't print any more so return
                        return;
                    }
                }

                //Draw the character
                cursX += drawChar(*str++, fnt, x + cursX, y + cursY);
            }
        } else {
            //Check if the character will fit on the screen
            if ((fnt->glyph(*str).width() + cursX) > w) {
                //Check if we can fit another line
                if ((cursY + 2 * fnt->height()) < h) {
                    //Yes we can
                    cursX = 0;
                    cursY += fnt->height();
                } else {
                    //Nope, we can't print any more so return
                    return;
                }
            }

            //Draw the character
            cursX += drawChar(*str++, fnt, x + cursX, y + cursY);
        }
    }
}

int Canvas::width()
{
    return m_Width;
}

int Canvas::height()
{
    return m_Height;
}

void Canvas::drawCircleHelper(int x, int y, int r, unsigned int corner, unsigned int c)
{
    int f = 1 - r;
    int ddF_x = 1;
    int ddF_y = -2 * r;
    int i = 0;
    int j = r;

    while (i < j) {
        if (f >= 0) {
            j--;
            ddF_y += 2;
            f += ddF_y;
        }
        i++;
        ddF_x += 2;
        f += ddF_x;
        if (corner & 0x4) {
            drawPixel(x + i, y + j, c);
            drawPixel(x + j, y + i, c);
        }
        if (corner & 0x2) {
            drawPixel(x + i, y - j, c);
            drawPixel(x + j, y - i, c);
        }
        if (corner & 0x8) {
            drawPixel(x - j, y + i, c);
            drawPixel(x - i, y + j, c);
        }
        if (corner & 0x1) {
            drawPixel(x - j, y - i, c);
            drawPixel(x - i, y - j, c);
        }
    }
}

void Canvas::fillCircleHelper(int x, int y, int r, unsigned int corner, int delta, unsigned int c)
{
    int f = 1 - r;
    int ddF_x = 1;
    int ddF_y = -2 * r;
    int i = 0;
    int j = r;

    while (i < j) {
        if (f >= 0) {
            j--;
            ddF_y += 2;
            f += ddF_y;
        }
        i++;
        ddF_x += 2;
        f += ddF_x;

        if (corner & 0x1) {
            drawVLine(x + i, y - j, 2 * j + 1 + delta, c);
            drawVLine(x + j, y - i, 2 * i + 1 + delta, c);
        }
        if (corner & 0x2) {
            drawVLine(x - i, y - j, 2 * j + 1 + delta, c);
            drawVLine(x - j, y - i, 2 * i + 1 + delta, c);
        }
    }
}

void Canvas::printF(unsigned int color, int x, int y, const char* str){
    
    int cursX = 0;
    int cursY = 0;
    //int posX = x, posY = y;
    while(*str != NULL) {
//        printf("X %d Y %d\r\n", cursX, cursY);
        //Check for a new line character
        if ((cursX + 6) > 128){
            cursX = 0;
            cursY += 8;
        }
        if ((cursY + 8) > 128){
            cursY = 0;
            cursX = 0;
            wait(2);
            clear(0x000000);
        }
        if (*str == '\n') {
            //Go to a new line
            cursX = 0;
            cursY += 8;

            //We're done for this character
            str++;
            continue;
        }

        //Check for a carriage return character
        if (*str == '\r') {
            //Ignore it, we're done for this character
            str++;
            continue;
        }

        //Draw the character
        putC(color,x + cursX, y + cursY, *str++);
//        wait(0.05);
        cursX += 6;
    }
    
}

void Canvas::putC(unsigned int color, int x, int y, char c){
    int col = y;
    for(int j = 0; j < 5; j++){
        //printf("index %d val %d\n", Font5x7[((int)c-32)*5 + j], ((int)c-32)*5 + j);
        y = col;
        char val = Font5x7[((int)c-32)*5 + j];
        for(int i = 0; i < 7; i++){
            if(val & 0x1){
                drawPixel(x, y, color);
            }    
            val = val >> 1;
            y++; 
        }
        x++;
    } 
}
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

#ifndef CANVAS_H
#define CANVAS_H

#include "mbed.h"
#include "Image.h"
#include "Font.h"

/** Canvas abstract class.
 *  Used as a base class for objects that provide 2D drawing capabilities.
 */
class Canvas
{
public:
    /** Create a Canvas object with the specified width and height
     *
     * @param w The canvas width.
     * @param h The canvas height.
     */
    Canvas(int w, int h);

    /** Draw a single pixel at the specified coordinates
    *
    * @param x The X coordinate.
    * @param y The Y coordinate.
    * @param c The color of the pixel as a 32-bit ARGB value.
    */
    virtual void drawPixel(int x, int y, unsigned int c) = 0;
    virtual void putC(unsigned int color, int x, int y, char c);
    void printF(unsigned int color, int x, int y, const char* str);
    /** Fill the entire canvas with the specified color using a filled rectangle
     *
     * @param c The color to fill with as a 32-bit ARGB value (black by default).
     */
    virtual void clear(unsigned int c = 0xFF000000);

    /** Draw a line between the specified coordinates using Bresenham's line algorithm
     *
     * @param x0 The starting X coordinate.
     * @param y0 The starting Y coordinate.
     * @param x1 The ending X coordinate.
     * @param y1 The ending Y coordinate.
     * @param c The color of the line as a 32-bit ARGB value.
     */
    virtual void drawLine(int x0, int y0, int x1, int y1, unsigned int c);

    /** Draw a fast horizontal line of the specified width, at the specified coordinates
     *
     * @param x The starting X coordinate.
     * @param y The starting Y coordinate.
     * @param w The width of the line.
     * @param c The color of the line as a 32-bit ARGB value.
     */
    virtual void drawHLine(int x, int y, int w, unsigned int c);

    /** Draw a fast vertical line of the specified height, at the specified coordinates
     *
     * @param x The starting X coordinate.
     * @param y The starting Y coordinate.
     * @param h The height of the line.
     * @param c The color of the line as a 32-bit ARGB value.
     */
    virtual void drawVLine(int x, int y, int h, unsigned int c);

    /** Draw an unfilled rectangle of the specified width and height, at the specified coordinates using fast lines
     *
     * @param x The starting X coordinate.
     * @param y The starting Y coordinate.
     * @param w The width of the rectangle.
     * @param h The height of the rectangle.
     * @param c The color of the rectangle as a 32-bit ARGB value.
     */
    virtual void drawRect(int x, int y, int w, int h, unsigned int c);

    /** Draw a filled rectangle of the specified width and height, at the specified coordinates using fast lines
     *
     * @param x The starting X coordinate.
     * @param y The starting Y coordinate.
     * @param w The width of the rectangle.
     * @param h The height of the rectangle.
     * @param c The color of the rectangle as a 32-bit ARGB value.
     */
    virtual void fillRect(int x, int y, int w, int h, unsigned int c);

    /** Draw an unfilled triangle with the specified vertices using lines
     *
     * @param x0 The first vertex X coordinate.
     * @param y0 The first vertex Y coordinate.
     * @param x1 The second vertex X coordinate.
     * @param y1 The second vertex Y coordinate.
     * @param x2 The third vertex X coordinate.
     * @param y2 The third vertex Y coordinate.
     * @param c The color of the triangle as a 32-bit ARGB value.
     */
    void drawTriangle(int x0, int y0, int x1, int y1, int x2, int y2, unsigned int c);

    /** Draw a filled triangle with the specified vertices using Adafruit's algorithm
     *
     * @param x0 The first vertex X coordinate.
     * @param y0 The first vertex Y coordinate.
     * @param x1 The second vertex X coordinate.
     * @param y1 The second vertex Y coordinate.
     * @param x2 The third vertex X coordinate.
     * @param y2 The third vertex Y coordinate.
     * @param c The color of the triangle as a 32-bit ARGB value.
     */
    void fillTriangle(int x0, int y0, int x1, int y1, int x2, int y2, unsigned int c);

    /** Draw an unfilled circle of the specified radius, at the specified coordinates using the midpoint circle algorithm
     *
     * @param x The center X coordinate.
     * @param y The center Y coordinate.
     * @param r The radius of the circle.
     * @param c The color of the circle as a 32-bit ARGB value.
     */
    void drawCircle(int x, int y, int r, unsigned int c);

    /** Draw a filled circle of the specified radius, at the specified coordinates using Adafruit's modified midpoint circle algorithm
     *
     * @param x The center X coordinate.
     * @param y The center Y coordinate.
     * @param r The radius of the circle.
     * @param c The color of the circle as a 32-bit ARGB value.
     */
    void fillCircle(int x, int y, int r, unsigned int c);

    /** Draw an unfilled rounded rectangle of the specified width, height, and corner radius, at the specified coordinates using Adafruit's algorithm
     *
     * @param x The starting X coordinate.
     * @param y The starting Y coordinate.
     * @param w The width of the rectangle.
     * @param h The height of the rectangle.
     * @param r The radius of the corners.
     * @param c The color of the rectangle as a 32-bit ARGB value.
     */
    void drawRoundRect(int x, int y, int w, int h, int r, unsigned int c);

    /** Draw a filled rounded rectangle of the specified width, height, and corner radius, at the specified coordinates using Adafruit's algorithm
     *
     * @param x The starting X coordinate.
     * @param y The starting Y coordinate.
     * @param w The width of the rectangle.
     * @param h The height of the rectangle.
     * @param r The radius of the corners.
     * @param c The color of the rectangle as a 32-bit ARGB value.
     */
    void fillRoundRect(int x, int y, int w, int h, int r, unsigned int c);

    /** Draw an Image object at the specified coordinates
     *
     * @param img Pointer to the image to draw.
     * @param x The starting X coordinate.
     * @param y The starting Y coordinate.
     */
    void drawImage(Image* img, int x, int y);

    /** Draw a character at the specified coordinates without wrapping
     *
     * @param c The character to draw.
     * @param fnt Pointer to the font to draw with.
     * @param x The starting X coordinate.
     * @param y The starting Y coordinate.
     *
     * @returns The width of the drawn character.
     */
    int drawChar(char c, Font* fnt, int x, int y);

    /** Draw a string at the specified coordinates without wrapping
     *
     * @param str Pointer to the string to draw.
     * @param fnt Pointer to the font to draw with.
     * @param x The starting X coordinate.
     * @param y The starting Y coordinate.
     */
    void drawString(const char* str, Font* fnt, int x, int y);

    /** Draw a string within the specified rectangle with wrapping
     *
     * @param str Pointer to the string to draw.
     * @param fnt Pointer to the font to draw with.
     * @param x The starting X coordinate.
     * @param y The starting Y coordinate.
     * @param w The width of the bounding rectangle.
     * @param h The height of the bounding rectangle.
     */
    void drawString(const char* str, Font* fnt, int x, int y, int w, int h);

    /** Get the width of the canvas
     *
     * @returns The width of the canvas.
     */
    int width();

    /** Get the height of the canvas
     *
     * @returns The height of the canvas.
     */
    int height();

protected:
    //The canvas width/height
    int m_Width, m_Height;

    //Drawing helper functions
    void drawCircleHelper(int x, int y, int r, unsigned int corner, unsigned int color);
    void fillCircleHelper(int x, int y, int r, unsigned int corner, int delta, unsigned int color);
};

#endif
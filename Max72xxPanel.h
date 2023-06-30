/******************************************************************
 A library for controling a set of 8x8 LEDs with a MAX7219 or
 MAX7221 displays.

 This is a plugin for Adafruit's core graphics library, providing
 basic graphics primitives (points, lines, circles, etc.).
 You need to download and install Adafruit_GFX to use this library.

 Adafruit invests time and resources providing this open
 source code, please support Adafruit and open-source hardware
 by purchasing products from Adafruit!

 Written by Mark Ruys, 2013.
 BSD license, check license.txt for more information.
 All text above must be included in any redistribution.

 Datasheet: http://datasheets.maximintegrated.com/en/ds/MAX7219-MAX7221.pdf

 ******************************************************************/

#ifndef Max72xxPanel_h
#define Max72xxPanel_h

#if (ARDUINO >= 100)
  #include <Arduino.h>
#else
  #include <WProgram.h>
  #include "pins_arduino.h"
#endif

class Max72xxPanel : public Adafruit_GFX {

public:

  /*
   * Create a new controler
   * Parameters:
   * csPin		pin for selecting the device
   * hDisplays  number of displays horizontally
   * vDisplays  number of displays vertically
   */
  Max72xxPanel(byte csPin, byte hDisplays=1, byte vDisplays=1);

	/*
	 * Define how the displays are ordered. The first display (0)
	 * is the one closest to the Arduino.
	 */
	void setPosition(byte display, byte x, byte y);

	/*
	 * Define if and how the displays are rotated. The first display
	 * (0) is the one closest to the Arduino. rotation can be:
	 *   0: no rotation
	 *   1: 90 degrees clockwise
	 *   2: 180 degrees
	 *   3: 90 degrees counter clockwise
	 */
	void setRotation(byte display, byte rotation);

	/*
	 * Implementation of Adafruit's setRotation(). Probably, you don't
	 * need this function as you can achieve the same result by using
	 * the previous two functions.
	 */
	void setRotation(byte rotation);

  /*
   * Draw a pixel on your canvas. Note that for performance reasons,
   * the pixels are not actually send to the displays. Only the internal
   * bitmap buffer is modified.
   */
  void drawPixel(int16_t x, int16_t y, uint16_t color);

  /*
   * As we can do this much faster then setting all the pixels one by
   * one, we have a dedicated function to clear the screen.
   * The color can be 0 (blank) or non-zero (pixel on).
   */
  void fillScreen(uint16_t color);

  /*
   * Set the shutdown (power saving) mode for the device
   * Paramaters:
   * status	If true the device goes into power-down mode. Set to false
   *		for normal operation.
   */
  void shutdown(boolean status);

  /*
   * Set the brightness of the display.
   * Paramaters:
   * intensity	the brightness of the display. (0..15)
   */
  void setIntensity(byte intensity);

  /*
   * After you're done filling the bitmap buffer with your picture,
   * send it to the display(s).
   */
  void write();

  //
  // ===== Functions added to the standard Max72xxPanel library
  //


  // Reset the display to it's initial state
  void reset();
  
  /*
   *  By passing true the client is saying that the displays are in 
   *  sequential order and that they're all the same rotation: 1 or 3. 
   *  Furthermore, the client guarantees that it will not apply a
   *  Adafruit_GFX-level rotation, nor will it change any of this
   *  in the future. These situations are common and allow the
   *  implementation to take some shortcuts.
   */
  void setOptimizeHint(bool hint) { optimizeHint = hint; }

  /*
   * Act as if the given line (hortizontal row of 8x8 modules) was the entire
   * display. Drawing outside the region will be clipped. The y coordinate of
   * drawing operations will be translated accordingly.
   */
  void focusOnLine(int line);
  int  currentFocusedLine() { return focusedLine; }
  int  swapFocus(int line);
  // void setDebug(bool turnDebugOn) { debug = turnDebugOn; }

  int16_t focusedHeight() { return focusedLine == NoFocus ? _height : 8; }
  uint16_t readPixel(int16_t xx, int16_t yy);

  static constexpr int NoFocus = -1;


private:
  byte SPI_CS; /* SPI chip selection */

  /* Send out a single command to the device */
  void spiTransfer(byte opcode, byte data=0);

  /*
   * ADDED: Return a pointer to the byte in the buffer that contains the
   * specified pixel. The last parameter, y, is an 'out' parameter.
   * The lower three bits of y indicate the bit position within the
   * specified byte of the requested pixel.
   */
  inline byte* byteForPixel(int16_t xx, int16_t yy, byte& y);

  /* We keep track of the led-status for 8 devices in this array */
  byte *bitmap;
  byte bitmapSize;

  byte hDisplays;
  byte *matrixPosition;
  byte *matrixRotation;

  bool optimizeHint = false;
  int focusedLine = NoFocus;
  int _ty = 0;
  byte* focusedLinePtr = nullptr;
  uint16_t lineSizeInBytes = 0;
  // bool debug;
};

#endif	// Max72xxPanel_h




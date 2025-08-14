#ifndef TWOLINESCHARACTERDISPLAY_H
#define TWOLINESCHARACTERDISPLAY_H

#include <Arduino.h>
#include "LiquidCrystal3.h"

class TwoLinesCharacterDisplay {
public:
    TwoLinesCharacterDisplay(LiquidCrystal* lcd);
    void setup();
    
    void setLine1(const String& text);
    void setLine2(const String& text);
    void setLines(const String& line1, const String& line2);
    
    void clear();
    void clearLine(int lineNumber);

private:
    LiquidCrystal* _lcd;
    uint8_t _cols;
    uint8_t _rows;
    String _currentLine1;
    String _currentLine2;
    
    void _updateLine(int lineNumber, const String& text);
};

#endif
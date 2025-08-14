#include "TwoLinesCharacterDisplay.h"

TwoLinesCharacterDisplay::TwoLinesCharacterDisplay(LiquidCrystal* lcd) 
    : _lcd(lcd), _cols(16), _rows(2), _currentLine1(""), _currentLine2("") {
}

void TwoLinesCharacterDisplay::setup() {
    _lcd->begin(_cols, _rows);
    _lcd->print("OK");
}

void TwoLinesCharacterDisplay::setLine1(const String& text) {
    if (text != _currentLine1) {
        _currentLine1 = text;
        _updateLine(0, _currentLine1);
    }
}

void TwoLinesCharacterDisplay::setLine2(const String& text) {
    if (text != _currentLine2) {
        _currentLine2 = text;
        _updateLine(1, _currentLine2);
    }
}

void TwoLinesCharacterDisplay::setLines(const String& line1, const String& line2) {
    bool needUpdate = (line1 != _currentLine1 || line2 != _currentLine2);
    
    if (needUpdate) {
        _lcd->clear();
        setLine1(line1);
        setLine2(line2);
    }
}

void TwoLinesCharacterDisplay::clear() {
    _lcd->clear();
    _currentLine1 = "";
    _currentLine2 = "";
}

void TwoLinesCharacterDisplay::clearLine(int lineNumber) {
    if (lineNumber < 0 || lineNumber >= _rows) return;
    
    String emptyLine = "";
    for (int i = 0; i < _cols; i++) {
        emptyLine += " ";
    }
    
    if (lineNumber == 0) {
        setLine1(emptyLine);
    } else if (lineNumber == 1) {
        setLine2(emptyLine);
    }
}

void TwoLinesCharacterDisplay::_updateLine(int lineNumber, const String& text) {
    if (lineNumber < 0 || lineNumber >= _rows) return;
    
    _lcd->setCursor(0, lineNumber);
    _lcd->print(text);
}


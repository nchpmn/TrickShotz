#ifndef GRAPHICS_H
#define GRAPHICS_H

// MAIN TITLE SCREEN
// Fullscreen image - could be trimmed down to save progmem
constexpr uint8_t title[] PROGMEM {
    128, 64,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x80, 0x80, 0x00, 0x80, 0xC0, 0xE0, 0xE0, 0xE0, 0x60, 0x60, 0x60, 0xE0, 0xE0, 0x00, 0x00, 0x70, 0xF8, 0xF8, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFC, 0xFC, 0xFC, 0x00, 0x00, 0xC0, 0xF0, 0xF8, 0xFC, 0x3C, 0x1E, 0x0E, 0x0E, 0x0E, 0x0E, 0x1E, 0x3E, 0xFC, 0xF8, 0xF0, 0xC0, 0x0E, 0x0E, 0x0E, 0x0E, 0xFC, 0xFC, 0xFC, 0x1C, 0x1C, 0x1C, 0x1C, 0x1C, 0x00, 0x1C, 0x1C, 0x3C, 0x38, 0x38, 0xB8, 0xF8, 0xF8, 0xF8, 0x78, 0x30, 0x10, 0xF0, 0xF0, 0xF0, 0xE0, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xC0, 0xE0, 0xF0, 0x70, 0x30, 0x38, 0x38, 0x38, 0x38, 0x00, 0x3C, 0xFE, 0xFE, 0xF0, 0xE0, 0xF8, 0xFE, 0x9F, 0x8F, 0x03, 0x00, 0x00, 0x0F, 0x1F, 0x1F, 0x1C, 0x1C, 0x1C, 0x38, 0x38, 0xF8, 0xF8, 0xF0, 0xE0, 0x00, 0xFF, 0xFF, 0xFF, 0x0E, 0x0E, 0x07, 0x07, 0x07, 0x07, 0x0F, 0xFF, 0xFF, 0xF0, 0x00, 0x1F, 0x7F, 0xFF, 0xF8, 0xE0, 0xC0, 0x80, 0x80, 0x80, 0xC0, 0xE0, 0xF0, 0xFF, 0xFF, 0x3F, 0x07, 0x00, 0x00, 0x00, 0xF8, 0xFF, 0xFF, 0x1F, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0xE0, 0xF0, 0xF8, 0x7E, 0x1F, 0x0F, 0x07, 0x03, 0x01, 0x00, 0xE0, 0xFF, 0xFF, 0x7F, 0x07, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0xC0, 0xC0, 0xE0, 0xE0, 0x70, 0x70, 0x70, 0xF0, 0xF0, 0xE0, 0x0E, 0x3E, 0xFF, 0xFC, 0xE0, 0x80, 0x3F, 0x7F, 0xFF, 0xF1, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xE0, 0xE0, 0x70, 0x03, 0x1F, 0x7F, 0x3F, 0x31, 0x03, 0x07, 0x0F, 0x0F, 0x1E, 0x1C, 0x08, 0x0E, 0x0E, 0x0E, 0x0E, 0x0E, 0x0E, 0x07, 0x07, 0x07, 0x03, 0x01, 0x00, 0x03, 0x03, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x03, 0x03, 0x03, 0x03, 0x03, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x07, 0x07, 0x00, 0x00, 0x00, 0x04, 0x0E, 0x0F, 0x0F, 0x0F, 0x0F, 0x1E, 0x1E, 0x1C, 0x1C, 0x1C, 0x3C, 0x20, 0x3C, 0x7B, 0x7B, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x80, 0xC0, 0xE0, 0xE0, 0x70, 0xF8, 0xF8, 0xFC, 0x8E, 0x0E, 0x06, 0x01, 0x07, 0x1F, 0x7F, 0xFE, 0xFC, 0xFC, 0x8E, 0x0F, 0x1F, 0x1F, 0x3D, 0x38, 0x38, 0x11, 0x07, 0x0F, 0x0F, 0x0C, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x03, 0x0F, 0x1F, 0x1F, 0x1C, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF8, 0x9C, 0xE4, 0xB6, 0xD6, 0xF4, 0x3C, 0x00, 0x00, 0xFC, 0x04, 0x04, 0xFC, 0xF8, 0x30, 0xFC, 0xC4, 0x84, 0xC4, 0x00, 0x00, 0xFF, 0x04, 0x04, 0xFC, 0xF8, 0x00, 0xFC, 0xCC, 0x84, 0x84, 0xFC, 0x30, 0x00, 0xFC, 0x04, 0x04, 0xFC, 0xF8, 0x04, 0x04, 0xFC, 0xF8, 0x00, 0x00, 0xFC, 0x04, 0x04, 0xFC, 0xF8, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

#endif
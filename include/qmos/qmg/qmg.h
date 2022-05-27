#ifndef QMG_INCLUDE_H
#define QMG_INCLUDE_H
#include <qmos/kernel.h>
#include <qmos/qmg/fonts.h>

void qmg_draw_char(char c, uint16_t x, uint16_t y, uint8_t color);
void qmg_draw_string(const char *s, uint16_t x, uint16_t y, uint8_t color);

#endif//QMG_INCLUDE_H

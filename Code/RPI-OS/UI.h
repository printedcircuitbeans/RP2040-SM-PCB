
void drawMainFace(uint16_t Color, uint16_t bg, uint16_t HR, uint16_t MIN, uint16_t SEC, uint16_t DD, uint16_t MM, uint16_t YR){
//128 x 160 screen

}

void DrawThickRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t wallwdth, uint16_t color) {
  for(uint16_t i = 0; i < wallwdth; i++){
    drawRectWH(x + i, y + i, w - i * 2, h - i * 2, color);
  }
}

#include "./codemon.h"

void DrawRectangleLinesExPro(Rectangle rec, Vector2 origin, float rotation, float thickness, Color color) {
    rlPushMatrix();
    
    rlTranslatef(rec.x, rec.y, 0.0f);
    
    rlTranslatef(origin.x, origin.y, 0.0f);
    rlRotatef(rotation, 0.0f, 0.0f, 1.0f);
    rlTranslatef(-origin.x, -origin.y, 0.0f);
    
    DrawRectangleLinesEx((Rectangle){ 0, 0, rec.width, rec.height }, thickness, color);
    
    rlPopMatrix();
}

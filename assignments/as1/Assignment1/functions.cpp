#include "functions.h"

void drawFilledCircle(float red, float green, float blue, float center_x, float center_y, float radius) {
    glColor3f(red, green, blue);

    // draw circle
    glBegin(GL_POLYGON);
    for (int i = 0; i < circleVertices; i++) {
        float theta = ((float)i / circleVertices) * (M_PI * 2.0f);

        glVertex2f(center_x + radius * cos(theta), center_y + radius * sin(theta));
    }
    glEnd();
}

void drawWireframeCircle(float red, float green, float blue, float center_x, float center_y, float radius, float lineWidth) {
    glColor3f(red, green, blue);

    // set line width
    glLineWidth(lineWidth);

    // draw circle
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < circleVertices; i++) {
        float theta = ((float)i / circleVertices) * (M_PI * 2.0f);

        glVertex2f(center_x + radius * cos(theta), center_y + radius * sin(theta));
    }
    glEnd();

    // reset line width
    glLineWidth(1.0f);
}
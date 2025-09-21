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


void drawArc(float red, float green, float blue, float center_x, float center_y, float radius, float lineWidth, float arcStart, float arcEnd) {
    glColor3f(red, green, blue);

    // set line width
    glLineWidth(lineWidth);

    // get total arc
    float totalArc = arcEnd - arcStart;

    // draw circle
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i <= circleVertices; i++) {
        float theta = arcStart + ((float)i / circleVertices) * totalArc;

        glVertex2f(center_x + radius * cos(theta), center_y + radius * sin(theta));
    }
    glEnd();

    // reset line width
    glLineWidth(1.0f);
}

void drawRect(float red, float green, float blue, float start_x, float start_y, float end_x, float end_y, float lineWidth) {
    glColor3f(red, green, blue);

    // set line width
    glLineWidth(lineWidth);

    // draw line
    glBegin(GL_LINE_STRIP);
    glVertex2f(start_x, start_y);
    glVertex2f(end_x, end_y);
    glEnd();

    // reset line width
    glLineWidth(1.0f);
}
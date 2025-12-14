#include <GL/freeglut.h>
#include "audio_analyzer.h"
#include <iostream>

// Global audio analyzer
AudioAnalyzer* g_analyzer = nullptr;
AudioFeatures g_currentFeatures;

void onAudioFeaturesUpdated(const AudioFeatures& features)
{
    g_currentFeatures = features;

    // TODO: Add your own audio-reactive logic here

    // Example: Print when loud sound detected
    if (features.magnitude > 0.5f)
    {
        // std::cout << "Loud sound detected! Magnitude: " << features.magnitude << std::endl;
    }
}

/**
 * Called when magnitude (volume) changes significantly
 */
void onMagnitudeChange(float magnitude)
{
    // TODO: Implement volume-reactive visualizations
}

/**
 * Called when pitch is detected
 */
void onPitchDetected(float pitch)
{
    // TODO: Map pitch to visual properties
}

/**
 * Called when spectral centroid changes (brightness of sound)
 */
void onSpectralCentroidChange(float centroid)
{
    // TODO: Use spectral centroid for brightness effects
}

// ============================================================================
// OPENGL VISUALIZATION FUNCTIONS
// ============================================================================

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // ***********************************************************************
    // ---- EXAMPLE VISUALIZATION (Replace with your own!) ----

    // Draw waveform
    glColor3f(0.0f, 1.0f, 0.0f);
    glBegin(GL_LINE_STRIP);
    for (size_t i = 0; i < g_currentFeatures.waveform.size(); i++)
    {
        float x = -1.0f + 2.0f * i / g_currentFeatures.waveform.size();
        float y = g_currentFeatures.waveform[i] * 0.4f;
        glVertex2f(x, y + 0.5f);
    }
    glEnd();

    // Draw spectrum bars
    glColor3f(1.0f, 0.5f, 0.0f);
    size_t numBars = std::min(g_currentFeatures.spectrum.size(), (size_t)100);
    float barWidth = 2.0f / numBars;

    for (size_t i = 0; i < numBars; i++)
    {
        float x = -1.0f + i * barWidth;
        float height = g_currentFeatures.spectrum[i] * 0.5f;

        glBegin(GL_QUADS);
        glVertex2f(x, -0.5f);
        glVertex2f(x + barWidth * 0.8f, -0.5f);
        glVertex2f(x + barWidth * 0.8f, -0.5f + height);
        glVertex2f(x, -0.5f + height);
        glEnd();
    }

    // Draw magnitude indicator
    glColor3f(1.0f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glVertex2f(-0.9f, -0.9f);
    glVertex2f(-0.9f + g_currentFeatures.magnitude * 1.8f, -0.9f);
    glVertex2f(-0.9f + g_currentFeatures.magnitude * 1.8f, -0.85f);
    glVertex2f(-0.9f, -0.85f);
    glEnd();

    // ***********************************************************************

    glutSwapBuffers();
}

/**
 * Called continuously - use for animations
 */
void idle()
{
    // ***********************************************************************
    // You might want to do animation here.
    // 
    // ***********************************************************************

    #pragma region Audio Feature Processing DO NOT REMOVE
    // Update callbacks based on current features
    static float lastMagnitude = 0.0f;
    if (std::abs(g_currentFeatures.magnitude - lastMagnitude) > 0.05f)
    {
        onMagnitudeChange(g_currentFeatures.magnitude);
        lastMagnitude = g_currentFeatures.magnitude;
    }

    if (g_currentFeatures.pitch > 50.0f)
    { // Valid pitch range
        onPitchDetected(g_currentFeatures.pitch);
    }

    if (g_currentFeatures.spectralCentroid > 0.0f)
    {
        onSpectralCentroidChange(g_currentFeatures.spectralCentroid);
    }
    #pragma endregion

    glutPostRedisplay();
}

/**
 * Handle window resizing
 */
void reshape(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1.0, 1.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
}

/**
 * Handle keyboard input
 */
void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 27: // ESC
    case 'q':
    case 'Q':
        if (g_analyzer)
        {
            g_analyzer->stop();
            delete g_analyzer;
        }
        exit(0);
        break;

        // Optional: Add more keyboard controls
        // Example: Different visualization modes, color schemes, etc.
    case '1':
        std::cout << "Mode 1 selected" << std::endl;
        break;
    case '2':
        std::cout << "Mode 2 selected" << std::endl;
        break;
    }
}

/*
    Function to handle OpenGL initialization params
*/
void initGL() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_LINE_SMOOTH);
    glLineWidth(2.0f);
}

// ============================================================================
// MAIN FUNCTION
// ============================================================================
int main(int argc, char** argv)
{
    // Initialize GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("IGME 309 Final Project - Audio Visualizer");

    // OpenGL setup
    initGL();

    // Initialize audio analyzer
    // Mode 1: microphone mode (recommended)
    //g_analyzer = new AudioAnalyzer(44100, 1024);
    //
    // Mode 2: file mode.
    // WAV file only.
    g_analyzer = new AudioAnalyzer("jingle_bells.wav");

    g_analyzer->setAudioCallback(onAudioFeaturesUpdated);

    if (!g_analyzer->start())
    {
        std::cerr << "Failed to start audio analyzer!" << std::endl;
        delete g_analyzer;
        return 1;
    }

    // Register GLUT callbacks
    glutDisplayFunc(display);
    glutIdleFunc(idle);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);

    // Start main loop
    glutMainLoop();

    // Cleanup
    delete g_analyzer;
    return 0;
}

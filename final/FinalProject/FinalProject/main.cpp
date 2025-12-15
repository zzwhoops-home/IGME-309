#include <GL/glew.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/freeglut.h>
#endif

#include "audio_analyzer.h"
#include "ParticleSystem.h"
#include "Tree.h"
#include "Branch.h"
#include "Lake.h"
#include <iostream>
#include <deque>

// Global audio analyzer
AudioAnalyzer* g_analyzer = nullptr;
AudioFeatures g_currentFeatures;

// store pitch average
std::deque<float> pitchDeque;

const float CAMERA_DISTANCE = 59.0f;
const float CAMERA_HEIGHT = 15.0f;

Tree* tree;
ParticleSystem* particleSystem;
Lake* lake;

float twilight_color[4] = {0.5f, 0.2f, 0.8f, 1.0f};
float sunsetrise_color[4] = {0.6f, 0.1f, 0.1f, 1.0f};
float day_color[4] = {0.3f, 0.6f, 0.8f, 1.0f};

// tracking the game time - millisecond 1)
unsigned int curTime = 0;
unsigned int preTime = 0;
float deltaTime = 0.0f;

void onAudioFeaturesUpdated(const AudioFeatures& features)
{
    g_currentFeatures = features;

    // TODO: Add your own audio-reactive logic here
    particleSystem->setIntensity(features.rms);

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
    //std::cout << magnitude << std::endl;

}

/**
 * Called when pitch is detected
 */
void onPitchDetected(float pitch)
{
    // TODO: Map pitch to visual properties
    //std::cout << pitch << std::endl;
}

/**
 * Called when spectral centroid changes (brightness of sound)
 */
void onSpectralCentroidChange(float centroid)
{
    //std::cout << centroid << std::endl;
    // TODO: Use spectral centroid for brightness effects
}

void updateBGColor()
{
    float ratio = clamp((g_analyzer->getCurrentTime() / g_analyzer->getDuration()), 0.0f, 1.0f) * 2;
    std::cout << ratio << std::endl;

    float r, g, b, a;

    if (ratio > 0.0f && ratio <= 0.4f)
    {
        r = lerp(twilight_color[0], sunsetrise_color[0], ratio / 0.4f);
        g = lerp(twilight_color[1], sunsetrise_color[1], ratio / 0.4f);
        b = lerp(twilight_color[2], sunsetrise_color[2], ratio / 0.4f);
        a = lerp(twilight_color[3], sunsetrise_color[3], ratio / 0.4f);
    }
    else if (ratio > 0.4f && ratio <= 0.7f)
    {
        r = lerp(sunsetrise_color[0], day_color[0], (ratio - 0.4f) / 0.3f);
        g = lerp(sunsetrise_color[1], day_color[1], (ratio - 0.4f) / 0.3f);
        b = lerp(sunsetrise_color[2], day_color[2], (ratio - 0.4f) / 0.3f);
        a = lerp(sunsetrise_color[3], day_color[3], (ratio - 0.4f) / 0.3f);
    }
    else if (ratio > 1.3f && ratio <= 1.6f)
    {
        r = lerp(day_color[0], sunsetrise_color[0], (ratio - 1.3f) / 0.3f);
        g = lerp(day_color[1], sunsetrise_color[1], (ratio - 1.3f) / 0.3f);
        b = lerp(day_color[2], sunsetrise_color[2], (ratio - 1.3f) / 0.3f);
        a = lerp(day_color[3], sunsetrise_color[3], (ratio - 1.3f) / 0.3f);
    }
    else if (ratio > 1.6f)
    {
        r = lerp(sunsetrise_color[0], twilight_color[0], (ratio - 1.6f) / 0.4f);
        g = lerp(sunsetrise_color[1], twilight_color[1], (ratio - 1.6f) / 0.4f);
        b = lerp(sunsetrise_color[2], twilight_color[2], (ratio - 1.6f) / 0.4f);
        a = lerp(sunsetrise_color[3], twilight_color[3], (ratio - 1.6f) / 0.4f);
    }
    else
    {
        r = day_color[0];
        g = day_color[1];
        b = day_color[2];
        a = day_color[3];
    }
    
    glClearColor(r, g, b, a);

    //std::cout << ratio << " " << r << " " << g << " " << b << " " << a << std::endl;
}

// ============================================================================
// OPENGL VISUALIZATION FUNCTIONS
// ============================================================================

void display()
{
    // calls glClearColor();
    updateBGColor();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    glMatrixMode(GL_MODELVIEW);

    float theta = static_cast<float>(curTime) / 10000;

    gluLookAt(
        CAMERA_DISTANCE * cos(theta), CAMERA_HEIGHT, CAMERA_DISTANCE * sin(theta), // eye position
        0.0f, 15.0f, 0.0f, // target
        0.0f, 1.0f, 0.0f // up vector
    );

    tree->draw();

    glDepthMask(GL_FALSE);
    lake->draw();
    glDepthMask(GL_TRUE);

    particleSystem->draw();

    glutSwapBuffers();
}

/**
 * Called continuously - use for animations
 */
void idle()
{
    curTime = glutGet(GLUT_ELAPSED_TIME); // returns the number of milliseconds since glutInit() was called.
    deltaTime = (float)(curTime - preTime) / 1000.0f; // frame-different time in seconds 

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

    // update particle system
    particleSystem->update(deltaTime, static_cast<float>(curTime));

    // update tree
    tree->update_tree(static_cast<float>(deltaTime), g_currentFeatures.pitch);

    // lake
    lake->update_mesh(g_currentFeatures.spectrum);

    preTime = curTime; // the curTime become the preTime for the next frame

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

    // use perspective drawing
    gluPerspective(
        45.0f, // fov
        float(w) / float(h), // aspect ratio
        1.0f, // near clip
        100.0f // far clip
    );

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
        std::cout << "Created new tree!" << std::endl;
        tree->generate_tree();
        break;
    case '2':
        std::cout << "Mode 2 selected" << std::endl;
        break;
    }
}


/**
* Function to handle OpenGL initialization params
*/
void initGL() {
    glClearColor(twilight_color[0], twilight_color[1], twilight_color[2], twilight_color[3]);

    // for 3D rendering
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_COLOR_MATERIAL);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    GLfloat light_ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    GLfloat light_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

    GLfloat position[] = { 25.0f, 0.0f, 25.0f, 0.2f };
    glLightfv(GL_LIGHT0, GL_POSITION, position);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_LINE_SMOOTH);
    glLineWidth(2.0f);

    glewInit();
}

void initTree()
{
    // **Gemini-generated so I didn't have to type everything out**
    // --- Define the default configuration parameters ---
    int MIN_DEPTH = 4;
    int MAX_DEPTH = 6;
    int MIN_CHILDREN = 2;
    int MAX_CHILDREN = 4;

    float START_HEIGHT = 11.0f; // The main trunk is 10 units tall
    float START_WIDTH = 1.5f;   // The main trunk is 1 unit wide

    float HEIGHT_FALLOFF = 0.75f; // Each child branch is 75% the length of its parent
    float WIDTH_FALLOFF = 0.50f;   // Each child branch is 80% the width of its parent

    float MIN_ROTATION = -3.0f;   // -28 degrees (Min random angle, use radians)
    float MAX_ROTATION = 3.0f;    // +28 degrees (Max random angle)

    float MAX_VERT_ANGLE = 0.9f;
    
    // default color (can change)
    const float TREE_COLOR[3] = { 0.78f, 0.10f, 0.00f };

    // --- Instantiate the Tree ---
    tree = new Tree(
        MIN_DEPTH,
        MAX_DEPTH,
        MIN_CHILDREN,
        MAX_CHILDREN,
        START_HEIGHT,
        START_WIDTH,
        HEIGHT_FALLOFF,
        WIDTH_FALLOFF,
        MIN_ROTATION,
        MAX_ROTATION,
        MAX_VERT_ANGLE,
        TREE_COLOR
    );

    tree->generate_tree();
}

void initRain(int NUM_PARTICLES)
{
    particleSystem = new ParticleSystem(NUM_PARTICLES);
}

void initLake()
{
    vec3 offset = vec3(0.0f, -2.0f, 0.0f);
    lake = new Lake(offset, 32, 150.0f, 150.0f);

    lake->init();
}

void initWorld()
{
    // create tree
    initTree();

    // create rain particle system
    initRain(3333);

    // create lake
    initLake();
}

void cleanup()
{
    delete tree;
    delete particleSystem;
    delete g_analyzer;

    tree = nullptr;
    particleSystem = nullptr;
    g_analyzer = nullptr;
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

    // objects setup
    initWorld();

    // Initialize audio analyzer
    // Mode 1: microphone mode (recommended)
    //g_analyzer = new AudioAnalyzer(44100, 1024);
    //
    // Mode 2: file mode.
    // WAV file only.
    g_analyzer = new AudioAnalyzer("blessing_song.wav");

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

    // seed random
    srand(static_cast<unsigned int>(time(NULL)));

    // Start main loop
    glutMainLoop();

    // Cleanup memory
    cleanup();
    return 0;
}

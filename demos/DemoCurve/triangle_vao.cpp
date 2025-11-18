//#include <GL/glew.h>
//#include <GL/freeglut.h>
//#include <iostream>
//
//// Global variables
//GLuint VAO, VBO;
//
//// -----------------------------------------------------------------------------
//// API INTRODUCTION (Fixed-Function VAO/VBO)
//// -----------------------------------------------------------------------------
////
//// glGenVertexArrays(n, &id)
////     Creates VAO objects and returns their IDs.
////
//// glBindVertexArray(id)
////     Activates a VAO. All vertex attribute/state setup goes into it.
////
//// glGenBuffers(n, &id)
////     Creates buffer objects (VBO, EBO, etc.).
////
//// glBindBuffer(GL_ARRAY_BUFFER, id)
////     Activates the buffer for uploading vertex data.
////
//// glBufferData(GL_ARRAY_BUFFER, size, data, usage)
////     Uploads a data array to the currently bound VBO.
////
//// glVertexPointer(size, type, stride, offset)
////     Describes where position data is located inside the VBO.
////
//// glColorPointer(size, type, stride, offset)
////     Describes where color data is located inside the VBO.
////
//// glEnableClientState(GL_VERTEX_ARRAY)
//// glEnableClientState(GL_COLOR_ARRAY)
////     Enables the attribute arrays for drawing (fixed-function pipeline).
////
//// glDrawArrays(GL_TRIANGLES, start, count)
////     Draws primitives using the currently bound VAO.
//// -----------------------------------------------------------------------------
////
//// Also check demo_vbo.cpp in week 6 sample code.
////
//
//// Initialize OpenGL resources
//void initGL()
//{
//    // Vertex data: position (3 floats) + color (3 floats) per vertex
//    float vertices[] = {
//        // positions         // colors
//        0.0f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
//        -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
//        0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f };
//
//    // TODO: Generate VAO
//    // WRITE YOUR CODE HERE
//    glGenVertexArrays(1, &VAO);
//
//    // TODO: Generate VBO
//    // WRITE YOUR CODE HERE
//    glGenBuffers(1, &VBO);
//
//    // TODO: Bind VAO
//    // WRITE YOUR CODE HERE
//    glBindVertexArray(VAO);
//
//    // TODO: Bind VBO to GL_ARRAY_BUFFER
//    // WRITE YOUR CODE HERE
//    glBindBuffer(GL_ARRAY_BUFFER, VBO);
//
//    // TODO: Upload vertex data to GPU
//    // WRITE YOUR CODE HERE
//    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
//
//    // TODO: Configure position attribute (vertex array)
//    // WRITE YOUR CODE HERE
//    glVertexPointer(3, GL_FLOAT, 6 * sizeof(float), (void*)0);
//
//    // TODO: Enable vertex array
//    // WRITE YOUR CODE HERE
//    glEnableClientState(GL_VERTEX_ARRAY);
//
//    // TODO: Configure color attribute (color array)
//    // WRITE YOUR CODE HERE
//    glColorPointer(3, GL_FLOAT, 6 * sizeof(float), (void*)(3 * sizeof(float)));
//
//    // TODO: Enable color array
//    // WRITE YOUR CODE HERE
//    glEnableClientState(GL_COLOR_ARRAY);
//
//    // TODO: Unbind VAO
//    // WRITE YOUR CODE HERE
//    glBindVertexArray(0);
//
//    std::cout << "OpenGL initialized successfully\n";
//}
//
//// Display callback - called every frame
//void display()
//{
//    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
//    glClear(GL_COLOR_BUFFER_BIT);
//
//    glUseProgram(0); // use fixed-function pipeline
//
//    // TODO: Bind VAO
//    // WRITE YOUR CODE HERE
//    glBindVertexArray(VAO);
//
//    // TODO: Draw triangle
//    // WRITE YOUR CODE HERE
//    glDrawArrays(GL_TRIANGLES, 0, 3);
//
//    // TODO: Unbind VAO
//    // WRITE YOUR CODE HERE
//    glBindVertexArray(0);
//
//    glutSwapBuffers();
//}
//
//// Keyboard callback
//void keyboard(unsigned char key, int x, int y)
//{
//    if (key == 27)
//        exit(0); // ESC to quit
//}
//
//int main(int argc, char** argv)
//{
//    glutInit(&argc, argv);
//    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
//    glutInitWindowSize(800, 600);
//    glutCreateWindow("Triangle with VAO - Practice");
//
//    glewExperimental = GL_TRUE;
//    if (glewInit() != GLEW_OK)
//    {
//        std::cerr << "Failed to initialize GLEW\n";
//        return -1;
//    }
//
//    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << "\n";
//
//    initGL();
//
//    glutDisplayFunc(display);
//    glutKeyboardFunc(keyboard);
//
//    std::cout << "\nPRACTICE: Complete the TODO sections to draw a triangle using a VAO.\n";
//    std::cout << "Press ESC to exit.\n\n";
//
//    glutMainLoop();
//    return 0;
//}

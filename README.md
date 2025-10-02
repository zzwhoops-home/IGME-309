# Setup (for myself to remember)

In this example, the version of visual studio is VS 2022 community. All required files, including freeglut, glew and glm, have
been downloaded in the folder called MyLib. Note that MyLib includes files for both x32 and x64 versions.

If you would like to download those files yourself, please read the following paragraphs:

- freeglut: This helps us creating a window for drawing, initializing OpenGL context, and making the gl callback
function available (e.g., mouse, keyboard, display, idle, reshape, etc). Here is the download link:
[freeglut Link](http://freeglut.sourceforge.net/).
- glew: This is GL Extension Wrangler. This adds support for querying and loading OpenGL extensions based on the
platform that executes your programs. This will be needed in order to enable advanced graphics programming
features like shader programming (GLSL) and buffer objects. Here is the download link: [glew Link](http://glew.sourceforge.net/).
- glm: This is a GL math library. This is a neat little math library particularly for OpenGL. This provides classes and
functions for vectors, matrices, quaternions, etc. When people start learning OpenGL, most need to build a small
math library themselves for tasks like transformations, collision/intersection detections, etc. Here is the link to
download: [GLM Link](http://glm.g-truc.net).

The following setup steps assume you are using the given MyLib folder. The steps demonstrate how to set for Win32 platform. If your program targets on x64 platform, change x32 to x64 in the path.

Set path to .h:

1. Go to project properties (right click on the project icon in Solution Explorer, then go to Properties, or
Alt+F7)
2. Under C/C++ -> General, look to right
3. There is Additional Include Directories field.
4. Link your .h path there by adding $(SolutionDir)/MyLib/include.

Set path to .lib:

1. Go to project properties
2. Under Linker -> General, look to right
3. There is Additional Library Directories field.
4. Link your .lib path there by adding $(SolutionDir)/MyLib/lib/x64.
5. Under Linker -> Input, look to right
6. There is Additional Dependencies field.
7. Link your libs there by adding glew32.lib;freeglut.lib.

Set path to .dll:

1. Go to project properties
2. Under Configuration Properties -> Debugging, look to the right.
3. There's an Environment field.
4. Link your .dll path there by adding PATH=%PATH%;$(SolutionDir)/MyLib/dll/x64.

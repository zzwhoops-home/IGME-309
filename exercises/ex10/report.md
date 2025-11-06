# Exercise 10

This exercise used the same bunny we first used to learn about camera rotations around a fixed point. I found the exercise straightforward enough in concept, though I realized that `prepareBufferObjects()` deletes the vertices, which made the drawing weird at first. After I figured that out, it worked fine.

To calculate each of the minimum and maximum points of the bounding box, I just iterated through the vertices and called `std::max` and `std::min` each three times, one for each coordinate. This updated `maxVert` and `minVert` as needed. Then, to draw the box I drew two `GL_LINE_LOOP`s for the top and bottom faces, then connecting `GL_LINES` for the vertical edges.

I learned mostly that `GL_LINE_LOOP` doesn't work well if I try to put all eight vertices at once, it tries to draw them in order but there's a very specific order you have to draw to reach all eight vertices in eight function calls, and that doesn't form a complete box I believe, so it works better to draw two faces separately and then connect them.

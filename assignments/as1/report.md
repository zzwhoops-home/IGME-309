# Assignment 1: Draw a panda - Learning Report
For this assignment, I used the given panda as a reference image, but I just recolored the parts of the panda to make an angry panda with red eyes.

I also created a log statement that would just log the `x` and `y` values that the `keyboard()` function gets whenever GLUT uses it as a callback. This helped me figure out where exactly I should place circles to make everything a little less tedious.

I learned a lot about OpenGL basics from doing this exercise. I was already quite familiar with drawing circles mathematically so that wasn't necessarily a challenge. However, when I first wanted `circleVertices` to update the # of vertices used to draw circles dynamically, I called `display()`. I found out that this was bad practice after asking Gemini if that's what I should do. I found the function `glutPostRedisplay()` so as to alert GLUT that a redraw needs to happen. Calling `display()`, which is already bound to a callback, could cause issues.

For drawing the open arc, I searched online and found that you have to use `GL_LINE_STRIP` in the `glBegin()` call so that the arc doesn't automatically connect back to its origin. I added two parameters, `arcStart` and `arcEnd` which correspond the start and end points of the arc in radians. I suppose this is slightly above and beyond, though not significantly.

I also added a `drawRect()` function which is rather simple and uses `GL_LINE_STRIP` to draw a line from the start point to end point. This code is a simple set of two `glVertex2f` calls.

A good takeaway from this project was to really understand the various drawing modes that GLUT allows you to use. From there, it's really a matter of implementation; you just have to do it with the right tool to begin with.
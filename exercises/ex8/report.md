# Exercise 8: Simple Pendulum

This exercise had us design a pendulum using translate and rotate operations. At first, I was a bit stuck because I didn't think of these operations in the right way.

Eventually, it helped me to understand that each transformation can be thought of as transforming the entire coordinate system. If you apply a rotation, the entire coordinate system becomes tilted. This is the same way you might think about transforming from the standard basis in a Cartesian coordinate system; in fact, matrix transformations are what is happening on the backend.

I set up the rest of the program to run using a `pendulumDistance` variable, in which you can adjust how long the pendulum is. As an addition, I changed the `update()` function to simulate an actual, frictionless & dragless pendulum by calculating the angular acceleration $\alpha = \frac{d^2\theta}{dt^2} = -\frac{g}{L} sin(\theta)$. Every frame, I then added $\alpha$ `* deltaTime` to the `rotateSpeed` variable and `rotateSpeed * deltaTime` to the `rotationAngle` variable.

I also added a `drawCircle()` function which draws a circle using `GL_TRIANGLE_FAN` for the pendulum ball.

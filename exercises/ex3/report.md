# Exercise 3: Reshaping Triangle Report
## Original Solution
I'd like to talk about my original solution, which does not actually meet all the requirements of the rubric. The first thought I had was to think of keeping the ratios all the same. I thought of the triangle as the equilateral triangle that we defined, in which it is perfectly centered and proportioned if we have a square window and the projection goes from 0 to 10 for each axis. This way, the center of the triangle is exactly 5, 5.

I then thought of the projection as we resize the window as having to keep the square window centered on the screen no matter. This, naturally leads to two cases:
1. The window is wider than it is tall `(y >= x)`
2. The window is taller than it is wide `(x > y)`

My mistake was to define a `CONSTANT_HEIGHT` and `CONSTANT_WIDTH` so as to keep one axis consistent: from a minimum of 0 to max of 10, with the other axis scaling as needed. The other axis would then be scaled by the aspect ratio of the screen and then the region from 0 to 10 would be centered. Certainly, this is not what the rubric is asking.

Let us define $x$ as the window width, $y$ as the window height, and our constants height and width as $h$ and $w$. This causes a "margin" to appear which in the case that the window is wider, I'm calling it $a$ such that the projection should go from $-a\rightarrow h+a$ on the x-axis and $0\rightarrow h$ on the y-axis. In other case we have $b$ as the margin, so the projection goes from $-b\rightarrow w+b$ on the y-axis and $0\rightarrow w$ on the x-axis. I hope I'm making sense because this is all wrong anyway.

The margin from scaling by the aspect ratio is then
$$
2a=h \left(\frac{x}{y}\right)-h
\\
a=\frac{h}{2} \left( \frac{x}{y} \right)-\frac{h}{2}
$$
Which we must split in half to distribute the margin evenly on both sides. Calculating through and doing the same calculation for the case where height is higher:

$$
left=-a=-\frac{h}{2} \left( \frac{x}{y} \right)+\frac{h}{2}
\\
right=h+a=\frac{h}{2} \left( \frac{x}{y} \right)+\frac{h}{2}
\\
bottom=-b=-\frac{w}{2} \left( \frac{y}{x} \right)+\frac{w}{2}
\\
top=w+b=\frac{w}{2} \left( \frac{y}{x} \right)+\frac{w}{2}
$$

All this to find out that this was the solution to make the triangle stay in the center of the screen *but not remain the same size*. I think I ended up doing more work so that's a valuable lesson to really think something all the way through before you try it. I don't know how to draw diagrams in Markdown like you can in LaTeX so I hope this made some level of sense.

## Improved Solution & Learning
The intuition I had, then, of improving the solution to function properly was that well, the window scales. By default, we have a screen width and height of 400x400. The top of the triangle is a line from vertex (3, 7) to (7, 7). The projection must maintain this length no matter what the screen size is, while also maintaining the ratio of the rest of the triangle. So I realized I hadn't wasted my time completely, but needed to refine my process.

I realized that instead of having to have the projection center on the triangle, I can just center the triangle at 0, 0 instead. Hopefully that's valid. The vertices of the upside-down triangle are then (-2, 2), (2, 2), and (0, -2) instead.

By thinking this way, if everything is already centered, the bounds of the projection can just be a simple scaling based on the size of the window. I felt quite stupid realizing this but I did figure out how to keep shapes at a fixed *ratio* as opposed to *size*. I'd think it's quite useful for building games which may be played at different aspect ratios and screen sizes.

The final solution is as follows:

Define $\lambda$ as a scaling ratio for translating from the window dimensions to the orthographic projection. `0.01` seems to work quite well. We get the window width and height, which we will say *now* is $W$ and $H$.
$$
w=\lambda W
\\
h=\lambda H
$$
We want everything to be centered, so we say that the `halfWidth` and `halfHeight` are $\frac{1}{2} w$ and $\frac{1}{2} h$ respectively. So:
$$
left=-\frac{1}{2}w
\\
right=\frac{1}{2}w
\\
bottom=-\frac{1}{2}h
\\
top=\frac{1}{2}h
$$

Our final result is then

```gluOrtho2D(-halfWidth, halfWidth, -halfHeight, halfHeight)```

That's it. It was an overcomplicated yet fruitful learning experience.
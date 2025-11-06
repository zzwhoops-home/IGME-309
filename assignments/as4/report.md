# Assignment 4

This assignment dealt with the hierarchical rotation of body parts composing the figure of a robot. Parts which were "parents", or "attached" to other parts would rotate together, and parts which were "children" would not affect parts they shouldn't. It was an interesting thought exercise, and I think there could be improvements made by building out a data structure that would handle a parent's children so that the actual drawing process could be done using a for loop. However, I just hard coded the whole drawing with some helper functions.

I wrote a function, `drawRect` to handle the drawing of rectangles, and also a `drawTransformed` function that would apply the transformations and draw the shape at a given index. This allowed me to use a translation to offset the point of rotation to the center of the shape, then pop that translation off the matrix stack so it applies uniquely to each object. I was then able to draw each shape by simply defining its coordinates based on that of the parent, making it much easier to work with.

I don't think I implemented this perfectly; it's not a very scalable solution. However, it does rotate the parts from any anchor point I specify - top, right, left, or bottom depending on the part.

To rotate the selected part, use 'a' and 'd' to rotate the figure left and right, respectively. Use 'up' and 'down' to select a different part.
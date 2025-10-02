# Exercise 6: VBO, IBO, Draw Methods

Intro

One takeaway I learned from doing this assignment was how `GL_STATIC_DRAW` works. I just wasn't really sure what the point of it was, but I looked it up and I found that `GL_STATIC_DRAW` has two other alternatives, `GL_STREAM_DRAW` and `GL_DYNAMIC_DRAW`, which concern how the data may be modified and udpated. The idea is that this will alert the system to do what it thinks will be most "optimized". `GL_STATIC_DRAW`, specifically, is essentially saying that we will load vertex data once and never modify it.

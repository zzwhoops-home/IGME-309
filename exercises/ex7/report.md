# Exercise 7: Orbiting Camera

I guess this exercise didn't have a lot in terms of coding. I simply added `deltaTime` to `theta` to keep a consistent rotation speed based on framerate. The y coordinate does not change, so I did not change y. However, for x and z, I varied x according to $ rcos(\theta) $ and z according to $ rsin(\theta) $, which will result in a circular orbit. I then just called `reshape()` as instructed which updates the camera. An improvement I made was just to add a `rotationSpeed` variable to control how fast the rotation is.

# Assignment 3: Particle System

This assignment was my first foray into 3D using OpenGL. The principles are largely the same, nothing too conceptually difficult since the camera system was written for me. I implemented a function, `reset(int i)` which I could call not only for the initialization step, but also whenever a particle "expired". I would simply just reset its position to the origin and assign it a new velocity and color. The difference I made from the original was that I randomized the colors of the 

## Performance Testing

I did performance testing by creating a fixed-size array that would track 20 FPS changes (taking 20 seconds each). It would then print out the mean $\mu$ and sample standard deviation $s^2$, and calculate a 2-sided 95% CI. I would wait three seconds for the initial lag to subside before starting to track this calculation.
- 5 particles: $ \mu = 721.35 \pm 12.06 $ FPS 
- 50 particles: $ \mu = 715.45 \pm 14.89 $ FPS
- 500 particles: $ \mu = 698.9 \pm 10.03 $ FPS 
- 5,000 particles: $ \mu = 578.35 \pm  15.47 $ FPS
- 50,000 particles: $ \mu = 222.70 \pm 7.37 $ FPS
- 500,000 particles: $ \mu = 30.30 \pm 0.44 $ FPS
- 5,000,000 particles: $ \mu \approx 1.1 $ FPS
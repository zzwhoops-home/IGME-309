# Exercise 4: Bouncing Ball

## Initial Solution

So, this exercise required me to implement simple collision between a ball and a platform that you could control with your keyboard.

I started by writing the bar movement. I took a look at the variables and at first, just checked if `barPos[0]` (x-position) was less than 0.0f (the left-edge of the screen), but I quickly realized that I can't read and there's a comment saying the `barPos` array is actually the position of the middle of the bar. So, I changed my code to check `if (barPos[0] - (barSize / 2.0f) >= 0.0f)`. I realized soon after that this would still make the bar go off the screen, so I included `STEP_SIZE` as part of the calculation: `if (barPos[0] - (barSize / 2.0f) - STEP_SIZE >= 0.0f)`. The same calculation occurs for the right-side of the screen: `if (barPos[0] + (barSize / 2.0f) + STEP_SIZE <= canvasSize[0])`.

For the collision checks, I figured that you could simply check the height of the ball and make sure it is in range of the bar. This caused one issue, though: if you moved the bar over the ball after you missed, it would start vibrating because the ball was "under" the bar. I changed it so that if the ball's center is between 0-1 of its radius under the bar, the collision would occur. This is not a perfect system but prevents the previous error from occuring.

## Improvements and Takeaways

I improved my code by creating two booleans in the collision checking code (`ballYCollisionCheck` and `ballXCollisionCheck`) to make it cleaner and more readable. These would perform the y position check and x-position checks which deal with the ball being at the right height to collide with the bar and also the right position, respectively.

A takeaway is to really understand how mechanics are implemented before getting started with the assignment. In this case, it was position, which I assumed start a corner of the rectangle when it really was the center. The code that you write is different in each case.

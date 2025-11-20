# Assignment 5: Monster Hunting!

This assignment required us to implement a custom "hunter", and I had some ideas. After implementing circle collision, I found that the hunter did best by just having a maximum fire rate and bullet speed. I'm not sure how well that stacks up when placed with everyone else but whatever.

The first thing I did was to overhaul the velocity by applying forces similar to how we did our "NPC World" assignment in IGME-202. I apply a "flee force" which grows significantly as the nearest enemy gets closer to you. I also decelerate the player slowly every frame, and I made sure to clamp the speed to the maximum possible value of the hunter so as to not cheat.

I tried to look into finding the ideal position on such a continuous 2D space and dug a little into convex hulls and Voronoi diagrams but that was a bit much since I had so much other work from other classes. Then, I tried to just implement a brute force grid search which would split the screen into a 100x100 grid and find the grid that would maximize the minimum distance from every enemy. That proved to be immensely laggy and so I changed the grid to 10x10 and used squared distance instead. I apply a force to the player based on this ideal location. It is still dominated by the flee force, though.

Finally, I added a feature that would find the nearest player on the screen and apply a force that would move a hunter behind the nearest player. I figure if I can't run away from the monsters because I've set speed and armor to 0, I can drag someone else down first.

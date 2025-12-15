# Final Project - Report - Zachary Deng

## Design summary

This final project was a lot of fun to make! I think I created something that aligns well with my original artistic vision, even if I didn't implement every detail I wanted. I'll start wtih some of the minor things I missed or changed to make it simpler.

### Things removed

- The tree doesn't have fruit growing on the end. This was just an additional step that took extra time and although it would've been nice to have it since I wanted to have the "Tree of Good and Evil", I didn't have time in the end.

## Implementation summary

My program was structured into multiple different classes that had `draw()`, `update()`, and various other functions to handle creating their underlying structures. I wanted to structure it this way so that the code didn't get all cluttered in main and so I could have an `initWorld()` function which initializes all the objects, 

### Tree

For the tree, I ended up creating a `Tree` and `Branch` class. The tree handles the creation of the first "branch", which is the tree trunk and the initialization of the tree parameters. The `Branch` will recursively create new child branches that extend from the ends of its parent branch to create this pattern which fans out. I'll talk about this more in the Lessons learned section. 

### Rain

For the rain, I really just cannibalized Assignment 3's code as much as I could and implemented it the same way, just with a random initial position along the horizontal plane and a smaller horizontal force. These disappeared when they hit the ground instead of falling forever and were moved back up to be used again. I also added some extra parameters to control the rain's intensity so I could integrate it with the rest of the program. These were an `activeParticles` parameter and a `maxRMS` parameter. The `maxRMS` normalizes and clamps the RMS to about `1.0`, and the `activeParticles` is calculated from this value. The particles which are "active" have a higher opacity, the particles which are not essentially disappear.

The rain looked a bit more like snow and was harder to see if I made the particles blue, so perhaps this is just blizzarding snow. I added a wind effect which rotates a velocity vector around randomly in a circle, increasing in intensity based on the RMS.

## Lessons learned

I learned a lot while working on this project, especially dealing with 3D space. I didn't have time to figure out how to get my solution to work with shaders and I wish I did but it was just such a busy semester. I ended up going with a simple immediate-mode drawing mechanism.

The tree was drawn with the mechanism of a random walk. I actually had a good review of spherical coordinates which involved me defining a coordinate system with a local y-axis as the plane perpendicular to a parent branch's length. Then, from calculating an orthonormal basis, calculating spherical coordinates $P = (r \cos (\theta) \sin(\phi), r \sin(\theta) \sin(\phi), r \cos (\theta))$. I calculated an "up" direction from this and 

For calculating pitch, I messed around with the code because it was giving me a constant 44.1 kHz or 48 kHz no matter what I tried. I ended up just using Gemini's code for the pitch because it wasn't part of the project requirements, but I wanted to get it to work. I just learned that you could use RMS as a "silence gate", ignore the 20 indices, and also first search through earlier peaks to check for subharmonics because I noticed the pitch would jump up randomly. 
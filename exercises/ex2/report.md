# Exercise 2: WebGL Triangle Drawing
I followed the instructions in the [provided article](https://adammurray.link/webgl/tutorials/triangle/) and a supplementary link that explained some more concepts in detail [here](https://javascript.plainenglish.io/breaking-down-the-webgl-triangle-setup-302a03ffa056).

Overall, there are many new concepts here which are kind of confusing. I understood the steps at first on how to create the triangle but a lot of the setup felt quite abstract. After implementing the code there is still some learning I have to do to understand why it is not as simple as just drawing a shape with vertices.

I didn't go into too much depth or "improve" my code somehow, but it does look like the vertex/fragment shader setup is something that I could create a helper function for.

After reading some more articles and giving Gemini my code and asking it to explain each step, here's what I learned:
1. The vertex and fragment shaders are created. The vertex shaders manage object geometry, while the fragment shaders act on every pixel and manage colors. They're linked to a shader program on the GPU.
2. The vertex data is prepared on the CPU, which is then written to the GPU's memory via buffers. The CPU will then send a draw call to the GPU, like instructions on how to actually use the information in the buffers.
3. The GPU runs the compiled vertex shader (with the positions of the triangle) and fragment shader (which makes the triangle red), and the pixels are then written to the screen.
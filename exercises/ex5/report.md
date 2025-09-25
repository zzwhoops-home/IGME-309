# Exercise 5: .OBJ Mesh Loader

## Original Solution

I'd forgotten how to read files and so I had to just do a Google search on how this is done with `std::getline()` in C++.
My original solution was to simply read in the files, line by line, and then implement what is best described as the `.split()` functionality for strings in Python or JavaScript. Using `.substr()` and `.find()`, I could `find` the index of a space for each line, and then use `substr` to extract the contents between two spaces. I used `stof` and `stoi` to convert these extracted strings into `float`s and `int`s, respectively.

## Improved Solution & Learning

I made one mistake in logic after I tried running the program with `data2.obj`. Everything was very jagged and you could not discern the "IGM" logo in any shape or form. I didn't know what to do about this, but then I read the assignment description and the tip that `.obj` indices start at 1. I wasn't sure what this meant, but when asking AI, I found that I was adding my indices wrong. The line `f 1 2 3` was being added into the array as index 1, 2, and 3. However, as arrays are 0-indexed, I was pulling the vertices in an offset way that resulted in nonsense being drawn onto the screen. This is my biggest takeaway from doing this exercise, to be careful of indexing and to understand that possible fixes for drawings that don't make sense should consider indexing issues. So, I simply subtracted 1 from each index, so `f 1 2 3` is stored as creating a face with vertices 0, 1, and 2.

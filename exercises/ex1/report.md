# Exercise 1: Number Guessing Game Report
My original solution was fairly straightforward: get a number between 1 and 100 (inclusive) by using `Math.random()`. The range of this result is multiplied by `MAX_NUM` (100) and then 1 is added to move the range up from 0-99 to 1-100 since we're wrapping the whole thing in a `Math.floor()`.

The guess is parsed and it is checked accordingly. If the number is exact (`v === rnd`), the user is informed that their answer is correct. An `else` clause makes sure the number is valid at all. After asking Gemini how I could improve the game, it told me that I could be more specific in my error checking.

For the improved version, I added a check for the lower and upper bounds. This way, if the user guesses a number that is out of range, they will be informed instead of being told that their guess is wrong. I also created a helper function, `checkGuess()` which just handles the checking and printing of feedback on the user's guess.

The knowledge point is the reminder that you should always check for edge cases and also provide enough information to the user so that they know how to use your application. It might be confusing why a number which is out of range is just "invalid" and not "out of range". Also, it helps to add helper functions to keep code well-structured and reusable.
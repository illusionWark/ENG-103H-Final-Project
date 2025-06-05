# ENG-103H-Final-Project
ENG 103H final project repository. Lock breaking game with score

This game will be similar to lock breaking arcade games, where you have to get the ring in a certain zone,
then hit a button to “crack the lock.” It will have all lights on, then will select one at random, make it a different color, 
then “spin” another light around the circle. If the user hits the correct input at the right time, 
then make the built-in LED blink equal to the number of points scored. 
If the player doesn’t do this, the board will play a sad sound, blink red, and make you lose all of your points.

The light will spin around in one of two directions, and the user must hit the correct button correlating to the direction when the spinning light is on top of the selected light.
THe light spinning clockwise means you need to hit the right button, if it is spinning counterclockwise, you need to hit the left button
If that is the case, the user gets a point. If not, the user fails and loses all of their points. 

Inputs:
	Left Button - For lock breaking input
	Right Button - For lock breaking input
	Switch - For turning device “on” and “off”

Outputs:
	Neopixels - For the main game portion
	Speaker - To help identify when a point has been scored / when the user loses
	Built in LED - Displays score

# Sound Box

This was a collaboration with an University student which I helped to build and code a sound box.


# Files

There are two arduinos in this project, the main one is taking care of detecting the touch of 6 pads and the second one is taking care of the looping of the song + 2 touch pad.
The main network between these two devices is Serial communication, which sends a small amount of information between each others.

The tricky part was to play two sound at the same time, which wasnt possible with the current MP3 shield.
In this case, the first Arduino play the instrument which the user pressed and then sends to the second Arduino which sound track it has to loop. If one of the 2 pads, that the first arduino couldn't read, are pressed, the second Arduino is sending this information to the first one.



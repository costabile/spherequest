# Introduction #
I'll summarize my changes here so you guys know what's been done.



# Details #

**As of: Saturday Feb. 27th**
  * Main thing is I changed keyboard control from moving the camera to moving the sphere.  The camera follows the sphere around.  This took me a while to figure out cause I was using the wrong crap for calculations. Blah.
  * A couple of constants are defined at the top of the file for the distance the sphere moves and the angle it turns with each keypress.  If movement needs to be smoother, fiddle around with these.
  * I'm working on adding lighting so that everything doesn't look so 2D.  Problem is, when I enable lighting, all objects lose their colour.  I'll look into this later, unless you guys know why that happens.
  * I improved the HUD, in the sense that it now sticks to the camera when you move around.  However, when you turn, it flips out a bit.  I haven't figured out the correct position calculation as of yet.
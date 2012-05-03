/* Interface is just a wrapper class, it actually does not deal at all with the 
 * actual creation of the lower peices, it simply holds them. It is the Interface
 * that has the responsibility to bring the peices together, you interface to the
 * other sub peices out side of this, that's what makes this system convenient.
 * you will notice however that the interface does keep the size of the display.
 * While cameras do this to aswell, the cameras may not always be concerned with
 * what the parent window is actually at. for example a camera that is for a 
 * HUD overlay doesn't care what the main window size is. This keeps track of the
 * main window size so that when a camera is punched in, it can choose to take
 * that info or not.
 * Also, mx and my are the most recent position of the mouse. and lbutton and
 * rbutton hold the most recent state of the left and right mouse buttons.
 */
#ifndef INTERFACE_CPP
#define INTERFACE_CPP
#include "Interface.h"



/*.S.D.G.*/

#endif

/* Copyright (c) 2007 Scott Lembcke
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
 
#include <stdlib.h>
#include <math.h>

#include "chipmunk.h"
#include "drawSpace.h"
#include "ChipmunkDemo.h"

static cpSpace *space;

// Init is called by the demo code to set up the demo.
static cpSpace *
init(void)
{
	// Create a space, a space is a simulation world. It simulates the motions of rigid bodies,
	// handles collisions between them, and simulates the joints between them.
	space = cpSpaceNew();
	
	// Lets set some parameters of the space:
	// More iterations make the simulation more accurate but slower
	space->iterations = 1;
	// Give it some gravity
//	space->gravity = cpv(0, -100);
	
	// Create A ground segment along the bottom of the screen
	// By attaching it to &space->staticBody instead of a body, we make it a static shape.
	cpShape *ground = cpSegmentShapeNew(space->staticBody, cpv(-320,-240), cpv(320,-240), 0.0f);
	// Set some parameters of the shape.
	// For more info: http://code.google.com/p/chipmunk-physics/wiki/cpShape
	ground->e = 1.0f; ground->u = 1.0f;
	ground->layers = NOT_GRABABLE_MASK; // Used by the Demo mouse grabbing code
	// Add the shape to the space as a static shape
	// If a shape never changes position, add it as static so Chipmunk knows it only needs to
	// calculate collision information for it once when it is added.
	// Do not change the postion of a static shape after adding it.
	cpSpaceAddShape(space, ground);
	
	for(int i=-5; i<=5; i++){
		cpFloat radius = 25.0f;
		cpFloat mass = 1.0f;
//		cpBody *ballBody = cpBodyNew(INFINITY, cpMomentForCircle(mass, 0.0f, radius, cpvzero));
		cpBody *ballBody = cpBodyNew(mass, INFINITY);
		ballBody->p = cpv((5 + 2*radius)*i, 200);
		cpSpaceAddBody(space, ballBody);
		
		cpShape *ballShape = cpSpaceAddShape(space, cpCircleShapeNew(ballBody, radius, cpvzero));
		ballShape->e = 0.0f; ballShape->u = 0.9f;
		
		float stiffness = 30;
		float damping = 2.0f*sqrt(stiffness*ballBody->m)*cpfpow(1.1f, i);
//		float stiffness = 30;
//		float damping = 0.0;
		cpSpaceAddConstraint(space, cpDampedSpringNew(space->staticBody, ballBody, cpv((5 + 2*radius)*i, 0), cpvzero, 0, stiffness, damping));
	}
	
	return space;
}

// Update is called by the demo code each frame.
static void
update(int ticks)
{
	// Chipmunk allows you to use a different timestep each frame, but it works much better when you use a fixed timestep.
	// An excellent article on why fixed timesteps for game logic can be found here: http://gafferongames.com/game-physics/fix-your-timestep/
	cpSpaceStep(space, 1.0f/60.0f);
}

// destroy is called by the demo code to free all the memory we've allocated
static void
destroy(void)
{
	ChipmunkDemoFreeSpaceChildren(space);
	cpSpaceFree(space);
}

chipmunkDemo Simple = {
	"Simple",
	NULL,
	init,
	update,
	destroy,
};

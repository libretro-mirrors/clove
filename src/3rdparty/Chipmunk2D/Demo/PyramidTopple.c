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

static void
update(int ticks)
{
	int steps = 3;
	cpFloat dt = 1.0f/60.0f/(cpFloat)steps;
	
	for(int i=0; i<steps; i++)
		cpSpaceStep(space, dt);
}

#define WIDTH 4.0f
#define HEIGHT 30.0f

static void
add_domino(cpSpace *space, cpVect pos, cpBool flipped)
{
	cpFloat mass = 1.0f;
	cpFloat moment = cpMomentForBox(mass, WIDTH, HEIGHT);
	
	cpBody *body = cpSpaceAddBody(space, cpBodyNew(mass, moment));
	body->p = pos;

	cpShape *shape = (flipped ? cpBoxShapeNew(body, HEIGHT, WIDTH) : cpBoxShapeNew(body, WIDTH, HEIGHT));
	cpSpaceAddShape(space, shape);
	shape->e = 0.0f; shape->u = 0.6f;
}

static cpSpace *
init(void)
{
	cpResetShapeIdCounter();
	
	space = cpSpaceNew();
	space->iterations = 30;
	space->gravity = cpv(0, -300);
	space->sleepTimeThreshold = 0.5f;
	space->collisionSlop = 0.5f;
	
	// Add a floor.
	cpShape *shape = cpSpaceAddShape(space, cpSegmentShapeNew(space->staticBody, cpv(-600,-240), cpv(600,-240), 0.0f));
	shape->e = 1.0f; shape->u = 1.0f;
	shape->layers = NOT_GRABABLE_MASK;
	
	
	// Add the dominoes.
	int n = 12;
	for(int i=0; i<n; i++){
		for(int j=0; j<(n - i); j++){
			cpVect offset = cpv((j - (n - 1 - i)*0.5f)*1.5f*HEIGHT, (i + 0.5f)*(HEIGHT + 2*WIDTH) - WIDTH - 240);
			add_domino(space, offset, cpFalse);
			add_domino(space, cpvadd(offset, cpv(0, (HEIGHT + WIDTH)/2.0f)), cpTrue);
			
			if(j == 0){
				add_domino(space, cpvadd(offset, cpv(0.5f*(WIDTH - HEIGHT), HEIGHT + WIDTH)), cpFalse);
			}
			
			if(j != n - i - 1){
				add_domino(space, cpvadd(offset, cpv(HEIGHT*0.75f, (HEIGHT + 3*WIDTH)/2.0f)), cpTrue);
			} else {
				add_domino(space, cpvadd(offset, cpv(0.5f*(HEIGHT - WIDTH), HEIGHT + WIDTH)), cpFalse);
			}
		}
	}
	
	return space;
}

static void
destroy(void)
{
	ChipmunkDemoFreeSpaceChildren(space);
	cpSpaceFree(space);
}

chipmunkDemo PyramidTopple = {
	"Pyramid Topple",
	NULL,
	init,
	update,
	destroy,
};

#include <SDL2/SDL.h>
#include <stdio.h>

#include "Quadtree.h"
#include "Object.h"

int main( int argc, char *argv[] )
{
	SDL_Window *window = SDL_CreateWindow( "Quadtree",
						SDL_WINDOWPOS_UNDEFINED,
						SDL_WINDOWPOS_UNDEFINED,
						1024,
						768,
						0 );

	SDL_Renderer *renderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED );

	Quadtree tree( 0, 1024, 0, 768 ) ;
	vector<Object> objects;
	double objLeft = 0;
	double objTop = 0;
	double objRight = 0;
	double objDown = 0;
	
	bool running = true;
	while ( running ) {
		SDL_Event event;
		double mouseX = event.button.x;
		double mouseY = event.button.y;
		while ( SDL_PollEvent( &event ) ) {
			if ( event.type == SDL_QUIT ) {
				running = false;
			} else if ( event.type == SDL_MOUSEBUTTONDOWN ) {
				objLeft = mouseX;
				objTop = mouseY;
			} else if ( event.type == SDL_MOUSEBUTTONUP ) {
				objRight = mouseX;
				objDown = mouseY;
				objects.push_back( Object( objLeft, objRight, objTop, objDown ) );
			}
		}

		for ( unsigned int n = 0; n < objects.size(); ++n ) {
			tree.AddObject( &objects[n] );
		}

		/** Log */
		vector<Object*> getObjects;
		getObjects = tree.GetObjectsAt( mouseX, mouseY );
		printf( "Objects at %lf %lf: %u\n", mouseX, mouseY, getObjects.size() );

		/** Rendering */
		SDL_SetRenderDrawColor(renderer, 48, 10, 36, 255);
		SDL_RenderClear( renderer );
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		tree.Display( renderer );
		SDL_Rect rect;
		for ( unsigned int n = 0; n < objects.size(); ++n ) {
			rect.x = objects[n].left;
			rect.y = objects[n].top;
			rect.w = objects[n].right - objects[n].left;
			rect.h = objects[n].down - objects[n].top;
			SDL_RenderDrawRect( renderer, &rect );
		}
		SDL_RenderPresent( renderer );
		tree.Clear();
		SDL_Delay( 8 );
	}

	SDL_DestroyRenderer( renderer );
	SDL_DestroyWindow( window );

	return 0;
}

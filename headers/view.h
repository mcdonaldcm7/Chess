#ifndef VIEW_H_
#define VIEW_H_

#include <SDL2/SDL.h>

bool init(void);
void wrapUp(void);
SDL_Window* createWindow(const char*, int, int, int, int, unsigned int);
SDL_Texture* loadImage(const char*, SDL_Renderer*);
SDL_Renderer* createRenderer(SDL_Window*, unsigned int);

#endif

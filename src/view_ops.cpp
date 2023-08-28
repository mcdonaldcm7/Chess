#include "../headers/view.h"
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>

/*
 * init - Performs initialization operations for SDL and SDL_image
 *
 * Return: true if initialization was a success, Otherwise false
 */
bool init(void)
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		SDL_Log("Failed to initialize SDL!!!\nSDL Error Output: %s\n",
				SDL_GetError());
		return (false);
	}

	if (!IMG_Init(IMG_INIT_PNG))
	{
		SDL_Log("Failed to initialize SDL_image!!!\nSDL_image Error Output: %s\n",
				IMG_GetError());
		return (false);
	}
	return (true);
}

/*
 * createWindow - Creates and returns window based on the arguments passed
 *
 * @title: Title to intialize the window with
 * @width: Width to initialize the window with
 * @height: height to initialize the window with
 * @x: Position of the window on the x-axis
 * @y: Position of the window on the y-axis
 * @flags: Flags to set for the window
 *
 * Return: Pointer to the created window if successful, Otherwise a nullptr
 */
SDL_Window* createWindow(const char* title, int width, int height, int x,
		int y, unsigned int flags)
{
	SDL_Window* window;

	window = SDL_CreateWindow(title, x, y, width, height, flags);
	if (window == nullptr)
	{
		SDL_Log("Failed to create window!!!\nSDL Error Output: %s\n",
				SDL_GetError());
		SDL_Quit(); // Clean up SDL if initialization fails
		return (nullptr);
	}
	return (window);
}

/*
 * loadImage - Loads and returns a texture from the specified image path
 *
 * @path: Path to the image
 *
 * Return: Pointer to a texture loaded from the image
 */
SDL_Texture* loadImage(const char* path, SDL_Renderer* renderer)
{
	SDL_Surface* image;
	SDL_Texture* texture;

	image = IMG_Load(path);
	if (image == nullptr)
	{
		SDL_Log("Unable to load image from %s\nError Output: %s\n",
				path, IMG_GetError());
		return (nullptr);
	}
	texture = nullptr;
	if (image != nullptr)
	{
		texture = SDL_CreateTextureFromSurface(renderer, image);
		if (texture == nullptr)
		{
			SDL_Log("Unable to create texture from %s\nError Output: %s",
					path, SDL_GetError());
		}
		SDL_FreeSurface(image);
	}

	return (texture);
}

/*
 * createRenderer - Creates and returns an SDL_Renderer object from a window
 *
 * @window: Window where rendering is displayed
 * @flags: SDL_RendererFlags to assign to the renderer
 *
 * Return: Pointer to the created SDL_Renderer
 */
SDL_Renderer* createRenderer(SDL_Window* window, unsigned int flags)
{
	SDL_Renderer* renderer;

	renderer = nullptr;
	if (!window)
	{
		SDL_Log("NULL SDL_Window passed!!!\n");
		return (renderer);
	}
	renderer = SDL_CreateRenderer(window, -1, flags);
	if (renderer == nullptr)
	{
		SDL_Log("Failed to create renderer!!!\nError Output: %s\n",
				SDL_GetError());
	}
	return (renderer);
}

/*
 * wrapUp - Closes SDL and SDL_image
 *
 * Return: Nothing
 */
void wrapUp(void)
{
	SDL_Quit();
	IMG_Quit();
}

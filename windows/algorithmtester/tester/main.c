/*   WreckerTester V.03
 *   Main.c
 */
#include <SFML/Audio.h>
#include <SFML/Graphics.h>
#include <SFML\System.h>

#include <stdlib.h>

#include "perlinnoise.h"

#define WIDTH 512
#define HEIGHT 512
#define FEATURE_SIZE 24

int main()
{
	sfVideoMode mode = { WIDTH, HEIGHT, 32 };
	sfRenderWindow* window;
	sfEvent event;

	/* Create the main window */
	window = sfRenderWindow_create(mode, "SFML window", sfResize | sfClose, NULL);
	if (!window)
	{
		return 1;
	}

	int x, y;
	double value;
	double v0, v1, v2; /* values from different octaves. */
	uint32_t rgb;
	struct osn_context *ctx;

	open_simplex_noise(77374, &ctx);

	sfImage * img = sfImage_create(WIDTH, HEIGHT);
	for (y = 0; y < HEIGHT; y++) {
		for (x = 0; x < WIDTH; x++) {
			/* Use three octaves: frequency N, N/2 and N/4 with relative amplitudes 4:2:1. */
			v0 = open_simplex_noise4(ctx, (double)x / FEATURE_SIZE / 4,
				(double)y / FEATURE_SIZE / 4, 0.0, 0.0);
			v1 = open_simplex_noise4(ctx, (double)x / FEATURE_SIZE / 2,
				(double)y / FEATURE_SIZE / 2, 0.0, 0.0);
			v2 = open_simplex_noise4(ctx, (double)x / FEATURE_SIZE / 1,
				(double)y / FEATURE_SIZE / 1, 0.0, 0.0);
			value = v0 * 4 / 7.0 + v1 * 2 / 7.0 + v2 * 1 / 7.0;

			rgb = 0x010101 * (uint32_t)((value + 1) * 127.5);

			sfImage_setPixel(img, x, y, sfColor_fromInteger(rgb));
		}
	}

	sfTexture* tex = sfTexture_createFromImage(img,NULL);

	sfSprite* sprite = sfSprite_create();
	sfSprite_setTexture(sprite, tex, 1);
	sfVector2f pos;
	pos.x = 0;
	pos.y = 0;
	sfSprite_setPosition(sprite, pos);

	/* Start the game loop */
	while (sfRenderWindow_isOpen(window))
	{

		/* Process events */
		while (sfRenderWindow_pollEvent(window, &event))
		{
			/* Close window : exit */
			if (event.type == sfEvtClosed)
				sfRenderWindow_close(window);
			else{
				
			}
		}
		/* Clear the screen */
		sfRenderWindow_clear(window, sfRed);

		sfRenderWindow_drawSprite(window, sprite, NULL);

		/* Update the window */
		sfRenderWindow_display(window);
	}

	/* Cleanup resources */
	sfRenderWindow_destroy(window);

	return 0;
}
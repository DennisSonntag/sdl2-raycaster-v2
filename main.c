#include <SDL2/SDL.h>

#include <stdbool.h>

#include "types.h"
#include "utils.h"

#define width 500
#define height 500
const i32 ray_length = width * 2;

typedef struct {
	f32 x1, y1, x2, y2;
} Vector;

typedef struct {
	f32 x, y;
} Point_f;

Vector new_ray(SDL_Point center_point, i32 angle) {
	f32 x2 = center_point.x + ray_length * cos(angle * M_PI / 180);
	f32 y2 = center_point.y + ray_length * sin(angle * M_PI / 180);

	return (Vector){center_point.x, center_point.y, x2, y2};
}

f32 distance_between(SDL_Point p1, Point_f p2) { return sqrt(powf(p2.x - p1.x, 2) + powf(p2.y - p1.y, 2)); }

void draw_vector(SDL_Renderer *renderer, Vector vector, SDL_Color color) {
	f32 vector_length = sqrt(pow(vector.x2 - vector.x1, 2) + pow(vector.y2 - vector.y1, 2));
	f32 vector_thickness = 2.5;

	f32 vector_angle = atan2(vector.y2 - vector.y1, vector.x2 - vector.x1);

	SDL_Surface *surface = SDL_CreateRGBSurfaceWithFormat(0, vector_length, vector_thickness, 32, SDL_PIXELFORMAT_RGBA8888);

	SDL_FillRect(surface, NULL, SDL_MapRGBA(surface->format, color.r, color.g, color.b, color.a));

	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);

	SDL_Rect rect = {vector.x1, vector.y1 - vector_thickness / 2, vector_length, vector_thickness};
	SDL_Point center = {0, vector_thickness / 2};
	SDL_RenderCopyEx(renderer, texture, NULL, &rect, vector_angle * 180 / M_PI, &center, SDL_FLIP_NONE);
	SDL_DestroyTexture(texture);
}

Point_f is_intersecting(Vector vector1, Vector vector2) {
	const f32 denominator = ((vector1.x1 - vector1.x2) * (vector2.y1 - vector2.y2)) - ((vector1.y1 - vector1.y2) * (vector2.x1 - vector2.x2));

	if (fabsf(denominator) < 0.000001f)
		return (Point_f){NAN, NAN};

	const f32 numerator1 = (((vector1.x1 - vector2.x1) * (vector2.y1 - vector2.y2)) - ((vector1.y1 - vector2.y1) * (vector2.x1 - vector2.x2))) / denominator;
	const f32 numerator2 = (((vector1.x1 - vector2.x1) * (vector1.y1 - vector1.y2)) - ((vector1.y1 - vector2.y1) * (vector1.x1 - vector1.x2))) / denominator;

	return (numerator1 >= 0 && numerator1 <= 1 && numerator2 >= 0 && numerator2 <= 1) ? ((Point_f){vector1.x1 + (numerator1 * (vector1.x2 - vector1.x1)), vector1.y1 + (numerator1 * (vector1.y2 - vector1.y1))}) : ((Point_f){NAN, NAN});
}

int main(int argc, char *argv[]) {
	SDL_Window *window = init("raycast", width, height);
	if (window == NULL)
		return -1;

	SDL_Renderer *renderer = SDL_GetRenderer(window);

	SDL_Point mouse_position;

	Vector boundries[3] = {
		{300, 100, 300, 300},
		{50, 100, 100, 400},
		{50, 300, 300, 300},
	};

	SDL_Color red = {255, 0, 0, 255};
	SDL_Color white = {255, 255, 255, 255};
	SDL_Color gray = {201, 200, 200, 255};

	SDL_Event event;

	u16 ray_num = 360;
	u16 ray_num_initial = 0;

	bool quit = false;
	while (!quit) {
		while (SDL_PollEvent(&event) != 0) {
			if (event.type == SDL_QUIT) {
				quit = true;
			} else if (event.type == SDL_KEYDOWN) {
				switch (event.key.keysym.sym) {
				case SDLK_DOWN:
					ray_num -= 10;
					break;
				case SDLK_UP:
					ray_num += 10;
					break;
				case SDLK_LEFT:
					ray_num_initial -= 10;
					break;
				case SDLK_RIGHT:
					ray_num_initial += 10;
					break;
				}
			}
		}

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		SDL_GetMouseState(&mouse_position.x, &mouse_position.y);

		for (int i = ray_num_initial; i < ray_num; i++) {
			Point_f *closest = NULL;
			Vector ray = new_ray(mouse_position, i);

			f32 record_distance = 1000000000.0;
			for (int j = 0; j < sizeof(boundries) / sizeof(boundries[0]); j++) {
				draw_vector(renderer, boundries[j], red);
				Point_f intersection_point = is_intersecting(ray, boundries[j]);

				if (!isnan(intersection_point.x)) {
					f32 distance = distance_between(mouse_position, intersection_point);
					if (distance < record_distance) {
						record_distance = distance;
						Point_f closest_point = {intersection_point.x, intersection_point.y};
						closest = &closest_point;
					}
				}
			}

			if (closest != NULL) {
				ray.x2 = closest->x;
				ray.y2 = closest->y;

				draw_vector(renderer, ray, white);
			} else {
				draw_vector(renderer, ray, gray);
			}
		}
		SDL_RenderPresent(renderer);
	}
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}

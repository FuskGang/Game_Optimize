#ifndef STRUCTS
#define STRUCTS

#include <SDL2/SDL.h>
#include "constants.h"

typedef struct
{
	void (*update)(void);
	void (*draw)(void);
} Delegate;

typedef struct
{
	char weapon_name[100];
	int damage;
	int max_radius;
	int max_bullet_count;
	int current_bullet_count;
	int hit_bullet_count;
} Weapon;

typedef struct
{
	int start_x, end_x;
	int start_y, end_y;
	int thickness, length;
	int degrees;
	double angle;
} Muzzle;

typedef struct
{
	SDL_Texture *texture;
	SDL_Rect position;
	Uint32 shoot_time;
	Uint32 radius_time;
	char is_shoot;
	char is_hit;
	double angle;
	int curr_radius;
	char is_radius_up;
} Bullet;

typedef struct
{
	char player_name[255];
	int points;
	SDL_Texture *texture;
	SDL_Color color;
	SDL_Rect size;
	SDL_Point bounding_box[4];
	Muzzle muzzle;
	Bullet bullets[MAX_BULLET];
	Weapon curr_weapon;
	Weapon weapons[3];
	int power;
	char damage_target;
	Uint32 touch_time;
	Uint32 input_time;
	Uint32 last_shoot_time;
	SDL_Point first_base_pixel;
	SDL_Point second_base_pixel;
	int degrees;
	char is_shoot;
	SDL_bool is_bot;
} Tank;

typedef struct Widget
{
	char name[MAX_NAME_LENGTH];
	int x;
	int y;
	char label[MAX_NAME_LENGTH];
	struct Widget *prev;
	struct Widget *next;
	void (*action)(void);
} Widget;

typedef struct
{
	Delegate delegate;
	SDL_Renderer *renderer;
	SDL_Window *window;
	SDL_Rect mouse;
	int keyboard[MAX_KEYBOARD_KEYS];
	Widget *active_widget;
	int fps;
} App;

#endif /* STRUCTS */

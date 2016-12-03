#include <pebble.h>
#include <math.h>
#include "helper.h"
#include "planet.h"

static Layer *s_layer;
static GRect s_bounds;

static short s_ready = 0;

static char *s_planet;
static uint s_line_size;
static int s_size, s_multiplier;
static double s_plan_multiplier;

static GColor s_water, s_land, s_mountain;
static GColor s_starCol;

static int s_start_x, s_start_y;

static void planet_proc(Layer *layer, GContext *ctx){
	graphics_context_set_fill_color(ctx, s_starCol);
	for(int x = 0; x < s_size ; x++){ // figure some math
		graphics_fill_rect(ctx, GRect(rand() % s_bounds.size.w, rand() % s_bounds.size.h, s_multiplier, s_multiplier), 0, GCornerNone);
	}
	
	if(s_ready == 1){
		time_t tmp = time(NULL);
		struct tm *tick_time = localtime(&tmp);

		int t = tick_time->tm_min;
		
		for(int y = 0; y < s_size; y++){
			double yPos = y * s_multiplier + s_start_y;
			char line[s_size];
			memcpy(line, s_planet + y * s_size + t, s_size);
			
			int max = s_size;
			for(int x = 0; x < max; x++){
				double xPos = x * s_multiplier + s_start_x;
				if( 
					dist(
						xPos, 
						s_bounds.size.w / 2 - s_multiplier * 0.5, 
						yPos, 
						s_bounds.size.h / 2 - s_multiplier / 2
					) < s_size * s_multiplier * s_plan_multiplier
				){
					int type = line[x];
					if(type == 48)
						graphics_context_set_fill_color(ctx, s_water);
					else if(type == 49)
						graphics_context_set_fill_color(ctx, s_land);
					else if(type == 50)
						graphics_context_set_fill_color(ctx, s_mountain);

					graphics_fill_rect(ctx, GRect(xPos, yPos, s_size, s_size), 0, GCornerNone);
				}
			}
		}
	}
}

static void init_ui(Layer *w_layer, GRect bounds){
	s_bounds = bounds;
	
	s_starCol = GColorWhite;
	
	s_layer = layer_create(bounds);
	layer_add_child(w_layer, s_layer);
	layer_set_update_proc(s_layer, planet_proc);
}

static void destroy_ui(){
	layer_destroy(s_layer);
}

void planet_show(Layer *w_layer, GRect bounds, int size){
	s_size = size;
	s_multiplier = floor(s_size / 2);
	
	s_start_x = (bounds.size.w / 2 - (s_size * s_multiplier) / 2) - s_multiplier / 2;
	s_start_y = (bounds.size.h / 2 - (s_size * s_multiplier) / 2);
	
	init_ui(w_layer, bounds);
}

void planet_hide(){
	destroy_ui();
}

void planet_draw(){
	layer_mark_dirty(s_layer);
}

void planet_update_colours(GColor water, GColor land, GColor mountain){
	s_water = water;
	s_land = land;
	s_mountain = mountain;
}

void planet_update(uint8_t *planet, uint size){
	s_planet = (char *)planet;
	s_line_size = size;
	
	s_plan_multiplier = 0.5; // Fix
	
	s_ready = 1;
}

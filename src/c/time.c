#include <pebble.h>
#include "time.h"
#include "helper.h"

static TextLayer *s_text_layer;

static void init_ui(Layer *w_layer, GRect size){
	GFont font = fonts_get_system_font(FONT_KEY_LECO_32_BOLD_NUMBERS);
	
	s_text_layer = text_layer_create(size);
	text_layer_set_background_color(s_text_layer, GColorClear);
	text_layer_set_text(s_text_layer, "0000");
	text_layer_set_font(s_text_layer, font);
	text_layer_set_text_alignment(s_text_layer, GTextAlignmentCenter);
	
	layer_add_child(w_layer, (Layer *)s_text_layer);
}

static void destroy_ui(){
	text_layer_destroy(s_text_layer);
}

void time_show(Layer *w_layer, GRect size){
	init_ui(w_layer, size);
}

void time_update_colour(GColor colour){
	text_layer_set_text_color(s_text_layer, colour);
}

void time_update_text(char s_time[]){
	text_layer_set_text(s_text_layer, s_time);
}

void time_hide(){
	destroy_ui();
}
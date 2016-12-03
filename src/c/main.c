#include <pebble.h>
#include <pebble-events/pebble-events.h>
#include <kiezelpay-core/kiezelpay.h>
#include <limits.h>
#include <math.h>
#include "planet.h"
#include "time.h"
#include "helper.h"

static Window *s_window;
static int s_size;

static double s_size_multiplier = 0.087;

typedef const enum {
	READY = 1,
	COLOUR = 2,
	CHUNK = 3
} statuses;

static void send_colour_request(int size, int multiplier){
	DictionaryIterator *iter;

	AppMessageResult res = app_message_outbox_begin(&iter);

	if(res == APP_MSG_OK){
		int status = 1;
		dict_write_int(iter, MESSAGE_KEY_status, &status, sizeof(int), true);
		dict_write_int(iter, MESSAGE_KEY_size, &size, sizeof(int), true);
		dict_write_int(iter, MESSAGE_KEY_multiplier, &multiplier, sizeof(int), true);
		
		res = app_message_outbox_send();
		
		if(res != APP_MSG_OK){
			APP_LOG(APP_LOG_LEVEL_ERROR, "Error sending the outbox: %d", (int)res);
		}
	}else{
		APP_LOG(APP_LOG_LEVEL_ERROR, "Error sending the outbox: %d", (int)res);
	}
}

static void send_planet_request(){
	DictionaryIterator *iter;

	AppMessageResult res = app_message_outbox_begin(&iter);

	if(res == APP_MSG_OK){
		int status = 2;
		dict_write_int(iter, MESSAGE_KEY_status, &status, sizeof(int), true);
		
		res = app_message_outbox_send();
		
		if(res != APP_MSG_OK){
			APP_LOG(APP_LOG_LEVEL_ERROR, "Error sending the outbox: %d", (int)res);
		}
	}else{
		APP_LOG(APP_LOG_LEVEL_ERROR, "Error sending the outbox: %d", (int)res);
	}
}

static uint8_t *s_planet_chunks;
static int s_planet_chunks_idx;
static int s_planet_chunk_size = 0;
static void receive_planet_info(DictionaryIterator *received, void *context){
	Tuple *status_tu;
	
	status_tu = dict_find(received, MESSAGE_KEY_status);
	
	if(status_tu){
		int status = status_tu->value->int32;
		
		APP_LOG(APP_LOG_LEVEL_INFO, "Reveived Status: %d", status);
		if(status == READY){
			send_colour_request(s_size, floor(s_size / 2));
		}else if(status == COLOUR){
			Tuple *water_t = dict_find(received, MESSAGE_KEY_water);
			Tuple *land_t = dict_find(received, MESSAGE_KEY_land);
			Tuple *mountain_t = dict_find(received, MESSAGE_KEY_mountain);
			Tuple *text_t = dict_find(received, MESSAGE_KEY_text);
			
			if(water_t && land_t && mountain_t && text_t){
				
				GColor water, land, mountain, text;
				
				water.argb = water_t->value->uint8;
				land.argb = land_t->value->uint8;
				mountain.argb = mountain_t->value->uint8;
				text.argb = text_t->value->uint8;
				
				planet_update_colours(water, land, mountain);
				time_update_colour(text);
				
				send_planet_request();
			}
		}else if(status == CHUNK){
			Tuple *malloc_t = dict_find(received, MESSAGE_KEY_malloc);
			Tuple *chunk_t = dict_find(received, MESSAGE_KEY_chunk);
			
			if(malloc_t && s_planet_chunk_size == 0){
				s_planet_chunks_idx = 0;
				s_planet_chunk_size = chunk_t->length;
				s_planet_chunks = (uint8_t *)malloc(chunk_t->length * s_size);
				
				APP_LOG(APP_LOG_LEVEL_INFO, "Sizes: %d %d", s_size, chunk_t->length);
			}
			
			if(chunk_t){
				memcpy(&s_planet_chunks[s_planet_chunks_idx * s_planet_chunk_size], chunk_t->value->cstring, chunk_t->length);
				
				s_planet_chunks_idx++;
			}
			
			APP_LOG(APP_LOG_LEVEL_DEBUG, "Num completed: %i", s_planet_chunks_idx);
			if(s_planet_chunks_idx == s_size){
				APP_LOG(APP_LOG_LEVEL_DEBUG, "COMPELETED TRANSMISSION");
				planet_update(s_planet_chunks, s_planet_chunk_size);
				planet_draw();
			}
		}
	}
}

void update_time(){
	time_t tmp = time(NULL);
	struct tm *tick_time = localtime(&tmp);
	
	static char buffer[] = "0000";
  	strftime(buffer, sizeof(buffer), clock_is_24h_style() ? "%H%M" : "%I%M", tick_time);
	
	planet_draw();
	time_update_text(buffer);
}

void tick_handler(struct tm *tick, TimeUnits unitsChanged){
	update_time();
}

static void main_window_load(Window *window){
	window_set_background_color(window, GColorFromHEX(0x000000));
	
	Layer *window_layer = window_get_root_layer(window);
	GRect bounds = layer_get_bounds(window_layer);
	
	s_size = floor(min(bounds.size.w, bounds.size.h) * s_size_multiplier);
	
	GRect pos = GRect(0, bounds.size.h / 2 - 32 / 2, bounds.size.w, 32);
	
	planet_show(window_layer, bounds, s_size);
	time_show(window_layer, pos);
}

static void main_window_unload(Window *window){
	planet_hide();
	time_hide();
}

static void init(void){
	kiezelpay_init();
	
	events_app_message_register_inbox_received(receive_planet_info, NULL);

	const u_long inbox_size = ULONG_MAX;
	const int outbox_size = INT_MAX;
	events_app_message_request_inbox_size(inbox_size);
	events_app_message_request_outbox_size(outbox_size);
	
	events_app_message_open();
	
	s_window = window_create();
	
	window_set_window_handlers(s_window, (WindowHandlers){
		.load = main_window_load,
		.unload = main_window_unload,
	});
	
	window_stack_push(s_window, true);
	
	update_time();
	tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
}

static void deinit(void){
	kiezelpay_deinit();
	
	app_message_deregister_callbacks();
	
	window_destroy(s_window);
}

int main(void){
	init();
	app_event_loop();
	deinit();
}
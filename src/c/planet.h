#include <pebble.h>

void planet_show(Layer *w_layer, GRect bounds, int size);
void planet_hide(void);

void planet_draw(void);

void planet_update_colours(GColor water, GColor land, GColor mountain);
void planet_update(uint8_t *planet, uint size);
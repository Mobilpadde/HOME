#include <pebble.h>
#include <math.h>
#include "helper.h"

// https://forums.pebble.com/t/sqrt-function/3919/2
float my_sqrt(float num){
	float a, p, e = 0.001, b;

	a = num;
	p = a * a;
	while(p - num >= e){
		b = (a + (num / a)) / 2;
		a = b;
		p = a * a;
	}
	
	return a;
}

double dist(int x1, int x2, int y1, int y2){
	return (double)my_sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}

int min(int x, int y){
	if(x < y)
		return x;
	else 
		return y;
}

// https://forums.pebble.com/t/solved-substring-like-function-in-c/15305
char *sub_str(const char *input, int offset, int len, char *dest){
	int input_len = strlen(input);
		
	if(offset + len > input_len)
		return NULL;

	strncpy(dest, input + offset, len);
	return dest;
}

// http://stackoverflow.com/a/27448879/754471
char *convert(uint8_t *a){
  char* buffer2;
  int i;

  buffer2 = malloc(9);
  if (!buffer2)
    return NULL;

  buffer2[8] = 0;
  for (i = 0; i <= 7; i++)
    buffer2[7 - i] = (((*a) >> i) & (0x01)) + '0';

  //puts(buffer2);

  return buffer2;
}

// https://www.reddit.com/r/pebbledevelopers/comments/35o9bi/pebble_time_random_color/cr83lnv/
GColor random_color(){
  return (GColor8) { .argb = ((rand() % 0b00111111) + 0b11000000)  };
}
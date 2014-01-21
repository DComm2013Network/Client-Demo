#ifndef PACKET_PROCESSING_H
#define PACKET_PROCESSING_H

#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/net.h>
#include <string.h>

typedef struct Position{
	int player;
	int x;
	int y;
}Position;

void read_UDP(Position *pXY);
void send_UDP(Position *pXY);

#endif

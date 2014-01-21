#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_net.h>
#include <string.h>

typedef struct Position{
	int player;
	int x;
	int y;
}Position;

Position read_UDP();
void send_UDP(Position *pXY);

int main(void){
	Position pXY = {1, 1, 1};
	send_UDP(&pXY);
	pXY = {1, 2, 2};
	pXY = read_UDP();

	printf("Player: %d, X: %d, Y: %d", pXY.player, pXY.x, pXY.y);
}

Position read_UDP(){
	UDPsocket sd;       /* Socket descriptor */
	UDPpacket *p;       /* Pointer to packet memory */
	int quit = 0;
	Position pXY;

	if (SDLNet_Init() < 0) 
	{
		fprintf(stderr, "SDLNet_Init: %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}
 
	/* Open a socket */
	if (!(sd = SDLNet_UDP_Open(2000))) // opens UDP on port 2000
	{
		fprintf(stderr, "SDLNet_UDP_Open: %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}
 
	/* Make space for the packet */
	if (!(p = SDLNet_AllocPacket(sizeof(Position)))) //allocs a packet size, the position struct
	{
		fprintf(stderr, "SDLNet_AllocPacket: %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}

	while (!quit)
	{
		/* Wait a packet. UDP_Recv returns != 0 if a packet is coming */
		if (SDLNet_UDP_Recv(sd, p)) 
			pXY = (Position)p->data;	// casting issues damnit		
	}
 	
	/* Clean and exit */
	SDLNet_FreePacket(p);
	SDLNet_Quit();
	
	return pXY;
}

void send_UDP(Position *pXY){

	UDPsocket sd;
	IPaddress srvadd;
	UDPpacket *p;
	int quit = 0;
 
	/* Initialize SDL_net */
	if (SDLNet_Init() < 0)
	{
		fprintf(stderr, "SDLNet_Init: %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}
 
	/* Open a socket on random port */
	if (!(sd = SDLNet_UDP_Open(2000))) //opens port 2000
	{
		fprintf(stderr, "SDLNet_UDP_Open: %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}
 
	/* Resolve server name  */
	if (SDLNet_ResolveHost(&srvadd, "127.0.0.1", 2000) == -1) // trying to send it to myself for testing purposes
	{
		fprintf(stderr, "SDLNet_ResolveHost");
		exit(EXIT_FAILURE);
	}
 
	/* Allocate memory for the packet */
	if (!(p = SDLNet_AllocPacket(sizeof(Position))))
	{
		fprintf(stderr, "SDLNet_AllocPacket: %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}
 
	while (!quit)
	{
 
		p->address.host = srvadd.host;	/* Set the destination host */
		p->address.port = srvadd.port;	/* And destination port */
		p->len = sizeof(Position);
		p->data = pXY; //casting issues.. still figuring it out

		SDLNet_UDP_Send(sd, -1, p); /* This sets the p->channel */
	}
 
	SDLNet_FreePacket(p);
	SDLNet_Quit();
 
	return EXIT_SUCCESS;
}

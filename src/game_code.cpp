
#define MOSAIC 1
#define MY_GAME 1
#define NETWORK_SIMPLE 0
#define MESSAGING 0
#define PONG_BAD 0
#define PARTICLES 0
#define TYPING 0
#define COLLISION 0
#define SPRITE 0
#define ENTITY 0

#define RENDERING 0

#define PARSING 0

#define RAYTRACER 1


#if MOSAIC
#include "mosaic.cpp"

#elif MY_GAME
#include "examples/my_game.cpp"

#elif NETWORK_SIMPLE
#include "examples/network_simple.cpp"

#elif MESSAGING
#include "examples/messaging_example.cpp"

#elif PONG_BAD
#include "examples/pong_bad.cpp"

#elif PARTICLES
#include "examples/pretty.cpp"

#elif TYPING
#include "examples/typing_game.cpp"

#elif COLLISION
#include "examples/collision_example.cpp"

#elif SPRITE
#include "examples/sprite_basics.cpp"

#elif ENTITY
#include "examples/entity_example.cpp"

#elif RENDERING
#include "examples/rendering_example.cpp"

#elif PARSING
#include "examples/parsing_example.cpp"

#elif RAYTRACER
#include "examples/raytracing.cpp"


#endif


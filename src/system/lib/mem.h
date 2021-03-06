/*
 * Created: November 2015
 * Author : Mikael Å, Anton R,
 * 
 *
 * Contains the memory structure used by bjarne.c and auto.c
 */ 
#ifndef MEM_H
#define MEM_H
#define F_CPU 20000000

#define EMPTY			0 //The a_stack was empty //Stand still
#define FORWARD 		1 //Go forward 1 tile
#define	SPIN_R			2 //Turn 90 right
#define SPIN_L			3 //Turn 90 left
#define	SPIN_180 		4 //Turn 180 left
#define PARALLELIZE		5 //Turn until parallel with wall on right side
#define	BACKWARD		6 //Back-up one tile
#define NUDGE_FORWARD	7
#define P_WEAK			8 //Weak parallelization
#define P_WEAK_L		9
#define NUDGE_TO_WALL	0xA
#define	LAST_NUDGE		0xB

#define NORTH	0
#define	EAST	1
#define	SOUTH	2	
#define	WEST	3

#define FINDHOME 	1
#define FINDUNEXP 	0


#include <avr/io.h>

 // Map-element structures
typedef struct {
	uint8_t x;
	uint8_t y;
	uint8_t t;
} mapchange;

typedef struct node node;

struct node{
		uint8_t x;
		uint8_t y;
		node* parent;
		//uint8_t visited;
		uint8_t tileType;
};

typedef struct queue queue;

struct queue{
		mapchange inbox[100];
		mapchange outbox[100];

		int8_t sizeofIn;
		int8_t sizeofOut;
};

//Queue functions
queue changeQ;				//Change Queue for sending map data to pc.
int8_t enqueue(uint8_t x, uint8_t y, uint8_t t);
mapchange dequeue();

// Memory functions
void wmem(uint8_t data, uint8_t x, uint8_t y); 
void wmem_auto(uint8_t data, uint8_t x, uint8_t y);
uint8_t rmem(uint8_t x, uint8_t y);					
void init_mem();

// Stack functions
uint8_t pstack(uint8_t x, uint8_t y, uint8_t t);
mapchange gstack(); 

//----------------Constants------------------------------------
uint8_t UNEXP;	//Tile: Unexplored
uint8_t FLOOR;	//Tile: Floor
uint8_t WALL;	//Tile: Wall
uint8_t OUTSIDE;	//Tile: Outside
uint8_t IWALL;
uint8_t STACK_MAX;  //Size of stack

// Stack constants
int8_t c_stack_top ; 		//The top of the stack
uint8_t next_action;

uint16_t distance_covered;
uint8_t land_o_hoy;

uint8_t map_complete;
uint8_t debug;

uint8_t map_enclosed;
uint8_t home_x;
uint8_t home_y;

uint8_t lets_go_home;
uint8_t button_autonom; // 0 when in manual, 1 when in auto

uint8_t island_x;
uint8_t island_y;
uint8_t follow_island;

uint8_t robot_pos_x; // Starts in the middle of the map
uint8_t robot_pos_y;

uint8_t target_x; //Target tile
uint8_t target_y;

uint8_t motor_r;
uint8_t motor_l;

uint8_t dir;
uint8_t curr_action;

uint8_t first_time_on_island;

uint8_t s_ir_h_f;
uint8_t s_ir_h_b;
uint8_t s_ir_v_f;
uint8_t s_ir_v_b;
uint8_t s_ir_front;

//	Token parallel left/right

int8_t t_p_h;	// front-back
int8_t t_p_v;

uint8_t t_vagg_h_f; //10 - wall close, 01 - wall far away, 00 - no wall
uint8_t t_vagg_h_b; 
uint8_t t_vagg_v_f; 
uint8_t t_vagg_v_b; 
uint8_t t_vagg_front;

uint8_t s_gyro_u;
uint8_t s_gyro_l;
uint16_t s_gyro;
uint8_t t_gyro;	

uint8_t t_reflex;
uint8_t s_reflex;


#endif
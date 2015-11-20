#include "brain.h"
#include "mem.h"
#include <avr/io.h>

void find_next_wall();
void find_empty_tile();

void bfs(uint8_t startx, uint8_t starty, uint8_t targetx, uint8_t targety);
void bfs_t(uint8_t startx, uint8_t starty, uint8_t target_tile);
void dfs(uint8_t startx, uint8_t starty, target_tile);

uint8_t follow_wall = 1;
uint8_t startup = 1;
//F�r DFS
uint8_t check = 0;
uint8_t visted[32][32]; //�r detta ok utan initialisering?


void think(){


	if(read_a_top() == 0){

		if(follow_wall){ //Om vi ska f�lja h�gerv�ggen

			find_next_wall();

		}
		else{ //Om vi ska kartl�gga mitten

			find_empty_tile();

		}

	}

	//Om actionstacken �r tom, r�kna ut n�sta target
	//Fyll p� actionstacken

	return;
}

void find_next_wall(){

	if(startup){

		paction(PARALLELIZE);
		startup = 0;
	}
	else{
		


	}


//Kod f�r att hitta n�sta target, en tile vi kan �ka p� 
	//j�mte n�sta troliga v�gg (s� vi kan se om den finns d�r)
	return;
}

void find_empty_tile(){


//Hitta en unexplored tile vi kan �ka til
	bfs_t(startx, starty, target_tile);

	return;
}

uint8_t bfs(uint8_t startx, uint8_t starty, uint8_t targetx, uint8_t targety){
//Anv�nds n�r vi vill hitta n�rmsta v�gen till k�nd position
	uint8_t targetFound = 0;
	
	uint8_t layer = 0;
	//each index of layerArray contains an array of all the squares in that layer
	uint8_t layerArray[34][4][2];
	uin
	layerArray[layer] = {startx, starty};
	while(!targetFound){
		
	}

	return;
}

void bfs_t(uint8_t startx, uint8_t starty, uint8_t target_tile){
//Anv�nds n�r vi vill hitta n�rmsta v�gen till en speciell tile-typ

	return;
}

uint8_t dfs(uint8_t startx, uint8_t starty, uint8_t target_tile){
//Anv�nds f�r att kolla om v�ggen runt �r sluten.	
	//Return 1 om s�kning lyckades, 0 om ingen v�g finns
	
	/*	uint8_t UNEXP 		= 1;	//Tile: Unexplored
		uint8_t FLOOR 		= 2;	//Tile: Floor
		uint8_t WALL 		= 3;	//Tile: Wall
		uint8_t OUTSIDE 	= 4;	//Tile: Outside*/

	if (visted[startx][starty] != 1){

		if (rmem(startx, starty) == target_tile){
			return 1;
		}
		else if(rmem(startx, starty) == WALL){
			return 0;
		}
		else{
			check = dfs(startx-1, starty, target_title);
			if (check == 1){return 1;}
			check = dfs(startx, starty+1, target_title);
			if (check == 1){return 1;}
			check = dfs(startx+1, starty, target_title);
			if (check == 1){return 1;}
			check = dfs(startx, starty-1, target_title);
			if (check == 1){return 1;}
			return 0;
		}
		visted[startx][starty] = 1;
	}
	return 0;
}

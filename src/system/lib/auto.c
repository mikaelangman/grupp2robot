#include "auto.h"
#include "usart.h"
#include "mem.h"
#include <avr/io.h>

#define MAX_SPEED_R 10 //1 to 10, 10 is highest
#define MAX_SPEED_L 10
#define GYRO_NO_TURNING 0xB5 //KOMMER NOG �NDRAS
#define FOLlOW_WALL 0
#define MAP_REST 1

uint8_t cur_action = 0;
int distance_LIDAR;
uint8_t first_time;
int8_t deviation_from_dir = 0;


void update_sensor_data(); 
void init_auto();
void action_done();


void init_auto(){
	state = FOLlOW_WALL ;
	distance_LIDAR = 0;
	first_time = 1;
	s_LIDAR_u = 0;
	s_LIDAR_l = 0;
	s_LIDAR = 0;
	t_LIDAR = 0;	//Antal 40 cm rutor till v�gg 0 - 20 cm   1 - 20+40cm   2 20+80cm

	s_ir_h_f = 0;
	s_ir_h_b = 0;
	s_ir_v_f = 0;
	s_ir_v_b = 0;

	//	Token parallell v�nster/h�ger

	t_p_h = 0;	// 0- parallell, 1- lite off , 2- mer off , FF - ej anv�ndbart
	t_p_v = 0;	// 0- parallell, 1- lite off , 2- mer off , FF - aj anv�ndbart

	t_vagg_h_f = 0; // 0- ingen v�gg , 1 - v�gg inom 20 cm ish, 2 - v�gg 20+40 cm
	t_vagg_h_b = 0; // 0- ingen v�gg , 1 - v�gg inom 20 cm ish, 2 - v�gg 20+40 cm
	t_vagg_v_f = 0; // 0- ingen v�gg , 1 - v�gg inom 20 cm ish, 2 - v�gg 20+40 cm
	t_vagg_v_b = 0; // 0- ingen v�gg , 1 - v�gg inom 20 cm ish, 2 - v�gg 20+40 cm

	s_gyro_u = 0;
	s_gyro_l = 0;
	s_gyro = 0;
	t_gyro = 0;		// best�m vilka v�rden vi vill ha

	
uint8_t current_state = 0; //0 - start, 1 - st� still, 2 - k�ra, 3 - snurra
	//----------------------------
}
void update_sensor_data(){
	//fr�n 08 ---> 1 7 rader
	transmitByte_down(0x1D); //fr�ga efter all data

	s_LIDAR_u = receiveByte_down();
	s_LIDAR_l = receiveByte_down();
	s_ir_h_f = receiveByte_down();
	s_ir_h_b = receiveByte_down();
	s_ir_v_f = receiveByte_down();
	s_ir_v_b = receiveByte_down();
	s_gyro_u = receiveByte_down();
	s_gyro_l = receiveByte_down();

	t_LIDAR = receiveByte_down();
	t_p_h = receiveByte_down();;
	t_p_v = receiveByte_down();
	t_gyro = receiveByte_down();
	t_vagg_h_f = receiveByte_down();
	t_vagg_h_b = receiveByte_down();
	t_vagg_v_f = receiveByte_down();
	t_vagg_v_b = receiveByte_down();

	s_LIDAR = ((s_LIDAR_u << 8) + s_LIDAR_l);
	s_gyro = ((s_gyro_u << 8) + s_gyro_l);
//	deviation_from_dir += (GYRO_NO_TURNING - s_gyro);

	return;
}

void autonom (){

	


	if(cur_action == EMPTY){ //Om vi inte har en order, kolla om det finns n�gon ny

		cur_action = read_a_top();
	}
	
	/* 
	#define EMPTY		0 //The a_stack was empty //Stand still
	#define FORWARD 	1 //Go forward 1 tile
	#define	SPIN_R		2 //Turn 90 right
	#define SPIN_L		3 //Turn 90 left
	#define	SPIN_180 	4 //Turn 180 left
	#define PARALLELIZE	5 //Turn until parallel with wall on right side
	#define	BACKWARD	6 //Back-up one tile

	void setSpeed(uint8_t lspeed, uint8_t rspeed, uint8_t ldir, uint8_t rdir);
	*/

	switch(cur_action){
		case (EMPTY):
			setSpeed(0, 0, 0, 0);
			action_done();
		break;

		case (FORWARD):
			if (first_time){
				distance_LIDAR = s_LIDAR - 40; //LIDAR distance - 40 cm
				first_time = 0;
			}
				if (t_p_h == 0){ //Parallellt
					setSpeed(100, 100, 1, 1);
				}	
				else if (t_p_h == 1){ //p�v�g fr�n v�ggen
					setSpeed(100, 95, 1, 1);
				}	
				else if (t_p_h == 2){ //p�v�g fr�n v�ggen
					setSpeed(100, 80, 1, 1);
				}	
				else if (t_p_h == 3){ //p�v�g in i v�ggen
					setSpeed(95, 100, 1, 1);
				}	
				else if (t_p_h == 4){ //p�v�g in i v�ggen
					setSpeed(80, 100, 1, 1);
				}	
			else{ //vet inte hur vi st�r riktigt
				
				int scale_left;
				int scale_right;

				if(deviation_from_dir < 0){
					scale_right = -deviation_from_dir;
					scale_left = 0;
				}
				else{
					scale_left = deviation_from_dir;
					scale_right = 0;
				}

				

				setSpeed(100-scale_left ,100-scale_right ,1,1);
			}
			
			if (s_LIDAR <= distance_LIDAR) {
				first_time = 1;
				action_done();
			}
		break;
//---------------------------------SV�NGA--------------------------------
		case (SPIN_R):
			//GER GYROT V�RDEN PER SEKUND???
			setSpeed(100, 100, 1, 0); //H�ger hjulpar bak�t

			//N�R KLAR
			dir = dir +=1;
			deviation_from_dir = 0;
			action_done();
		break;

		case (SPIN_L):
			//GER GYROT V�RDEN PER SEKUND???


			if(dir == 0){
				dir = 3;
			}
			else{
				dir -=1;
			}


			deviation_from_dir = 0;
			action_done();
			setSpeed(100, 100, 0, 1); //H�ger hjulpar bak�t
		break;

		case (SPIN_180):
			//GER GYROT V�RDEN PER SEKUND???
			setSpeed(100, 100, 1, 0);


			deviation_from_dir = 0;
			dir +=2;
			action_done();

		break;
//------------------------------------------------------------------------
		case(PARALLELIZE):
			if (t_p_h == 0){ //Parallellt
				setSpeed(0, 0, 1, 0);
				dir = NORTH;
				action_done();
			}	
			else if (t_p_h == 1 || t_p_h == 2){ //p�v�g fr�n v�ggen
				setSpeed(40 * t_p_h, 40 * t_p_h, 1, 0); //Speed till 40 eller 80 beroende p� hur fel vi �r
			}	
			else if (t_p_h == 3 || t_p_h == 4){ //p�v�g in i v�ggen
				setSpeed(40 * (t_p_h - 2), 40 * (t_p_h -2), 0, 1); //Speed till 40 eller 80 beroende p� hur fel vi �r
			}
			else{ //vet inte hur vi st�r riktigt
				setSpeed(100, 100, 1, 0);
			}
		break;

		case(BACKWARD):
			if (first_time){
				distance_LIDAR = s_LIDAR_u*256 + s_LIDAR_l + 40; //LIDAR distance + 40 cm
				first_time = 0;
			}
			//Kolla s� vi �ker typ parallellt

			setSpeed(100, 100, 0, 0);
			if ((s_LIDAR_u*256 + s_LIDAR_l) >= distance_LIDAR) {
				first_time = 1;
				action_done();
			}
		break;
	}

	//L�s actionstack och utf�r;
	// UPPDATERA KARTA
	//n�r action �r utf�rd, poppa stack

	

	return;
}

void action_done(){

	if(dir>3){
		dir -=4;
	}

	uint8_t old_action = cur_action;
	pop_a_stack(); //Ta bort actionen fr�n actionstacken
	cur_action = read_a_top();	//Ta in actionen under
	if (old_action != cur_action){ 
		setSpeed(0, 0, 1, 1); //Stanna om vi inte ska forts�tta i samma riktning
	}
	//------------UPPDATERA KARTDATA ----------------
	//R�knat med 0,0 i �vre h�gra h�rnet
	switch(dir){
		int i;
		case (0): //LIDAR to the NORTH
			wmem_auto(WALL, robot_pos_x, robot_pos_y - t_LIDAR); //LIDAR WALL
			for (i = 1; i < t_LIDAR; i++){ //LIDAR SEES FLOOR
				wmem_auto(FLOOR, robot_pos_x, robot_pos_y - i);
			}

			if (t_vagg_h_f + t_vagg_h_b == 4){ //H�GER IR WALL + 1 FLOOR
				wmem_auto(WALL, robot_pos_x + 2, robot_pos_y); 
				wmem_auto(FLOOR, robot_pos_x + 1, robot_pos_y);
			}

			if (t_vagg_h_f + t_vagg_h_b == 2){ //H�GER IR WALL
				wmem_auto(WALL, robot_pos_x + 1, robot_pos_y); 
			}

			if (t_vagg_v_f + t_vagg_v_b == 4){ //V�NSTER IR WALL + 1 FLOOR
				wmem_auto(WALL, robot_pos_x - 2, robot_pos_y); 
				wmem_auto(FLOOR, robot_pos_x - 1, robot_pos_y);
			}

			if (t_vagg_v_f + t_vagg_v_b == 2){ //V�NSTER IR WALL
				wmem_auto(WALL, robot_pos_x - 1, robot_pos_y); 
			}
			


		break;
		case (1): //LIDAR to the EAST
			wmem_auto(WALL, robot_pos_x + t_LIDAR, robot_pos_y); //LIDAR WALL
			for (i = 1; i < t_LIDAR; i++){ //LIDAR SEES FLOOR
				wmem_auto(FLOOR, robot_pos_x + i, robot_pos_y);
			}

			if (t_vagg_h_f + t_vagg_h_b == 4){ //H�GER IR WALL + 1 FLOOR
				wmem_auto(WALL, robot_pos_x, robot_pos_y+2); 
				wmem_auto(FLOOR, robot_pos_x, robot_pos_y+1);
			}

			if (t_vagg_h_f + t_vagg_h_b == 2){ //H�GER IR WALL
				wmem_auto(WALL, robot_pos_x, robot_pos_y+1); 
			}

			if (t_vagg_v_f + t_vagg_v_b == 4){ //V�NSTER IR WALL + 1 FLOOR
				wmem_auto(WALL, robot_pos_x, robot_pos_y-2); 
				wmem_auto(FLOOR, robot_pos_x, robot_pos_y-1);
			}

			if (t_vagg_v_f + t_vagg_v_b == 2){ //V�NSTER IR WALL
				wmem_auto(WALL, robot_pos_x, robot_pos_y-1); 
			}



		break;
		case (2): //LIDAR to the SOUTH
			wmem_auto(WALL, robot_pos_x, robot_pos_y + t_LIDAR); //LIDAR WALL
			for (i = 1; i < t_LIDAR; i++){ //LIDAR SEES FLOOR
				wmem_auto(FLOOR, robot_pos_x, robot_pos_y + i);
			}

			if (t_vagg_h_f + t_vagg_h_b == 4){ //H�GER IR WALL + 1 FLOOR
				wmem_auto(WALL, robot_pos_x - 2, robot_pos_y); 
				wmem_auto(FLOOR, robot_pos_x - 1, robot_pos_y);
			}

			if (t_vagg_h_f + t_vagg_h_b == 2){ //H�GER IR WALL
				wmem_auto(WALL, robot_pos_x - 1, robot_pos_y); 
			}

			if (t_vagg_v_f + t_vagg_v_b == 4){ //V�NSTER IR WALL + 1 FLOOR
				wmem_auto(WALL, robot_pos_x + 2, robot_pos_y); 
				wmem_auto(FLOOR, robot_pos_x + 1, robot_pos_y);
			}

			if (t_vagg_v_f + t_vagg_v_b == 2){ //V�NSTER IR WALL
				wmem_auto(WALL, robot_pos_x + 1, robot_pos_y); 
			}


		break;
		case (3): //LIDAR to the WEST
			wmem_auto(WALL, robot_pos_x - t_LIDAR, robot_pos_y); //LIDAR WALL
			for (i = 1; i < t_LIDAR; i++){ //LIDAR SEES FLOOR
				wmem_auto(FLOOR, robot_pos_x - i, robot_pos_y);
			}

			if (t_vagg_h_f + t_vagg_h_b == 4){ //H�GER IR WALL + 1 FLOOR
				wmem_auto(WALL, robot_pos_x, robot_pos_y-2); 
				wmem_auto(FLOOR, robot_pos_x, robot_pos_y-1);
			}

			if (t_vagg_h_f + t_vagg_h_b == 2){ //H�GER IR WALL
				wmem_auto(WALL, robot_pos_x, robot_pos_y-1); 
			}

			if (t_vagg_v_f + t_vagg_v_b == 4){ //V�NSTER IR WALL + 1 FLOOR
				wmem_auto(WALL, robot_pos_x, robot_pos_y+2); 
				wmem_auto(FLOOR, robot_pos_x, robot_pos_y+1);
			}

			if (t_vagg_v_f + t_vagg_v_b == 2){ //V�NSTER IR WALL
				wmem_auto(WALL, robot_pos_x, robot_pos_y+1); 
			}

		break;
	}

}
	
void setSpeed(uint8_t lspeed, uint8_t rspeed, uint8_t ldir , uint8_t rdir){
	
		if(ldir){
			PORTA |= (1 << DDA7);
			
		}
		else{

			
			PORTA &= 0x7F;

		}

		if(rdir){

			PORTA |= (1 << DDA6);
		}
		else{

			
			PORTA &= 0xBF;
		}

		uint16_t rdone = rspeed * MAX_SPEED_R;
		uint16_t ldone = lspeed * MAX_SPEED_L;


	if (rdone > 38){

		rdone -= 38;

	}


//	PORTA |= (dir_left << DDA7) | (dir_right << DDA6); //DDA7 �r v�nster, DDA6 �r h�ger 	
	OCR1A = rdone;//set the duty cycle(out of 1023) H�ger	
	OCR3A = ldone;//set the duty cycle(out of 1023) V�nster
	
}
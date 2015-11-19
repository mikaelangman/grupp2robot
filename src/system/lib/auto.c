#include "auto.h"
#include "usart.h"
#include "mem.h"
#include <avr/io.h>

uint8_t cur_action = 0;


void update_sensor_data(); 
void init_auto();
void action_done();


void init_auto(){
	s_LIDAR_u = 0;
	s_LIDAR_l = 0;
	t_LIDAR = 0;	//Antal 40 cm rutor till v�gg 0 - 20 cm   1 - 20+40cm   2 20+80cm

	s_ir_h_f = 0;
	s_ir_h_b = 0;
	s_ir_v_f = 0;
	s_ir_v_b = 0;

	//	Token parallell v�nster/h�ger

	t_p_h = 0;	// 0- ej parallell, 1 - parallell
	t_p_v = 0;	// 0- ej parallell, 1 - parallell

	t_vagg_h_f = 0; // 0- ingen v�gg , 1 - v�gg inom 20 cm ish, 2 - v�gg 20+40 cm
	t_vagg_h_b = 0; // 0- ingen v�gg , 1 - v�gg inom 20 cm ish, 2 - v�gg 20+40 cm
	t_vagg_v_f = 0; // 0- ingen v�gg , 1 - v�gg inom 20 cm ish, 2 - v�gg 20+40 cm
	t_vagg_v_b = 0; // 0- ingen v�gg , 1 - v�gg inom 20 cm ish, 2 - v�gg 20+40 cm

	s_gyro_u = 0;
	s_gyro_l = 0;
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


	return;
}

void autonom (){

	if(cur_action == EMPTY){ //Om vi inte har en order, kolla om det finns n�gon ny

		cur_action = read_a_top();

	}

	//L�s actionstack och utf�r;
	//uppdatera kartan m.h.a. sensortokens
	//n�r action �r utf�rd, poppa stack



	return;
}

void action_done(){

	pop_a_stack(); //Ta bort actionen fr�n actionstacken
	cur_action = read_a_top();	//Ta in actionen under

	/*	uint8_t UNEXP 		= 1;	//Tile: Unexplored
		uint8_t FLOOR 		= 2;	//Tile: Floor
		uint8_t WALL 		= 3;	//Tile: Wall
		uint8_t OUTSIDE 	= 4;	//Tile: Outside

		t_LIDAR = 0; //Antal 40 cm rutor till v�gg 0 - 20 cm   1 - 20+40cm   2 20+80cm
		t_vagg_h_f = 0; // 0- ingen v�gg , 1 - v�gg inom 20 cm ish, 2 - v�gg 20+40 cm
		t_vagg_h_b = 0; // 0- ingen v�gg , 1 - v�gg inom 20 cm ish, 2 - v�gg 20+40 cm
		t_vagg_v_f = 0; // 0- ingen v�gg , 1 - v�gg inom 20 cm ish, 2 - v�gg 20+40 cm
		t_vagg_v_b = 0; // 0- ingen v�gg , 1 - v�gg inom 20 cm ish, 2 - v�gg 20+40 cm


		uint8_t robot_pos_x;	// Start in the middle of the map
		uint8_t robot_pos_y;
	*/

	//R�knat med 0,0 i �vre h�gra h�rnet
	switch(dir){
		case (0): //LIDAR to the NORTH
			wmem_auto(WALL, robot_pos_x, robot_pos_y - t_LIDAR); //LIDAR WALL

			for (int i = 1; i < t_LIDAR; i++){ //LIDAR SEES FLOOR
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

			for (int i = 1; i < t_LIDAR; i++){ //LIDAR SEES FLOOR
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

			for (int i = 1; i < t_LIDAR; i++){ //LIDAR SEES FLOOR
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

			for (int i = 1; i < t_LIDAR; i++){ //LIDAR SEES FLOOR
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
	
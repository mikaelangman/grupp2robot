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
	//Hitta v�ggar och stoppa in i kartminnet m.h.a. wmem_auto(tiletyp, x, y)

}
#include "auto.h"


// VI BORDE TA IN DESSA:

//	sensor/token_sensor_fram/bak/v�nster/h�ger upper/lower

uint8_t s_LIDAR_u = 0;
uint8_t s_LIDAR_l = 0;
uint8_t t_LIDAR = 0;	//Antal 40 cm rutor till v�gg 0 - 20 cm   1 - 20+40cm   2 20+80cm

uint8_t s_ir_h_f = 0;
uint8_t s_ir_h_b = 0;
uint8_t s_ir_v_f = 0;
uint8_t s_ir_v_b = 0;

//	Token parallell v�nster/h�ger

uint8_t t_p_h = 0;	// 0- ej parallell, 1 - parallell
uint8_t t_p_v = 0;	// 0- ej parallell, 1 - parallell

uint8_t t_vagg_h_f = 0; // 0- ingen v�gg , 1 - v�gg inom 20 cm ish, 2 - v�gg 20+40 cm
uint8_t t_vagg_h_b = 0; // 0- ingen v�gg , 1 - v�gg inom 20 cm ish, 2 - v�gg 20+40 cm
uint8_t t_vagg_v_f = 0; // 0- ingen v�gg , 1 - v�gg inom 20 cm ish, 2 - v�gg 20+40 cm
uint8_t t_vagg_v_b = 0; // 0- ingen v�gg , 1 - v�gg inom 20 cm ish, 2 - v�gg 20+40 cm

uint8_t s_gyro_u = 0;
uint8_t s_gyro_l = 0;
uint8_t t_gyro = 0;		// best�m vilka v�rden vi vill ha

uint8_t s_reflex = 0;
uint8_t t_reflex_u = 0;
uint8_t t_reflex_l = 0;
//----------------------------

void autonom (){
	//Start av autonoma l�get
	if (t_p_h == 0){
		//snurra tills parallell
	}
	else{ //parallell och vi kan b�rja
		//k�a kartdata (minst 2 st)
		
	}
	
	while (1){
		if ((t_vagg_h_f == 0 && t_vagg_h_b == 0) || (t_vagg_h_f == 2 && t_vagg_h_b == 2)){ // KAN VI �KA H�GER?
			// SPIN 90DEG
		}
		else if (t_LIDAR >= 1){ // KAN VI �KA FRAM�T? >1 - Minst en ruta fram�t �r k�rbar
			//K�r fram�t
		}
		else if ((t_vagg_v_f == 0 && t_vagg_v_b == 0) || (t_vagg_v_f == 2 && t_vagg_v_b == 2)){ // KAN VI �KA V�NSTER?
			// SPIN 180DEG
		}
		else {
			//SPIN 270DEG
		}
		if (t_LIDAR
		
		//F�r varje 40 cm vi �kt, k�a kartdata f�r v�gg h�ger och v�gg v�nster
		
	}
	

	
}
#include <DS1307.h>
#include <LiquidCrystal.h>
#include "extvar.h"
#include <dht11.h>

#include <EEPROM.h>




//gsm includes etc//
#include "gsm_final.h"
GSM gsm;

//**gsm includes etc//

//real time clock//
#include <Wire.h>
#include <DS1307.h>
//**real time clock//

//rf (refrig) includes etc//
#include <nRF905.h>
#include <SPI.h>
#define PACKET_TYPE_DATA	0
#define PACKET_TYPE_ACK		1
#define MAX_PACKET_SIZE (NRF905_MAX_PAYLOAD - 2)
typedef struct {
	byte dstAddress[NRF905_ADDR_SIZE];
	byte type;
	byte len;
	byte data[MAX_PACKET_SIZE];
} packet_s;
//**rf (refrig) includes etc//

//othonh//
LiquidCrystal lcd(6, 5, 33, 35, 37, 39);
//**othonh//

//keypad//
#include <Keypad_I2C.h>
#include <Keypad.h>  
#include <Wire.h>



#define I2CADDR 0x38
const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
char hexaKeys[ROWS][COLS] = {
	{ '1','4','7','*' },
	{ '2','5','8','0' },
	{ '3','6','9','#' },
	{ 'A','B','C','D' }
};

//**keypad//



byte rowPins[ROWS] = { 3, 2, 1, 0 }; //connect to the row pinouts of the keypad
byte colPins[COLS] = { 7, 6, 5, 4 };

Keypad_I2C customKeypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS, I2CADDR);

//**keypad//


#define send_request_tol_1 ("*t=%s;r_l=%i;r_r=%i;w_t=%i;&\0")

#define send_request_tol_2 ("*t=%s;r_l=%i;r_r=%i;w_t=%i;&\0")

#define send_request_refrig ("*r=%s;&\0")


//tol 2 vars (serial com resive)//
char recive_table_tol_2[200];
bool has_com_tol_2;
int has_com_count_tol_2;

bool relay_l_tol_2;     //!!//
bool relay_r_tol_2;     //!!//
int work_temp_tol_2;     //!!//
int door_stat_tol_2;
char door_last_open_tol_2[20];
int temp_tol_2;
int hum_tol_2;
int hum_soil_tol_2;
bool has_com_error_tol_2;
int has_com_error_count_tol_2;

int res_work_temp_tol_2;

bool res_relay_l_tol_2_s;
bool res_relay_r_tol_2_s;

bool res_relay_l_tol_2;
bool res_relay_r_tol_2;

bool has_setting_set_tol_2;
//**tol 2 vars (serial com resive)//

//relay_l_t1,relay_r_t1,relay_l_t2,relay_r_t2,work_temp_t1,work_temp_t2,pump_mode,pump_on_off,debug_lvl,
//relay_l_tol_1,relay_r_tol_1,


//tol 1 vars (serial com resive)//
char recive_table_tol_1[200];
bool has_com_tol_1;
int has_com_count_tol_1;

bool relay_l_tol_1;     //!!//
bool relay_r_tol_1;     //!!//
int work_temp_tol_1;     //!!//
int door_stat_tol_1;
char door_last_open_tol_1[20];
int temp_tol_1;
int hum_tol_1;
int hum_soil_tol_1;
bool has_com_error_tol_1;
int has_com_error_count_tol_1;

int res_work_temp_tol_1;

bool res_relay_l_tol_1_s;
bool res_relay_r_tol_1_s;

bool res_relay_l_tol_1;
bool res_relay_r_tol_1;

bool has_setting_set_tol_1;



//**tol 1 vars (serial com resive)//

//refrigerator vars (rf com resive)//
char recive_table_refrig[200];
bool has_com_refrig;
int has_com_count_refrig;
int pump_relay_mode;     //!!//
int door_stat_refrig;
char door_last_open_refrig[20];
int temp_refrig;
int hum_refrig;
bool has_com_error_refrig;
int has_com_error_count_refrig;
char send_table_refrig[28];
char pump_time[20];

bool relay_state;

bool res_relay_refrig;

bool has_setting_set_refrig;





//**refrig vars (rf com resive)//






//vars form gsm//
char g_gsm_time[25];
char g_gsm_pump_time[20];
int g_relay_r_tol_1;
int g_relay_l_tol_1;
int g_relay_l_tol_2;
int g_relay_r_tol_2;
int g_work_temp_t1;
int g_work_temp_t2;
int g_pump_c_state;
int g_null_check;
int g_reset_t1;
int g_reset_t2;
int g_reset_error;
int g_time_anl;
int g_debug_lvl;

extern char time_a[25];
extern char c_pump_time[20];
extern int c_relay_l_t1;
extern int c_relay_r_t1;
extern int c_relay_l_t2;
extern int c_relay_r_t2;
extern int c_work_temp_t1;
extern int c_work_temp_t2;
extern int c_pump_on_off;
extern int c_reset_t1;
extern int c_reset_t2;
extern int c_reset_error;
extern int c_time_anl;
extern int c_debug_lvl;

//**vars form gsm//







char send_table[700];



//system vars//
int s_system_temp;
bool s_battary_mode;
int s_error_code;
char time_system[20];
char s_time_date_last_resive[20];
char s_time_date[20];
int errors[20];

int serial_debug_lvl;
int lcd_debug_lvl;


//**system vars//

//vars gia error apo to gsm//
bool gsm_connect_error;
int gsm_connect_error_count;

bool gsm_recive_error;
int gsm_recive_error_count;

bool gsm_send_error;
int gsm_send_error_count;

int usb_debug_lvl_s;


//elenxos gia counter loop
int loop_check_counter;

const char menu_code[] = "3110\0";



#define an_current_sensor A1
#define led_blue_pin 41
#define led_red_pin 43
dht11 temp_sensor;
#define system_temp_pin 49
#define inter_boton 18
#define reset_t1_pin 45
#define reset_t2_pin 48
#define relay_charger 40
#define charger_sensor 22 //0 otan exei fws 1 otan den exei
#define button_pin 2
#define button_ipin INT0


bool int_to_bool(int a)
{
	if (a == 1)
	{
		return true;
	}
	else
	{
		return false;
	}
}


int bool_to_int(bool a)
{
	if (a == true)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

void take_ext_time_var()
{
	for (int i = 0; i < 20; i++)
	{
		g_gsm_time[i] = time_a[i];
	}
	g_gsm_time[19] = '\0';
}

//pernei ths times apo to gps kai ths pernaei sta gsm_vars
void take_ext_vars()
{
	usb_debugln(F("*Taking vars from gsm"), 2);
	for (int i = 0; i < 20; i++)
	{
		g_gsm_time[i] = time_a[i];
	}
	for (int i = 0; i < 20; i++)
	{
		g_gsm_pump_time[i] = c_pump_time[i];
	}
	delay(100);
	g_gsm_time[19] = '\0';
	g_gsm_pump_time[19] = '\0';
	g_relay_r_tol_1 = c_relay_r_t1;
	g_relay_l_tol_1 = c_relay_l_t1;
	g_relay_l_tol_2 = c_relay_l_t2;
	g_relay_r_tol_2 = c_relay_r_t2;
	g_work_temp_t1 = c_work_temp_t1;
	g_work_temp_t2 = c_work_temp_t2;
	g_pump_c_state = c_pump_on_off;
	g_reset_t1 = c_reset_t1;
	g_reset_t2 = c_reset_t2;
	g_reset_error = c_reset_error;
	g_debug_lvl = c_debug_lvl;
	g_time_anl = c_time_anl;
}


//reset ta error se 0 
void reset_error()
{
	has_com_error_tol_2 = false;
	has_com_error_count_tol_2 = 0;
	has_com_tol_2 = false;
	has_com_count_tol_2 = 0;
	has_com_error_tol_1 = false;
	has_com_error_count_tol_1 = 0;
	has_com_tol_1 = false;
	has_com_count_tol_1 = 0;
	has_com_refrig = false;
	has_com_count_refrig = 0;
	has_com_error_refrig = false;
	has_com_error_count_refrig = 0;
	gsm_connect_error = false;
	gsm_connect_error_count = 0;
	gsm_recive_error = false;
	gsm_recive_error_count = 0;
	gsm_send_error = false;
	gsm_send_error_count = 0;
	s_error_code = 0;
}


//reset arduinos//
void reset_t1()
{
	digitalWrite(reset_t1_pin, HIGH);
	delay(1000);
	digitalWrite(reset_t1_pin, LOW);
	delay(1000);
}


void reset_t2()
{
	digitalWrite(reset_t2_pin, HIGH);
	delay(1000);
	digitalWrite(reset_t2_pin, LOW);
	delay(1000);
}

void reset_check()
{
	errors[0] = 0;// pinakas lathos gia apothikeush oti egeine reset
	errors[1] = 0;
	if ((has_com_tol_1 == false) && ((has_com_count_tol_1 > 3) && (has_com_count_tol_1 < 6)))
	{
		usb_debug(F("~~~Reset unit 1 :Error~~~"), 1);
		reset_t1();
		delay(2000);
		send_vars_tol_1();
		errors[0] = 1;
	}
	else if ((has_com_error_tol_1 == true) && ((has_com_error_count_tol_1 > 3) && (has_com_error_count_tol_1 < 6)))
	{
		usb_debug(F("~~~Reset unit 1 :Error~~~"), 1);
		reset_t1();
		delay(2000);
		send_vars_tol_1();
		errors[0] = 1;
	}



	if ((has_com_tol_2 == false) && ((has_com_count_tol_2 > 3) && (has_com_count_tol_2 < 6)))
	{
		usb_debug(F("~~~Reset unit 2 :Error~~~"), 1);
		reset_t2();
		delay(2000);
		send_vars_tol_2();
		errors[1] = 1;
	}
	else if ((has_com_error_tol_2 == true) && ((has_com_error_count_tol_2 > 3) && (has_com_error_count_tol_2 < 6)))
	{
		usb_debug(F("~~~Reset unit 2 :Error~~~"), 1);
		reset_t2();
		delay(2000);
		send_vars_tol_2();
		errors[1] = 1;

	}

}
//reset arduinos//



void print_vars()
{
	usb_debug(F("~~~PRINT ALL SYSTEM VARS~~~"), 2);
	usb_debug((F("has_com_tol_2 : ")), 2);
	usb_debugln(has_com_tol_2, 2);
	usb_debug((F("has_com_count_tol_2 : ")), 2);
	usb_debugln(has_com_count_tol_2, 2);
	usb_debug((F("relay_l_tol_2 : ")), 2);
	usb_debugln(relay_l_tol_2, 2);
	usb_debug((F("relay_r_tol_2 : ")), 2);
	usb_debugln(relay_r_tol_2, 2);
	usb_debug((F("work_temp_tol_2 : ")), 2);
	usb_debugln(work_temp_tol_2, 2);
	usb_debug((F("door_stat_tol_2 : ")), 2);
	usb_debugln(door_stat_tol_2, 2);
	usb_debug((F("door_last_open_tol_2 : ")), 2);
	usb_debugln(door_last_open_tol_2, 2);
	usb_debug((F("temp_tol_2 : ")), 1);
	usb_debugln(temp_tol_2, 2);
	usb_debug((F("hum_tol_2 : ")), 1);
	usb_debugln(hum_tol_2, 2);
	usb_debug((F("hum_soil_tol_2 : ")), 1);
	usb_debugln(hum_soil_tol_2, 2);
	usb_debug((F("has_com_error_tol_2 : ")), 1);
	usb_debugln(has_com_error_tol_2, 2);
	usb_debug((F("has_com_error_count_tol_2 : ")), 1);
	usb_debugln(has_com_error_count_tol_2, 2);
	usb_debug((F("res_work_temp_tol_2 : ")), 1);
	usb_debugln(res_work_temp_tol_2, 2);
	usb_debug((F("res_relay_l_tol_2_s : ")), 1);
	usb_debugln(res_relay_l_tol_2_s, 2);
	usb_debug((F("res_relay_r_tol_2_s : ")), 1);
	usb_debugln(res_relay_r_tol_2_s, 2);
	usb_debug((F("res_relay_l_tol_2 : ")), 1);
	usb_debugln(res_relay_l_tol_2, 2);
	usb_debug((F("res_relay_r_tol_2 : ")), 1);
	usb_debugln(res_relay_r_tol_2, 2);
	usb_debug((F("has_setting_set_tol_2 : ")), 1);
	usb_debugln(has_setting_set_tol_2, 2);
	usb_debug((F("has_com_tol_1 : ")), 1);
	usb_debugln(has_com_tol_1, 2);
	usb_debug((F("has_com_count_tol_1 : ")), 1);
	usb_debugln(has_com_count_tol_1, 2);
	usb_debug((F("relay_l_tol_1 : ")), 1);
	usb_debugln(relay_l_tol_1, 2);
	usb_debug((F("relay_r_tol_1 : ")), 1);
	usb_debugln(relay_r_tol_1, 2);
	usb_debug((F("work_temp_tol_1 : ")), 1);
	usb_debugln(work_temp_tol_1, 2);
	usb_debug((F("door_stat_tol_1 : ")), 1);
	usb_debugln(door_stat_tol_1, 2);
	usb_debug((F("door_last_open_tol_1 : ")), 1);
	usb_debugln(door_last_open_tol_1, 2);
	usb_debug((F("temp_tol_1 : ")), 1);
	usb_debugln(temp_tol_1, 2);
	usb_debug((F("hum_tol_1 : ")), 1);
	usb_debugln(hum_tol_1, 2);
	usb_debug((F("hum_soil_tol_1 : ")), 1);
	usb_debugln(hum_soil_tol_1, 2);
	usb_debug((F("has_com_error_tol_1 : ")), 1);
	usb_debugln(has_com_error_tol_1, 2);
	usb_debug((F("has_com_error_count_tol_1 : ")), 1);
	usb_debugln(has_com_error_count_tol_1, 2);
	usb_debug((F("res_work_temp_tol_1 : ")), 1);
	usb_debugln(res_work_temp_tol_1, 2);
	usb_debug((F("res_relay_l_tol_1_s : ")), 1);
	usb_debugln(res_relay_l_tol_1_s, 2);
	usb_debug((F("res_relay_r_tol_1_s : ")), 1);
	usb_debugln(res_relay_r_tol_1_s, 2);
	usb_debug((F("res_relay_l_tol_1 : ")), 1);
	usb_debugln(res_relay_l_tol_1, 2);
	usb_debug((F("res_relay_r_tol_1 : ")), 1);
	usb_debugln(res_relay_r_tol_1, 2);
	usb_debug((F("has_setting_set_tol_1 : ")), 1);
	usb_debugln(has_setting_set_tol_1, 2);
	usb_debug((F("has_com_refrig : ")), 1);
	usb_debugln(has_com_refrig, 2);
	usb_debug((F("has_com_count_refrig : ")), 1);
	usb_debugln(has_com_count_refrig, 2);
	usb_debug((F("pump_relay_mode : ")), 1);
	usb_debugln(pump_relay_mode, 2);
	usb_debug((F("door_stat_refrig : ")), 1);
	usb_debugln(door_stat_refrig, 2);
	usb_debug((F("door_last_open_refrig : ")), 1);
	usb_debugln(door_last_open_refrig, 2);
	usb_debug((F("temp_refrig : ")), 1);
	usb_debugln(temp_refrig, 2);
	usb_debug((F("hum_refrig : ")), 1);
	usb_debugln(hum_refrig, 2);
	usb_debug((F("has_com_error_refrig : ")), 1);
	usb_debugln(has_com_error_refrig, 2);
	usb_debug((F("has_com_error_count_refrig : ")), 1);
	usb_debugln(has_com_error_count_refrig, 2);
	usb_debug((F("pump_time : ")), 1);
	usb_debugln(pump_time, 2);
	usb_debug((F("res_relay_refrig : ")), 1);
	usb_debugln(res_relay_refrig, 2);
	usb_debug((F("has_setting_set_refrig : ")), 1);
	usb_debugln(has_setting_set_refrig, 2);
	usb_debug((F("g_gsm_time : ")), 1);
	usb_debugln(g_gsm_time, 2);
	usb_debug((F("g_gsm_pump_time : ")), 1);
	usb_debugln(g_gsm_pump_time, 2);
	usb_debug((F("g_relay_r_tol_1 : ")), 1);
	usb_debugln(g_relay_r_tol_1, 2);
	usb_debug((F("g_relay_l_tol_1 : ")), 1);
	usb_debugln(g_relay_l_tol_1, 2);
	usb_debug((F("g_relay_l_tol_2 : ")), 1);
	usb_debugln(g_relay_l_tol_2, 2);
	usb_debug((F("g_relay_r_tol_2 : ")), 1);
	usb_debugln(g_relay_r_tol_2, 2);
	usb_debug((F("g_work_temp_t1 : ")), 1);
	usb_debugln(g_work_temp_t1, 2);
	usb_debug((F("g_work_temp_t2 : ")), 1);
	usb_debugln(g_work_temp_t2, 2);
	usb_debug((F("g_pump_c_state : ")), 1);
	usb_debugln(g_pump_c_state, 2);
	usb_debug((F("g_null_check : ")), 1);
	usb_debugln(g_null_check, 2);
	usb_debug((F("g_reset_t1 : ")), 1);
	usb_debugln(g_reset_t1, 2);
	usb_debug((F("g_reset_t2 : ")), 1);
	usb_debugln(g_reset_t2, 2);
	usb_debug((F("g_reset_error : ")), 1);
	usb_debugln(g_reset_error, 2);
	usb_debug((F("g_time_anl : ")), 1);
	usb_debugln(g_time_anl, 2);
	usb_debug((F("g_debug_lvl : ")), 1);
	usb_debugln(g_debug_lvl, 2);
	usb_debug((F("s_system_temp : ")), 1);
	usb_debugln(s_system_temp, 2);
	usb_debug((F("s_battary_mode : ")), 1);
	usb_debugln(s_battary_mode, 2);
	usb_debug((F("s_error_code : ")), 1);
	usb_debugln(s_error_code, 2);
	usb_debug((F("time_system : ")), 1);
	usb_debugln(time_system, 2);
	usb_debug((F("s_time_date_last_resive : ")), 1);
	usb_debugln(s_time_date_last_resive, 2);
	usb_debug((F("s_time_date : ")), 1);
	usb_debugln(s_time_date, 2);
	usb_debug((F("gsm_connect_error : ")), 1);
	usb_debugln(gsm_connect_error, 2);
	usb_debug((F("gsm_connect_error_count : ")), 1);
	usb_debugln(gsm_connect_error_count, 2);
	usb_debug((F("gsm_recive_error : ")), 1);
	usb_debugln(gsm_recive_error, 2);
	usb_debug((F("gsm_recive_error_count : ")), 1);
	usb_debugln(gsm_recive_error_count, 2);
	usb_debug((F("gsm_send_error : ")), 1);
	usb_debugln(gsm_send_error, 2);
	usb_debug((F("gsm_send_error_count : ")), 1);
	usb_debugln(gsm_send_error_count, 2);
	usb_debug((F("usb_debug_lvl_s : ")), 1);
	usb_debugln(usb_debug_lvl_s, 2);
	usb_debug((F("loop_check_counter : ")), 1);
	usb_debugln(loop_check_counter, 2);
	usb_debug(F("~~~PRINT ALL SYSTEM VARS~~~"), 2);
}


//dinei sto time system thn wra pou exei lavei apo to gps
void give_system_ext_time()
{
	usb_debug(F("*give_system_ext_time"), 2);
	time_system[0] = g_gsm_time[0];
	time_system[1] = g_gsm_time[1];
	time_system[2] = ':';
	time_system[3] = g_gsm_time[3];
	time_system[4] = g_gsm_time[4];
	time_system[5] = ':';
	time_system[6] = g_gsm_time[6];
	time_system[7] = g_gsm_time[7];
	time_system[8] = ' ';
	time_system[9] = g_gsm_time[9];
	time_system[10] = g_gsm_time[10];
	time_system[11] = '/';
	time_system[12] = g_gsm_time[12];
	time_system[13] = g_gsm_time[13];
	time_system[14] = '/';
	time_system[15] = g_gsm_time[15];
	time_system[16] = g_gsm_time[16];
	time_system[17] = g_gsm_time[17];
	time_system[18] = g_gsm_time[18];
	time_system[19] = '\0';
	delay(100);
	write_time(time_system);
	delay(100);
	take_time();
}












//grafei kai diavazei ths vasikes metavlhtes apo thn eeprom
void write_perm_values()
{
	if (perm_check_change() == true)
	{
		usb_debug(F("*write_perm_values"), 1);
		EEPROM.write(9, bool_to_int(relay_l_tol_1));
		EEPROM.write(10, bool_to_int(relay_r_tol_1));
		EEPROM.write(11, bool_to_int(relay_l_tol_2));
		EEPROM.write(12, bool_to_int(relay_r_tol_2));
		EEPROM.write(13, work_temp_tol_1);
		EEPROM.write(14, work_temp_tol_2);
		EEPROM.write(15, pump_relay_mode);
		EEPROM.write(16, usb_debug_lvl_s);
	}
}

bool change_check() //elenxos an exei ala3ei kati gia na grapsei ths metavlites sto prem
{
	if ((bool_to_int(relay_l_tol_1) == g_relay_l_tol_1) && (((bool_to_int(relay_r_tol_1) == g_relay_r_tol_1) && (bool_to_int(relay_l_tol_2) == g_relay_l_tol_2)) && ((bool_to_int(relay_r_tol_2) == g_relay_r_tol_2) && (work_temp_tol_1 == g_work_temp_t1)) && ((work_temp_tol_2 == g_work_temp_t2)) && (pump_relay_mode == g_pump_c_state) && (usb_debug_lvl_s == g_debug_lvl)))
	{
		return false;
	}
	return true;
}

void read_perm_values()
{
	usb_debug(F("*read_perm_values"), 1);
	relay_l_tol_1 = int_to_bool(EEPROM.read(9));
	relay_r_tol_1 = int_to_bool(EEPROM.read(10));
	relay_l_tol_2 = int_to_bool(EEPROM.read(11));
	relay_r_tol_2 = int_to_bool(EEPROM.read(12));
	work_temp_tol_1 = EEPROM.read(13);
	work_temp_tol_2 = EEPROM.read(14);
	pump_relay_mode = EEPROM.read(15);
	usb_debug_lvl_s = EEPROM.read(16);
}


bool perm_check_change()
{
	bool check = false;

	if (g_relay_l_tol_1 != EEPROM.read(9))
		check = true;
	if (g_relay_r_tol_1 != EEPROM.read(10))
		check = true;
	if (g_relay_l_tol_2 != EEPROM.read(11))
		check = true;
	if (g_relay_r_tol_2 != EEPROM.read(12))
		check = true;
	if (g_work_temp_t1 != EEPROM.read(13))
		check = true;
	if (g_work_temp_t2 != EEPROM.read(14))
		check = true;
	if (g_pump_c_state != EEPROM.read(15))
		check = true;
	if (g_debug_lvl != EEPROM.read(16))
		check = true;

	return check;
}







// Real time clock //
void read_time() {
	usb_debugln(F("*Read time"), 2);
	char buffer[1];
	int hh = RTC.get(DS1307_HR, true);
	delay(100);
	int mm = RTC.get(DS1307_MIN, false);
	delay(100);
	int ss = RTC.get(DS1307_SEC, false);

	dtostrf(hh, 1, 0, buffer);
	if (buffer[1] == '\0')
	{
		s_time_date[0] = '0';
		s_time_date[1] = buffer[0];
	}
	else
	{
		s_time_date[0] = buffer[0];
		s_time_date[1] = buffer[1];

	}
	delay(100);

	s_time_date[2] = ':';

	dtostrf(mm, 1, 0, buffer);
	if (buffer[1] == '\0')
	{
		s_time_date[3] = '0';
		s_time_date[4] = buffer[0];
	}
	else
	{
		s_time_date[3] = buffer[0];
		s_time_date[4] = buffer[1];

	}
	delay(100);

	s_time_date[5] = ':';

	dtostrf(ss, 1, 0, buffer);
	if (buffer[1] == '\0')
	{
		s_time_date[6] = '0';
		s_time_date[7] = buffer[0];
	}
	else
	{
		s_time_date[6] = buffer[0];
		s_time_date[7] = buffer[1];

	}
	s_time_date[8] = ' ';

	char buffer_1[4];
	int dd = RTC.get(DS1307_DATE, false);
	delay(100);
	int mo = RTC.get(DS1307_MTH, false);
	delay(100);
	int yy = RTC.get(DS1307_YR, false);


	dtostrf(dd, 1, 0, buffer);
	if (buffer[1] == '\0')
	{
		s_time_date[9] = '0';
		s_time_date[10] = buffer[0];
	}
	else
	{
		s_time_date[9] = buffer[0];
		s_time_date[10] = buffer[1];

	}
	delay(100);

	s_time_date[11] = '/';

	dtostrf(mo, 1, 0, buffer);
	if (buffer[1] == '\0')
	{
		s_time_date[12] = '0';
		s_time_date[13] = buffer[0];
	}
	else
	{
		s_time_date[12] = buffer[0];
		s_time_date[13] = buffer[1];

	}
	s_time_date[14] = '/';

	dtostrf(yy, 3, 0, buffer_1);
	s_time_date[15] = buffer_1[0];
	s_time_date[16] = buffer_1[1];
	s_time_date[17] = buffer_1[2];
	s_time_date[18] = buffer_1[3];
	s_time_date[19] = '\0';
	delay(100);
	usb_debugln(s_time_date, 2);
}

void take_time()
{
	usb_debugln(F("*Take_time"), 2);
	errors[14] = 0;
	read_time();
	if (check_ifex(s_time_date, "01/01/2000"))
	{
		errors[14] = 1;
		usb_debugln(F("S:Clock time error"), 1);
	}
	else
	{
		errors[14] = 0;
	}
	for (int i = 0; i < 20; i++)
	{
		time_system[i] = s_time_date[i];
	}
	time_system[20] = '\0';
	if (errors[14] == 1)
	{
		lcd_print(0, "Lathos roloi");
		delay(10000);
	}
	usb_debugln(time_system, 2);
}

int my_str2int(const char *s)
{
	int res = 0;
	while (*s) {
		res *= 10;
		res += *s++ - '0';
	}

	return res;
}

void write_time(const char* rol)  // tou tipou "20/01/12 14/01/1992"
{
	usb_debugln(F("*Write time"), 2);
	usb_debug(F("*Time to write :"), 2);
	usb_debugln(rol, 2);
	RTC.stop();
	int dd = 00;
	int mo = 00;
	int yy = 0000;
	int hh = 00;
	int mm = 00;
	int ss = 00;

	int n = 0;
	char ar[4];
	char ar_7[5];
	if (rol[6] != ' ')
	{
		ar[0] = rol[0];
		ar[1] = rol[1];
		ar[2] = '\0';
		hh = my_str2int(ar);
		ar[0] = rol[3];
		ar[1] = rol[4];
		ar[2] = '\0';
		mm = my_str2int(ar);
		ar[0] = rol[6];
		ar[1] = rol[7];
		ar[2] = '\0';
		ss = my_str2int(ar);
		ar[0] = rol[9];
		ar[1] = rol[10];
		ar[2] = '\0';
		dd = my_str2int(ar);
		ar[0] = rol[12];
		ar[1] = rol[13];
		ar[2] = '\0';
		mo = my_str2int(ar);
		ar[0] = rol[17];
		ar[1] = rol[18];
		yy = my_str2int(ar);
	}


	RTC.set(DS1307_HR, hh);
	RTC.set(DS1307_MIN, mm);
	RTC.set(DS1307_SEC, ss);
	RTC.set(DS1307_DATE, dd);
	RTC.set(DS1307_MTH, mo);
	RTC.set(DS1307_YR, yy);


	RTC.start();
	delay(1000);
	take_time();
}
// **Real time clock //





//amp sensor//
float amplitude_current;
float effective_value;

float amp_sensor()
{
	int sensor_max;
	sensor_max = getMaxValue();
	amplitude_current = (float)sensor_max / 1024 * 5 / 800 * 2000000;
	effective_value = amplitude_current / 1.414;//minimum_current=1/1024*5/800*2000000/1.414=8.6(mA) 
												//Only for sinusoidal alternating current
	return amplitude_current;
}

int getMaxValue()
{
	int sensorValue;             //value read from the sensor
	int sensorMax = 0;
	uint32_t start_time = millis();
	while ((millis() - start_time) < 1000)//sample for 1000ms
	{
		sensorValue = analogRead(an_current_sensor);
		if (sensorValue > sensorMax)
		{
			/*record the maximum sensor value*/
			sensorMax = sensorValue;
		}
	}
	return sensorMax;
}

void battary_check()
{
	if (amp_sensor() > 14)
	{
		s_battary_mode = false;
	}
	else
	{
		s_battary_mode = true;
	}
}

void System_led()
{
	if (s_battary_mode == false)
	{
		digitalWrite(led_blue_pin, HIGH);
		digitalWrite(led_red_pin, LOW);
	}
	else
	{
		digitalWrite(led_blue_pin, LOW);
		digitalWrite(led_red_pin, HIGH);
	}
}

void charger_init()
{
	battary_check();
	int count = 0;
	if (s_battary_mode == false)
	{
		while ((digitalRead(charger_sensor) == 1) && (count < 6))
		{
			delay(1000);
			digitalWrite(relay_charger, LOW);
			delay(1000);
			digitalWrite(relay_charger, HIGH);

			count++;
			delay(800);
		}
	}

}
//**amp sensor//


//system temp sensor //
int check_temp_sensor() //epistrefei 1 an olla ok me ton estitira , 0 an uparxei problhma 
{
	bool check = false;
	int ret = -1;
	int chk = temp_sensor.read(system_temp_pin);
	switch (chk)
	{
	case 0: check = true; break; //ok
	case -1: check = false; break;
	case -2: check = false; break;
	default: check = false; break;
	}
	if (check == true)
	{
		ret = 1;
	}
	else if (check == false)
	{
		ret = 0;
	}
	return ret;
}

int take_temp() // h epistrefei thermokrasia h -999 gia sfalma 
{
	int ret = -999;
	delay(100);
	if (check_temp_sensor() == 1)
	{
		ret = (int)temp_sensor.temperature, 2;
	}
	return ret;
}

int take_hum()// h epistrefei igrasia h -999 gia sfalma 
{
	int ret = -999;
	delay(100);
	if (check_temp_sensor() == 1)
	{
		ret = (int)temp_sensor.humidity, 2;
	}
	return ret;
}

void take_system_temp()
{
	s_system_temp = take_temp();
}

//**system temp sensor //



//pump checks//
char pump_auto()
{
	usb_debugln(F("Pump Auto"), 2);
	char ret = 'f';
	int hum_work = 500; // igrasia anoigmatos ths antlias 
	float hum_all;
	int state = 0;

	if (hum_soil_tol_1 == -1)// error sto 1o
	{
		state = 1;
	}
	if (hum_soil_tol_2 == -1) //error sto 2o
	{
		state = 2;
	}
	if ((hum_soil_tol_1 == -1) && (hum_soil_tol_2 == -1)) // error kai sta 2
	{
		state = 3;
	}


	if (state != 3)
	{
		if (state == 0)
		{
			hum_all = (hum_soil_tol_1 + hum_soil_tol_2) / 2;
			if (hum_all < hum_work)
			{
				ret = 't';
			}
			else
			{
				ret = 'f';
			}
		}
		else if (state == 1)
		{
			if (hum_soil_tol_2 < hum_work)
			{
				ret = 't';
			}
			else
			{
				ret = 'f';
			}
		}
		else if (state == 2)
		{
			if (hum_soil_tol_1 < hum_work)
			{
				ret = 't';
			}
			else
			{
				ret = 'f';
			}
		}
		else if (state == 3)
		{
			ret = 'f';
		}
	}
	return ret;
}

char pump_timer()
{
	usb_debugln(F("Pump Timer"), 2);
	char ret = 'f';
	if (check_ifex(time_system, "01/01/2000"))
	{
		usb_debug(F("Time system error"), 1);
		ret = 'f';
	}
	else
	{
		char o_h[3];
		char o_m[3];
		char o_s[3];
		char o_d[3];
		char o_mo[3];
		char o_y[5];

		char r_h[3];
		char r_m[3];
		char r_s[3];
		char r_d[3];
		char r_mo[3];
		char r_y[5];

		o_h[0] = time_system[0];
		o_h[1] = time_system[1];
		o_m[0] = time_system[3];
		o_m[1] = time_system[4];
		o_s[0] = time_system[6];
		o_s[1] = time_system[7];
		o_d[0] = time_system[9];
		o_d[1] = time_system[10];
		o_mo[0] = time_system[12];
		o_mo[1] = time_system[13];
		o_y[0] = time_system[15];
		o_y[1] = time_system[16];
		o_y[2] = time_system[17];
		o_y[3] = time_system[18];


		r_h[0] = pump_time[0];
		r_h[1] = pump_time[1];
		r_m[0] = pump_time[3];
		r_m[1] = pump_time[4];
		r_s[0] = pump_time[6];
		r_s[1] = pump_time[7];
		r_d[0] = pump_time[9];
		r_d[1] = pump_time[10];
		r_mo[0] = pump_time[12];
		r_mo[1] = pump_time[13];
		r_y[0] = pump_time[15];
		r_y[1] = pump_time[16];
		r_y[2] = pump_time[17];
		r_y[3] = pump_time[18];

		o_h[2] = '\0';
		o_m[2] = '\0';
		o_s[2] = '\0';
		o_d[2] = '\0';
		o_mo[2] = '\0';
		o_y[4] = '\0';

		r_h[2] = '\0';
		r_m[2] = '\0';
		r_s[2] = '\0';
		r_d[2] = '\0';
		r_mo[2] = '\0';
		r_y[4] = '\0';

		int o_h_i = atoi(o_h);
		int o_m_i = atoi(o_m);
		int o_s_i = atoi(o_s);
		int o_d_i = atoi(o_d);
		int o_mo_i = atoi(o_mo);
		int o_y_i = atoi(o_y);

		int r_h_i = atoi(r_h);
		int r_m_i = atoi(r_m);
		int r_s_i = atoi(r_s);
		int r_d_i = atoi(r_d);
		int r_mo_i = atoi(r_mo);
		int r_y_i = atoi(r_y);

		bool check = true;

		if (o_y_i != r_y_i)
		{
			check = false;
		}
		else
		{
			if (o_mo_i > r_mo_i)
			{
				check = false;
			}
			else
			{
				if (o_mo_i == r_mo_i)
				{
					if (o_d_i > r_d_i)
					{
						check = false;
					}
					else
					{
						if (o_d_i == r_d_i)
						{
							if (o_h_i > r_h_i)
							{
								check = false;
							}
							else
							{
								if (o_h_i == r_h_i)
								{
									if (o_m_i > r_m_i)
									{
										check = false;
									}
									else
									{
										check = true;
									}
								}
								else
									check = true;
							}
						}
						else
							check = true;
					}
				}
				else
					check = true;
			}
		}

		if (check == true)
			ret = 't';
		else
			ret = 'f';
	}
	return ret;
}
//**pump checks//



//serial comunication//

void prep_for_new_recive()
{
	usb_debugln(F("*prep_for_new_recive"), 2);
	Serial3.flush();
	int i = 0;
	if (Serial2.available())
	{
		delay(100);
		while (Serial2.available() && i < 500) {
			Serial2.read();
		}
	}
	Serial2.flush();
	i = 0;
	if (Serial2.available())
	{
		delay(100);
		while (Serial2.available() && i < 500) {
			Serial2.read();
		}
	}
	delay(20000);
}

void initiate_serial_com()
{
	usb_debugln(F("*Initiate seriala com"), 2);
	Serial3.begin(4800);
	Serial2.begin(4800);
	Serial3.flush();
	Serial2.flush();
}

void serial_com_tol_1()
{
	serial_recive_tol_1();
	if (has_com_tol_1 == true)
	{
		check_com_tol_1();
	}
	if (has_com_tol_1 == true)
	{
		take_setings_tol_1();
	}
	Serial2.flush();
}

void serial_com_tol_2()
{

	serial_recive_tol_2();
	if (has_com_tol_2 == true)
	{
		check_com_tol_2();
	}
	if (has_com_tol_2 == true)
	{
		take_setings_tol_2();
	}
	Serial3.flush();
}

bool serial_recive_tol_1()  //diavazei kai apothikevei auto pou exei lavei sto serial.
{
	usb_debugln(F("*Serial recive tol 1"), 2);
	has_com_tol_1 = false;
	has_com_count_tol_1++;
	bool ret = false;
	int i = 0;
	if (Serial2.available()) {
		delay(100); //allows all serial sent to be received together
		while (Serial2.available() && i<200) {
			recive_table_tol_1[i++] = Serial2.read();
		}
		recive_table_tol_1[i++] = '\0';
		has_com_tol_1 = true;
		has_com_count_tol_1 = 0;
	}
	if (i>0)
	{
		ret = true;
	}
	Serial2.flush();
	return ret;
}

bool serial_recive_tol_2()  //diavazei kai apothikevei auto pou exei lavei sto serial.
{
	usb_debugln(F("*Serial recive tol 2"), 2);
	has_com_tol_2 = false;
	bool ret = false;
	int i = 0;
	if (Serial3.available()) {
		delay(100); //allows all serial sent to be received together
		while (Serial3.available() && i<200) {
			recive_table_tol_2[i++] = Serial3.read();
		}
		recive_table_tol_2[i++] = '\0';
		has_com_tol_2 = true;
	}
	if (i>0)
	{
		ret = true;
	}
	Serial3.flush();
	return ret;
}

void serial_send_tol_1()
{
	usb_debugln(F("send table to tol 1 :"), 2);
	usb_debugln(send_table, 2);

	Serial2.write(send_table);
}

void serial_send_tol_2()
{
	usb_debugln(F("send table to tol 2 :"), 2);
	usb_debugln(send_table, 2);

	Serial3.write(send_table);
}

void check_com_tol_1()
{
	has_com_tol_1 = false;
	has_com_count_tol_1++;

	if ((check_ifex(recive_table_tol_1, "*") == true) && (check_ifex(recive_table_tol_1, "&") == true))
	{
		has_com_tol_1 = true;
		has_com_count_tol_1 = 0;

	}
}

void check_com_tol_2()
{
	has_com_tol_2 = false;
	has_com_count_tol_2++;

	if ((check_ifex(recive_table_tol_2, "*") == true) && (check_ifex(recive_table_tol_2, "&") == true))
	{
		has_com_tol_2 = true;
		has_com_count_tol_2 = 0;
	}
}

void take_setings_tol_1()
{
	usb_debugln(F("*Take setting tol 1"), 2);
	int relay_l_temp = -1;
	int relay_r_temp = -1;
	int relay_l_temp_stat = -1;
	int relay_r_temp_stat = -1;
	int has_com_error_temp = -1;
	check_com_tol_1();
	if (has_com_tol_1 == true)
	{
		relay_l_temp = assignval1("r_l=", 1);
		relay_r_temp = assignval1("r_r=", 1);

		relay_l_temp_stat = assignval1("r_l_s=", 1);
		relay_r_temp_stat = assignval1("r_r_s=", 1);

		has_com_error_temp = assignval1("h_e=", 1);

		if ((relay_l_temp != -1) && (relay_r_temp != -1))
		{
			if (relay_l_temp == 1)
				res_relay_l_tol_1 = true;
			if (relay_r_temp == 1)
				res_relay_r_tol_1 = true;
			if (relay_l_temp == 0)
				res_relay_l_tol_1 = false;
			if (relay_r_temp == 0)
				res_relay_r_tol_1 = false;
		}

		if ((relay_l_temp_stat != -1) && (relay_r_temp_stat != -1))
		{
			if (relay_l_temp_stat == 1)
				res_relay_l_tol_1_s = true;
			if (relay_r_temp_stat == 1)
				res_relay_r_tol_1_s = true;
			if (relay_l_temp_stat == 0)
				res_relay_l_tol_1_s = false;
			if (relay_r_temp_stat == 0)
				res_relay_r_tol_1_s = false;
		}

		if (has_com_error_temp != -1)
		{
			if (has_com_error_tol_1 == true)
			{
				if (has_com_error_temp == 1)
					has_com_error_count_tol_1++;
				has_com_error_tol_1 = true;
			}
			if (has_com_error_temp == 1)
				has_com_error_tol_1 = true;

			if (has_com_error_temp == 0)
			{
				has_com_error_tol_1 = false;
				has_com_error_count_tol_1 = 0;
			}
		}
		res_work_temp_tol_1 = assignval3("w_t=", 1);
		temp_tol_1 = assignval3("te=", 1);
		hum_tol_1 = assignval3("hu=", 1);
		hum_soil_tol_1 = assignval3("h_s=", 1);
		door_stat_tol_1 = assignval3("d_s=", 1);
		assignval2("d_l=", 1);
	}
}

void take_setings_tol_2()
{
	usb_debugln(F("*Take setting tol 2"), 2);
	int relay_l_temp = -1;
	int relay_r_temp = -1;
	int relay_l_temp_stat = -1;
	int relay_r_temp_stat = -1;
	int has_com_error_temp = -1;
	check_com_tol_2();
	if (has_com_tol_2 == true)
	{
		relay_l_temp = assignval1("r_l=", 2);
		relay_r_temp = assignval1("r_r=", 2);

		relay_l_temp_stat = assignval1("r_l_s=", 2);
		relay_r_temp_stat = assignval1("r_r_s=", 2);


		has_com_error_temp = assignval1("h_e=", 2);

		if ((relay_l_temp != -1) && (relay_r_temp != -1))
		{
			if (relay_l_temp == 1)
				res_relay_l_tol_2 = true;
			if (relay_r_temp == 1)
				res_relay_r_tol_2 = true;
			if (relay_l_temp == 0)
				res_relay_l_tol_2 = false;
			if (relay_r_temp == 0)
				res_relay_r_tol_2 = false;
		}

		if ((relay_l_temp_stat != -1) && (relay_r_temp_stat != -1))
		{
			if (relay_l_temp_stat == 1)
				res_relay_l_tol_2_s = true;
			if (relay_r_temp_stat == 1)
				res_relay_r_tol_2_s = true;
			if (relay_l_temp_stat == 0)
				res_relay_l_tol_2_s = false;
			if (relay_r_temp_stat == 0)
				res_relay_r_tol_2_s = false;
		}

		if (has_com_error_temp != -1)
		{
			if (has_com_error_tol_2 == true)
			{
				if (has_com_error_temp == 1)
				{
					has_com_error_tol_2 = true;
					has_com_error_count_tol_2++;
				}
				if (has_com_error_temp == 1)
					has_com_error_tol_2 = true;
				if (has_com_error_temp == 0)
				{
					has_com_error_tol_2 = false;
					has_com_error_count_tol_2 = 0;
				}
			}
		}
		res_work_temp_tol_2 = assignval3("w_t=", 2);
		temp_tol_2 = assignval3("te=", 2);
		hum_tol_2 = assignval3("hu=", 2);
		hum_soil_tol_2 = assignval3("h_s=", 2);
		door_stat_tol_2 = assignval3("d_s=", 2);
		assignval2("d_l=", 2);
	}
}

int assignval1(const char *find, int who)
{
	int i = -1;
	int meg_str = strlen(find);
	int meg_arr;
	if (who == 1)
	{
		meg_arr = strlen(recive_table_tol_1);
	}
	else if (who == 2)
	{
		meg_arr = strlen(recive_table_tol_2);
	}
	char temp_arr[meg_arr + 1];
	for (int i = 0; i < meg_arr + 1; i++)
	{
		if (who == 1)
		{
			temp_arr[i] = recive_table_tol_1[i];
		}
		else if (who == 2)
		{
			temp_arr[i] = recive_table_tol_2[i];
		}
	}
	char *ret;
	char *pos;
	int poss;
	if (strstr(temp_arr, find))
	{
		ret = strstr(temp_arr, find);
		pos = strchr(ret, ';');
		poss = pos - ret;
		int meg = poss - meg_str;
		char apote[meg];
		for (int i = 0; i < meg; i++)
		{
			apote[i] = ret[meg_str++];
		}
		apote[meg] = '\0';
		//gia ta relay//
		if ((check_ifex(apote, "true")) == true)
		{
			i = 1;
		}
		if ((check_ifex(apote, "false")) == true)
		{
			i = 0;
		}
	}
	return i;
}

int assignval3(const char *find, int who)
{
	int ther = -999;
	int i = -1;
	int apotel = -999;
	int meg_arr;
	int meg_str = strlen(find);
	if (who == 1)
	{
		meg_arr = strlen(recive_table_tol_1);
	}
	else if (who == 2)
	{
		meg_arr = strlen(recive_table_tol_2);
	}
	else if (who == 3)
	{
		meg_arr = strlen(recive_table_refrig);
	}
	char temp_arr[meg_arr + 1];
	for (int i = 0; i < meg_arr + 1; i++)
	{
		if (who == 1)
		{
			temp_arr[i] = recive_table_tol_1[i];
		}
		else if (who == 2)
		{
			temp_arr[i] = recive_table_tol_2[i];
		}
		else if (who == 3)
		{
			temp_arr[i] = recive_table_refrig[i];
		}
	}
	char *ret;
	char *pos;
	int poss;
	if (strstr(temp_arr, find))
	{
		ret = strstr(temp_arr, find);
		pos = strchr(ret, ';');
		poss = pos - ret;
		int meg = poss - meg_str;
		char apote[meg];
		for (int i = 0; i < meg; i++)
		{
			apote[i] = ret[meg_str++];
		}
		apote[meg] = '\0';
		for (int i = 0; i < meg; i++)
		{
			ther = atoi(apote);
		}
		apotel = ther;
	}
	return apotel;
}

void assignval2(const char *find, int who)
{
	int i = -1;
	int meg_arr;
	int meg_str = strlen(find);
	if (who == 1)
	{
		meg_arr = strlen(recive_table_tol_1);
	}
	else if (who == 2)
	{
		meg_arr = strlen(recive_table_tol_2);
	}
	char temp_arr[meg_arr + 1];
	for (int i = 0; i < meg_arr + 1; i++)
	{
		if (who == 1)
		{
			temp_arr[i] = recive_table_tol_1[i];
		}
		else if (who == 2)
		{
			temp_arr[i] = recive_table_tol_2[i];
		}
	}
	char *ret;
	char *pos;
	int poss;
	if (strstr(temp_arr, find))
	{
		ret = strstr(temp_arr, find);
		pos = strchr(ret, ';');
		poss = pos - ret;
		int meg = poss - meg_str;
		char apote[meg];
		for (int i = 0; i < meg; i++)
		{
			apote[i] = ret[meg_str++];
		}
		apote[meg] = '\0';
		for (int i = 0; i < meg; i++)
		{
			if (who == 1)
			{
				door_last_open_tol_1[i] = apote[i];
			}
			else if (who == 2)
			{
				door_last_open_tol_2[i] = apote[i];
			}
		}
	}
}

bool check_ifex(char *arr, const char* str) //elenxei an uparxei mia le3ei mesa se ena string paromio me assignval koita ekei gia plirofories
{
	bool apot = false;

	int meg_arr = strlen(arr);
	int meg_str = strlen(str);
	char temp_arr[meg_arr + 1];
	for (int i = 0; i < meg_arr + 1; i++)
	{
		temp_arr[i] = arr[i];
	}
	char *ret;
	ret = strstr(temp_arr, str);
	int meg_ret = strlen(ret);
	for (int q = meg_str; q < meg_ret; q++)
	{
		ret[q] = ' ';
	}
	if (strstr(ret, str))
	{
		apot = true;
	}
	else
	{
		apot = false;
	}
	return apot;
}
//**serial comunication//

//Rf comunication//


void initiate_rf()
{
	nRF905_init();     //anagastika to kalw sthn arxh
	delay(50);
	nRF905_enterStandBy();  //Standby mode gia na mporw na ala3w ths rithmiseis
	delay(50);
	nRF905_setAutoRetransmit(NRF905_AUTO_RETRAN_DISABLE); //auto retransmit off
	delay(50);
	nRF905_setTransmitPower(NRF905_PWR_10); // max transmite power
	delay(50);
	nRF905_setLowRxPower(NRF905_LOW_RX_DISABLE); // max recive power
	delay(50);
	nRF905_interrupt_off();  //interrupts off
	delay(50);
	nRF905_leaveStandBy();   // e3odos apo standby mode
	delay(50);
}

bool rf_recive()
{
	nRF905_receive();
	packet_s packet;
	has_com_refrig = false;
	bool ret = false;
	int count = 0;
	while ((ret == false) && (count < 100))
	{
		if (getPacket(&packet))
		{
			ret = true;
			for (int i = 0; i < packet.len; i++)
			{
				recive_table_refrig[i] = packet.data[i];
				delay(1);
			}
			recive_table_refrig[packet.len] = '\0';
		}
		count++;
		delay(100);
	}
	return ret;
}

void rf_send()
{
	packet_s packet;
	byte dataSize;
	dataSize = 28;
	packet.type = PACKET_TYPE_DATA;
	packet.len = dataSize;
	for (int i = 0; i < packet.len; i++)
	{
		packet.data[i] = send_table_refrig[i];
	}
	sendPacket(&packet);
}


void check_com_refrig()
{
	if ((check_ifex(recive_table_refrig, "*") == true) && (check_ifex(recive_table_refrig, "&") == true))
	{
		has_com_refrig = true;
		has_com_count_refrig = 0;
	}
}

void take_setings_refrig()
{
	usb_debugln(F("*Take setting refrig"), 2);
	int relay_temp = -1;
	int has_com_error_temp = -1;
	if (rf_recive() == true)
	{
		check_com_refrig();
		if (has_com_refrig == true)
		{
			relay_temp = assignval3("r=", 3);
			has_com_error_temp = assignval3("he=", 3);
			if (relay_temp != -1)
			{
				if (relay_temp == 1)
					res_relay_refrig = true;
				if (relay_temp == 0)
					res_relay_refrig = false;
			}

			if (has_com_error_temp != -1)
			{
				if (has_com_error_refrig == true)
				{
					if (has_com_error_temp == 1)
					{
						has_com_error_refrig = true;
						has_com_error_count_refrig++;
					}
				}

				if (has_com_error_temp == 1)
					has_com_error_refrig = true;

				if (has_com_error_temp == 0)
				{
					has_com_error_refrig = false;
					has_com_error_count_refrig = 0;
				}
			}

			temp_refrig = assignval3("t=", 3);
			hum_refrig = assignval3("h=", 3);
			door_stat_refrig = assignval3("ds=", 3);

			if (door_stat_refrig == 1)
			{
				int temp_size = strlen(time_system);
				for (int i = 0; i < temp_size; i++)
				{
					door_last_open_refrig[i] = time_system[i];
				}
			}
		}
	}
}


static bool getPacket(void* _packet)
{
	// Void pointer to packet_s pointer hack
	// Arduino puts all the function defs at the top of the file before packet_s being declared :/
	packet_s* packet = (packet_s*)_packet;

	byte buffer[NRF905_MAX_PAYLOAD];

	// See if any data available
	if (!nRF905_getData(buffer, sizeof(buffer)))
		return false;

	// Convert byte array to packet
	packet->type = buffer[0];
	packet->len = buffer[1];

	// Sanity check
	if (packet->len > MAX_PACKET_SIZE)
		packet->len = MAX_PACKET_SIZE;

	memcpy(packet->data, &buffer[2], packet->len);

	return true;
}

static void sendPacket(void* _packet)
{
	// Void pointer to packet_s pointer hack
	// Arduino puts all the function defs at the top of the file before packet_s being declared :/
	packet_s* packet = (packet_s*)_packet;

	// Convert packet data to plain byte array
	byte totalLength = packet->len + 2;
	byte tmpBuff[totalLength];
	tmpBuff[0] = packet->type;
	tmpBuff[1] = packet->len;
	memcpy(&tmpBuff[2], packet->data, packet->len);

	// Set address of device to send to
	//nRF905_setTXAddress(packet->dstAddress);

	// Set payload data
	nRF905_setData(tmpBuff, totalLength);

	// Send payload (send fails if other transmissions are going on, keep trying until success)
	while (!nRF905_send());
}
//**Rf comunication//


//Comunication//
void take_vars_tol_1()
{
	usb_debugln(F("*Serial recive tol 2"), 2);
	if (serial_recive_tol_1() == true)
	{
		usb_debugln(F("*has data"), 2);
		check_com_tol_1();
		if (has_com_tol_1 == true)
		{
			usb_debugln(F("*Com check ok"), 2);
			take_setings_tol_1();
		}
	}
}

void take_vars_tol_2()
{
	usb_debugln(F("*Serial recive tol 2"), 2);
	if (serial_recive_tol_2() == true)
	{
		usb_debugln(F("*has data"), 2);
		check_com_tol_2();
		if (has_com_tol_2 == true)
		{
			usb_debugln(F("*Com check ok"), 2);
			take_setings_tol_2();
		}
	}
}

void send_vars_tol_1()
{
	usb_debugln(F("*send vars tol 1 *"), 2);
	sprintf(send_table, send_request_tol_1, time_system, boolean_to_int(relay_r_tol_1), boolean_to_int(relay_l_tol_1), g_work_temp_t1);
	serial_send_tol_1();
	usb_debugln(send_table, 2);
	delay(100);
}

void send_vars_tol_2()
{
	usb_debugln(F("*send vars tol 2 *"), 2);
	sprintf(send_table, send_request_tol_2, time_system, boolean_to_int(relay_r_tol_2), boolean_to_int(relay_l_tol_2), g_work_temp_t2);
	serial_send_tol_2();
	usb_debugln(send_table, 2);
	delay(100);
}

void send_vars_refrig()
{
	usb_debugln(F("*send vars refrig"), 2);
	char a[2];
	if (pump_relay_mode == 0) //off
	{
		a[0] = 'f';
	}
	else if (pump_relay_mode == 1) //on
	{
		a[0] = 't';
	}
	else if (pump_relay_mode == 2)// auto   
	{
		a[0] = pump_auto();
	}
	else if (pump_relay_mode == 3)//timer
	{
		a[0] = pump_timer();
	}
	a[1] = '\0';

	if (a[0] == 't') /// gia elenxo rithmiseon
	{
		relay_state = true;
	}
	else
		relay_state = false;

	sprintf(send_table_refrig, send_request_refrig, a);
	usb_debugln(F("*send_table_refrig*"), 2);
	usb_debugln(send_table_refrig, 2);
	delay(10);
	rf_send();
}

void send_setings()
{
	for (int i = 0; i < 5; i++)
	{
		delay(50);
		send_vars_tol_1();
		usb_debugln(F("*Send settings tol 1"), 2);
		delay(50);
		send_vars_tol_2();
		usb_debugln(F("*Send settings tol 2"), 2);
		delay(400);
	}
	for (int i = 0; i < 20; i++)
	{
		send_vars_refrig();
		delay(400);
		usb_debugln(F("*send_vars_refrig"), 2);
	}
}

void resive_info_ser_rf()
{
	delay(100);
	usb_debugln(F("*Serial Com tol 1"), 2);
	serial_com_tol_1();
	delay(100);
	serial_com_tol_2();
	usb_debugln(F("*Serial Com tol 2"), 2);
	delay(100);
	take_setings_refrig();
	usb_debugln(F("*take_setings_refrig"), 2);
	delay(100);
	usb_debugln(F("*check_var_set"), 2);
	check_var_set();
}
//**Serial comunication//

void error_check()
{

	/*

	0:egeine reset sto ard 1
	1 : egeine reset sto ard 2
	2 : provlhma epikoinonias sto ard 1 (send)
	3 : provlhma epikoinonias sto ard 2 (send)
	4 : provlhma epikoinonias sto refrig(send)
	5 : provlhma epikoinonias sto ard 1 (resive)
	6 : provlhma epikoinonias sto ard 2 (recive)
	7 : provlhma epikoinonias sto refrig(recive)
	8 : provlhma sto gsm(connect_error)
	9 : provlhma sto gsm(recive_error)
	10 : provlhma sto gsm(send_error)
	11 : provlhma efarmoghs rithmiseon se ard 1
	12 : provlhma efarmoghs rithmiseon se ard 2
	13 : provlhma efarmoghs rithmiseon se refrig
	14 : provlhma me to roloi

	*/
	errors[2] = 0;
	errors[3] = 0;
	errors[4] = 0;
	errors[5] = 0;
	errors[6] = 0;
	errors[7] = 0;
	errors[8] = 0;
	errors[9] = 0;
	errors[10] = 0;
	errors[11] = 0;
	errors[12] = 0;
	errors[13] = 0;

	if (has_setting_set_tol_1 == false)
	{
		usb_debugln(F("   Error : No Setting set unit 1"), 1);
		errors[11] = 1;
	}
	if (has_setting_set_tol_2 == false)
	{
		usb_debugln(F("   Error : No Setting set unit 2"), 1);
		errors[12] = 1;
	}
	if (has_setting_set_refrig == false)
	{
		usb_debugln(F("   Error : No Setting set unit 3"), 1);
		errors[13] = 1;
	}


	if (has_com_tol_1 == false)
	{
		set_error_vars_tol_1();
		usb_debugln(F("   Error : No recive unit 1"), 1);
		errors[5] = 1;
	}
	if (has_com_tol_2 == false)
	{
		set_error_vars_tol_2();
		usb_debugln(F("   Error : No recive unit 2"), 1);
		errors[6] = 1;
	}
	if (has_com_refrig == false)
	{
		set_error_vars_refrig();
		usb_debugln(F("   Error : No recive unit 3"), 1);
		errors[7] = 1;
	}


	if ((has_com_error_tol_1 == true) && (has_com_error_count_tol_1 > 1))
	{

		usb_debugln(F("   Error : Unit 1 has recive problem"), 1);
		errors[2] = 1;
	}
	if ((has_com_error_tol_2 == true) && (has_com_error_count_tol_2 > 1))
	{

		usb_debugln(F("   Error : Unit 2 has recive problem"), 1);
		errors[3] = 1;
	}
	if ((has_com_error_refrig == true) && (has_com_error_refrig > 1))
	{

		usb_debugln(F("   Error : Unit 3 has recive problem"), 1);
		errors[4] = 1;
	}

	if ((gsm_connect_error == true) && (gsm_connect_error_count > 1))
	{
		usb_debugln(F("   Error : Gsm connect error"), 1);
		errors[8] = 1;
	}
	if ((gsm_recive_error == true) && (gsm_recive_error_count > 1))
	{
		usb_debugln(F("   Error : Gsm recive error"), 1);
		errors[9] = 1;
	}
	if ((gsm_send_error == true) && (gsm_send_error_count > 1))
	{
		usb_debugln(F("   Error : Gsm send error"), 1);
		errors[10] = 1;
	}


	delay(3000);
}

void check_var_set()  // elenxos efarmogis rithmiseeon sta serial kai sto refrig 
{
	usb_debugln(F("*Check var set"), 2);

	usb_debugln(F("*Resive table Unit 1"), 2);
	usb_debugln(recive_table_tol_1, 2);
	usb_debugln(F("**Resive table Unit 2"), 2);
	usb_debugln(recive_table_tol_2, 2);
	usb_debugln(F("*Resive table Unit 3"), 2);
	usb_debugln(recive_table_refrig, 2);


	//serial 1//
	has_setting_set_tol_1 = false;
	if ((res_relay_l_tol_1_s == relay_r_tol_1) && (res_relay_r_tol_1_s == relay_l_tol_1))
	{
		if (work_temp_tol_1 == res_work_temp_tol_1)
		{
			has_setting_set_tol_1 = true;
		}
	}

	//serial 2//
	has_setting_set_tol_2 = false;
	if ((res_relay_l_tol_2_s == relay_r_tol_2) && (res_relay_r_tol_2_s == relay_l_tol_2))
	{
		if (work_temp_tol_2 == res_work_temp_tol_2)
		{
			has_setting_set_tol_2 = true;
		}
	}

	//serial refrig//
	has_setting_set_refrig = false;
	if (relay_state == res_relay_refrig)
	{
		has_setting_set_refrig = true;
	}

}


void serial_and_rf_com()
{
	usb_debugln(F("~~~Send setting to units~~~"), 1);
	lcd_debug("S:Send units");

	send_setings(); //me delay 800*3

	prep_for_new_recive(); //katharizei ta serial channel exei delay 15000

	usb_debugln(F("~~~Resive vars from units~~~"), 1);
	lcd_debug("S:Recive units");

	resive_info_ser_rf();//serial and refrig resive , kai elenxos efarmoghs rithmiseon sto ard. 

	if ((has_setting_set_tol_1 == false) || (has_com_tol_1 == false))
	{
		usb_debugln(F("   Retry communication unit 1"), 1);
		lcd_debug("S:Ret unit 1");
		delay(100);
		send_vars_tol_1();
		prep_for_new_recive();
		serial_com_tol_1();
		check_var_set();
	}

	if ((has_setting_set_tol_2 == false) || (has_com_tol_2 == false))
	{
		usb_debugln(F("   Retry communication unit 2"), 1);
		lcd_debug("S:Ret unit 2");
		delay(100);
		send_vars_tol_2();
		prep_for_new_recive();
		serial_com_tol_2();
		check_var_set();
	}

	if ((has_setting_set_refrig == false) || (has_com_refrig == false))
	{
		usb_debugln(F("   Retry communication unit 3"), 1);
		lcd_debug("S:Ret unit 3");
		delay(100);
		for (int i = 0; i < 20; i++)
		{
			send_vars_refrig();
			delay(400);
		}
		take_setings_refrig();
		check_var_set();
	}

	delay(3000);
}


void init_vars()
{

	for (int i = 0; i < 199; i++)
	{
		recive_table_tol_2[i] = '0';
	}
	recive_table_tol_2[199] = '\0';

	door_last_open_tol_2[0] = 'e';
	door_last_open_tol_2[1] = 'r';
	door_last_open_tol_2[2] = 'r';
	door_last_open_tol_2[3] = 'o';
	door_last_open_tol_2[4] = 'r';
	door_last_open_tol_2[5] = '\0';
	for (int i = 0; i < 199; i++)
	{
		recive_table_tol_1[i] = '0';
	}
	recive_table_tol_1[199] = '\0';

	door_last_open_tol_1[0] = 'e';
	door_last_open_tol_1[1] = 'r';
	door_last_open_tol_1[2] = 'r';
	door_last_open_tol_1[3] = 'o';
	door_last_open_tol_1[4] = 'r';
	door_last_open_tol_1[5] = '\0';

	for (int i = 0; i < 199; i++)
	{
		recive_table_refrig[i] = '0';
	}
	recive_table_refrig[199] = '\0';

	door_last_open_refrig[0] = 'e';
	door_last_open_refrig[1] = 'r';
	door_last_open_refrig[2] = 'r';
	door_last_open_refrig[3] = 'o';
	door_last_open_refrig[4] = 'r';
	door_last_open_refrig[5] = '\0';

	for (int i = 0; i < 19; i++)
	{
		pump_time[i] = '0';
	}
	pump_time[19] = '\0';

	for (int i = 0; i < 24; i++)
	{
		g_gsm_time[i] = '0';
	}
	g_gsm_time[24] = '\0';
	for (int i = 0; i < 29; i++)
	{
		send_table_refrig[i] = '0';
	}
	send_table_refrig[29] = '\0';
	for (int i = 0; i < 19; i++)
	{
		s_time_date_last_resive[i] = '0';
	}
	s_time_date_last_resive[19] = '\0';

	has_com_tol_2 = false;
	relay_l_tol_2 = false;
	relay_r_tol_2 = false;
	has_com_error_tol_2 = false;
	res_relay_l_tol_2 = false;
	res_relay_r_tol_2 = false;
	res_relay_l_tol_2_s = false;
	res_relay_r_tol_2_s = false;
	has_setting_set_tol_2 = false;
	relay_l_tol_1 = false;
	relay_r_tol_1 = false;
	has_com_tol_1 = false;
	has_com_error_tol_1 = false;
	res_relay_l_tol_1 = false;
	res_relay_r_tol_1 = false;
	res_relay_l_tol_1_s = false;
	res_relay_r_tol_1_s = false;
	has_setting_set_tol_1 = false;
	has_com_refrig = false;
	has_com_error_refrig = false;
	res_relay_refrig = false;
	has_setting_set_refrig = false;


	pump_relay_mode = 0;
	has_com_count_tol_2 = 0;
	work_temp_tol_2 = 0;
	door_stat_tol_2 = 0;
	temp_tol_2 = -999;
	hum_tol_2 = -999;
	hum_soil_tol_2 = -999;
	has_com_error_count_tol_2 = 0;
	res_work_temp_tol_2 = -999;
	has_com_count_tol_1 = 0;
	work_temp_tol_1 = -999;
	door_stat_tol_1 = -999;
	temp_tol_1 = -999;
	hum_tol_1 = -999;
	hum_soil_tol_1 = -999;
	has_com_error_count_tol_1 = 0;
	res_work_temp_tol_1 = -999;
	has_com_count_refrig = 0;
	door_stat_refrig = -999;
	temp_refrig = -999;
	hum_refrig = -999;
	has_com_error_count_refrig = 0;
	g_relay_r_tol_1 = 0;
	g_relay_l_tol_1 = 0;
	g_relay_l_tol_2 = 0;
	g_relay_r_tol_2 = 0;
	g_work_temp_t1 = 0;
	g_work_temp_t2 = 0;
	g_pump_c_state = 0;
	g_null_check = 0;
	g_reset_t1 = 0;
	g_reset_t2 = 0;
	g_reset_error = 0;
	g_time_anl = 0;
	g_debug_lvl = 0;
}

void set_error_vars_tol_1()
{
	door_last_open_tol_1[0] = 'e';
	door_last_open_tol_1[1] = 'r';
	door_last_open_tol_1[2] = 'r';
	door_last_open_tol_1[3] = 'o';
	door_last_open_tol_1[4] = 'r';
	door_last_open_tol_1[5] = '\0';
	res_relay_l_tol_1 = false;
	res_relay_r_tol_1 = false;
	res_relay_l_tol_1_s = false;
	res_relay_r_tol_1_s = false;
	has_setting_set_tol_1 = false;
	work_temp_tol_1 = -999;
	door_stat_tol_1 = -999;
	temp_tol_1 = -999;
	hum_tol_1 = -999;
	hum_soil_tol_1 = -999;
	has_com_error_count_tol_1 = -999;
	res_work_temp_tol_1 = -999;
}

void set_error_vars_tol_2()
{
	door_last_open_tol_2[0] = 'e';
	door_last_open_tol_2[1] = 'r';
	door_last_open_tol_2[2] = 'r';
	door_last_open_tol_2[3] = 'o';
	door_last_open_tol_2[4] = 'r';
	door_last_open_tol_2[5] = '\0';
	res_relay_l_tol_2 = false;
	res_relay_r_tol_2 = false;
	res_relay_l_tol_2_s = false;
	res_relay_r_tol_2_s = false;
	has_setting_set_tol_2 = false;
	work_temp_tol_2 = -999;
	door_stat_tol_2 = -999;
	temp_tol_2 = -999;
	hum_tol_2 = -999;
	hum_soil_tol_2 = -999;
	has_com_error_count_tol_2 = -999;
	res_work_temp_tol_2 = -999;
}

void set_error_vars_refrig()
{
	door_last_open_refrig[0] = 'e';
	door_last_open_refrig[1] = 'r';
	door_last_open_refrig[2] = 'r';
	door_last_open_refrig[3] = 'o';
	door_last_open_refrig[4] = 'r';
	door_last_open_refrig[5] = '\0';
	res_relay_refrig = false;
	has_setting_set_refrig = false;
	door_stat_refrig = 0;
	temp_refrig = 0;
	hum_refrig = 0;
	has_com_error_count_refrig = 0;
}


///////////////GSM COM/////////////////////
bool is_gsm_open;
bool is_gsm_conn;
bool is_settings_recived;
bool is_send_complete;


/*
Gia send!!!!
Rl_t1
Rr_t1
Ds_t1
Dl_t1
Te_t1
Hu_t1
Sh_t1
Hce_t1
Rl_t2
Rr_t2
Ds_t2
Dl_t2
Te_t2
Hu_t2
Sh_t2
Hce_t2
Rl_re
Ds_re
Dl_re
Te_re
Hu_re
Hc_re
Te_sy
Ce_sy
Ge_sy
Bm_sy
Ec_sy
Ti_sy
Lr_sy
Hc_t1
Hc_t2
Hce_re
Pm_re
Gre_sy
Grc_sy
Gse_sy
Gsc_sy
*/



#define WRITE_REQUEST ("GET /add.php?Rl_t1=%i&Rr_t1=%i&Ds_t1=%i&Dl_t1=%s&Te_t1=%i&Hu_t1=%i&Sh_t1=%i&Hc_t1=%i&Rl_t2=%i&Rr_t2=%i&Ds_t2=%i&Dl_t2=%s&Te_t2=%i&Hu_t2=%i&Sh_t2=%i&Hc_t2=%i&Rl_re=%i&Ds_re=%i&Dl_re=%s&Te_re=%i&Hu_re=%i&Hc_re=%i&Te_sy=%i&Bm_sy=%i&Ec_sy=%i&Ti_sy=%s&Lr_sy=%s&Hc_t1=%i&Hc_t2=%i&Hce_re=%i&Pm_re=%i&Gre_sy=%i&Grc_sy=%i&Gse_sy=%i&Gsc_sy=%i&Hs_t1=%i&Hs_t2=%i&Hs_re=%i&Wt_t1=%i&Wt_t2=%i HTTP/1.1\r\nHost:greenhouseonline.tk:80\r\nContent-Length:0\r\n\r\n")
void make_send_arr()
{
	usb_debugln(F("~~~Make send array~~~"), 2);
	sprintf(send_table, WRITE_REQUEST, boolean_to_int(relay_l_tol_1), boolean_to_int(relay_r_tol_1), door_stat_tol_1, door_last_open_tol_1, temp_tol_1, hum_tol_1, hum_soil_tol_1, errors[2], boolean_to_int(relay_l_tol_2), boolean_to_int(relay_r_tol_2), door_stat_tol_2, door_last_open_tol_2, temp_tol_2, hum_tol_2, hum_soil_tol_2, errors[3], res_relay_refrig, door_stat_refrig, door_last_open_refrig, temp_refrig, hum_refrig, errors[4], s_system_temp, boolean_to_int(s_battary_mode), s_error_code, time_system, s_time_date_last_resive, errors[5], errors[6], errors[7], pump_relay_mode, boolean_to_int(gsm_recive_error), gsm_recive_error_count, boolean_to_int(gsm_send_error), gsm_send_error_count, boolean_to_int(has_setting_set_tol_1), boolean_to_int(has_setting_set_tol_2), boolean_to_int(has_setting_set_refrig), res_work_temp_tol_1, res_work_temp_tol_2);
	delay(300);
	usb_debugln(send_table, 2);
}

int boolean_to_int(bool eis)
{
	int apot = -1;
	if (eis == true)
		apot = 1;
	else
		apot = 0;
	return apot;
}

void Gsm_start() //ok
{
	is_gsm_open = false;
	if (gsm.gsm_begin() == true)
	{
		is_gsm_open = true;
	}
}

void Gsm_connect() //ok
{
	is_gsm_conn = false;
	if (gsm.gsm_connect() == true)
	{
		is_gsm_conn = true;
	}
}

void Gsm_get_settings()
{
	is_settings_recived = false;
	if (gsm.gsm_getpage(time_system) == 1)
	{
		is_settings_recived = true;
		take_ext_vars();
	}
}



void Gsm_send_data()
{
	is_send_complete = false;
	if (gsm.gsm_send(send_table) == 1)
	{
		is_send_complete = true;
	}
}



void send_gsm_vars()
{
	usb_debugln(F("~~~Gsm Send~~~"), 1);
	lcd_debug("S:Gsm Send");
	usb_debugln(F("Make send array"), 2);
	make_send_arr();
	gsm_send_error = true;
	if (is_gsm_conn == true)
	{
		Gsm_send_data();
		if (is_send_complete == true)
		{
			usb_debugln(F("Gsm send complete"), 1);
			lcd_debug("S:Gsm Send ok");
			gsm_send_error_count = 0;
			gsm_send_error = false;
		}
		else
		{
			usb_debugln(F("   Error : gsm send error"), 1);
			lcd_debug("S:Gsm Send error");
			gsm_send_error_count++;
			gsm_send_error = true;
		}

	}
	else
	{
		usb_debugln(F("   Error : gsm send error"), 1);
		lcd_debug("S:Gsm Send error");
		gsm_send_error_count++;
		gsm_send_error = true;
	}
	delay(3000);
}

void gsm_net_start()
{
	usb_debugln(F("~~~Network Start~~~"), 1);
	lcd_debug("S:Net Start");
	gsm_connect_error = true;
	Gsm_start();
	if (is_gsm_open == true)
	{
		usb_debugln(F("   Gsm is opened"), 1);
		lcd_debug("S:Gsm Opened");
		Gsm_connect();
		if (is_gsm_conn == true)
		{
			usb_debugln(F("   Gsm is connected"), 1);
			lcd_debug("S:Gsm con ok");
			is_gsm_conn = true;
			gsm_connect_error = false;
			gsm_connect_error_count = 0;
		}
		else
		{
			usb_debugln(F("   Error : connect error"), 1);
			lcd_debug("S:Connect error");
			gsm_connect_error_count++;
		}
	}
	else
	{
		usb_debugln(F("   Error : connect error"), 1);
		lcd_debug("S:Connect error");
		gsm_connect_error_count++;
	}
}


void get_gsm_vars()
{
	usb_debugln(F("~~~Start gsm recive~~~"), 1);
	lcd_debug("S:Gsm recive");
	gsm_recive_error = true;
	if (is_gsm_conn == true)
	{
		Gsm_get_settings();
		if (is_settings_recived == true)
		{
			usb_debugln(F("   I have recived"), 1);
			lcd_debug("S:has recive");
			gsm_recive_error_count = 0;
			gsm_recive_error = false;

			if (setting_changed() == true) //an exei alaxthei kati to grafei sthn epprom gia monima 
			{
				usb_debugln(F("   Changed settings recived write to eeprom"), 1);
				write_perm_values();
			}

			usb_debugln(F("   Feed gsm data to system vars"), 1);
			lcd_debug("S:set vars");
			apply_gsm_vars_to_system();
		}
		else
		{
			usb_debugln(F("   Error : gsm recive error"), 1);
			lcd_debug("E:gsm recive");
			gsm_recive_error_count++;
			gsm_recive_error = true;
		}
	}
	else
	{
		usb_debugln(F("   Error : gsm recive error"), 1);
		lcd_debug("E:recive error");
		gsm_recive_error_count++;
		gsm_recive_error = true;
	}

	if (gsm_recive_error > 5)
	{
		//aply saved vals to system//
	}
	delay(3000);
}



bool setting_changed() //elenxei an exoun alaxthei h rithmoseis apo to gsm me ths torines
{

	bool temp = false;


	if (pump_relay_mode != g_pump_c_state)
	{
		temp = true;
	}

	if (relay_l_tol_1 != bool_to_int(g_relay_l_tol_1))
	{
		temp = true;
	}

	if (relay_r_tol_1 != bool_to_int(g_relay_r_tol_1))
	{
		temp = true;
	}

	if (work_temp_tol_1 != g_work_temp_t1)
	{
		temp = true;
	}

	if (relay_l_tol_2 != bool_to_int(g_relay_l_tol_2))
	{
		temp = true;
	}

	if (relay_r_tol_2 != bool_to_int(g_relay_r_tol_2))
	{
		temp = true;
	}

	if (work_temp_tol_2 != g_work_temp_t2)
	{
		temp = true;
	}

	if (usb_debug_lvl_s != g_debug_lvl)
	{
		temp = true;
	}

	return temp;
}

void apply_gsm_vars_to_system() //dinei ths metavlites tou gsm sths metavlites susthmatos
{

	usb_debugln(F("~~~Apply gsm vars to system~~~"), 2);
	pump_relay_mode = g_pump_c_state;
	relay_l_tol_1 = int_to_bool(g_relay_l_tol_1);
	relay_r_tol_1 = int_to_bool(g_relay_r_tol_1);
	work_temp_tol_1 = g_work_temp_t1;
	relay_l_tol_2 = int_to_bool(g_relay_l_tol_2);
	relay_r_tol_2 = int_to_bool(g_relay_r_tol_2);
	work_temp_tol_2 = g_work_temp_t2;
	usb_debug_lvl_s = g_debug_lvl;
	for (int i = 0; i < 20; i++)
	{
		pump_time[i] = g_gsm_pump_time[i];
	}
	pump_time[19] = '\0';

	if (g_reset_t1 == 1)
	{
		reset_t1();
	}

	if (g_reset_t2 == 1)
	{
		reset_t2();
	}

	if (g_reset_error == 1)
	{
		reset_error();
	}

	if (g_time_anl == 1)
	{
		give_system_ext_time();
	}

	if (g_time_anl == 0)
	{
		s_time_date_last_resive[0] = g_gsm_time[0];
		s_time_date_last_resive[1] = g_gsm_time[1];
		s_time_date_last_resive[2] = ':';
		s_time_date_last_resive[3] = g_gsm_time[3];
		s_time_date_last_resive[4] = g_gsm_time[4];
		s_time_date_last_resive[5] = ':';
		s_time_date_last_resive[6] = g_gsm_time[6];
		s_time_date_last_resive[7] = g_gsm_time[7];
		s_time_date_last_resive[8] = ' ';
		s_time_date_last_resive[9] = g_gsm_time[9];
		s_time_date_last_resive[10] = g_gsm_time[10];
		s_time_date_last_resive[11] = '/';
		s_time_date_last_resive[12] = g_gsm_time[12];
		s_time_date_last_resive[13] = g_gsm_time[13];
		s_time_date_last_resive[14] = '/';
		s_time_date_last_resive[15] = g_gsm_time[15];
		s_time_date_last_resive[16] = g_gsm_time[16];
		s_time_date_last_resive[17] = g_gsm_time[17];
		s_time_date_last_resive[18] = g_gsm_time[18];
		s_time_date_last_resive[19] = '\0';
		delay(100);
	}
}

void post_mega_reset()
{

	if (is_gsm_conn == true)
	{
		delay(3000);
		gsm.gsm_mega_reset(time_system);
	}
}


void give_internet_time()
{
		lcd_debug("S:Start gnet");

		gsm_net_start(); //start gsm network
		delay(2000);
		usb_debugln(F("~~~Start gsm get time~~~"), 1);
		lcd_debug("S:Gsm rec time");
		if (is_gsm_conn == true)
		{
			if (gsm.gsm_get_time() == 1)
			{
				is_settings_recived = true;
				take_ext_time_var();
				give_system_ext_time();
				take_time();
			}
			else
			{
				usb_debugln(F("   Error : gsm recive error"), 1);
				lcd_debug("E:gsm recive");
				gsm_recive_error_count++;
				gsm_recive_error = true;
			}
		}
		else
		{
			usb_debugln(F("   Error : gsm recive error"), 1);
			lcd_debug("E:recive error");
			gsm_recive_error_count++;
			gsm_recive_error = true;
		}
		delay(3000);
}
/////////////////**GSM COM**///////////////






//interupt(menu)//
bool menu_request;

void interrupt_set()
{
	menu_request = false;
	//pinMode(button_pin, INPUT);    // makes pin 3 an input pin
	//digitalWrite(button_pin, LOW); // writing a HIGH to an INPUT pin
	attachInterrupt(button_ipin, change, RISING);

}

void change()
{
	if (menu_request == false)
	{
		detachInterrupt(button_ipin);
		menu_request = true;
		lcd_print(0, "Menu");
		lcd_print(1, "Perimenete..");
		Serial.println("lala 1");
	}
	else
		Serial.println("lala 2");
}

/*
void menu_check()
{
	if (menu_request == true)
	{
		delay(1000);
		lcd_menu();
		menu_request = false;
		interrupt_set();
	}
}
*/
//**interupt//



void pin_init()
{
	pinMode(led_blue_pin, OUTPUT);
	pinMode(led_red_pin, OUTPUT);
	pinMode(an_current_sensor, INPUT);
	//pinMode(inter_boton, INPUT);
	pinMode(reset_t1_pin, OUTPUT);
	pinMode(reset_t2_pin, OUTPUT);
	pinMode(relay_charger, OUTPUT);
	pinMode(charger_sensor, INPUT);

	digitalWrite(led_blue_pin, LOW);
	digitalWrite(led_red_pin, LOW);
	digitalWrite(reset_t1_pin, HIGH);
	digitalWrite(reset_t2_pin, HIGH);
	digitalWrite(relay_charger, HIGH);
}

void mega_checks()
{
	take_system_temp(); //elenxos system temp
	battary_check();  //elenxos gia battery
	System_led();  // lampakia mpatarias/revmatos
	delay(500);
	take_time(); // diavazei thn wra apo to roloi
	delay(500);
}

void first_com()
{
	usb_debugln(F("~~~gsm_net_start~~~"), 1);
	lcd_debug("S:gsm_net_start");
	gsm_net_start(); //start gsm network
	delay(8000);
	usb_debugln(F("~~~post_mega_reset~~~"), 1);
	lcd_debug("S:post_reset");
	post_mega_reset();
	delay(8000);
	usb_debugln(F("~~~get_gsm_vars~~~"), 1);
	lcd_debug("S:Get settings");
	get_gsm_vars();
	if (gsm_recive_error != false)
	{
		read_perm_values();
		usb_debugln(F("Error : no_gsm_com_settings"), 1);
		lcd_debug("S:No gsm set");
	}
	delay(5000);
}



void debug_lvl(int a)
{

	/*
	0 : normal mode, no serial, no lcd
	1 : debug lvl 1, serial simple, no lcd
	2 : debug lvl 2, serial pro, no lcd
	3 : debug lvl 3, serial pro gsm, no lcd
	4 : debug lvl 4, lcd simple
	5 : debug lvl 5, lcd simple gsm  lcd
	*/

	switch (a)
	{

	case 0:
		serial_debug_lvl = 0;
		lcd_debug_lvl = 0;
		gsm.set_usb_debug_lnl(0);
		break;

	case 1:
		serial_debug_lvl = 1;
		lcd_debug_lvl = 0;
		gsm.set_usb_debug_lnl(0);
		break;

	case 2:
		serial_debug_lvl = 2;
		lcd_debug_lvl = 0;
		gsm.set_usb_debug_lnl(0);
		break;

	case 3:
		serial_debug_lvl = 2;
		lcd_debug_lvl = 0;
		gsm.set_usb_debug_lnl(1);
		break;

	case 4:
		serial_debug_lvl = 0;
		lcd_debug_lvl = 1;
		gsm.set_usb_debug_lnl(0);
		break;

	case 5:
		serial_debug_lvl = 0;
		lcd_debug_lvl = 1;
		gsm.set_usb_debug_lnl(2);
		break;
	default:
		serial_debug_lvl = 0;
		lcd_debug_lvl = 0;
		gsm.set_usb_debug_lnl(0);
		break;
	}
}

void clock_init()
{
	RTC.stop();
	delay(100);
	RTC.start();
}

bool first;
void setup()
{
	first = true;
	pin_init();
	Serial.begin(9600);
	initiate_serial_com();
	initiate_rf();
	//charger_init(); //gia ton fortisth(alazei ton epilogea mpatarias
	init_vars();
	read_perm_values(); //dinei ths thmes pou einai apothikevmenes sthn flash sto systhma
	debug_lvl(0);  // 0 : no serial, no lcd , 1 : serial simple, no lcd , 2 : serial pro, no lcd , 3 : serial pro gsm, no lcd , 4 : lcd simple , 5 : lcd simple gsm lcd // gia na pernei apo to susthma usb_debug_lvl_s
	customKeypad.begin(); //keypad_start
	initscreen();
	clock_init();
	mega_checks(); //elenxei tous estithres kai pernei thn wra tou mega kai reumatos 
				   //interrupt_set();  //energopihsh interrupt
	usb_debugln(F("~~~Setup!~~~"), 1);
	lcd_debug("S:Setup");
	//delay(3000);
	//menu_check();
	//take_time();
	delay(3000);
	give_internet_time();
	delay(2000); // ~7 sec delay
	first_com();// post mega start , get init vars from gsm h presets
}


void loop()
{
	//menu_check(); //elenxei an exei ginei request gia menu 
	if (first == true)
	{
		lcd_print(0, "First run");
		lcd_print(1, "--");
		first = false;
	}
	else
	{
		lcd_on_run(1);
	}

	usb_debugln(F("~~~Loop~~~"), 1);
	usb_debugln(F("------------"), 1);
	usb_debugln(F("~~~Mega_checks~~~"), 1);
	lcd_debug("S:Mega_checks");
	lcd_debug("S:Loop");

	mega_checks(); //mega_checks

	usb_debugln(F("~~~Start gsm network~~~"), 1);
	lcd_debug("S:Start gnet");

	gsm_net_start(); //start gsm network

	usb_debugln(F("~~~Get_gsm_vars~~~"), 1);
	lcd_debug("S:Get gsm vars");

	get_gsm_vars(); //gsm resive (pernei ths metavlites apo to gsm kai ths pernaei sths kiries metavlites)

	serial_and_rf_com();

	usb_debugln(F("~~~Reset check~~~"), 1);
	lcd_debug("S:Reset check");

	reset_check();//an uparxei kapio thema me ta ard elenxos gia reset 

	usb_debugln(F("~~~Error check~~~"), 1);
	lcd_debug("S:Error check");

	error_check();//elexnos gia lathoi se coms

	usb_debugln(F("~~~Send gsm vars~~~"), 1);
	lcd_debug("S:Send gvars");

	send_gsm_vars();//gsm send

	usb_debugln(F("~~~Gsm Disconnect~~~"), 1);
	lcd_debug("S:Gsm Disconn");

	gsm.gsm_disconnect();

	usb_debugln(F("~~~Lcd print errors~~~"), 1);
	lcd_debug("S:Lcd print errors");

	lcd_errors();//emfanish mhnhmaton lathon se monitor kai ***delay*** 2:30 lepta peripou 

	usb_debugln(F("~~~End loop~~~"), 1);
	lcd_debug("S:End loop");
}




//on screen menu//

#define prob_str ("Provlhma %i-%i\0")
#define prob_1 ("Reset 1\0")
#define prob_2 ("Reset 2\0")
#define prob_3 ("Send 1\0")
#define prob_4 ("Send 2\0")
#define prob_5 ("Send refrig\0")
#define prob_6 ("Recive 1\0")
#define prob_7 ("Recive 2\0")
#define prob_8 ("Recive refrig\0")
#define prob_9 ("Gsm connect\0")
#define prob_10 ("Gsm recive\0")
#define prob_11 ("Gsm send\0")
#define prob_12 ("Settings 1\0")
#define prob_13 ("Settings 2\0")
#define prob_14 ("Settings refr\0")
#define prob_15 ("Clock error\0")

void lcd_errors()
{
	usb_debugln(F("~~~Arduino on \"standby\" for about 3'~~~"), 1);
	char text_to_show[20];
	int probs = 0;
	int probs_count = 0;
	for (int i = 0; i < 14; i++)
	{
		if (errors[i] == 1)
			probs++;
	}
	if (probs != 0)
	{
		int count = 0;
		for (int i = 0; i < 28; i++)
		{
			if (probs_count >= probs)
				probs_count = 1;
			else
				probs_count++;

			sprintf(text_to_show, prob_str, probs_count, probs);
			lcd_print(0, text_to_show);
			while (errors[count] == 0)
			{
				count++;
				if (count == 14)
				{
					count = 0;
				}
			}

			switch (count) {
			case 0:
				lcd_print(1, prob_1);
				break;
			case 1:
				lcd_print(1, prob_2);
				break;
			case 2:
				lcd_print(1, prob_3);
				break;
			case 3:
				lcd_print(1, prob_4);
				break;
			case 4:
				lcd_print(1, prob_5);
				break;
			case 5:
				lcd_print(1, prob_6);
				break;
			case 6:
				lcd_print(1, prob_7);
				break;
			case 7:
				lcd_print(1, prob_8);
				break;
			case 8:
				lcd_print(1, prob_9);
				break;
			case 9:
				lcd_print(1, prob_10);
				break;
			case 10:
				lcd_print(1, prob_11);
				break;
			case 11:
				lcd_print(1, prob_12);
				break;
			case 12:
				lcd_print(1, prob_13);
				break;
			case 13:
				lcd_print(1, prob_14);
				break;
			case 14:
				lcd_print(1, prob_15);
				break;
			default:
				lcd_print(1, "------");
				break;
			}
			count++;
			if (count == 14)
			{
				count = 0;
			}
			delay(4885); //*28 vgenei ~2:30 xronometrimeno    //1885 gia test
		}
	}
	else
	{
		lcd_on_run(2);
		delay(136780); //~ peripou 2:20 lepta    
	}

}

void lcd_print(int a, const char* text)
{
	int row = a;
	if ((a != 1) && (a != 0))
	{
		row = 0;
	}
	if (row == 0)
	{
		lcd.clear();
	}
	char arr[16];
	bool find = false;
	int poss = 0;
	for (int i = 0; i < 16; i++)
	{
		arr[i] = text[i];
		if ((text[i] == '\0') && (find == false))
		{
			find = true;
			poss = i;
		}
	}
	if (find == false)
	{
		arr[16] = '\0';
		poss = 16;
	}
	else
	{
		for (int i = poss + 1; i < 16; i++)
		{
			arr[i] = ' ';
		}
	}
	int aff = 16 - poss; //kena
	int dier = 0;
	if (aff != 0)
	{
		dier = aff / 2;
	}
	int temp_pos = 0;
	for (int i = 0; i < dier; i++)
	{
		delay(10);
		lcd.setCursor(i, row);
		lcd.print(' ');
		temp_pos = i;
	}
	for (int i = 0; i < 16; i++)
	{
		delay(10);
		lcd.setCursor(temp_pos + 1, row);
		temp_pos++;
		if (i < poss)
		{
			lcd.print(arr[i]);
		}
		else
		{
			lcd.print(' ');
		}
	}
}   // tiponu sto kentro ths othonhs



char wait_for_key()
{
	char key = ' ';
	bool search = false;
	while (search == false)    // A == ^ , B == > , C == < , D == | , * == Back , # == ok  
	{
		key = ' ';
		char customKey = customKeypad.getKey();
		if (customKey != NO_KEY)
		{
			key = customKey;
			search = true;
		}
	}
	return key;
}


#define txt_ther ("= %ic\0")
#define txt_igr ("= %i\0")
#define txt_edaf ("= %i\0")
#define txt_door ("= %s\0")
#define txt_doorl ("= %s\0")
#define txt_an1 ("= %s\0")
#define text_an2 ("= %s\0")
#define txt_coms ("= %s\0")

#define string_a ("%s\0")

#define relay_c ("Now= %s\0")
#define work_temp ("Now= %ic\0")

#define t_d ("%s %s\0")


#define lor ("U1:%ic  U2:%ic\0")
#define lor1 ("U3:%ic\0")
#define lor2 ("U3:%ic.\0")

void lcd_on_run(int a)
{
	char text_to_show[20];
	sprintf(text_to_show, lor, temp_tol_1 , temp_tol_2);
	lcd_print(0, text_to_show);
	if (a == 1)
	{
		sprintf(text_to_show, lor1, temp_refrig);
	}
	else
	{
		sprintf(text_to_show, lor2, temp_refrig);
	}
	lcd_print(1, text_to_show);
}



void initscreen()
{
	lcd.begin(16, 2);
	lcd.clear();
	lcd_print(0, "..Setup..");
}








//prints//


void usb_debugln(const char* a, int c)
{
	if (c <= serial_debug_lvl)
	{
		Serial.println(a);
	}
}

void usb_debugln(int a, int c)
{
	if (c <= serial_debug_lvl)
	{
		Serial.println(a);
	}
}

void usb_debugln(char a, int c)
{
	if (c <= serial_debug_lvl)
	{
		Serial.println(a);
	}
}

void usb_debugln(float a, int c)
{
	if (c <= serial_debug_lvl)
	{
		Serial.println(a);
	}
}

void usb_debug(const char* a, int c)
{
	if (c == serial_debug_lvl)
	{
		Serial.print(a);
	}
}

void usb_debug(int a, int c)
{
	if (c <= serial_debug_lvl)
	{
		Serial.print(a);
	}
}

void usb_debug(char a, int c)
{
	if (c <= serial_debug_lvl)
	{
		Serial.print(a);
	}
}

void usb_debug(float a, int c)
{
	if (c <= serial_debug_lvl)
	{
		Serial.print(a);
	}
}

void usb_debug(bool a, int c)
{
	if (c <= serial_debug_lvl)
	{
		if (a == true)
		{
			Serial.print("True");
		}
		else
			Serial.print("False");
	}
}

void usb_debugln(bool a, int c)
{
	if (c <= serial_debug_lvl)
	{
		if (a == true)
		{
			Serial.println("True");
		}
		else
			Serial.println("False");
	}
}

void usb_debugln(const __FlashStringHelper *string, int c)
{
	if (c <= serial_debug_lvl)
	{
		Serial.println(string);
	}
}

void usb_debug(const __FlashStringHelper *string, int c)
{
	if (c <= serial_debug_lvl)
	{
		Serial.print(string);
	}
}


void lcd_debug(const char *string)
{
	if (lcd_debug_lvl == 1)
	{
		delay(100);
		lcd_print(0, "Debug : ");
		delay(100);
		lcd_print(1, string);
		delay(5000);
	}
}
//**prints//





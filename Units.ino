#include <dht11.h>


//gia comunication//
#define true_temp ("true")
#define false_temp ("false")
//**gia comunication//

//gia send (comunication)//
char send_char[400];
#define send_str ("*r_l=%s;r_r=%s;d_s=%i;d_l=%s;te=%i;hu=%i;h_s=%i;w_t=%i;h_e=%s;r_l_s=%s;r_r_s=%s;&\0")
//**gia send (comunication)//

//metavlites gia initiate//
#define start_work_temp 23;
#define start_relay_l false;
#define start_relay_r false;
int has_com_error_num = 40;
int reset_mega_num = 90;
//**metavlites gia initiate//


// pins ktlp gia sensors//
#define soil_humidity_sensor A1 
dht11 temp_sensor;
#define temp_sensor_pin 3
#define Relay_r_pin 5
#define Relay_l_pin 4
#define door_sensor_pin 2
#define Relay_mega_reset_pin 7
// ***pins ktlp gia sensors//

bool time_changed;

//metavlhtes apostolh k fancions//

boolean relay_l_pos;
boolean relay_r_pos;
boolean relay_r_check;
boolean relay_l_check;
int door_stat;
char door_last_open[31];
int hum;
int hum_soil;
int thermokrasia;
boolean com_error_bool;
int has_com_metr;
int mega_reset_metr;
boolean reset_mega;
//**metavlhtes apostolh k fancions//


//serial vars//
char recive_table[500];
char send_table[500];
boolean has_com;
char time_arr[30];
int work_temp;
boolean relay_l_or;
boolean relay_r_or;
//**serial vars//



//humidity sensor//
int take_soil_humidity()
{
	boolean check = false;
	int ret = -1;
	int hum_1 = analogRead(soil_humidity_sensor);
	delay(1000);
	int hum_2 = analogRead(soil_humidity_sensor);
	int max = hum_2;
	if (hum_1 > hum_2)
	{
		max = hum_1;
		check = true;
	}
	if (check == true)
	{
		int temp = hum_2 - hum_1;
		if (temp >= 300)
		{
			ret = hum_2;
		}
	}
	if (check == false)
	{
		int temp = hum_1 - hum_2;
		if (temp >= 300)
		{
			ret = hum_1;
		}
	}
	if (hum_1 == hum_2)
	{
		ret = hum_1;
	}
	if ((ret != -1) && (ret < 300))
	{
		ret = -1;
	}
	return ret;
}
//**humidity sensor//



//temp sensor//
int check_temp_sensor() //epistrefei 1 an olla ok me ton estitira , 0 an uparxei problhma 
{
	boolean check = false;
	int ret = -1;
	int chk = temp_sensor.read(temp_sensor_pin);
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
//temp sensor//



// relay //
void set_relay(char orie, boolean on_off) // char(l,r) gia aristero h de3io relay , boolean on_off true gia on false gia off
{
	if (orie == 'l')
	{
		if (on_off == true)
		{
			digitalWrite(Relay_l_pin, HIGH);
			relay_l_pos = true;
		}
		else
		{
			digitalWrite(Relay_l_pin, LOW);
			relay_l_pos = false;
		}
	}
	else if (orie == 'r')
	{
		{
			if (on_off == true)
			{
				digitalWrite(Relay_r_pin, HIGH);
				relay_r_pos = true;
			}
			else
			{
				digitalWrite(Relay_r_pin, LOW);
				relay_r_pos = false;
			}
		}
	}
}

int read_relay(char orie)   // epistrefei 1 an to relay einai anoixto 0 an einai klisto kai -1 gia error
{
	int ret = -1;
	int val = 0;
	int check[8];
	boolean check_bool = false;
	if (orie == 'l')
	{
		for (int i = 0; i < 8; i++)
		{
			delay(15);
			check[i] = digitalRead(Relay_l_pin);
		}
		for (int i = 0; i < 8; i++)
		{
			if (check[0] != check[i])
			{
				check_bool = true;
			}
		}
		if (check_bool == false)
		{
			ret = check[0];
		}
	}
	else if (orie == 'r')
	{
		for (int i = 0; i < 8; i++)
		{
			delay(15);
			check[i] = digitalRead(Relay_r_pin);
		}
		for (int i = 0; i < 8; i++)
		{
			if (check[0] != check[i])
			{
				check_bool = true;
			}
		}
		if (check_bool == false)
		{
			ret = check[0];
		}
	}
	return ret;
}

void change_relay()
{
	set_relay('r', relay_r_pos);
	set_relay('l', relay_l_pos);
}

void check_relay_status() //an doulevoun ola kala me to relay kai ton estitira eixoun tha kanei ta relay_r_check true
{
	int re_l = read_relay('l');
	int re_r = read_relay('r');
	relay_r_check = false;
	relay_l_check = false;
	if (re_l != -1)
	{
		if (re_l == 1)
		{
			relay_l_check = true;
		}
		else
		{
			relay_l_check = false;
		}
	}
	else
	{
		relay_l_check = false;
	}
	if (re_r != -1)
	{
		if (re_r == 1)
		{
			relay_r_check = true;
		}
		else
		{
			relay_r_check = false;
		}
	}
	else
	{
		relay_r_check = false;
	}
}
// **relay//


//door sensor//
int check_door()  // epistrefei 1 otan h porta einai anoixth (den uparxei magnitismos) 0 otan h porta einai klistei //// -1 sthn periptosh lathous
{
	int ret = -1;
	int check[8];
	boolean check_bool = false;
	for (int i = 0; i < 8; i++)
	{
		delay(15);
		check[i] = digitalRead(door_sensor_pin);
	}
	for (int i = 0; i < 8; i++)
	{
		if (check[0] != check[i])
		{
			check_bool = true;
		}
	}
	if (check_bool == false)
	{
		ret = check[0];
	}
}

void check_door_status() //-1 gia error , 1 an einai anoixtoi , 0 an einai klistei 
{
	door_stat = -1;
	int temp_door = check_door();
	if (temp_door != -1)
	{
		if (temp_door == 1)
		{
			door_stat = temp_door;
			strcpy(door_last_open, time_arr);
		}
		else if (temp_door == 0)
		{
			door_stat = temp_door;
		}
	}
}
//**door sensor//







//serial comunication//
boolean serial_recive()  //diavazei kai apothikevei auto pou exei lavei sto serial.
{
	has_com = false;
	boolean ret = false;
	int i = 0;
	if (Serial.available()) {
		delay(100); //allows all serial sent to be received together
		while (Serial.available() && i<500) {
			recive_table[i++] = Serial.read();
		}
		recive_table[i++] = '\0';
	}
	if (i>0)
	{
		ret = true;
	}
	Serial.flush();
	return ret;
}

void serial_send()
{
	Serial.write(send_table);
}

boolean check_ifex(char *arr, const char* str) //elenxei an uparxei mia le3ei mesa se ena string paromio me assignval koita ekei gia plirofories
{
	boolean apot = false;

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

void check_com()
{
	if ((check_ifex(recive_table, "*") == true) && (check_ifex(recive_table, "&") == true))
	{
		has_com = true;
	}
}

boolean take_setings()
{
	int relay_l_temp = -1;
	int relay_r_temp = -1;
	check_com();
	if (has_com == true)
	{
		relay_l_temp = assignval1("r_l=");
		relay_r_temp = assignval1("r_r=");
		assignval2("t=");
		assignval3("w_t=");
		if ((relay_l_temp != -1) && (relay_r_temp != -1))
		{
			if (relay_l_temp == 1)
				relay_l_or = true;
			if (relay_r_temp == 1)
				relay_r_or = true;
			if (relay_l_temp == 0)
				relay_l_or = false;
			if (relay_r_temp == 0)
				relay_r_or = false;
		}
	}
}

int assignval1(const char *find) //gia na pernei 0 h 1 apo to html get kai  na ta kanei int //
{
	int i = -1;
	int meg_str = strlen(find);
	int meg_arr = strlen(recive_table);
	char temp_arr[meg_arr + 1];
	for (int i = 0; i < meg_arr + 1; i++)
	{
		temp_arr[i] = recive_table[i];
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
		if ((check_ifex(apote, "1")) == true)
		{
			i = 1;
		}
		if ((check_ifex(apote, "0")) == true)
		{
			i = 0;
		}
	}
	return i;
}

void assignval2(const char *find) //gia na pernei thn wra apo to html get //
{
	time_changed = false;
	int i = -1;
	int meg_str = strlen(find);
	int meg_arr = strlen(recive_table);
	char temp_arr[meg_arr + 1];
	for (int i = 0; i < meg_arr + 1; i++)
	{
		temp_arr[i] = recive_table[i];
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
			if (apote[i] != time_arr[i])
			{
				time_changed = true;
			}
		}
		for (int i = 0; i < meg; i++)
		{
			time_arr[i] = apote[i];
		}
	}
}

void assignval3(const char *find) //gia na pernei thn wra apo to html get //
{
	int ther = -999;
	int i = -1;
	int meg_str = strlen(find);
	int meg_arr = strlen(recive_table);
	char temp_arr[meg_arr + 1];
	for (int i = 0; i < meg_arr + 1; i++)
	{
		temp_arr[i] = recive_table[i];
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
		if (ther != -999)
		{
			if ((ther <= 60) && (ther >= -10))
			{
				work_temp = ther;
			}
			else
			{
				work_temp = -999;
			}
		}
	}
	//Serial.println(ther);
}

//**serial comunication//





//gia settings klp//
void fan_change()
{
	thermokrasia = take_temp();
	if (thermokrasia != -999)
	{
		if (relay_l_or == true)
		{
			if (thermokrasia >= work_temp)
			{
				set_relay('l', true);
			}
			else
			{
				set_relay('l', false);
			}
		}
		else
		{
			set_relay('l', false);
		}
		if (relay_r_or == true)
		{
			if (thermokrasia >= work_temp)
			{
				set_relay('r', true);
			}
			else
			{
				set_relay('r', false);
			}
		}
		else
		{
			set_relay('r', false);
		}
	}
	else
	{
		set_relay('r', false);
		set_relay('l', false);
	}
}

void change_setings()
{
	if (serial_recive() == true)
	{
		check_com();
		if (has_com == true)
		{
			take_setings();
		}
	}
}

void fill_send_array()
{
	delay(100);
	hum_soil = take_soil_humidity();
	delay(100);
	hum = take_hum();
	delay(100);
	thermokrasia = take_temp();
	delay(100);
	check_relay_status(); //an olla kala me estitires epistrefei tru h false sta,   relay_r_check , relay_l_check 
	delay(100);
	check_door_status();   //analoga me to an einai anoixtoi kapia porta alazei ta,   door_stat , door_last_open
	delay(100);

	char temp_arr_l_o[10];
	char temp_arr_r_o[10];
	char temp_arr_r[10];
	char temp_arr_l[10];
	char temp_arr_com[10];

	if (relay_r_check == true)
	{
		sprintf(temp_arr_r, true_temp);
	}
	else
	{
		sprintf(temp_arr_r, false_temp);
	}
	if (relay_l_check == true)
	{
		sprintf(temp_arr_l, true_temp);
	}
	else
	{
		sprintf(temp_arr_l, false_temp);
	}
	if (com_error_bool == true)
	{
		sprintf(temp_arr_com, true_temp);
	}
	else
	{
		sprintf(temp_arr_com, false_temp);
	}

	if (relay_r_or == true)
	{
		sprintf(temp_arr_r_o, true_temp);
	}
	else
	{
		sprintf(temp_arr_r_o, false_temp);
	}
	if (relay_l_or == true)
	{
		sprintf(temp_arr_l_o, true_temp);
	}
	else
	{
		sprintf(temp_arr_l_o, false_temp);
	}


	sprintf(send_char, send_str, temp_arr_l, temp_arr_r, door_stat, door_last_open, thermokrasia, hum, hum_soil, work_temp, temp_arr_com, temp_arr_l_o, temp_arr_r_o);
	Serial.write(send_char);
}

void initiate_time_arr()
{
	time_arr[0] = 'e';
	time_arr[1] = 'r';
	time_arr[2] = 'r';
	time_arr[3] = 'o';
	time_arr[4] = 'r';
	time_arr[5] = '\0';

	door_last_open[0] = 'o';
	door_last_open[1] = 'k';
	door_last_open[2] = '\0';
}

void has_com_check()
{
	if (has_com == false)
	{
		has_com_metr++;
		mega_reset_metr++;


	}
	else if (has_com == true)
	{
		has_com_metr = 0;
		mega_reset_metr = 0;
		com_error_bool = false;
		reset_mega = false;
	}
	if (has_com_metr > has_com_error_num)
	{
		com_error_bool = true;
	}
	if (reset_mega == false)
	{
		if (mega_reset_metr > reset_mega_num)
		{
			reset_mega = true;
			delay(1000);
			digitalWrite(Relay_mega_reset_pin, LOW);
			delay(1000);
			digitalWrite(Relay_mega_reset_pin, HIGH);
			delay(1000);
		}
	}
}
//**gia settings klp//

void Serial_flush()
{
	int i = 0;
	if (Serial.available()) {
		delay(100); //allows all serial sent to be received together
		while (Serial.available() && i<200)
		{
			i++;
			Serial.read();
		}
	}
}


void setup()
{
	initiate_time_arr();                 //initiate settings vars
	has_com_metr = 0;
	mega_reset_metr = 0;
	reset_mega = false;
	com_error_bool = false;
	work_temp = start_work_temp;
	relay_r_pos = start_relay_r;
	relay_l_pos = start_relay_l;
	Serial.begin(4800);
	pinMode(Relay_r_pin, OUTPUT);		//initiate inputs outputs
	pinMode(door_sensor_pin, INPUT);
	pinMode(Relay_l_pin, OUTPUT);
	pinMode(Relay_mega_reset_pin, OUTPUT);
	digitalWrite(Relay_mega_reset_pin, HIGH);
	Serial_flush();					//diagrafei to serial buss
	fan_change();                       //anoigei tous anemistires sthn arxh an einai panw apo to start_work_temp h thermokrasia
	int met = 0;
	while ((!Serial.available()) && (met < 1500))  //perimenei 2.5 letpa peripou gia recive 
	{
		delay(100);
		met++;
	}
	change_setings();					 //lamvanei kai alazei ths rithiseis simfona me ayta pou eixe lavei
	has_com_check();
	if (time_changed == false)
	{
		change_setings();					 //lamvanei kai alazei ths rithiseis simfona me ayta pou eixe lavei
		has_com_check();
	}
	delay(1000);
	fill_send_array();					//gemizei to send_array kai stelnei ta dedomena
}


int count_for_wlile_1 = 0;

void loop()

{
	change_setings();					 //lamvanei kai alazei ths rithiseis simfona me ayta pou eixe lavei
	has_com_check();
	if (time_changed == false)


	count_for_wlile_1 = 0;
	fan_change();					//alazei tous anemistires simfona me ths rithmiseis kai shmfona me thn thermokrasia
	while (count_for_wlile_1 < 15)		//120 enai PERIPOU ena lepto giati exei ta delay mesa
	{
		delay(150);
		check_door_status();			//elenxei to statous tou door kai alazei ths times an vrethei kapia alagh
		delay(300);
		count_for_wlile_1++;
	}
	fill_send_array();					//gemizei to send_array kai stelnei ta dedomena
}
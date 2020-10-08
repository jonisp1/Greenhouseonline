#include <dht11.h>

//gia RF//
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
//**gia RF//



//gia comunication//
#define true_temp ("true")
#define false_temp ("false")
//**gia comunication//

//gia send (comunication)//
#define send_str ("*r=%i;ds=%i;t=%i;h=%i;he=%i;&\0")
//**gia send (comunication)//

//metavlites gia initiate//
#define start_relay false;
int has_com_error_num = 200;
//**metavlites gia initiate//


// pins ktlp gia sensors//
dht11 temp_sensor;
#define temp_sensor_pin 6
#define Relay_pin 4
#define door_sensor_pin 5
// ***pins ktlp gia sensors//



//metavlhtes apostolh k fancions//
boolean relay_pos;
int door_stat;
int hum;
int thermokrasia;
boolean com_error_bool;
int has_com_metr;
//**metavlhtes apostolh k fancions//


//serial vars//
char recive_table[100];
char send_table[100];
boolean has_com;
boolean has_new_com;
//**serial vars//



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
	int ret = -99;
	delay(100);
	if (check_temp_sensor() == 1)
	{
		ret = (int)temp_sensor.temperature, 2;
	}
	return ret;
}

int take_hum()// h epistrefei igrasia h -999 gia sfalma 
{
	int ret = -99;
	delay(100);
	if (check_temp_sensor() == 1)
	{
		ret = (int)temp_sensor.humidity, 2;
	}
	return ret;
}
//temp sensor//



// relay //
void set_relay(boolean on_off) //boolean on_off true gia on false gia off
{
	if (on_off == true)
	{
		digitalWrite(Relay_pin, HIGH);
		relay_pos = true;
	}
	else
	{
		digitalWrite(Relay_pin, LOW);
		relay_pos = false;
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
	int temp_door = check_door();
	if (temp_door != -1)
	{
		if (temp_door == 1)
		{
			door_stat = temp_door;
		}
	}
}
//**door sensor//



//Rf comunication//
boolean rf_recive()
{
	nRF905_receive();
	delay(1000);
	packet_s packet;
	has_new_com = false;
	bool ret = false;
	if (getPacket(&packet))
	{
		ret = true;
		for (int i = 0; i < packet.len; i++)
		{
			recive_table[i] = packet.data[i];
			delay(1);
		}
		recive_table[packet.len] = '\0';
		has_new_com = true;
		Serial.println(recive_table);
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
		packet.data[i] = send_table[i];
	}
	sendPacket(&packet);
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
	Serial.println(recive_table);
	if ((check_ifex(recive_table, "*") == true) && (check_ifex(recive_table, "&") == true))
	{
		has_com = true;
	}
}

void take_setings()
{
	int relay_temp = -1;
	relay_temp = assignval1("r=");
	delay(1000);
	if (relay_temp != -1)
	{
		if (relay_temp == 1)
			relay_pos = true;
		if (relay_temp == 0)
			relay_pos = false;
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
		if ((check_ifex(apote, "t")) == true)
		{
			i = 1;
		}
		if ((check_ifex(apote, "f")) == true)
		{
			i = 0;
		}
	}
	return i;
}

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



//gia settings klp//
void pump_change()
{
	if (relay_pos == true)
	{
		set_relay(true);
	}
	else
	{
		set_relay(false);
	}
}

void change_setings()
{
	if (has_new_com == true)
	{
		Serial.println(recive_table);
		check_com();
		if (has_com == true)
		{
			take_setings();
		}
		pump_change();
	}
}

void fill_send_array()
{
	delay(100);
	hum = take_hum();
	delay(100);
	thermokrasia = take_temp();
	delay(100);
	check_door_status();   //analoga me to an einai anoixtoi kapia porta alazei ta,   door_stat , door_last_open
	delay(100);
	int com_err = -1;
	int relay_s = -1;
	if (com_error_bool == true)
	{
		com_err = 1;
	}
	else
	{
		com_err = 0;
	}
	if (relay_pos == true)
	{
		relay_s = 1;
	}
	else
	{
		relay_s = 0;
	}
	sprintf(send_table, send_str, relay_s, door_stat, thermokrasia, hum, com_err);
	delay(10);
	Serial.println(send_table);
	for (int i = 0; i < 50; i++)
	{
		rf_send();
		delay(10);
	}
}

void has_com_check()
{
	if (has_new_com == false)
	{
		has_com_metr++;
	}
	else if (has_new_com == true)
	{
		has_com_metr = 0;
		com_error_bool = false;
	}
	if (has_com_metr > has_com_error_num)
	{
		com_error_bool = true;
		relay_pos = false;
		pump_change();
	}
	if (has_com_metr > 9000)
	{
		has_com_metr = has_com_error_num + 1;
	}
}
//**gia settings klp//




void setup()
{
	initiate_rf();
	has_com_metr = 0;
	com_error_bool = false;
	relay_pos = start_relay;
	Serial.begin(9600);
	pinMode(Relay_pin, OUTPUT);			//initiate inputs outputs
	pinMode(door_sensor_pin, INPUT);
	pump_change();
}

void loop()
{
	bool check = false;
	delay(100);
	check_door_status();			//elenxei to statous tou door kai alazei ths times an vrethei kapia alagh
	delay(100);
	Serial.print("has_new_com: ");
	Serial.println(has_new_com);
	if (rf_recive() == true)
	{
		check = true;
		check_com();
		change_setings();
	}
	has_com_check();
	fill_send_array(); // 50 send me 10ms delay
	if (check == true)
	{
		door_stat = 0;
	}
}
#include <EEPROM.h>
#include <Adafruit_NeoPixel.h>
#include <EasyStringStream.h>
#include <GyverBME280.h>
GyverBME280 bme;
#include <DS1307ESP.h>
DS1307ESP rtc;
#include <SoftwareSerial.h>
#include "MHZ19.h"
#define RXD1 18  // Пин подключения RX sim800
#define TXD1 19  // Пин подключения TX sim800
#define RXD2 16  // Пин подключения RX Dwin
#define TXD2 17  // Пин подключения TX Dwin
const int COpin = 34;
int COvalue = 0;

int eepromTel1 = 0, eeprom_sound = 20, eeprom_alarm_1 = 21, eeprom_alarm_2 = 22, eeprom_hour = 23, eeprom_min = 25, eeprom_hour2 = 27, eeprom_min2 = 29, eepromTel2 = 40, eepromTel3 = 60, eepromTel4 = 80, eepromTel5 = 100, eepromTel6 = 120;
int eeprom_CO_alarm = 32, eeprom_temp_h = 33, eeprom_temp_l = 34;
int eeprom_temp_h_value = 145, eeprom_temp_l_value = 140;


int change_data[43] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
int M_CO[9];    //={0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; // Среднее для СО
int i777 = -1;  // Счетчик СО
//------------------ALARM----------------------
int freq = 0;  // Частота пьезо динамика
int channel = 0;
int resolution = 8;  // = 8;
int alarm_hour;
int alarm_min;
int alarm_hour2;
int alarm_min2;
int guard_time_v;

int temp_h_value;
int temp_l_value;

bool alarm_flag;
bool alarm_flag2;
bool alarm_sound = false;

bool night_mode_flag = false;
bool night_mode_flag_2 = false;

bool guard_flag_1 = false;
bool guard_flag_5 = false;
bool guard_flag_30 = false;
bool guard_flag_60 = false;
bool guard_up = false;
bool pir_icon_flag = false;


bool CO_alarm = true;
bool CO_alarm_sound = true;
bool temp_h_alarm = false;
bool temp_l_alarm = false;

bool sound;

int guard_1 = 5;
int guard_5 = 300;
int guard_30 = 1800;
int guard_60 = 3600;


int alarm_v;

byte alarm_period[] = { 0x5A, 0xA5, 0x05, 0x82, 0x00, 0xA0, 0x00, alarm_v };

byte alarm_off[] = { 0x5A, 0xA5, 0x05, 0x82, 0x50, 0x28, 0x00, 0x00 };   // Отправка состояния флага в дисплей(контроль)
byte alarm_on[] = { 0x5A, 0xA5, 0x05, 0x82, 0x50, 0x28, 0x00, 0x01 };    // Отправка состояния флага в дисплей(контроль)
byte alarm_off2[] = { 0x5A, 0xA5, 0x05, 0x82, 0x50, 0x30, 0x00, 0x00 };  // Отправка состояния флага в дисплей(контроль)
byte alarm_on2[] = { 0x5A, 0xA5, 0x05, 0x82, 0x50, 0x30, 0x00, 0x01 };   // Отправка состояния флага в дисплей(контроль)

byte CO_alarm_on[] = { 0x5A, 0xA5, 0x05, 0x82, 0x50, 0x36, 0x00, 0x01 };   // Отправка состояния флага в дисплей(контроль)
byte CO_alarm_off[] = { 0x5A, 0xA5, 0x05, 0x82, 0x50, 0x36, 0x00, 0x00 };  // Отправка состояния флага в дисплей(контроль)

byte temp_h_on[] = { 0x5A, 0xA5, 0x05, 0x82, 0x50, 0x38, 0x00, 0x01 };   // Отправка состояния флага в дисплей(контроль)
byte temp_h_off[] = { 0x5A, 0xA5, 0x05, 0x82, 0x50, 0x38, 0x00, 0x00 };  // Отправка состояния флага в дисплей(контроль)

byte temp_l_on[] = { 0x5A, 0xA5, 0x05, 0x82, 0x50, 0x40, 0x00, 0x01 };   // Отправка состояния флага в дисплей(контроль)
byte temp_l_off[] = { 0x5A, 0xA5, 0x05, 0x82, 0x50, 0x40, 0x00, 0x00 };  // Отправка состояния флага в дисплей(контроль)


byte alarm_icon_off[] = { 0x5A, 0xA5, 0x05, 0x82, 0x50, 0x14, 0x00, 0x00 };  // Иконка будильника на главной OFF
byte alarm_icon_on[] = { 0x5A, 0xA5, 0x05, 0x82, 0x50, 0x14, 0x00, 0x01 };   // Иконка будильника на главной ON

byte CO_icon_off[] = { 0x5A, 0xA5, 0x05, 0x82, 0x59, 0x93, 0x00, 0x01 };  // 5A A5 05 82 59 93 00 00 Череп
byte CO_icon_on[] = { 0x5A, 0xA5, 0x05, 0x82, 0x59, 0x93, 0x00, 0x00 };   //

byte icon__logo_off[] = { 0x5A, 0xA5, 0x05, 0x82, 0x59, 0x94, 0x00, 0x01 };  // // 5994 Иконка лого при загрузке СО 5A A5 05 82 59 94 00 00/ON 01/OFF
byte icon__logo_on[] = { 0x5A, 0xA5, 0x05, 0x82, 0x59, 0x94, 0x00, 0x00 };

byte night_mode_on[] = { 0x5A, 0xA5, 0x05, 0x82, 0x50, 0x70, 0x00, 0x01 };   // Флаг вкл ночной режим
byte night_mode_off[] = { 0x5A, 0xA5, 0x05, 0x82, 0x50, 0x70, 0x00, 0x00 };  // Флаг вкл ночной режим

byte night_mode_bright_off[] = { 0x5A, 0xA5, 0x07, 0x82, 0x00, 0x82, 0x01, 0x00 };           // Снизить яркость до 1%
byte night_mode_bright_on[] = { 0x5A, 0xA5, 0x07, 0x82, 0x00, 0x82, 0x64, 0x00 };            // Вернуть яркость до 100%
byte night_mode_list_23[] = { 0x5A, 0xA5, 0x07, 0x82, 0x00, 0x84, 0x5A, 0x01, 0x00, 0x17 };  // Отобразить страницу ночного режима
byte night_mode_list_0[] = { 0x5A, 0xA5, 0x07, 0x82, 0x00, 0x84, 0x5A, 0x01, 0x00, 0x00 };   // Отобразить главную
byte alarm_list_6[] = { 0x5A, 0xA5, 0x07, 0x82, 0x00, 0x84, 0x5A, 0x01, 0x00, 0x06 };        // Отобразить cтраницу будильника

byte sound_icon_on[] = { 0x5A, 0xA5, 0x05, 0x82, 0x59, 0x92, 0x00, 0x00 };   // Иконка звука на главной ON
byte sound_icon_off[] = { 0x5A, 0xA5, 0x05, 0x82, 0x59, 0x92, 0x00, 0x01 };  // Иконка звука на главной OFF

byte buz_on[] = { 0x5A, 0xA5, 0x07, 0x82, 0x00, 0x80, 0x5A, 0x00, 0x00, 0x38 };   // Включение звука
byte buz_off[] = { 0x5A, 0xA5, 0x07, 0x82, 0x00, 0x80, 0x5A, 0x00, 0x00, 0x30 };  // Выключение звука

byte guard_icon_on[] = { 0x5A, 0xA5, 0x05, 0x82, 0x50, 0x16, 0x00, 0x01 };   // 5016 Иконка "Будильник режим охраны" - ON
byte guard_icon_off[] = { 0x5A, 0xA5, 0x05, 0x82, 0x50, 0x16, 0x00, 0x00 };  // 5016 Иконка "Будильник режим охраны"  OFF

byte guard_time[] = { 0x5A, 0xA5, 0x05, 0x82, 0x50, 0x74, 0x00, guard_time_v };  // Вывод таймеров постановки на охрану
byte guard_time_0[] = { 0x5A, 0xA5, 0x05, 0x82, 0x50, 0x74, 0x00, 0x00 };        // Сброс таймера отправка 0


int CO_v;
byte CO[] = { 0x5A, 0xA5, 0x05, 0x82, 0x50, 0x78, 0x00, CO_v };  // Отправка показаний датчика CO в порт


//---------------Глобальные-даты-время---------
// rtc.DSadjust(0, 19, 21, 2022, 3, 16); // 00:19:21 16 Mar 2022
int rtc_hour;
int rtc_min;
int rtc_sec;
int rtc_dd;
int rtc_mm;
int rtc_yy;
//--------------END-Глобальные-даты-время------
//--------------END-ALARM----------------------
//---------------LED---------------------------
#define PIXEL_PIN 15
bool flag_led = true;
//byte flag_led_e;
byte led_on[] = { 0x5A, 0xA5, 0x05, 0x82, 0x50, 0x32, 0x00, 0x01 };   // Иконка будильника на главной OFF
byte led_off[] = { 0x5A, 0xA5, 0x05, 0x82, 0x50, 0x32, 0x00, 0x00 };  // Иконка будильника на главной ON
//--------------END-LED------------------------
SoftwareSerial co2Serial;
const int rx_pin = 25;  //Пин подключения RX co2 MHZ19
const int tx_pin = 26;  //Пин подключения TX co2 MHZ19
//------------Таймеры---------------------------
uint32_t Timer1, Timer2, Timer3, Timer4, Timer5, Timer6, Timer7, Timer8;  // Таймеры функций
const int period_alarm = 1200;                                            // Проверка будильника
const int period_time = 1000;                                             // Интервал чтения времени и отправки в дисплей
const int period_bme = 3500;                                              // Интервал чтения с датчика BME280 и отправки в дисплей
const int period_co2 = 6300;                                              // Интервал чтения с датчика MHZ19 и отправки в дисплей
const int period_1_min = 60000;                                           // Интервал проверки флагов будильников из EEPROM
const int period_2_min = 120000;                                          // Интервал 2 min
const int period_5_min = 300000;                                          // Интервал 5 min
const int period_120_sec = 120000;                                        // Интервал 120 sec
//--------------END-----------------------------
long ppm;                // Переменная для отправки данных с датчика co2 в дисплей и отправки в дисплей
float CYear;             // Переменная для расчета календаря год
int CMonth;              // Переменная для расчета календаря месяц
int CDay;                // Переменная для расчета календаря  день месяца
#include "iCalendar.h";  // Подключение функции расчета и отправки календаря
//long long buf_dwin[50];  // Массив для функции обработки нажатий в дисплее
char test[12];  // Буфер преобразования int в String
String Tel1, Tel2, Tel3, Tel4, Tel5, Tel6;

EasyStringStream stack(test, 13);               // Длина строки с учетом 0го символа для +71112223344
byte Buffer_Len = 0;                            // Длина массива для функции обработки нажатий в дисплее
bool flag = false;                              // Флаг старта обработки для функции обработки нажатий в дисплее
long long buf_dwin[50];                         // Массив для функции обработки нажатий в дисплее
MHZ19 *mhz19_uart = new MHZ19(rx_pin, tx_pin);  // Пины для датчика CO2
// Массивы для отправки в дисплей

byte dwin_CO2[] = { 0x5A, 0xA5, 0x05, 0x82, 0x50, 0x00, 0x00, 0x00 };       // Значение co2
byte dwin_temp[] = { 0x5A, 0xA5, 0x05, 0x82, 0X51, 0x00, 0x00, 0x00 };      // Значение температуры с датчика BME280
byte dwin_hpa[] = { 0x5A, 0xA5, 0x05, 0x82, 0X52, 0x00, 0x00, 0x00 };       // Значение атмосферного давления с датчика BME280
byte dwin_hum[] = { 0x5A, 0xA5, 0x05, 0x82, 0X53, 0x00, 0x00, 0x00 };       // Значение  влажности с датчика BME280
byte dwin_day[] = { 0x5A, 0xA5, 0x05, 0x82, 0X57, 0x00, 0x00, 0x00 };       // Расчетное значение день месяца
byte dwin_month[] = { 0x5A, 0xA5, 0x05, 0x82, 0X58, 0x00, 0x00, 0x00 };     // Расчетное значение номера месяца
byte dwin_day_week[] = { 0x5A, 0xA5, 0x05, 0x82, 0X64, 0x50, 0x00, 0x00 };  // Расчетное значение день недели
byte dwin_year[] = { 0x5A, 0xA5, 0x05, 0x82, 0X59, 0x00, 0x00, 0x00 };      // Расчетное значение год
byte dwin_year_rtc[] = { 0x5A, 0xA5, 0x05, 0x82, 0X59, 0x10, 0x00, 0x00 };  // Значение год с RTC
byte dwin_hour[] = { 0x5A, 0xA5, 0x05, 0x82, 0x54, 0x00, 0x00, 0x00 };      // Расчетное значение час
byte dwin_min[] = { 0x5A, 0xA5, 0x05, 0x82, 0x55, 0x00, 0x00, 0x00 };       // Расчетное значение минута
byte dwin_sec[] = { 0x5A, 0xA5, 0x05, 0x82, 0x56, 0x00, 0x00, 0x00 };       // Расчетное значение секунда


byte temp_h_value_send[] = { 0x5A, 0xA5, 0x05, 0x82, 0X50, 0x42, 0x00, 0x00 };  // Отптравка значение оповещения если тепм больше
byte temp_l_value_send[] = { 0x5A, 0xA5, 0x05, 0x82, 0X50, 0x44, 0x00, 0x00 };  // Отптравка значение оповещения если тепм меньше

//char string_Tel1[] = "";  // String c телефоном в дисплей

int dwin_year_rtc_v;
int dwin_CO2_v, dwin_temp_v, dwin_hpa_v, dwin_hum_v, dwin_day_v, dwin_month_v, dwin_year_v, dwin_hour_v, dwin_min_v, dwin_sec_v, dwin_week_v;  // переменные для передачи значений в дисплей
//------------------PIR------------------------
const int timeSeconds = 10;   // количество секунд, в течение которых LED will try after motion detected.
const int motionSensor = 23;  // Пин подключения датчика движения
unsigned long now = millis();
unsigned long lastTrigger = 0;
bool startTimer = false;
int dwin_mdetected_v;
byte dwin_mdetected[] = { 0x5A, 0xA5, 0x05, 0x82, 0x50, 0x10, 0x00, 0x00 };

void IRAM_ATTR detectsMovement() {  // IRAM_ATTR Запуск кода прерывания в памяти RAM, для ускорения работы
  startTimer = true;
  lastTrigger = millis();
  pir_icon_flag = true;
}
//-------------PIR-end--------------------------

Adafruit_NeoPixel strip = Adafruit_NeoPixel(8, PIXEL_PIN, NEO_GRB + NEO_KHZ800);

//=======================================================================================================================
//============================================================SETUP======================================================

void setup() {

  Serial.begin(115200);
  Serial1.begin(115200, SERIAL_8N1, RXD1, TXD1);                      // Инициализация порта подключения модуля SIM800
  Serial2.begin(115200, SERIAL_8N1, RXD2, TXD2);                      // Инициализация порта подключения дисплея
  co2Serial.begin(115200, SWSERIAL_8N1, rx_pin, tx_pin, false, 128);  // Инициализация порта подключения датчика CO2


  //----------------ALARM--ЗВУК------------------
  ledcSetup(channel, freq, resolution);
  ledcAttachPin(13, channel);
  //--------------END-ALARM----------------------

  //----------------LED-STRIP--------------------
  strip.begin();
  strip.show();  // Initialize all pixels to 'off'
  //strip.setBrightness(50);
  //------------END-LED-STRIP--------------------
  //------PIR---------
  pinMode(motionSensor, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(motionSensor), detectsMovement, RISING);
  //pinMode(led, OUTPUT);
  //digitalWrite(led, LOW);
  //----------PIR-end-------------
  mhz19_uart->begin(rx_pin, tx_pin);      // Инициализация датчика CO2
  mhz19_uart->setAutoCalibration(false);  // Отключение ежесуточной автокалибровки датчика CO2
  delay(3000);
  Serial.print("MH-Z19 now warming up...  status:");
  Serial.println(mhz19_uart->getStatus());
  delay(1000);
  rtc.begin();  // Инициализация модуля RTC
  delay(1000);
  bme.begin();  // Инициализация датчика BME280
  delay(1000);
  //---------------EEPROM----------------------
  /* Begin with EEPROM by deciding how much EEPROM memory you want to use.
    The ESP32's maximum EEPROM size is 4096 bytes (4 KB), but we're just using 512 bytes here.
  */
  EEPROM.begin(256);
  delay(500);

  Tel1 = EEPROM.readString(eepromTel1);
  Tel2 = EEPROM.readString(eepromTel2);
  Tel3 = EEPROM.readString(eepromTel3);
  Tel4 = EEPROM.readString(eepromTel4);
  Tel5 = EEPROM.readString(eepromTel5);
  Tel6 = EEPROM.readString(eepromTel6);
  sound = EEPROM.read(eeprom_sound);
  alarm_flag = EEPROM.read(eeprom_alarm_1);
  alarm_flag2 = EEPROM.read(eeprom_alarm_2);
  alarm_hour = EEPROM.read(eeprom_hour);
  alarm_hour2 = EEPROM.read(eeprom_hour2);
  alarm_min = EEPROM.read(eeprom_min);
  alarm_min2 = EEPROM.read(eeprom_min2);
  //CO_alarm = EEPROM.read(eeprom_CO_alarm);
  temp_h_alarm = EEPROM.read(eeprom_temp_h);                     // Состояние флага если темп больше
  temp_l_alarm = EEPROM.read(eeprom_temp_l);                     // Состояние флага если темп меньше
  temp_h_value = EEPROM.get(eeprom_temp_h_value, temp_h_value);  // Значение температуры если темп больше
  temp_l_value = EEPROM.get(eeprom_temp_l_value, temp_l_value);  // Значение температуры если темп меньше

  byte alarm_hour_send[] = { 0x5A, 0xA5, 0x05, 0x82, 0x50, 0x21, 0x00, alarm_hour };
  byte alarm_min_send[] = { 0x5A, 0xA5, 0x05, 0x82, 0x50, 0x22, 0x00, alarm_min };
  byte alarm_hour2_send[] = { 0x5A, 0xA5, 0x05, 0x82, 0x50, 0x24, 0x00, alarm_hour2 };
  byte alarm_min2_send[] = { 0x5A, 0xA5, 0x05, 0x82, 0x50, 0x26, 0x00, alarm_min2 };


  temp_h_value_send[6] = highByte(temp_h_value);  // Отправка данных с EEprom об установленной температуре оповещений
  temp_h_value_send[7] = lowByte(temp_h_value);
  Serial2.write(temp_h_value_send, 8);

  temp_l_value_send[6] = highByte(temp_l_value);  // Отправка данных с EEprom об установленной температуре оповещений
  temp_l_value_send[7] = lowByte(temp_l_value);
  Serial2.write(temp_l_value_send, 8);


  Serial2.write(alarm_hour_send, 8);
  Serial2.write(alarm_hour2_send, 8);
  Serial2.write(alarm_min_send, 8);
  Serial2.write(alarm_min2_send, 8);
  Serial2.write(CO_icon_off, 8);


  //=====================================Проверка флагов Будильника и установка иконок в соответствие=============================
  if (alarm_flag == 0) {
    Serial2.write(alarm_off, 8);
  }
  if (alarm_flag2 == 0) {
    Serial2.write(alarm_off2, 8);
  }
  if (alarm_flag) {
    Serial2.write(alarm_on, 8);
  }
  if (alarm_flag2) {
    Serial2.write(alarm_on2, 8);
  }

  //=====================================Проверка флагов Оповещения и установка иконок в соответствие=============================

  if (CO_alarm == 1) {
    Serial2.write(CO_alarm_on, 8);
  }
  if (CO_alarm == 0) {
    Serial2.write(CO_alarm_off, 8);
  }

  if (temp_h_alarm == 1) {
    Serial2.write(temp_h_on, 8);
  }
  if (temp_h_alarm == 0) {
    Serial2.write(temp_h_off, 8);
  }

  if (temp_l_alarm == 1) {
    Serial2.write(temp_l_on, 8);
  }
  if (temp_l_alarm == 0) {
    Serial2.write(temp_l_off, 8);
  }

  if (sound == 1) {
    byte buz_on[] = { 0x5A, 0xA5, 0x07, 0x82, 0x00, 0x80, 0x5A, 0x00, 0x00, 0x38 };
    Serial2.write(buz_on, 10);
    Serial2.write(sound_icon_on, 8);
  }
  if (sound == 0) {
    byte buz_off[] = { 0x5A, 0xA5, 0x07, 0x82, 0x00, 0x80, 0x5A, 0x00, 0x00, 0x30 };
    Serial2.write(buz_off, 10);
    Serial2.write(sound_icon_off, 8);
  }

  char Buf_null1[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
  char tt1[] = { 0x5a, 0xa5, 0x0f, 0x82, 0x71, 0x00, 0x2b, 0x37, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 };
  Tel1.toCharArray(Buf_null1, 13);  // 7100 номер телефона сигнализация/владелец
  for (int i = 8; i < 18; i++) {
    tt1[i] = Buf_null1[i - 6];
  }
  Serial2.write(tt1, 18);

  char Buf_null2[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
  char tt2[] = { 0x5a, 0xa5, 0x0f, 0x82, 0x71, 0x10, 0x2b, 0x37, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 };
  Tel2.toCharArray(Buf_null2, 13);  // 7110 номер телефона Пожарная охрана
  for (int i = 8; i < 18; i++) {
    tt2[i] = Buf_null2[i - 6];
  }
  Serial2.write(tt2, 18);

  char Buf_null3[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
  char tt3[] = { 0x5a, 0xa5, 0x0f, 0x82, 0x71, 0x20, 0x2b, 0x37, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 };
  Tel3.toCharArray(Buf_null3, 13);  // 7120 номер телефона Милиция
  for (int i = 8; i < 18; i++) {
    tt3[i] = Buf_null3[i - 6];
  }
  Serial2.write(tt3, 18);

  char Buf_null4[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
  char tt4[] = { 0x5a, 0xa5, 0x0f, 0x82, 0x71, 0x30, 0x2b, 0x37, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 };
  Tel4.toCharArray(Buf_null4, 13);  // 7130 номер телефона Скорая
  for (int i = 8; i < 18; i++) {
    tt4[i] = Buf_null4[i - 6];
  }
  Serial2.write(tt4, 18);

  char Buf_null5[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
  char tt5[] = { 0x5a, 0xa5, 0x0f, 0x82, 0x71, 0x40, 0x2b, 0x37, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 };
  Tel5.toCharArray(Buf_null5, 13);  // 7130 номер телефона Скорая
  for (int i = 8; i < 18; i++) {
    tt5[i] = Buf_null5[i - 6];
  }
  Serial2.write(tt5, 18);

  char Buf_null6[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
  char tt6[] = { 0x5a, 0xa5, 0x0f, 0x82, 0x71, 0x50, 0x2b, 0x37, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 };
  Tel6.toCharArray(Buf_null6, 13);  // 7130 номер телефона Скорая
  for (int i = 8; i < 18; i++) {
    tt6[i] = Buf_null6[i - 6];
  }
  Serial2.write(tt6, 18);


  Serial2.write(guard_icon_off, 8);  // Выключение иконки охраны
  //----------END-EEPROM-----------------------

  if (!bme.begin(0x76)) Serial.println("Error!");  // Проверка доступности датчика BME280
  delay(500);
  rtc.DSread();  // Чтение модуля RTC
  //rtc.DSadjust(20, 37, 21, 2022, 12, 2);
  CYear = float(rtc.year) + 2000;  // Обновление переменных для расчета календаря с установленным временем в RTC
  //CYear = ((float)rtc.year);  // = 2022.00 Обновление переменных для расчета календаря с установленным временем в RTC
  //CYear = (rtc.year);  // дебаг без конвертации
  //rtc.DSadjust(11, 30, 30, 2022, 12, 13);  // 00:19:21 16 Mar 2022
  // rtc.DSadjust(1647216003); // 00:00:03 14 Mar 2022 UNIX time
  CMonth = (rtc.month);
  CDay = (rtc.dayOfMonth);
  //(rtc.year)(rtc.month)(rtc.dayOfMonth)(rtc.hour)(rtc.minute)(rtc.second)
  int rtc_hour = (rtc.hour);
  int rtc_min = (rtc.minute);
  int rtc_sec = (rtc.second);
  int rtc_dd = (rtc.dayOfMonth);
  int rtc_mm = (rtc.month);
  int rtc_yy = (rtc.year + 2000);

  weekUPD();

  dateReset();  // Отправляем текущую дату в дисплей
  time_load();  // Отправляем текущее время в дисплей ЧЧ:ММ
  Calendar();   // Рассчитываем календарь и отправляем в дисплей
  led_rso();
  delay(4000);
  Serial2.write(icon__logo_off, 8);
  led_cls();

}
//=======================================================================================================================
//------------------------------------------------------------SETUP=END--------------------------------------------------
//=======================================================================================================================

#include "parser.h";

//=======================================================================================================================
//------------------------------------------------------------LOOP-------------------------------------------------------
//=======================================================================================================================
void loop() {

  //detectsMovement();
  now = millis();
  if (startTimer && (now - lastTrigger > (timeSeconds * 1000))) {
    if (guard_up == 1) {
      guard_up = false;
      sendsms();
      Serial2.write(guard_icon_off, 8);
      led_cls();
    }
    Serial.print("Motion stopped...: ");
    Serial.println(rtc.getTime());
    dwin_mdetected_v = 0;
    dwin_mdetected[6] = highByte(dwin_mdetected_v);
    dwin_mdetected[7] = lowByte(dwin_mdetected_v);
    Serial2.write(dwin_mdetected, 8);
    startTimer = false;
    pir_icon_flag = false;
  }
  //forwardSerial();                        // Обмен данными с модулем Sim800

  dwin_parsing();                         // Ожидание обработки данных от дисплея
  if (millis() - Timer3 >= period_co2) {  // Таймер отправки данных с датчика со2 в дисплей
    Timer3 = millis();
    co2UPD();
  }
  if (millis() - Timer1 >= period_time) {  // Таймер отправки данных с датчика RTC в дисплей
    timeUPD();
    night_mode();
    guard();
    void_temp_alarm();
    void_CO_alarm();
    Timer1 = millis();
    if (pir_icon_flag) {
      pir_icon();
    }
  }

  if (millis() >= 300000 && (millis() - Timer7 >= period_time)) {  // Таймер отправки данных с датчика RTC в дисплей
    i777++;
    M_CO[i777] = analogRead(COpin);
    if (i777 == 9) {
      COvalue = (M_CO[0] + M_CO[1] + M_CO[2] + M_CO[3] + M_CO[4] + M_CO[5] + M_CO[6] + M_CO[7] + M_CO[8] + M_CO[9]) / 10;
      i777 = 0;
      COUPD();
    }
    //Serial.println(COvalue);
    Timer7 = millis();
  }


  if (millis() - Timer5 >= period_1_min) {  // Таймер контроля изменений флагов будильника
    Timer5 = millis();
    alarm_flag = EEPROM.read(eeprom_alarm_1);
    alarm_flag2 = EEPROM.read(eeprom_alarm_2);


    if (alarm_flag == 0) {
      //alarm_off[] = { 0x5A, 0xA5, 0x05, 0x82, 0x50, 0x28, 0x00, 0x01 };
      Serial2.write(alarm_off, 8);
    }
    if (alarm_flag2 == 0) {
      //alarm_off2[] = { 0x5A, 0xA5, 0x05, 0x82, 0x50, 0x30, 0x00, 0x01 };
      Serial2.write(alarm_off2, 8);
    }
    if (alarm_flag) {
      //alarm_on[] = { 0x5A, 0xA5, 0x05, 0x82, 0x50, 0x28, 0x00, 0x01 };
      Serial2.write(alarm_on, 8);
    }
    if (alarm_flag2) {
      //alarm_on2[] = { 0x5A, 0xA5, 0x05, 0x82, 0x50, 0x30, 0x00, 0x01 };
      Serial2.write(alarm_on2, 8);
    }
  }


  if (millis() - Timer2 >= period_bme) {  // Таймер отправки данных с датчика BME280 в дисплей
    Timer2 = millis();
    bme280UPD();
  }
  if (millis() - Timer4 >= period_alarm) {  // Таймер проверки будильника
    Timer4 = millis();
    if (alarm_flag) {
      alarm();
    }
    if (alarm_flag2) {
      alarm2();
    }
  }
  if (millis() - Timer8 >= period_120_sec) {  // Интервал 1.05 min
    Timer8 = millis();
    alarm_sound = true;
  }
  
  if (alarm_hour == alarm_hour2 && alarm_min == alarm_min2) {
    if (alarm_flag == 1 && alarm_flag2 == 1) {
      alarm_flag = false;
      alarm_flag2 = false;
      EEPROM.write(eeprom_alarm_1, alarm_flag);
      EEPROM.commit();
      delay(10);
      EEPROM.write(eeprom_alarm_2, alarm_flag2);
      EEPROM.commit();
      delay(10);
      Serial2.write(alarm_off2, 8);
      Serial2.write(alarm_off, 8);
    }
  }
}
//=======================================================================================================================
//------------------------------------------------------------LOOP=END---------------------------------------------------
//=======================================================================================================================

//=======================================================================================================================
//----------------------------------------------------------ПОДСВЕТКА----------------------------------------------------
//=======================================================================================================================
void led_cls() {  // off
  for (int ii = 0; ii < 8; ++ii) {
    strip.setPixelColor(ii, 0, 0, 0);
  }
  strip.show();
}

void led_rso() {  //
  strip.setPixelColor(0, 10, 10, 0);
  strip.setPixelColor(1, 10, 10, 0);
  strip.setPixelColor(2, 10, 10, 0);
  strip.setPixelColor(3, 10, 0, 0);
  strip.setPixelColor(4, 10, 0, 0);
  strip.setPixelColor(5, 10, 10, 10);
  strip.setPixelColor(6, 10, 10, 10);
  strip.setPixelColor(7, 10, 10, 10);
  strip.show();
}

void led_R1() {  //
  for (int ii = 0; ii < 8; ++ii) {
    strip.setPixelColor(ii, 30, 0, 0);
  }
  strip.show();
}
void led_R2() {  //
  for (int ii = 0; ii < 8; ++ii) {
    strip.setPixelColor(ii, 0, 10, 0);
  }
  strip.show();
}
void led_R3() {  //
  for (int ii = 0; ii < 8; ++ii) {
    strip.setPixelColor(ii, 0, 0, 10);
  }
  strip.show();
}
void led_R4() {  //
  for (int ii = 0; ii < 8; ++ii) {
    strip.setPixelColor(ii, 10, 10, 0);
  }
  strip.show();
}
void led_R5() {  //
  for (int ii = 0; ii < 8; ++ii) {
    strip.setPixelColor(ii, 10, 0, 15);
  }
  strip.show();
}
void led_blue() {  // Режим по умолчанию при включении
  for (int ii = 0; ii < 8; ++ii) {
    strip.setPixelColor(ii, 0, 0, 30);
  }
  strip.show();
}
void led_green() {
  for (int ii = 0; ii < 8; ++ii) {
    strip.setPixelColor(ii, 0, 30, 0);
  }
  strip.show();
}
//=======================================================================================================================
//-----------------------------------------------------END--ПОДСВЕТКА----------------------------------------------------
//=======================================================================================================================

//=======================================================================================================================
//-------------------------------------------------Отправка данных в дисплей---------------------------------------------
//=======================================================================================================================
void dateReset() {  // сбрасываем значения переменных дня месяца и года, отправляем значения с RTC модуля на дисплей
  rtc.DSread();
  dwin_year_rtc_v = (rtc.year);
  dwin_year[6] = highByte(dwin_year_rtc_v);
  dwin_year[7] = lowByte(dwin_year_rtc_v);
  Serial2.write(dwin_year_rtc, 8);

  dwin_month_v = (rtc.month);
  dwin_month[6] = highByte(dwin_month_v);
  dwin_month[7] = lowByte(dwin_month_v);
  Serial2.write(dwin_month, 8);

  dwin_day_v = (rtc.dayOfMonth);
  dwin_day[6] = highByte(dwin_day_v);
  dwin_day[7] = lowByte(dwin_day_v);
  Serial2.write(dwin_day, 8);
  // Serial2.flush();
  weekUPD();
}
void dateResetCal() {  // сбрасываем значения переменных дня месяца и года, отправляем значения с RTC модуля на дисплей
  rtc.DSread();
  //CYear = (rtc.year);
  CYear = ((float)rtc.year + 2000);
  CMonth = (rtc.month);
  CDay = (rtc.dayOfMonth);
  dwin_year_v = (rtc.year + 2000);
  dwin_year[6] = highByte(dwin_year_v);
  dwin_year[7] = lowByte(dwin_year_v);
  Serial2.write(dwin_year, 8);
  //Serial2.flush();

  dwin_month_v = (rtc.month);
  dwin_month[6] = highByte(dwin_month_v);
  dwin_month[7] = lowByte(dwin_month_v);
  Serial2.write(dwin_month, 8);
  //Serial2.flush();

  dwin_day_v = (rtc.dayOfMonth);
  dwin_day[6] = highByte(dwin_day_v);
  dwin_day[7] = lowByte(dwin_day_v);
  Serial2.write(dwin_day, 8);
  // Serial2.flush();
}
void dateUPDCal() {  // Отправляем значения ДД/ММ/ГГГГ из глобальных переменных в дисплей
  dwin_year_v = (CYear);
  dwin_year[6] = highByte(dwin_year_v);
  dwin_year[7] = lowByte(dwin_year_v);
  Serial2.write(dwin_year, 8);
  // Serial2.flush();
  dwin_month_v = (CMonth);
  dwin_month[6] = highByte(dwin_month_v);
  dwin_month[7] = lowByte(dwin_month_v);
  Serial2.write(dwin_month, 8);
  // Serial2.flush();
  dwin_day_v = (CDay);
  dwin_day[6] = highByte(dwin_day_v);
  dwin_day[7] = lowByte(dwin_day_v);
  Serial2.write(dwin_day, 8);
  // Serial2.flush();
  weekUPD();
}

void weekUPD() {
  dwin_week_v = (rtc.dayOfWeek);
  dwin_day_week[6] = highByte(dwin_week_v);
  dwin_day_week[7] = lowByte(dwin_week_v);
  Serial2.write(dwin_day_week, 8);  // День недели
}

void dateUPD() {  // Отправляем значения ДД/ММ/ГГГГ из глобальных переменных в дисплей
  dwin_year_v = (CYear);
  dwin_year[6] = highByte(dwin_year_v);
  dwin_year[7] = lowByte(dwin_year_v);
  Serial2.write(dwin_year, 8);
  // Serial2.flush();
  dwin_month_v = (CMonth);
  dwin_month[6] = highByte(dwin_month_v);
  dwin_month[7] = lowByte(dwin_month_v);
  Serial2.write(dwin_month, 8);
  // Serial2.flush();
  dwin_day_v = (CDay);
  dwin_day[6] = highByte(dwin_day_v);
  dwin_day[7] = lowByte(dwin_day_v);
  Serial2.write(dwin_day, 8);
  // Serial2.flush();
  weekUPD();
}
void timeUPD() {  // Отправляем М:Ч:С с RTC модуля на дисплей
  rtc.DSread();
  dwin_sec_v = (rtc.second);
  dwin_sec[6] = highByte(dwin_sec_v);
  dwin_sec[7] = lowByte(dwin_sec_v);
  if (rtc.second == 0) {
    dwin_min_v = (rtc.minute);
    dwin_min[6] = highByte(dwin_min_v);
    dwin_min[7] = lowByte(dwin_min_v);
    Serial2.write(dwin_min, 8);
    // Serial2.flush();
    //Serial.println("Minute update");
    if (rtc.second == 0 && rtc.minute == 0) {
      dwin_hour_v = (rtc.hour);
      dwin_hour[6] = highByte(dwin_hour_v);
      dwin_hour[7] = lowByte(dwin_hour_v);
      Serial2.write(dwin_hour, 8);
      // Serial2.flush();
      //Serial.println("Hour update");
    }
  }
  Serial2.write(dwin_sec, 8);
  Serial2.flush();
}

void time_load() {  // Отправляем М:Ч:С при включении
  rtc.DSread();
  dwin_min_v = (rtc.minute);
  dwin_hour_v = (rtc.hour);
  dwin_min[6] = highByte(dwin_min_v);
  dwin_min[7] = lowByte(dwin_min_v);
  dwin_hour[6] = highByte(dwin_hour_v);
  dwin_hour[7] = lowByte(dwin_hour_v);
  Serial2.write(dwin_min, 8);
  Serial2.write(dwin_hour, 8);
}
void pir_icon() {
  dwin_mdetected_v = 1;
  dwin_mdetected[6] = highByte(dwin_mdetected_v);
  dwin_mdetected[7] = lowByte(dwin_mdetected_v);
  Serial2.write(dwin_mdetected, 8);
  Serial2.write(alarm_period, 8);
}
//=======================================================================================================================
//----------------------------------------------------------БУДИЛЬНИКИ---------------------------------------------------
//=======================================================================================================================

void alarm() {  // Будильник 1
  if (alarm_hour == (rtc.hour) && alarm_min == (rtc.minute)) {
    //alarm_sound = true;
    if (alarm_hour == (rtc.hour) && alarm_min == (rtc.minute) + 1) {
      alarm_sound = false;
    }
    if (alarm_flag && alarm_sound) {
      byte beep1[] = { 0x5A, 0xA5, 0x05, 0x82, 0x00, 0xA0, 0x00, 0x0C };
      Serial2.write(beep1, 8);
      if ((rtc.second) < 1) {
        Serial2.write(alarm_list_6, 10);
        Serial2.write(night_mode_bright_on, 8);
        led_green();
      }
      if ((rtc.second) > 58) {
        Serial2.write(night_mode_list_0, 10);
        led_cls();
      }
    }
  }
}

void alarm2() {  // Будильник 1
  if (alarm_hour2 == (rtc.hour) && alarm_min2 == (rtc.minute)) {
    //alarm_sound = true;
    if (alarm_hour2 == (rtc.hour) && alarm_min2 == (rtc.minute) + 1) {
      alarm_sound = false;
    }
    if (alarm_flag2 && alarm_sound) {
      byte beep2[] = { 0x5A, 0xA5, 0x05, 0x82, 0x00, 0xA0, 0x00, 0x0C };
      Serial2.write(beep2, 8);
      if ((rtc.second) < 1) {
        Serial2.write(alarm_list_6, 10);
        Serial2.write(night_mode_bright_on, 8);
        led_green();
      }
      if ((rtc.second) > 58) {
        Serial2.write(night_mode_list_0, 10);
        led_cls();
      }
    }
  }
}

//=======================================================================================================================
//----------------------------------------------------END---БУДИЛЬНИКИ---------------------------------------------------
//=======================================================================================================================


//=======================================================================================================================
//----------------------------------------------------СОБЫТИЯ ОПОВЕЩЕНИЙ-------------------------------------------------
//=======================================================================================================================

void void_CO_alarm() { 
  if (COvalue > 1000) {
    Serial2.write(CO_icon_on, 8);
    led_R1();
    //Serial.println(COvalue);
    if (CO_alarm) {
      sendsms_CO_alarm();
      CO_alarm = false;
      //Serial.println(CO_alarm);
    }
    if (CO_alarm_sound) {
      byte beep3[] = { 0x5A, 0xA5, 0x05, 0x82, 0x00, 0xA0, 0x00, 0x32 };
      Serial2.write(beep3, 8);
    }
  }
}

void void_temp_alarm() {
  if (temp_h_alarm) {
    if (bme.readTemperature() >= temp_h_value) {
      sendsms_temp_alarm();
      temp_h_alarm = false;
      byte beep3[] = { 0x5A, 0xA5, 0x05, 0x82, 0x00, 0xA0, 0x00, 0x32 };
      Serial2.write(beep3, 8);
      Serial2.write(temp_h_off, 8);
    }
  }
  if (temp_l_alarm) {
    if (bme.readTemperature() <= temp_l_value) {
      sendsms_temp_alarm();
      temp_l_alarm = false;
      byte beep3[] = { 0x5A, 0xA5, 0x05, 0x82, 0x00, 0xA0, 0x00, 0x32 };
      Serial2.write(beep3, 8);
      Serial2.write(temp_l_off, 8);
    }
  }
}

//=======================================================================================================================
//-----------------------------------------------END--СОБЫТИЯ ОПОВЕЩЕНИЙ-------------------------------------------------
//=======================================================================================================================

//=======================================================================================================================
//-----------------------------------------------АВТО-Ночной режим-------------------------------------------------------
//=======================================================================================================================
void night_mode() {
  rtc.DSread();
  static int hour_start = 22;
  static int hour_stop = 22;
  if (night_mode_flag) {
    if ((hour_start == (rtc.hour) && (rtc.minute) == 0) && night_mode_flag_2 == false) {
      Serial2.write(night_mode_list_23, 10);
      Serial2.write(buz_off, 10);
      Serial2.write(night_mode_bright_off, 8);
      //Serial2.write(sound_icon_on, 8);
      night_mode_flag_2 = true;
    }
    if ((hour_stop == (rtc.hour) && (rtc.minute) == 1) && night_mode_flag_2 == true) {
      Serial2.write(night_mode_list_0, 10);
      if (sound) {
        Serial2.write(buz_on, 10);
      }
      Serial2.write(night_mode_bright_on, 8);
      //Serial2.write(sound_icon_off, 8);
      night_mode_flag_2 = false;
    }
  }
}
//=======================================================================================================================
//------------------------------------------END--АВТО-Ночной режим-------------------------------------------------------
//=======================================================================================================================

//=======================================================================================================================
//------------------------------------------Постановка-на-охрану---------------------------------------------------------
//=======================================================================================================================
void guard() {
  if (guard_flag_1) {
    //guard_1 = 60;
    guard_1--;
    guard_time_v = guard_1;
    guard_time[6] = highByte(guard_time_v);
    guard_time[7] = lowByte(guard_time_v);
    Serial2.write(guard_time, 8);
    if (guard_1 == 0) {
      guard_flag_1 = false;
      guard_up = true;
      guard_1 = 60;
      Serial2.write(night_mode_list_0, 10);
      Serial2.write(guard_icon_on, 8);
      led_R1();
    }
  }
  if (guard_flag_5) {
    guard_5--;
    guard_time_v = guard_5;
    guard_time[6] = highByte(guard_time_v);
    guard_time[7] = lowByte(guard_time_v);
    Serial2.write(guard_time, 8);
    if (guard_5 == 0) {
      guard_flag_5 = false;
      guard_up = true;
      guard_5 = 300;
      Serial2.write(night_mode_list_0, 10);
      Serial2.write(guard_icon_on, 8);
      led_R1();
    }
  }
  if (guard_flag_30) {
    guard_30--;
    guard_time_v = guard_30;
    guard_time[6] = highByte(guard_time_v);
    guard_time[7] = lowByte(guard_time_v);
    Serial2.write(guard_time, 8);
    if (guard_30 == 0) {
      guard_flag_30 = false;
      guard_up = true;
      guard_30 = 1800;
      Serial2.write(night_mode_list_0, 10);
      Serial2.write(guard_icon_on, 8);
      led_R1();
    }
  }
  if (guard_flag_60) {
    guard_60--;
    guard_time_v = guard_60;
    guard_time[6] = highByte(guard_time_v);
    guard_time[7] = lowByte(guard_time_v);
    Serial2.write(guard_time, 8);
    if (guard_60 == 0) {
      guard_flag_60 = false;
      guard_up = true;
      guard_60 = 3600;
      Serial2.write(night_mode_list_0, 10);
      Serial2.write(guard_icon_on, 8);
      led_R1();
    }
  }
}

//=======================================================================================================================
//-----------------------------------END----Постановка-на-охрану---------------------------------------------------------
//=======================================================================================================================

void bme280UPD() {  // Функция отправки данных на дисплей с датчика bme280
  bme.begin();
  dwin_temp_v = bme.readTemperature();
  dwin_temp[6] = highByte(dwin_temp_v);
  dwin_temp[7] = lowByte(dwin_temp_v);
  Serial2.write(dwin_temp, 8);
  //Serial2.flush();

  dwin_hpa_v = (bme.readPressure() / 133.32);
  dwin_hpa[6] = highByte(dwin_hpa_v);
  dwin_hpa[7] = lowByte(dwin_hpa_v);
  Serial2.write(dwin_hpa, 8);
  //Serial2.flush();

  dwin_hum_v = bme.readHumidity();
  dwin_hum[6] = highByte(dwin_hum_v);
  dwin_hum[7] = lowByte(dwin_hum_v);
  Serial2.write(dwin_hum, 8);
  //Serial2.flush();
}
void co2UPD() {  // Функция отправки данных на дисплей с датчика co2
  measurement_t m = mhz19_uart->getMeasurement();
  ppm = (m.co2_ppm);
  if (ppm < 399) {
    ppm = 400;
  }
  dwin_CO2_v = (ppm);
  dwin_CO2[6] = highByte(dwin_CO2_v);
  dwin_CO2[7] = lowByte(dwin_CO2_v);
  Serial2.write(dwin_CO2, 8);
  //Serial2.flush();
}
void COUPD() {     // Функция отправки данных на дисплей с датчика co
  CO_v = COvalue;  //analogRead(COpin);
  CO[6] = highByte(CO_v);
  CO[7] = lowByte(CO_v);
  Serial2.write(CO, 8);
  //Serial2.flush();
}
void sendsms() {  // Функция отправки SMS
  Serial.println("sms start...");
  String PHONE = Tel1;  // Номер для отправки СМС
  Serial.print("Send to number: ");
  Serial.println(PHONE);
  Serial1.println("AT");
  delay(100);
  Serial1.println("AT+IPR=0");
  delay(100);
  Serial1.println("AT&W");
  delay(100);
  Serial1.println("AT+CNMI=2,2,0,0,0");
  delay(100);
  String text = "ALARM: " + (rtc.getTime());
  Serial1.print("AT+CMGF=1\r");
  delay(100);
  Serial1.print("AT+CMGS=\"" + PHONE + "\"\r");
  delay(100);
  Serial1.print(text);
  delay(100);
  Serial1.write(0x1A);  //ascii код ctrl-26
  delay(100);
  Serial.println(F("sms send done!"));
}

void sendsms_CO_alarm() {  // Функция отправки SMS по превышению уровня угарного газа
  Serial.println("sms start...");
  String PHONE = Tel1;  // Номер для отправки СМС
  Serial.print("Send to number: ");
  Serial.println(PHONE);
  Serial1.println("AT");
  delay(100);
  Serial1.println("AT+IPR=0");
  delay(100);
  Serial1.println("AT&W");
  delay(100);
  Serial1.println("AT+CNMI=2,2,0,0,0");
  delay(100);
  String text = "ALARM CO!!! : " + (rtc.getTime()) + temp_h_value;
  Serial1.print("AT+CMGF=1\r");
  delay(100);
  Serial1.print("AT+CMGS=\"" + PHONE + "\"\r");
  delay(100);
  Serial1.print(text);
  delay(100);
  Serial1.write(0x1A);  //ascii код ctrl-26
  delay(100);
  Serial.println(F("sms send done!"));
}

void sendsms_temp_alarm() {  // Функция отправки SMS по превышению температуры
  Serial.println("sms start...");
  String PHONE = Tel1;  // Номер для отправки СМС
  Serial.print("Send to number: ");
  Serial.println(PHONE);
  Serial1.println("AT");
  delay(100);
  Serial1.println("AT+IPR=0");
  delay(100);
  Serial1.println("AT&W");
  delay(100);
  Serial1.println("AT+CNMI=2,2,0,0,0");
  delay(100);
  String text = "ALARM TEMP!!! : " + (rtc.getTime());
  Serial1.print("AT+CMGF=1\r");
  delay(100);
  Serial1.print("AT+CMGS=\"" + PHONE + "\"\r");
  delay(100);
  Serial1.print(text);
  delay(100);
  Serial1.write(0x1A);  //ascii код ctrl-26
  delay(100);
  Serial.println(F("sms send done!"));
}

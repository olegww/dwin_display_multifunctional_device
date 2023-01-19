
void IRAM_ATTR dwin_parsing() {  //  IRAM_ATTR Функция обработки данных с дисплея
  //Adafruit_NeoPixel strip(8, PIXEL_PIN, NEO_GRB + NEO_KHZ800);
  //strip.begin();

  static int countR1 = 0;
  static int countR2 = 0;
  static int calc_d = (CDay);
  static int calc_m = (CMonth);
  static int calc_y = (CYear);  // = 2022

  if (Serial2.available()) {
    buf_dwin[Buffer_Len] = Serial2.read();
    Buffer_Len++;
    flag = true;
    delay(1);
  } else {
    if (flag) {
      if (buf_dwin[4] == 0X65 && buf_dwin[7] == 0X21 && buf_dwin[8] == 0X31) {  // месяц >>
        calc_m++;
        if (calc_m == 13) {
          calc_m = 1;
          calc_y++;
        }
        CYear = (calc_y);
        CMonth = (calc_m);
        Calendar();
      }
      if (buf_dwin[4] == 0X65 && buf_dwin[7] == 0X40 && buf_dwin[8] == 0X32) {  // месяц <<
        calc_m--;
        if (calc_m == 0) {
          calc_m = 12;
          calc_y--;
        }
        CYear = (calc_y);
        CMonth = (calc_m);
        Calendar();
      }
      if (buf_dwin[4] == 0X65 && buf_dwin[5] == 0X02) {
        if (buf_dwin[8] == 0X01) {  // Год лист >>
          calc_y--;
          CYear = (calc_y);
          Calendar();
        }
        if (buf_dwin[8] == 0X02) {  // Год лист <<
          calc_y++;
          CYear = (calc_y);
          Calendar();
        }
      }
      if (buf_dwin[4] == 0X51 && buf_dwin[5] == 0X20) {  // Установка даты по сетке календаря
        int cal_toch = buf_dwin[8];
        if ((CYear >= 2000 && CYear <= 2060) && change_data[cal_toch] != 0) {
          rtc.DSadjust(rtc.hour, rtc.minute, rtc.second, CYear, CMonth, change_data[cal_toch]);  // 00:19:21 16 Mar 2022
          delay(5);
          rtc.DSread();
          delay(5);
          CDay = (rtc.dayOfMonth);
          Calendar();
        }
      }
      if (buf_dwin[4] == 0X59 && buf_dwin[7] == 0X41 && buf_dwin[8] == 0X61) {  // Нажатие кнопки Вернуться в меню из календаря
        dateUPDCal();
        rtc.DSread();
        delay(5);
        CYear = ((float)rtc.year + 2000);
        CMonth = (rtc.month);
        Calendar();
        calc_m = (CMonth);
        calc_y = (CYear);
      }

      if (buf_dwin[4] == 0X50 && buf_dwin[5] == 0X55) {  // сброс секунд
        if (buf_dwin[8] == 0X01) {
          rtc.DSread();
          int rtc_hour = (rtc.hour);
          int rtc_min = (rtc.minute);
          int rtc_sec = (rtc.second);
          int rtc_dd = (rtc.dayOfMonth);
          int rtc_mm = (rtc.month);
          int rtc_yy = (rtc.year);
          rtc.DSadjust(rtc_hour, rtc_min, 30, rtc_yy, rtc_mm, rtc_dd);  // часы, минуты, секунды, год, месяц, число
          delay(1);
          time_load();
        }
      }


      if (buf_dwin[4] == 0X50 && buf_dwin[5] == 0X52) {  // Установка часов минут

        if (buf_dwin[8] == 0X01) {  // Час минус
          rtc.DSread();
          int rtc_hour = (rtc.hour);
          rtc_hour--;
          if (rtc_hour == -1) {
            rtc_hour = 23;
          }
          int hour_send = rtc_hour;
          int rtc_min = (rtc.minute);
          int rtc_sec = (rtc.second);
          int rtc_dd = (rtc.dayOfMonth);
          int rtc_mm = (rtc.month);
          int rtc_yy = (rtc.year);
          rtc.DSadjust(hour_send, rtc_min, rtc_sec, rtc_yy, rtc_mm, rtc_dd);  // часы, минуты, секунды, год, месяц, число
          delay(1);
          time_load();
        }
        if (buf_dwin[8] == 0X02) {  // Час плюс
          rtc.DSread();
          int rtc_hour = (rtc.hour);
          rtc_hour++;
          if (rtc_hour == 24) {
            rtc_hour = 0;
          }
          int hour_send = rtc_hour;
          int rtc_min = (rtc.minute);
          int rtc_sec = (rtc.second);
          int rtc_dd = (rtc.dayOfMonth);
          int rtc_mm = (rtc.month);
          int rtc_yy = (rtc.year);
          rtc.DSadjust(hour_send, rtc_min, rtc_sec, rtc_yy, rtc_mm, rtc_dd);  // часы, минуты, секунды, год, месяц, число
          delay(1);
          time_load();
        }
        if (buf_dwin[8] == 0X03) {  // Минута минус
          rtc.DSread();
          int rtc_min = (rtc.minute);
          rtc_min--;
          if (rtc_min == -1) {
            rtc_min = 59;
          }
          int min_send = rtc_min;
          int rtc_hour = (rtc.hour);
          int rtc_sec = (rtc.second);
          int rtc_dd = (rtc.dayOfMonth);
          int rtc_mm = (rtc.month);
          int rtc_yy = (rtc.year);
          rtc.DSadjust(rtc_hour, min_send, rtc_sec, rtc_yy, rtc_mm, rtc_dd);  // часы, минуты, секунды, год, месяц, число
          delay(1);
          time_load();
        }
        if (buf_dwin[8] == 0X04) {  // Минута плюс
          rtc.DSread();
          int rtc_min = (rtc.minute);
          rtc_min++;
          if (rtc_min == 60) {
            rtc_min = 0;
          }
          int min_send = rtc_min;
          int rtc_hour = (rtc.hour);
          int rtc_sec = (rtc.second);
          int rtc_dd = (rtc.dayOfMonth);
          int rtc_mm = (rtc.month);
          int rtc_yy = (rtc.year);
          rtc.DSadjust(rtc_hour, min_send, rtc_sec, rtc_yy, rtc_mm, rtc_dd);  // часы, минуты, секунды, год, месяц, число
          delay(1);
          time_load();
        }
      }

      if (buf_dwin[4] == 0X50 && buf_dwin[5] == 0X34) {  // Режимы подсветки
        if (buf_dwin[8] == 0X01) {
          led_R1();
          Serial2.write(led_on, 8);
          //dfPlayer.volumeUp();
        }
        if (buf_dwin[8] == 0X02) {
          led_R2();
          Serial2.write(led_on, 8);
          //dfPlayer.volumeDown();
        }
        if (buf_dwin[8] == 0X03) {
          led_R3();
          Serial2.write(led_on, 8);
          //dfPlayer.previous();
        }
        if (buf_dwin[8] == 0X04) {
          led_R4();
          Serial2.write(led_on, 8);
          //dfPlayer.play(1);
        }
        if (buf_dwin[8] == 0X05) {
          led_R5();
          Serial2.write(led_on, 8);
          //dfPlayer.next();
        }
      }


      if (buf_dwin[4] == 0X50 && buf_dwin[5] == 0X32) {  // Подсветка вкл/выкл
        if (buf_dwin[8] == 0X00) {                       // Если 0
          led_cls();
          flag_led = false;
          //EEPROM.write(400, flag_led_e);
          //EEPROM.commit();
          //Serial.print("Flag led in pars OFF: ");
          //Serial.println(flag_led);
        }
        if (buf_dwin[8] == 0X01) {  // Если 1
          led_blue();
          flag_led = true;
          //EEPROM.write(0, flag_led_e);
          //EEPROM.commit();
          //Serial.print("Flag led in pars ON: ");
          //Serial.println(flag_led);
        }
      }
      //------------------------------------------------------Установка будильников-------------------------------
      if (buf_dwin[4] == 0X50 && buf_dwin[5] == 0X28) {  // Будильник 1 вкл/выкл запись в EEPROM
        if (buf_dwin[8] == 0X00) {                       // Если 0 флаг выкл
          alarm_flag = false;
          Serial.print("alarm_flag_OFF: ");
          Serial.println(alarm_flag);
          EEPROM.write(eeprom_alarm_1, alarm_flag);
          EEPROM.commit();
        }
        if (buf_dwin[8] == 0X01) {  // Если 1 флаг вкл
          alarm_flag = true;
          EEPROM.write(eeprom_alarm_1, alarm_flag);
          EEPROM.commit();
          Serial.print("alarm_flag_ON: ");
          Serial.println(alarm_flag);
        }
      }
      if (buf_dwin[4] == 0X50 && buf_dwin[5] == 0X30) {  // Будильник 2 вкл/выкл запись в EEPROM
        if (buf_dwin[8] == 0X00) {                       // Если 0
          alarm_flag2 = false;
          Serial.print("alarm_flag_OFF2: ");
          Serial.println(alarm_flag2);
          EEPROM.write(eeprom_alarm_2, alarm_flag2);
          EEPROM.commit();
        }
        if (buf_dwin[8] == 0X01) {  // Если 1
          alarm_flag2 = true;
          Serial.print("alarm_flag_ON2: ");
          Serial.println(alarm_flag2);
          EEPROM.write(eeprom_alarm_2, alarm_flag2);
          EEPROM.commit();
        }
      }
      //==========================================================================================================
      //------------------------------------------БУДИЛЬНИКИ------------------------------------------------------
      //==========================================================================================================

      if (buf_dwin[4] == 0X50 && buf_dwin[5] == 0X20) {  // Установка 1го будильника
        if (buf_dwin[8] == 0X01) {                       // Если нажата кнопка <<----- час
          alarm_hour--;
          if (alarm_hour == -1) {
            alarm_hour = 23;
          }
          byte alarm_hour_send[] = { 0x5A, 0xA5, 0x05, 0x82, 0x50, 0x21, 0x00, alarm_hour };
          Serial.print("alarm_hour: ");
          Serial.println(alarm_hour);
          Serial2.write(alarm_hour_send, 8);
          EEPROM.write(eeprom_hour, alarm_hour);
          EEPROM.commit();
        }
        if (buf_dwin[8] == 0X02) {  // Если нажата кнопка ---->> час
          alarm_hour++;
          if (alarm_hour >= 24) {
            alarm_hour = 0;
          }
          byte alarm_hour_send[] = { 0x5A, 0xA5, 0x05, 0x82, 0x50, 0x21, 0x00, alarm_hour };
          Serial.print("alarm_hour: ");
          Serial.println(alarm_hour);
          Serial2.write(alarm_hour_send, 8);
          EEPROM.write(eeprom_hour, alarm_hour);
          EEPROM.commit();
        }
        if (buf_dwin[8] == 0X03) {  // Если нажата кнопка <<----- минута
          alarm_min--;
          if (alarm_min <= -1) {
            alarm_min = 59;
          }

          byte alarm_min_send[] = { 0x5A, 0xA5, 0x05, 0x82, 0x50, 0x22, 0x00, alarm_min };
          Serial2.write(alarm_min_send, 8);
          EEPROM.write(eeprom_min, alarm_min);
          EEPROM.commit();
          alarm_min = EEPROM.read(eeprom_min);
          Serial.print("alarm_min: ");
          Serial.println(alarm_min);
        }
        if (buf_dwin[8] == 0X04) {  // Если нажата кнопка ------>> минута
          alarm_min++;
          if (alarm_min == 60) {
            alarm_min = 0;
          }
          byte alarm_min_send[] = { 0x5A, 0xA5, 0x05, 0x82, 0x50, 0x22, 0x00, alarm_min };
          Serial2.write(alarm_min_send, 8);
          EEPROM.write(eeprom_min, alarm_min);
          EEPROM.commit();
          alarm_min = EEPROM.read(eeprom_min);
          Serial.print("alarm_min: ");
          Serial.println(alarm_min);
        }
        if (buf_dwin[8] == 0X05) {  // Если нажата кнопка <<----- час2
          alarm_hour2--;
          if (alarm_hour2 == -1) {
            alarm_hour2 = 23;
          }
          byte alarm_hour2_send[] = { 0x5A, 0xA5, 0x05, 0x82, 0x50, 0x24, 0x00, alarm_hour2 };
          Serial.print("alarm_hour2: ");
          Serial.println(alarm_hour2);
          Serial2.write(alarm_hour2_send, 8);
          EEPROM.write(eeprom_hour2, alarm_hour2);
          EEPROM.commit();
        }
        if (buf_dwin[8] == 0X06) {  // Если нажата кнопка ---->> час2
          alarm_hour2++;
          if (alarm_hour2 == 24) {
            alarm_hour2 = 0;
          }
          byte alarm_hour2_send[] = { 0x5A, 0xA5, 0x05, 0x82, 0x50, 0x24, 0x00, alarm_hour2 };
          Serial.print("alarm_hour2: ");
          Serial.println(alarm_hour2);
          Serial2.write(alarm_hour2_send, 8);
          EEPROM.write(eeprom_hour2, alarm_hour2);
          EEPROM.commit();
        }
        if (buf_dwin[8] == 0X07) {  // Если нажата кнопка <<----- минута2
          alarm_min2--;
          if (alarm_min2 == -1) {
            alarm_min2 = 59;
          }
          byte alarm_min2_send[] = { 0x5A, 0xA5, 0x05, 0x82, 0x50, 0x26, 0x00, alarm_min2 };
          Serial.print("alarm_min2: ");
          Serial.println(alarm_min2);
          Serial2.write(alarm_min2_send, 8);
          EEPROM.write(eeprom_min2, alarm_min2);
          EEPROM.commit();
        }
        if (buf_dwin[8] == 0X08) {  // Если нажата кнопка ------>> минута2
          alarm_min2++;
          if (alarm_min2 == 60) {
            alarm_min2 = 0;
          }
          byte alarm_min2_send[] = { 0x5A, 0xA5, 0x05, 0x82, 0x50, 0x26, 0x00, alarm_min2 };
          Serial.print("alarm_min2: ");
          Serial.println(alarm_min2);
          Serial2.write(alarm_min2_send, 8);
          EEPROM.write(eeprom_min2, alarm_min2);
          EEPROM.commit();
        }
      }
      if (buf_dwin[4] == 0X50 && buf_dwin[5] == 0X31 && buf_dwin[8] == 0X01) {
        Serial2.write(night_mode_bright_on, 8);
        alarm_sound = false;
        led_cls();
      }
      //==========================================================================================================
      //--------------------------------------END=БУДИЛЬНИКИ------------------------------------------------------
      //==========================================================================================================
      //--------------------------------------Ночной режим вкл/выкл-----------------------------------------------
      //==========================================================================================================
      if (buf_dwin[4] == 0X50 && buf_dwin[5] == 0X70) {
        if (buf_dwin[8] == 0X00) {  // Если 0
          night_mode_flag = false;
          //Serial.print("night_mode_flag: ");
          //Serial.println(night_mode_flag);
        }
        if (buf_dwin[8] == 0X01) {  // Если 1
          night_mode_flag = true;
          Serial2.write(sound_icon_on, 8);
          //Serial.print("night_mode_flag: ");
          //Serial.println(night_mode_flag);
        }
      }
      // ---------------------------Звук вкл/выкл------------------------------------------
      if (buf_dwin[4] == 0X59 && buf_dwin[5] == 0X92) {  // Звук вкл/выкл
        if (buf_dwin[8] == 0X01) {                       // Если 0
          //byte buz_off[] = { 0x5A, 0xA5, 0x07, 0x82, 0x00, 0x80, 0x5A, 0x00, 0x00, 0x30 };
          Serial2.write(buz_off, 10);
          //Serial.println("buz_off: ");
          sound = 0;
          EEPROM.write(eeprom_sound, sound);
          EEPROM.commit();
          //Serial.print("sound: ");
          // Serial.println(sound);
        }
        if (buf_dwin[8] == 0X00) {  // Если 1
          //byte buz_on[] = { 0x5A, 0xA5, 0x07, 0x82, 0x00, 0x80, 0x5A, 0x00, 0x00, 0x38 };
          Serial2.write(buz_on, 10);
          //Serial.println("buz_on: ");
          sound = 1;
          EEPROM.write(eeprom_sound, sound);
          EEPROM.commit();
          //Serial.print("sound: ");
          //Serial.println(sound);
        }
      }

      if (buf_dwin[5] == 0X82 && buf_dwin[6] == 0X01) {  // Выход из режима подсветки и выключение звука
        if (buf_dwin[7] == 0X64) {                       // Тап выход из режима подсветки
          alarm_sound = false;
          alarm_flag = false;
          alarm_flag2 = false;
          Timer5 = millis();
          if (sound == 0) {
            Serial2.write(buz_off, 10);
          }
          if (sound == 1) {
            Serial2.write(buz_on, 10);
          }
        }
        if (buf_dwin[7] == 0X01) {  // Если яркость 1%// Тап вход в режим подсветки
          Serial2.write(buz_off, 10);
        }
      }

      //==========================================================================================================
      //----------------------------------------------Настройка-оповещений----------------------------------------
      //==========================================================================================================

      if (buf_dwin[4] == 0X50 && buf_dwin[5] == 0X36) {  // Вкл/выкл флаг оповещения превышения CO
        if (buf_dwin[8] == 0X00) {                       // Выкл
          CO_alarm = false;
          CO_alarm_sound = false;
          //EEPROM.write(eeprom_CO_alarm, CO_alarm);
          //EEPROM.commit();
          Serial2.write(CO_icon_off, 8);
        }
        if (buf_dwin[8] == 0X01) {  // Вкл
          CO_alarm = true;
          CO_alarm_sound = true;
          led_cls();
          //EEPROM.write(eeprom_CO_alarm, CO_alarm);
          //EEPROM.commit();
        }
      }
      if (buf_dwin[4] == 0X50 && buf_dwin[5] == 0X38) {  // Флаг если температура больше
        if (buf_dwin[8] == 0X00) {                       // Выкл
          temp_h_alarm = false;
          EEPROM.write(eeprom_temp_h, temp_h_alarm);
          EEPROM.commit();
        }
        if (buf_dwin[8] == 0X01) {  // Вкл
          temp_h_alarm = true;
          EEPROM.write(eeprom_temp_h, temp_h_alarm);
          EEPROM.commit();
        }
      }
      if (buf_dwin[4] == 0X50 && buf_dwin[5] == 0X40) {  // Флаг если температура меньше
        if (buf_dwin[8] == 0X00) {                       // Выкл
          temp_l_alarm = false;
          EEPROM.write(eeprom_temp_l, temp_l_alarm);
          EEPROM.commit();
        }
        if (buf_dwin[8] == 0X01) {  // Вкл
          temp_l_alarm = true;
          EEPROM.write(eeprom_temp_l, temp_l_alarm);
          EEPROM.commit();
        }
      }
      //----------------------------------------------СМС-ЕСЛИ ТЕМПЕРАТУРА БОЛЬШЕ---------------------------------

      if (buf_dwin[4] == 0X50 && buf_dwin[5] == 0X42) {  // Установка температуры  если больше <<<   >>>
        temp_h_value = buf_dwin[8];
        EEPROM.put(eeprom_temp_h_value, temp_h_value);
        EEPROM.commit();
      }

      if (buf_dwin[4] == 0X50 && buf_dwin[5] == 0X44) {  // Установка температуры  если меньше в т.ч. отрицательные значения <<<   >>>
        temp_l_value = buf_dwin[8];
        if (buf_dwin[7] == 255) {
          temp_l_value = buf_dwin[8] - 256;
        }
        EEPROM.put(eeprom_temp_l_value, temp_l_value);  //Пишем put для любых данных, читаем get
        EEPROM.commit();
      }

      //int eeprom_temp_h_value = 35, eeprom_temp_l_value = 37;
      // long long bx6 = buf_dwin[10] << 32 | buf_dwin[11] << 24 | buf_dwin[12] << 16 | buf_dwin[13] << 8 | buf_dwin[14];
      // int eeprom_CO_alarm = 32, eeprom_temp_h = 33, eeprom_temp_l = 34;
      //==========================================================================================================
      //----------------------------------------------Режимы охраны-----------------------------------------------
      //==========================================================================================================

      if (buf_dwin[4] == 0X50 && buf_dwin[5] == 0X72) {  // Режимы охраны
        if (buf_dwin[8] == 0X01) {                       // Если нажата 1 минута
          guard_flag_1 = true;
          guard_flag_5 = false;
          guard_flag_30 = false;
          guard_flag_60 = false;
          guard_1 = 60;
          Serial2.write(guard_time_0, 8);
        }
        if (buf_dwin[8] == 0X02) {  // Если нажата 5 минут
          guard_flag_1 = false;
          guard_flag_5 = true;
          guard_flag_30 = false;
          guard_flag_60 = false;
          guard_5 = 300;
          Serial2.write(guard_time_0, 8);
        }
        if (buf_dwin[8] == 0X03) {  // Если нажата 30 минут
          guard_flag_1 = false;
          guard_flag_5 = false;
          guard_flag_30 = true;
          guard_flag_60 = false;
          guard_30 = 1800;
          Serial2.write(guard_time_0, 8);
        }
        if (buf_dwin[8] == 0X04) {  // Если нажата 60 минут
          guard_flag_1 = false;
          guard_flag_5 = false;
          guard_flag_30 = false;
          guard_flag_60 = true;
          guard_60 = 3600;
          Serial2.write(guard_time_0, 8);
        }
      }
      if (buf_dwin[4] == 0X50 && buf_dwin[5] == 0X76) {  // Нажата кнопка Выкл охрана
        if (buf_dwin[8] == 0X01) {
          guard_flag_1 = false;  // Сброс флагов обратного отсчета
          guard_flag_5 = false;
          guard_flag_30 = false;
          guard_flag_60 = false;
          guard_up = false;  // Снятие режима охраны
          guard_1 = 60;
          guard_5 = 300;
          guard_30 = 1800;
          guard_60 = 3600;
          Serial2.write(guard_time_0, 8);
          Serial2.write(guard_icon_off, 8);
          led_cls();
        }
      }
      //==========================================================================================================
      //-----------------------------------------END--Режимы охраны-----------------------------------------------
      //==========================================================================================================

      //==========================================================================================================
      //----------------------------------------------Кнопка отправить смс----------------------------------------
      //==========================================================================================================
      if (buf_dwin[4] == 0X59 && buf_dwin[5] == 0X90) {
        if (buf_dwin[8] == 0X01) {  // Кнопка 01
          Serial.println("sms start...");
          String PHONE = Tel2;  // Номер для отправки СМС Пожарная охрана
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
          String text = "01";
          Serial1.print("AT+CMGF=1\r");
          delay(100);
          Serial1.print("AT+CMGS=\"" + PHONE + "\"\r");
          delay(100);
          Serial1.print(text);
          delay(100);
          Serial1.write(0x1A);  //ascii код ctrl-26
          delay(500);
          Serial2.write(night_mode_list_0, 10);
          Serial.println(F("sms send done!"));
        }
        if (buf_dwin[8] == 0X02) {  // Кнопка 02
          Serial.println("sms start...");
          String PHONE = Tel3;  // Номер для отправки СМС Милиция
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
          String text = "02";
          Serial1.print("AT+CMGF=1\r");
          delay(100);
          Serial1.print("AT+CMGS=\"" + PHONE + "\"\r");
          delay(100);
          Serial1.print(text);
          delay(100);
          Serial1.write(0x1A);  //ascii код ctrl-26
          delay(500);
          Serial2.write(night_mode_list_0, 10);
          Serial.println(F("sms send done!"));
        }
        if (buf_dwin[8] == 0X03) {  // Кнопка 03
          Serial.println("sms start...");
          String PHONE = Tel4;  // Номер для отправки СМС Скорая помощь
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
          String text = "03";
          Serial1.print("AT+CMGF=1\r");
          delay(100);
          Serial1.print("AT+CMGS=\"" + PHONE + "\"\r");
          delay(100);
          Serial1.print(text);
          delay(100);
          Serial1.write(0x1A);  //ascii код ctrl-26
          delay(500);
          Serial2.write(night_mode_list_0, 10);
          Serial.println(F("sms send done!"));
        }
        if (buf_dwin[8] == 0X04) {  // Кнопка 04
          Serial.println("sms start...");
          String PHONE = Tel5;  // Номер для отправки СМС Служба газа
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
          String text = "04";
          Serial1.print("AT+CMGF=1\r");
          delay(100);
          Serial1.print("AT+CMGS=\"" + PHONE + "\"\r");
          delay(100);
          Serial1.print(text);
          delay(100);
          Serial1.write(0x1A);  //ascii код ctrl-26
          delay(500);
          Serial2.write(night_mode_list_0, 10);
          Serial.println(F("sms send done!"));
        }
        if (buf_dwin[8] == 0X05) {  // Кнопка тревога на главной
          Serial.println("sms start...");
          String PHONE = Tel6;  // Номер для отправки СМС
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
          String text = "HELP!";
          Serial1.print("AT+CMGF=1\r");
          delay(100);
          Serial1.print("AT+CMGS=\"" + PHONE + "\"\r");
          delay(100);
          Serial1.print(text);
          delay(100);
          Serial1.write(0x1A);  //ascii код ctrl-26
          delay(500);
          Serial2.write(night_mode_list_0, 10);
          Serial.println(F("sms send done!"));
        }


        //sendsms();
        //dateReset();
        //dateUPD();
        //Calendar();
      }

      //==========================================================================================================
      //-----------------------------------------END--Кнопки отправить смс----------------------------------------
      //==========================================================================================================

      //==========================================================================================================
      //----------------------------------------------Запись телефонов--------------------------------------------
      //==========================================================================================================

      if (buf_dwin[4] == 0X70 && buf_dwin[5] == 0X50) {
        long long bx6 = buf_dwin[10] << 32 | buf_dwin[11] << 24 | buf_dwin[12] << 16 | buf_dwin[13] << 8 | buf_dwin[14];
        char buffbx6[15];
        sprintf(buffbx6, "+7%lld", bx6);
        Tel6 = buffbx6;
        Serial.println(Tel6);
        EEPROM.writeString(eepromTel6, Tel6);
        EEPROM.commit();
        Serial.println("stringData write eepromTel6 in EEPROM is Successful");
        //char Buf[13];
        char Buf6[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
        char tt6[] = { 0x5a, 0xa5, 0x0f, 0x82, 0x71, 0x50, 0x2b, 0x37, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 };
        Tel6.toCharArray(Buf6, 13);  // 7150 номер телефона Тревога
        for (int i = 8; i < 18; i++) {
          tt6[i] = Buf6[i - 6];
        }
        Serial2.write(tt6, 18);
      }
      if (buf_dwin[4] == 0X70 && buf_dwin[5] == 0X40) {
        long long bx5 = buf_dwin[10] << 32 | buf_dwin[11] << 24 | buf_dwin[12] << 16 | buf_dwin[13] << 8 | buf_dwin[14];
        char buffbx5[15];
        sprintf(buffbx5, "+7%lld", bx5);
        Tel5 = buffbx5;
        Serial.println(Tel5);
        EEPROM.writeString(eepromTel5, Tel5);
        EEPROM.commit();
        Serial.println("stringData write eepromTel5 in EEPROM is Successful");
        //char Buf[13];
        char Buf5[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
        char tt5[] = { 0x5a, 0xa5, 0x0f, 0x82, 0x71, 0x40, 0x2b, 0x37, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 };
        Tel5.toCharArray(Buf5, 13);  // 7140 номер телефона Газ
        for (int i = 8; i < 18; i++) {
          tt5[i] = Buf5[i - 6];
        }
        Serial2.write(tt5, 18);
      }
      if (buf_dwin[4] == 0X70 && buf_dwin[5] == 0X30) {
        long long bx4 = buf_dwin[10] << 32 | buf_dwin[11] << 24 | buf_dwin[12] << 16 | buf_dwin[13] << 8 | buf_dwin[14];
        char buffbx4[15];
        sprintf(buffbx4, "+7%lld", bx4);
        Tel4 = buffbx4;
        Serial.println(Tel4);
        EEPROM.writeString(eepromTel4, Tel4);
        EEPROM.commit();
        Serial.println("stringData write eepromTel4 in EEPROM is Successful");
        //char Buf[13];
        char Buf4[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
        char tt4[] = { 0x5a, 0xa5, 0x0f, 0x82, 0x71, 0x30, 0x2b, 0x37, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 };
        Tel4.toCharArray(Buf4, 13);  // 7130 номер телефона Скорая
        for (int i = 8; i < 18; i++) {
          tt4[i] = Buf4[i - 6];
        }
        Serial2.write(tt4, 18);
      }
      if (buf_dwin[4] == 0X70 && buf_dwin[5] == 0X20) {
        long long bx3 = buf_dwin[10] << 32 | buf_dwin[11] << 24 | buf_dwin[12] << 16 | buf_dwin[13] << 8 | buf_dwin[14];
        char buffbx3[15];
        sprintf(buffbx3, "+7%lld", bx3);
        Tel3 = buffbx3;
        Serial.println(Tel3);
        EEPROM.writeString(eepromTel3, Tel3);
        EEPROM.commit();
        Serial.println("stringData write eepromTel3 in EEPROM is Successful");
        //char Buf[13];
        char Buf3[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
        char tt3[] = { 0x5a, 0xa5, 0x0f, 0x82, 0x71, 0x20, 0x2b, 0x37, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 };
        Tel3.toCharArray(Buf3, 13);  // 7120 номер телефона Милиция
        for (int i = 8; i < 18; i++) {
          tt3[i] = Buf3[i - 6];
        }
        Serial2.write(tt3, 18);
      }

      if (buf_dwin[4] == 0X70 && buf_dwin[5] == 0X10) {
        long long bx2 = buf_dwin[10] << 32 | buf_dwin[11] << 24 | buf_dwin[12] << 16 | buf_dwin[13] << 8 | buf_dwin[14];
        char buffbx2[15];
        sprintf(buffbx2, "+7%lld", bx2);
        Tel2 = buffbx2;
        Serial.println(Tel2);
        EEPROM.writeString(eepromTel2, Tel2);
        EEPROM.commit();
        Serial.println("stringData write eepromTel2 in EEPROM is Successful");
        //char Buf[13];
        char Buf2[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
        char tt2[] = { 0x5a, 0xa5, 0x0f, 0x82, 0x71, 0x10, 0x2b, 0x37, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 };
        Tel2.toCharArray(Buf2, 13);  // 7110 номер телефона Пожарная охрана
        for (int i = 8; i < 18; i++) {
          tt2[i] = Buf2[i - 6];
        }
        Serial2.write(tt2, 18);
      }

      if (buf_dwin[4] == 0X70 && buf_dwin[5] == 0X00) {
        long long bx1 = buf_dwin[10] << 32 | buf_dwin[11] << 24 | buf_dwin[12] << 16 | buf_dwin[13] << 8 | buf_dwin[14];
        Serial.println(Tel1);
        char buffbx1[15];
        sprintf(buffbx1, "+7%lld", bx1);
        Tel1 = buffbx1;
        Serial.println(Tel1);
        EEPROM.writeString(eepromTel1, Tel1);
        EEPROM.commit();
        Serial.println("stringData write eepromTel1 in EEPROM is Successful");
        Serial.println(Tel1);
        Tel1 = EEPROM.readString(eepromTel1);
        char Buf1[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
        char tt1[] = { 0x5a, 0xa5, 0x0f, 0x82, 0x71, 0x00, 0x2b, 0x37, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
        Tel1.toCharArray(Buf1, 13);  // 7100 номер телефона сигнализация/владелец
        for (int i = 8; i < 18; i++) {
          tt1[i] = Buf1[i - 6];
        }
        Serial2.write(tt1, 18);

        Buf1[0];
      }
      /*

      if (buf_dwin[4] == 0X70 && buf_dwin[5] == 0X00) {
        long long bx3 = buf_dwin[10] << 32 | buf_dwin[11] << 24 | buf_dwin[12] << 16 | buf_dwin[13] << 8 | buf_dwin[14];
        stack.reset();
        delay(30);
        stack << "+7" << bx3;
        Tel1 = test;
        String PHONE = Tel1;
        Serial.println(Tel1);
        EEPROM.writeString(eepromTel1, Tel1);
        EEPROM.commit();
        Serial.println("stringData write eepromTel1 in EEPROM is Successful");
        Serial.println(Tel1);
        Tel1 = EEPROM.readString(eepromTel1);
        char Buf1[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
        char tt1[] = { 0x5a, 0xa5, 0x0f, 0x82, 0x71, 0x00, 0x2b, 0x37, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
        Tel1.toCharArray(Buf1, 13);  // 7100 номер телефона сигнализация/владелец
        for (int i = 8; i < 18; i++) {
          tt1[i] = Buf1[i - 6];
        }
        Serial2.write(tt1, 18);

        Buf1[0];
      }
      */

      /*
      // Пакет для отображения телефона String 5a a5 0f 82 70 60 2b 37 39 32 36 36 35 30 31 34 36 36
      // Изменение номера телефона в дисплее (5A A5 0C 83 70 00 04 00 00 00 02 28 53 97 5A) = 9266501466
      if (buf_dwin[4] == 0X70 && buf_dwin[5] == 0X00) {
        // Объединяем данные long long buf_dwin[20];
        long long bx1 = buf_dwin[10] << 32 | buf_dwin[11] << 24 | buf_dwin[12] << 16 | buf_dwin[13] << 8 | buf_dwin[14];
        //Serial.println(bx1);
        stack.reset();         // Обнуляем значение
        stack << "+7" << bx1;  // Объединяем +7 с введенным на dwin значением
        //Serial.println(stack.get());
        Tel1 = test;  // Записываем объединенные данные в EasyStringStream stack(test, 13);
        //Serial.println(phone_number);
        //Serial.println(PHONE);
        String PHONE = Tel1;
        EEPROM.writeString(eepromTel1, Tel1);
        EEPROM.commit();
        Serial.println("stringData write in EEPROM is Successful");
      }
      */
      Buffer_Len = 0;  // сброс номера элемента в массиве
      flag = false;
    }
  }
}


Example of use

                    String CENG_0;//вышка 0 информация
                    String CENG_1;//вышка 1 информация
                    String CENG_2;//вышка 2 информация
                    String String_CENG_INFO_not_encoded_in_base64;//строка не кодированная в base64
                    String http_BASE64_string;//строка http


     
                  String ALL_CENGs = Sim800l.Get_CELLS_INFO_type1(&CENG_0, &CENG_1, &CENG_2, &String_CENG_INFO_not_encoded_in_base64, &http_BASE64_string);//вызвали функцию получения информации по вышкам

                    ///////////////////////////////////////////////////////////////////////////////
                    tft.fillScreen(GREEN);//очистить
                    tft.setTextSize(1);
                    tft.setCursor(190, 10);//X,Y от крайнего верхнего угла экрана     
                    tft.println("---------------ALL_CENGs------------------");
                    tft.println(ALL_CENGs);
                    tft.println("---------------CENG_0---------------------");
                    tft.println(CENG_0);
                    tft.println("---------------CENG_1---------------------");
                    tft.println(CENG_1);
                    tft.println("---------------CENG_2---------------------");
                    tft.println(CENG_2);
                    tft.println("--String_CENG_INFO_not_encoded_in_base64---");
                    tft.println(String_CENG_INFO_not_encoded_in_base64);
                    tft.println("---------------http_BASE64_string-------");
                    tft.println(http_BASE64_string);
                    ///////////////////////////////////////////////////////////////////////////////

                       smsTEXT_toSend = http_BASE64_string;

                       Sim800l.sendSms(CurrentCallNumber, smsTEXT_toSend); //отправляем SMS

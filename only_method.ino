



//НАЧАЛО берём информацию о ближайших сотовых вышках через инженерное меню ТИП 1 как можно брать информацию(менее затратную по энергопотреблению)
String Sim800l::Get_CELLS_INFO_type1(String *String_CENG_0, String *String_CENG_1, String *String_CENG_2, String *String_CENG_INFO_not_encoded_in_base64, String *String_http_BASE64)//приняли адрес в памяти переменной(чтоб по этому адресу и возвращать значения)
{

      //ВКЛЮЧАЕМ ИНЖЕНЕРНОЕ МЕНЮ
      //команда для ВКЛЮЧЕНИЯ инженерного меню для получения детальной информации о сотовых вышках
      //AT+CENG=mode,Ncell
      //mode - режим 0-выключить; 1- включить;  2-  3-
      //Ncell - 0 - не показывать айди ближайших сотовых вышек;   1-показывать ID ближайших сотовых вышек
      Serial1.println("at+ceng=1,1");

      
      //считываем ответ
      _buffer=_readSerial();



///////////////////////////////////////////////////////


      //Получить информацию о ближайших сотовых вышках
      //AT+Ceng?
      
       Serial1.println("AT+Ceng?");
      
      //считываем ответ
      _buffer=_readSerial();//теперь ответ о всех вышках у нас в переменной _buffer типа String


///////////////////////////////////////////////////////
     
      //выключаем инженерное меню
       Serial1.println("at+ceng=0,0");

////////////////////////////////////////////////////////









//получили в _buffer что то типа этого
/*
       1,1

//+CENG: <cell>,"<arfcn_bcch>,<rxl>,<rxq>,<mcc>,<mnc>,<bsic>,<cellid>,<rla>,<txp>,<lac>,<TA>"

       +CENG: 0,"0213        ,   13,   00,  255,   03,    21,    12e1,   06,   00, 101a,255"       
       +CENG: 1,"0038,42,18,00f2,255,03,108a"
       +CENG: 2,"0016,38,44,120b,255,03,108a"
       +CENG: 3,"0111,35,19,1111,255,03,108a"
       +CENG: 4,"0016,30,14,00f1,255,03,108a"
       +CENG: 5,"0111,23,12,1031,255,03,108a"
       +CENG: 6,         "0018,    42,      18,    00f3,  255,   03,101a"
  
   +CENG:<cell>, "<arfcn_bcch>, <rxl>,  <bsic>,<cellid>,<mcc>,<mnc>,<lac>"
       OK
 */

//нужно выбрать с 3 вышек нужные нам данные






//начальные значения "0" чтоб знать или записали в них значения, потом
*String_CENG_0="0";
*String_CENG_1="0";
*String_CENG_2="0";



//НАЧАЛО CENG0 - ВЫШКА 0 - выбор значений в переменные
//получили данные вида
/*
 +CENG: <cell>,"<arfcn_bcch>,<rxl>,<rxq>,<mcc>,<mnc>,<bsic>,<cellid>,<rla>,<txp>,<lac>,<TA>"
      +CENG: 0,"0111        ,   33,   00,  255,   03,    21,    11e1,   06,   00, 101a,255"
 */
int index_CENG = _buffer.indexOf("+CENG: 0,\""); //находим начало строки
int index_new_line = _buffer.indexOf("\r\n", index_CENG);//находим конец строки

//0-ВЫШКА ДАННЫЕ, в ссылку на адрес в памяти(*String_CENG_0), выбрали данные такого типа 0117,33,00,255,03,21,14e1,06,00,108a,255
*String_CENG_0 = _buffer.substring(index_CENG+10, index_new_line-1);



//переводим с типа String в char array
String CENG0 = *String_CENG_0;
char char_CENG_0[255];
CENG0.toCharArray(char_CENG_0, 255);//теперь все данные в массиве символов char_CENG_0 
////////////////////////////////////





//извлекаем с char_CENG_0 значения - в нужные нам переменные
int CENG0_arfcn_bcch=0;  //ARFCN(Absolute radio frequency channel number) of BCCH carrier, in decimal format
int CENG0_rxl=0; //Receive level, in decimal format
int CENG0_rxq=0; //Receive quality, in decimal format
int CENG0_mcc=0; //Mobile country code, in decimal format
int CENG0_mnc=0; //Mobile network code, in decimal format
int CENG0_bsic=0; //Base station identity code, in decimal format
int CENG0_cellid=0x00; //Cell id, in hexadecimal format
int CENG0_rla=0; //Receive level access minimum, in decimal format
int CENG0_txp=0; //Transmit power maximum CCCH, in decimal format
int CENG0_lac=0x00; //Location area code, in hexadecimal format
int CENG0_TA=0; //Timing Advance, in decimal format




// с chararray char_CENG_0 выбрать все значения, в переменные
//%х   Считать шестнадцатеричное число
sscanf(char_CENG_0, "%lu, %lu, %lu, %lu, %lu, %lu, %x, %lu, %lu, %x, %lu", &CENG0_arfcn_bcch, &CENG0_rxl, &CENG0_rxq, &CENG0_mcc, &CENG0_mnc, &CENG0_bsic, &CENG0_cellid, &CENG0_rla, &CENG0_txp, &CENG0_lac, &CENG0_TA);



//КОНЕЦ CENG0 - ВЫШКА 0 - выбор значений в переменные
//////////////////////////////////////////////////////



























///////////////////////////////////////////////////////////////////
//НАЧАЛО загнать нормальные строки в *String_CENG_1 и в *String_CENG_2
// т.е. строка типа +CENG: 4,"0036,32,00,ffff,,,0000" и  +CENG: 4,"0036,32,00,0000,,,0000" где cellid равен 0000 или ffff ---->  неподходят
  


//будет 1 если в *String_CENG_1 есть нормальная информация
int index_ceng_1=0;




//в цикле перебираем строки и если есть нормальная информация о вышках заносим в *String_CENG_1 и *String_CENG_2
 for (int i=1; i < 6; i++)
 {

     //CENG_X
     String CENG_X = "+CENG: " + String(i) + ",\"";  //чтоб было на подобии этого +CENG: 1,\"
   
     index_CENG = _buffer.indexOf(CENG_X); //начало строки
     index_new_line = _buffer.indexOf("\r\n", index_CENG);//находим конец строки

     //извлекаем строку 
     String S_CENG_X = _buffer.substring(index_CENG+10, index_new_line-1);


     //записываем любые данные в *String_CENG_1 и *String_CENG_2
     if(*String_CENG_1=="0") 
     {
      *String_CENG_1 = S_CENG_X;
     }
     else if(*String_CENG_2=="0") 
     {
      *String_CENG_2 = S_CENG_X;
     } 


 
      //если информация о вышке нормальная и в *String_CENG_1 нет записи нормальной
      if( (((S_CENG_X.indexOf("0000"))==-1) && ((S_CENG_X.indexOf("ffff"))==-1)) && index_ceng_1==0 )
      {
        *String_CENG_1=S_CENG_X;
        index_ceng_1=1;//запомнили что в *String_CENG_1 уже есть нормальная информация 
      }
      //если информация о вышке нормальная и в переменной *String_CENG_1 уже есть информация о нормальной вышке
      else if( (((S_CENG_X.indexOf("0000"))==-1) && ((S_CENG_X.indexOf("ffff"))==-1)) && index_ceng_1==1 )
      {
        *String_CENG_2=S_CENG_X;//записываем нормальную информацию о вышке
        break;//выходим с цикла
      }
 
 }//кон цикла for 

////////////////////////////////////////////////////////////////////////////////////////////////////










//CENG1 - ВЫШКА 1
//получили данные вида
/*
       +CENG: 6,         "0018,    11,      18,    00f1,  255,   03,101a"  
   +CENG:<cell>, "<arfcn_bcch>, <rxl>,  <bsic>,<cellid>,<mcc>,<mnc>,<lac>"
*/


//переводим с типа String в char array
String CENG1 = *String_CENG_1;
char char_CENG_1[255];
CENG1.toCharArray(char_CENG_1, 255); //теперь все данные в массиве символов char_CENG_1
////////////////////////////////////




//извлекаем с char_CENG_1 значения - в нужные нам переменные

int CENG1_arfcn_bcch=0;  //ARFCN(Absolute radio frequency channel number) of BCCH carrier, in decimal format
int CENG1_rxl=0; //Receive level, in decimal format
int CENG1_bsic=0; //Base station identity code, in decimal format
int CENG1_cellid=0x00; //Cell id, in hexadecimal format
int CENG1_mcc=0; //Mobile country code, in decimal format
int CENG1_mnc=0; //Mobile network code, in decimal format
int CENG1_lac=0x00; //Location area code, in hexadecimal format





// с chararray char_CENG_1 выбрать все значения в переменные
//%х   Считать шестнадцатеричное число
sscanf(char_CENG_1, "%lu, %lu, %lu, %x, %lu, %lu, %x", &CENG1_arfcn_bcch, &CENG1_rxl, &CENG1_bsic, &CENG1_cellid, &CENG1_mcc, &CENG1_mnc, &CENG1_lac);
/////////////////////////////////////////////////////














//CENG2 - ВЫШКА 2
//получили данные вида
/*
       +CENG: 6,         "0011,    12,      18,    00f1,  255,   03,108a"  
   +CENG:<cell>, "<arfcn_bcch>, <rxl>,  <bsic>,<cellid>,<mcc>,<mnc>,<lac>"
*/

//переводим с типа String в char array
String CENG2 = *String_CENG_2;
char char_CENG_2[255];
CENG2.toCharArray(char_CENG_2, 255); //теперь все данные в массиве символов char_CENG_2
////////////////////////////////////




//извлекаем с char_CENG_2 значения - в нужные нам переменные

int CENG2_arfcn_bcch=0;  //ARFCN(Absolute radio frequency channel number) of BCCH carrier, in decimal format
int CENG2_rxl=0; //Receive level, in decimal format
int CENG2_bsic=0; //Base station identity code, in decimal format
int CENG2_cellid=0x00; //Cell id, in hexadecimal format
int CENG2_mcc=0; //Mobile country code, in decimal format
int CENG2_mnc=0; //Mobile network code, in decimal format
int CENG2_lac=0x00; //Location area code, in hexadecimal format





// с chararray char_CENG_2 выбрать все значения в переменные
//%х   Считать шестнадцатеричное число
sscanf(char_CENG_2, "%lu, %lu, %lu, %x, %lu, %lu, %x", &CENG2_arfcn_bcch, &CENG2_rxl, &CENG2_bsic, &CENG2_cellid, &CENG2_mcc, &CENG2_mnc, &CENG2_lac);
/////////////////////////////////////////////////////











//Составляем строку вида:
//mcc0,mnc0,lac0,cellid0,SS0 ;mcc1,mnc1,lac1,cellid1,SS1;mcc2,mnc2,lac2,cellid2,SS2
//250 ,02,7810  ,318    ,-81 ; 250,  02,4711,  24208,-83; 250,  02,4711,  24214,-89
//непонятно Signal Strength это rxl ???


*String_CENG_INFO_not_encoded_in_base64 = String(CENG0_mcc)+ "," +String(CENG0_mnc)+ "," +String(CENG0_lac)+ "," +String(CENG0_cellid)+ "," +String(CENG0_rxl)+ ";" 
+String(CENG1_mcc)+ "," +String(CENG1_mnc)+ "," +String(CENG1_lac)+ "," +String(CENG1_cellid)+ "," +String(CENG1_rxl)+ ";"
+String(CENG2_mcc)+ "," +String(CENG2_mnc)+ "," +String(CENG2_lac)+ "," +String(CENG2_cellid)+ "," +String(CENG2_rxl);




//переводим с типа String в char array
String CENG_INFO_not_encoded_in_base64 = *String_CENG_INFO_not_encoded_in_base64;
char _CENG_INFO_not_encoded_in_base64[255];
CENG_INFO_not_encoded_in_base64.toCharArray(_CENG_INFO_not_encoded_in_base64, 255);





/////////////////////////////////////////////////////////////      
//кодируем в base64
//для кодирования в base64 кодировку, для формирования http ссылки
#include "Base64.h"


//char* input = "250,02,7810,318,-81;250,02,4711,24208,-83;250,02,4711,24214,-89";
char _CENG_INFO_encoded_in_base64[255];
base64_encode(_CENG_INFO_encoded_in_base64, _CENG_INFO_not_encoded_in_base64, strlen(_CENG_INFO_not_encoded_in_base64));



//составляем ссылку http для отправки
*String_http_BASE64="http://api.mylnikov.org/mobile/main.py/get?v=1.1&search=" + String(_CENG_INFO_encoded_in_base64); 

     
     


return _buffer;//вернули весь ответ по вышкам      
}
//КОНЕЦ берём информацию о ближайших сотовых вышках через инженерное меню ТИП 1 как можно брать информацию(менее затратную по энергопотреблению)







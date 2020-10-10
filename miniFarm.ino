#include <Wire.h>
#include <DS3231.h>
#include "Nextion.h"
#include <EEPROM.h>
#include <SoftwareSerial.h>
#include "DHT.h"
#include <ResponsiveAnalogRead.h>
#define DHTTYPE DHT22

#define PHSensorPin  A9    //dissolved oxygen sensor analog output pin to arduino mainboard
#define VREF 5.0

#define Temp_Toprak A10
ResponsiveAnalogRead tT(Temp_Toprak, true);
#define Temp_Led A2
ResponsiveAnalogRead tL(Temp_Led, true);
#define Nem_Toprak A6
ResponsiveAnalogRead nT(Nem_Toprak, true);
#define Ortam_H_T 15
DHT dht(Ortam_H_T, DHTTYPE);

#define LED 32
#define gubre1 33
#define gubre2 34
#define gubre3 35
#define nemlendirici 36
//#define isitici 37
#define fan 46
#define sulama 37
#define kapiKilit 40
#define suDepo 42
#define g1Depo 43
#define g2Depo 44
#define g3Depo 45

#define add_NemAlt_Oda 110
#define add_NemUst_Oda 120
#define add_TempAlt_Oda 130
#define add_TempUst_Oda 140
#define add_NemAlt_Toprak 150
#define add_Sulama_Oda 160
#define add_Gubre_Oda 170
#define add_led_Acik 180
#define add_led_Kapali 190
#define add_led_Max 230

DS3231 clock;
RTCDateTime dt;

int analogBuffer;    //store the analog value in the array, readed from ADC
int analogBufferIndex = 0, copyIndex = 0;
float averageVoltage, phValue;

double toprak_Temperature=0;
double led_Temperature=0;
int nemT;
float tempOda;float humOda;

int ee1 = 60, ee2 = 80, ee3 = 20, ee4 = 30, ee5 = 35, ee6 = 5, ee7 = 5, ee8 = 4, ee9 = 4, e10 = 30;
bool isitma=false,nemlendirme=false;
int tee1 = 1, tee2 = 1, tee3 = 1, tee4 = 1, tee5 = 1, tee6 = 1, tee7 = 1, tee8 = 1, tee9 = 1, te10 = 1;
int sayfa=10;

bool kapiDurum=false;
int su=1,g1=1,g2=1,g3=1;
int suD = 0, gubreD;

int ledWF=1,suWF=1;
long int mTime=0,kT=0, lTime=0;
unsigned long int oldHour = 0;
int suAcSaati, gubreAcSaati;
int wifiDurum=0;

String st = ""; //LED DURUMU
char buffer[100] = {0};
int active = 0; //LED aktifliği
float sicaklikOffset = 0.0;
float phOffset = 0.0;
float phAlt = 5.5;
int phActive = 0;
int gubreleme = 0; //Gübreleme aktifliği
int sula = 0; //Sulama aktifliği

SoftwareSerial HMISerial(10, 11);

    NexPage ana_menu = NexPage(1, 0, "ana_menu");
    NexPage set = NexPage(2, 0, "set");
    NexPage opt = NexPage(3, 0, "opt");
    NexPage PageH = NexPage(4, 0, "pH");

    NexProgressBar j0 = NexProgressBar(0, 5, "j0");
    NexDSButton b0 = NexDSButton(0, 2, "b0");
    NexText t0 = NexText(0, 1, "sif");
    NexText t1 = NexText(0, 4, "er");
    NexVariable giris=NexVariable(0,7,"giris");

//  #################### SAYFA 1 ####################

    NexText oT1 = NexText(1, 3, "t0");
    NexText text = NexText(1, 4, "t1");
    NexText led1 = NexText(1, 5, "t2");
    NexText oN1 = NexText(1, 6, "t3");
    NexText tN1 = NexText(1, 7, "t4");
    NexNumber n0 = NexNumber(1, 9, "n0");
    NexNumber n1 = NexNumber(1, 10, "n1");
    NexText tarih = NexText(1, 19, "tarih");
    NexText sDT = NexText(1, 20, "sDT");
    NexText gDT = NexText(1, 22, "gDT");
    NexDSButton option = NexDSButton(1, 1, "opt");
    NexDSButton activate = NexDSButton(1, 16, "activate");
    NexDSButton wF = NexDSButton(1, 23, "wF");
    NexDSButton kilit = NexDSButton(1, 21, "kilit");
    NexDSButton ppH = NexDSButton(1, 8, "ppH");
    NexDSButton settings = NexDSButton(1, 2, "set");
    NexText durum = NexText(1, 18, "ledDurum");


//  #################### SAYFA 2 ####################

    NexDSButton geri1 = NexDSButton(2, 15, "geri1");
    NexText setG = NexText(2, 20, "setG");
    NexText newP = NexText(2, 10, "newP");
    NexNumber sa = NexNumber(2, 2, "saat");
    NexNumber da = NexNumber(2, 3, "dak");
    NexNumber gun = NexNumber(2, 16, "gun");
    NexNumber ay = NexNumber(2, 17, "ay");
    NexNumber yil = NexNumber(2, 19, "yil");
    NexDSButton tarihAyar = NexDSButton(2, 21, "tarihAyar");
    NexDSButton saatAyar = NexDSButton(2, 1, "saatAyar");

//  #################### SAYFA 3 ####################

    NexDSButton geri2 = NexDSButton(3, 1, "geri2");
    NexText sO = NexText(3, 17, "sO");
    NexText pA = NexText(3, 24, "pA");
    NexDSButton nemler = NexDSButton(3, 32, "nemler");
    NexDSButton templer = NexDSButton(3, 33, "templer");
    NexDSButton kayalar = NexDSButton(3, 34, "kayalar");
    NexDSButton nemOnay = NexDSButton(3, 35, "nemOnay");
    NexDSButton ofOnay = NexDSButton(3, 36, "ofOnay");
    NexDSButton limitK = NexDSButton(3, 14, "ledOnay");
    NexNumber n2 = NexNumber(3, 6, "n2");
    NexNumber n3 = NexNumber(3, 7, "n3");
    NexNumber n4 = NexNumber(3, 11, "n4");
    NexNumber n5 = NexNumber(3, 12, "n5");
    NexNumber n6 = NexNumber(3, 13, "n6");
    NexNumber n7 = NexNumber(3, 24, "n7");
    NexNumber n8 = NexNumber(3, 20, "n8");
    NexNumber n9 = NexNumber(3, 22, "n9");
    NexNumber lAc = NexNumber(3, 26, "lAc");
    NexNumber lKa = NexNumber(3, 28, "lKa");
    NexNumber lmS = NexNumber(3, 30, "lmS");

//  #################### SAYFA 4 ####################

    NexDSButton geri3 = NexDSButton(4, 7, "geri3");
    NexDSButton phOlc = NexDSButton(4, 2, "olc");
    NexText phO = NexText(4, 4, "phO");
    NexText sPd = NexText(4, 6, "sPd");
    NexText g0 = NexText(4, 1, "g0");
    NexDSButton gub1 = NexDSButton(4, 9, "gub1");
    NexDSButton gub2 = NexDSButton(4, 11, "gub2");
    NexDSButton gub3 = NexDSButton(4, 12, "gub3");
    NexText phAciklama = NexText(4, 1, "phAciklama");

NexTouch *nex_listen_list[] =
{
  &b0,
  &nemler,
  &templer,
  &kayalar,
  &limitK,
  &option,
  &ana_menu,
  &set,
  &opt,
  &PageH,
  &activate,
  &phOlc,
  &settings,
  &geri1,
  &geri2,
  &geri3,
  &ppH,
  &wF,
  &nemOnay,
  &ofOnay,
  &gub1,
  &gub2,
  &gub3,
  &kilit,
  &tarihAyar,
  &saatAyar,


  NULL
};

void setPin()
{
  pinMode(sulama, OUTPUT);
  pinMode(gubre1, OUTPUT);
  pinMode(gubre2, OUTPUT);
  pinMode(gubre3, OUTPUT);
  pinMode(nemlendirici, OUTPUT);
 
  pinMode(fan, OUTPUT);
  pinMode(LED, OUTPUT);
  pinMode(kapiKilit, OUTPUT);

  pinMode(suDepo,INPUT);
  pinMode(g1Depo,INPUT);
  pinMode(g2Depo,INPUT);
  pinMode(g3Depo,INPUT);
}

void b0PopCallback(void *ptr) // şifre ile giriş yapma
{
  uint32_t g;
  giris.getValue(&g);

    if(g==1)
    { 

    HMISerial.print("er.txt=");
    HMISerial.print("\"");
    HMISerial.print("Giris Yapiliyor");
    HMISerial.print("\"");
    HMISerial.write(0xff);
    HMISerial.write(0xff);
    HMISerial.write(0xff);

    for (int i = 0; i <= 10; i++)
    {
      j0.setValue(i * 10);
    }

    ana_menu.show();
    dt = clock.getDateTime();
      tarihSaat();
    sayfa=0;
    wifiButon();
    gbrDurum();
    veriOku();
    if(wifiDurum == 1)
      {
        wifiYaz();
      }
    memset(buffer, 0, sizeof(buffer));
    st.toCharArray(buffer, sizeof(buffer));
    durum.setText(buffer); //LED DURUMU

    
   
    } 

}

void pH()
{

  analogBuffer = analogRead(PHSensorPin);    //read the analog value and store into the buffer
  analogBufferIndex  ;
  averageVoltage = analogBuffer * (float)VREF / 1024.0; // read the value more stable by the median filtering algorithm
  phValue = (3.5 * averageVoltage) + phOffset;

  if (phValue < phAlt)
  {
    digitalWrite(gubre1, LOW);
    digitalWrite(gubre2, LOW);
    digitalWrite(gubre3, LOW);
    gubreleme = 1;
    dt = clock.getDateTime();
    gubreD = dt.minute;
    phAciklama.setText("Olcum tamamlandi. Gubreleme basladi.");
  }
  else
  {
    memset(buffer, 0, sizeof(buffer));
    String p = String(phValue);
    p.toCharArray(buffer, sizeof(buffer));
    sPd.setText(buffer);

    phAciklama.setText("Olcum tamamlandi. Gubreleme gerekmiyor.");
    phActive = 0;
  }

}

void phOlcPopCallback(void *ptr)
{
  memset(buffer, 0, sizeof(buffer));
  phO.getText(buffer, sizeof(buffer));
  //  EEPROM.put(210, buffer);
  String s = String(buffer);
  phOffset = s.toFloat();

  pH();
}



void wFPopCallback(void *ptr)
{
   if (wifiDurum == 0)
  {
    wifiDurum = 1;

    
    wF.setText("WiFi        Acik");
  }

  else if (wifiDurum == 1)
  {
    wifiDurum = 0;

   
    wF.setText("WiFi        Kapali");

  }
}

void wifiButon()
{
  if (wifiDurum == 1)
  {
    wF.setText("WiFi        Acik");
  }
  else if (wifiDurum == 0)
  {
    wF.setText("WiFi        Kapali");
  }

}

void ledKontrol()
{
  
  unsigned long int x = dt.unixtime - oldHour;
  

  if (active == 1)
  {
    
    if (x >= ee8 * 3600 )
    {
       ledKapa();
      
    }
  }

  else if (active == 0)
  {
    
    if (x >= ee9 * 3600 )
    {
        ledAc();
        
    }

  }
}

void ledAc()
{
  digitalWrite(LED, LOW);
  active = 1;
  ledWF=2;
  dt = clock.getDateTime();
  oldHour = dt.unixtime;

  st = "";
  st += dt.hour;
  st += ":";
  st += dt.minute;
  st += "'de ACILDI";
  memset(buffer, 0, sizeof(buffer));
  st.toCharArray(buffer, sizeof(buffer));
  durum.setText(buffer);
}

void ledKapa()
{
  digitalWrite(LED, HIGH);
  active = 0;
  ledWF=1;
  dt = clock.getDateTime();
  oldHour = dt.unixtime;

  st = "";
  st += dt.hour;
  st += ":";
  st += dt.minute;
  st += "'de KAPANDI";
  memset(buffer, 0, sizeof(buffer));
  st.toCharArray(buffer, sizeof(buffer));
  durum.setText(buffer);
}

void activatePopCallback(void *ptr)
{
  dt = clock.getDateTime();
  oldHour = dt.unixtime;

  if (active == 1)
  {
    digitalWrite(LED, HIGH);
    active = 0;
    ledWF=1;
    memset(buffer, 0, sizeof(buffer));
    st = "KULLANICI ";
    st += dt.hour;
    st += ":";
    st += dt.minute;
    st += "'da KAPATTI.";
    st.toCharArray(buffer, sizeof(buffer));
    durum.setText(buffer);

  }
  else if (active == 0)
  {
    active = 1;
    ledWF=2;
    digitalWrite(LED, LOW);
    st = "KULLANICI ";
    st += dt.hour;
    st += ":";
    st += dt.minute;
    st += "'da ACTI.";
    st.toCharArray(buffer, sizeof(buffer));
    durum.setText(buffer);
  }
}

void tarihAyarPopCallback(void *ptr)
{
  dt=clock.getDateTime();

  uint32_t day;
  gun.getValue(&day);

  uint32_t month;
  ay.getValue(&month);

  uint32_t year;
  yil.getValue(&year);

  clock.setDateTime(year, month, day, dt.hour, dt.minute, 00);
  clock.begin();
  setG.setText("Tarih Guncellendi.");
}

void saatAyarPopCallback(void *ptr)
{

  dt=clock.getDateTime();

  uint32_t saat;
  sa.getValue(&saat);

  uint32_t dak;
  da.getValue(&dak);


  clock.setDateTime(dt.year, dt.month, dt.day, saat, dak, 00);
  clock.begin();
  setG.setText("Saat Guncellendi.");
}

int d2 = 100;
int d;
int m2 = 100;
void tarihSaat()
{
  dt = clock.getDateTime();
  m2 = dt.minute;
  n0.setValue(dt.hour);
  n1.setValue(dt.minute);


  d = dt.day;
  if (d != d2)
  {
    int aY = dt.month;
    int y = dt.year;
    String tar = "";
    tar += d;
    tar += ".";
    tar += aY;
    tar += ".";
    tar += y;
    memset(buffer, 0, sizeof(buffer));
    tar.toCharArray(buffer, sizeof(buffer));
    tarih.setText(buffer);
  }
}
void settingsPopCallback(void *ptr)
{
  set.show();
  sayfa=2;
  dt = clock.getDateTime();
  int g = dt.day; int a = dt.month; int y = dt.year; int d = dt.minute; int s = dt.hour;
  sa.setValue(s); da.setValue(d);
  gun.setValue(g); ay.setValue(a); yil.setValue(y);
}

void geri1PopCallback(void *ptr)
{
  ana_menu.show();
  sayfa=0;
  dt = clock.getDateTime();
      tarihSaat();
  veriOku();
  gbrDurum();
  wifiButon();
  if(wifiDurum == 1)
      {
        wifiYaz();
      }
  memset(buffer, 0, sizeof(buffer));
  st.toCharArray(buffer, sizeof(buffer));
  durum.setText(buffer);
}

void geri2PopCallback(void *ptr)
{
  ana_menu.show();
  sayfa=0;
  dt = clock.getDateTime();
      tarihSaat();
  veriOku();
  gbrDurum();
  wifiButon();
  if(wifiDurum == 1)
      {
        wifiYaz();
      }
  memset(buffer, 0, sizeof(buffer));
  st.toCharArray(buffer, sizeof(buffer));
  durum.setText(buffer);
}

void geri3PopCallback(void *ptr)
{
  ana_menu.show();
  sayfa=0;
  gbrDurum();
  dt = clock.getDateTime();
      tarihSaat();
  veriOku();
  if(wifiDurum == 1)
      {
        wifiYaz();
      }
  memset(buffer, 0, sizeof(buffer));
  st.toCharArray(buffer, sizeof(buffer));
  durum.setText(buffer);
}

void optionPopCallback(void *ptr) //opt ekranını güncelle
{
  opt.show();
  sayfa=3;
  if(wifiDurum == 1)
      {
        wifiOku();
      }
  limitYaz();
  
}

void ppHPopCallback(void *ptr)
{
  PageH.show();
  sayfa=4;
  
  memset(buffer, 0, sizeof(buffer));
  String off = String(phOffset);
  off.toCharArray(buffer, sizeof(buffer));
  phO.setText(buffer);

  memset(buffer, 0, sizeof(buffer));
  String f = String(phValue);
  f.toCharArray(buffer, sizeof(buffer));
  sPd.setText(buffer);
}

void limitYaz()
{

  n2.setValue(ee1);
  n3.setValue(ee2);
  n4.setValue(ee3);
  n5.setValue(ee4);
  n6.setValue(ee5);
  n8.setValue(ee6);
  n9.setValue(ee7);
  lAc.setValue(ee8);
  lKa.setValue(ee9);
  lmS.setValue(e10);

  memset(buffer, 0, sizeof(buffer));
  String off = String(sicaklikOffset);
  off.toCharArray(buffer, sizeof(buffer));
  sO.setText(buffer);



  memset(buffer, 0, sizeof(buffer));
  String f = String(phAlt);
  f.toCharArray(buffer, sizeof(buffer));
  pA.setText(buffer);



}

double Sicaklik(int analogOkuma)
{
  double sicaklik;
  sicaklik = log(((10240000 / analogOkuma) - 4750));
  sicaklik = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * sicaklik * sicaklik)) * sicaklik);
  sicaklik = sicaklik - 273.15;
  return sicaklik;
}




void veriOku()
{

  tT.update(); tL.update(); nT.update();

  int tempT = tT.getValue();
  int tempL = tL.getValue();
  int nem = nT.getValue();

  toprak_Temperature = Sicaklik(tempT) + sicaklikOffset;
  String asd = (String)toprak_Temperature;
  asd += " C";
  memset(buffer, 0, sizeof(buffer));
  asd.toCharArray(buffer, sizeof(buffer));
  text.setText(buffer);
  

  led_Temperature = Sicaklik(tempL) + sicaklikOffset;
  asd = (String)led_Temperature;
  asd += " C";
  memset(buffer, 0, sizeof(buffer));
  asd.toCharArray(buffer, sizeof(buffer));
  led1.setText(buffer);

  if(led_Temperature >= e10 )
  {
    digitalWrite(fan,LOW);
  }
  
  else if (led_Temperature <= e10 - 5)
  {
    digitalWrite(fan,HIGH);
  }

  nemT = map(nem,320,1023,100,0);
  asd = (String)nemT;
  asd += " %";
  memset(buffer, 0, sizeof(buffer));
  asd.toCharArray(buffer, sizeof(buffer));
  tN1.setText(buffer);

  if(nemT <= ee5)
  {
    suAc();
  }


  tempOda = dht.readTemperature();
  humOda = dht.readHumidity();


  if(isnan(tempOda))
    {
      oT1.setText("HATA");
    }
  else
  {
    

    String asd = (String)tempOda;
    asd += " C";
    memset(buffer, 0, sizeof(buffer));
    asd.toCharArray(buffer, sizeof(buffer));
    oT1.setText(buffer); 

  }

  if (isnan(humOda))
  {
    oN1.setText("HATA");
  }
  else
  {
    if (humOda <= ee1)
      {
        digitalWrite(nemlendirici,LOW);
        nemlendirme=true;
      }
     if (humOda >= ee2)
      {
        digitalWrite(nemlendirici,HIGH);
        nemlendirme=false;
      }

    String asd = (String)humOda;
    asd += " RH%";
    memset(buffer, 0, sizeof(buffer));
    asd.toCharArray(buffer, sizeof(buffer));
    oN1.setText(buffer);

    
  }
}


void wifiYaz()
{
  String yaz = "q";
  yaz += toprak_Temperature;
  
  yaz += "w";
  yaz += led_Temperature;

  yaz += "x";
  yaz += nemT;

  yaz += "+";
  yaz += tempOda;

  yaz += "*";
  yaz += humOda;

  yaz += ",";
  yaz += ledWF;

  yaz += "/";
  yaz += suWF;

  yaz += "=";
  yaz += su;

  yaz += "%";
  yaz += g1;

  yaz += "&";
  yaz += g2;

  yaz += "(";
  yaz += g3;

  yaz += ")";
  yaz += dt.day ;

  yaz += "j";
  yaz += dt.hour;

  yaz += "i";
  yaz += dt.minute;

  yaz += "k";
  yaz += dt.month;

  yaz += "m";

  Serial.println(yaz);
}
  String aStr="";
void wifiOku()
{
  if (Serial.available())
  {

    digitalWrite(52, HIGH);
                
    aStr = Serial.readString();

    int i20 = aStr.indexOf('q');

    int i21 = aStr.indexOf('w');
    int xtemp = aStr.substring(i20 + 1, i21).toFloat();
    if (xtemp != 0)
    {
      ee1 = xtemp;
    }

    int i22 = aStr.indexOf('x');
    int xtemp2 = aStr.substring(i21 + 1, i22).toFloat();
    if (xtemp2 != 0)
    {
      ee2 = xtemp2;
    }

    int i23 = aStr.indexOf('+');
    int xtemp3 = aStr.substring(i22 + 1, i23).toFloat();
    if (xtemp3 != 0)
    {
      ee3 = xtemp3;
    }

    int i24 = aStr.indexOf('*');
    int xtemp4 = aStr.substring(i23 + 1, i24).toFloat();
    if (xtemp4 != 0)
    {
      ee4 = xtemp4;
    }

    int i25 = aStr.indexOf(',');
    int xtemp5 = aStr.substring(i24 + 1, i25).toFloat();
    if (xtemp5 != 0)
    {
      ee5 = xtemp5;
    }

    int i26 = aStr.indexOf('/');
    int xtemp6 = aStr.substring(i25 + 1, i26).toFloat();
    if (xtemp6 != 0)
    {
      ee6 = xtemp6;
    }

    int i27 = aStr.indexOf('=');
    int xtemp7 = aStr.substring(i26 + 1, i27).toFloat();
    if (xtemp7 != 0)
    {
      ee7 = xtemp7;
    }

    int i28 = aStr.indexOf('%');
    int xtemp8 = aStr.substring(i27 + 1, i28).toFloat();
    if (xtemp8 != 0)
    {
      ee8 = xtemp8;
    }

    int i29 = aStr.indexOf('&');
    int xtemp9 = aStr.substring(i28 + 1, i29).toFloat();
    if (xtemp9 != 0)
    {
      ee9 = xtemp9;
    }

    int i30 = aStr.indexOf('(');
    int xtemp10 = aStr.substring(i29 + 1, i30).toFloat();
    if (xtemp10 != 0)
    {
      e10 = xtemp10;
    }

  }

  if (ee1 != tee1 && ee1 != 0)
  {
    EEPROM.write(110, ee1);
    tee1 = ee1;
  }
  if (ee2 != tee2 && ee2 != 0)
  {
    EEPROM.write(120, ee2);
    tee2 = ee2;
  }
  if (ee3 != tee3 && ee3 != 0)
  {
    EEPROM.write(130, ee3);
    tee3 = ee3;
  }
  if (ee4 != tee4 && ee4 != 0)
  {
    EEPROM.write(140, ee4);
    tee4 = ee4;
  }
  if (ee5 != tee5 && ee5 != 0)
  {
    EEPROM.write(150, ee5);
    tee5 = ee5;
  }
  if (ee6 != tee6 && ee6 != 0)
  {
    EEPROM.write(160, ee6);
    tee6 = ee6;
  }
  if (ee7 != tee7 && ee7 != 0)
  {
    EEPROM.write(170, ee7);
    tee7 = ee7;
  }
  if (ee8 != tee8 && ee8 != 0)
  {
    EEPROM.write(180, ee8);
    tee8 = ee8;
  }
  if (ee9 != tee9 && ee9 != 0)
  {
    EEPROM.write(190, ee9);
    tee9 = ee9;
  }
  if (e10 != te10 && e10 != 0)
  {
    EEPROM.write(230, e10);
    te10 = e10;
  }

}


void nemOnayPopCallback(void *ptr)
{

  uint32_t x5;
  n6.getValue(&x5);
  if(ee5!=x5)
  {ee5=x5;}
  //EEPROM.update(add_NemAlt_Toprak,ee5);


  memset(buffer, 0, sizeof(buffer));
  pA.getText(buffer, sizeof(buffer));
   // EEPROM.put(220, buffer);
  String str = String(buffer);
  if(phAlt != str.toFloat())
  {phAlt = str.toFloat();}

  HMISerial.print("g0.txt=");
  HMISerial.print("\"");
  HMISerial.print("Toprak nem degeri ve pH degeri guncellendi.");
  HMISerial.print("\"");
  HMISerial.write(0xff);
  HMISerial.write(0xff);
  HMISerial.write(0xff);
}

uint32_t lm;
void ofOnayPopCallback(void *ptr)
{
  memset(buffer, 0, sizeof(buffer));
  sO.getText(buffer, sizeof(buffer));
  String s = String(buffer);
  
  if(sicaklikOffset != s.toFloat())
  {
  sicaklikOffset = s.toFloat();
  }
  
  lmS.getValue(&lm);
  if(e10 != lm)
  {e10=lm;}
   // EEPROM.update(230, lm);
  

  
  HMISerial.print("g0.txt=");
  HMISerial.print("\"");
  HMISerial.print("Sicaklik offset ve LED max guncellendi.");
  HMISerial.print("\"");
  HMISerial.write(0xff);
  HMISerial.write(0xff);
  HMISerial.write(0xff);
}

void nemlerPopCallback(void *ptr)
{
  uint32_t x;
  n2.getValue(&x);
  if(ee1!=x)
  {ee1=x;}
  //EEPROM.update(add_NemAlt_Oda, ee1);
  

  uint32_t x2;
  n3.getValue(&x2);
  if(ee2!=x2)
  {ee2=x2;}
  //EEPROM.update(add_NemUst_Oda, ee2);
  

  HMISerial.print("g0.txt=");
  HMISerial.print("\"");
  HMISerial.print("Alt ve ust nem degerleri guncellendi.");
  HMISerial.print("\"");
  HMISerial.write(0xff);
  HMISerial.write(0xff);
  HMISerial.write(0xff);
}

void templerPopCallback(void *ptr)
{
  uint32_t x3;
  n4.getValue(&x3);
  if(ee3!=x3)
  {ee3=x3;}
  // EEPROM.update(add_TempAlt_Oda, ee3);
  

  uint32_t x4;
  n5.getValue(&x4);
  if(ee4!=x4)
  {ee4=x4;}
  // EEPROM.update(add_TempUst_Oda, ee4);
  



  HMISerial.print("g0.txt=");
  HMISerial.print("\"");
  HMISerial.print("Alt ve ust sicaklik degerleri guncellendi.");
  HMISerial.print("\"");
  HMISerial.write(0xff);
  HMISerial.write(0xff);
  HMISerial.write(0xff);

}

void kayalarPopCallback(void *ptr)
{


  uint32_t x6;
  n8.getValue(&x6);
  if(ee6 != x6)
  {ee6 = x6;}
  //EEPROM.update(add_Sulama_Oda, ee6);
  

  uint32_t x7;
  n9.getValue(&x7);
  if(ee7 != x7)
  {ee7 = x7;}
  //EEPROM.update(add_Gubre_Oda, ee7);

  HMISerial.print("g0.txt=");
  HMISerial.print("\"");
  HMISerial.print("Sulama ve gubreleme sureleri guncellendi.");
  HMISerial.print("\"");
  HMISerial.write(0xff);
  HMISerial.write(0xff);
  HMISerial.write(0xff);
}

void limitKPopCallback(void *ptr) //opt ekranını eeproma kaydet
{
  uint32_t lAcS;
  lAc.getValue(&lAcS);
  if(ee8 != lAcS)
  {ee8 = lAcS;}
   // EEPROM.update(add_led_Acik, lAcS);
  

  uint32_t lKS;
  lKa.getValue(&lKS);
  if(ee9 != lKS)
  {ee9 = lKS;}
  //EEPROM.update(add_led_Kapali, lKS);
  

  HMISerial.print("g0.txt=");
  HMISerial.print("\"");
  HMISerial.print("LED acik ve kapali kalma sureleri guncellendi.");
  HMISerial.print("\"");
  HMISerial.write(0xff);
  HMISerial.write(0xff);
  HMISerial.write(0xff);
  
}
void kilitPopCallback(void *ptr)
{
  kapiDurum=true;
  digitalWrite(kapiKilit,HIGH);
  kT=millis();
}

void suAc()
{
  digitalWrite(sulama, LOW);
  dt = clock.getDateTime();
  suD = dt.minute;
  sula = 1;
  suWF=2;
  sDT.setText("Sulama Basladi.");
}

void suKapa()
{
  digitalWrite(sulama, HIGH);
  dt = clock.getDateTime();
  suD = dt.minute;
  sula = 0;
  suWF=1;
  sDT.setText("Sulama Bitti.");
}

void suKontrol()
{
  int s = dt.minute - suD;
  if (s < 0)
  {
    s = s + 60;
  }
  if (s >= ee6)
  {
    suKapa();
  }
}

void gubreKontrol()
{
  int s2 = dt.minute - gubreD;

  if (s2 < 0)
  {
    s2 = s2 + 60;
  }
  if (s2 >= ee7)
  {
    digitalWrite(gubre1, HIGH);
    digitalWrite(gubre2, HIGH);
    digitalWrite(gubre3, HIGH);
    gubreleme = 0;
    
  }
}
void gub1PopCallback(void *ptr)
{

  digitalWrite(gubre1, LOW);
  gubreleme = 1;
  dt = clock.getDateTime();
  gubreD = dt.minute;
  phAciklama.setText("1. Gubreleme basladi.");
  
}

void gub2PopCallback(void *ptr)
{

  digitalWrite(gubre2, LOW);
  gubreleme = 1;
  dt = clock.getDateTime();
  gubreD = dt.minute;
  phAciklama.setText("2. Gubreleme basladi.");


}

void gub3PopCallback(void *ptr)
{

  digitalWrite(gubre3, LOW);
  gubreleme = 1;
  dt = clock.getDateTime();
  gubreD = dt.minute;
  phAciklama.setText("3. Gubreleme basladi.");

}

void gbrDurum()
{
  if(gubreleme==1)
   gDT.setText("Gubreleniyor.");

  else 
   gDT.setText("Gubreleme bitti.");
}

void sulaDurum()
{
  if(sula==1)
   gDT.setText("Sulanıyor.");

  else 
   gDT.setText("Sulama bitti.");
}

void depoKontrol()
{
  if(digitalRead(suDepo) == HIGH)
    {
      su=2;
    }
    else
    {
      su=1;
    }

    if(digitalRead(g1Depo) == HIGH)
    {
      g1=2;
    }
    else
    {
      g1=1;
    }

    if(digitalRead(g2Depo) == HIGH)
    {
      g2=2;
    }
    else
    {
      g2=1;
    }

    if(digitalRead(g3Depo)== HIGH)
    {
      g3=2;
    }
    else
    {
      g3=1;
    }
}

void setup() 
{
    setPin();

    digitalWrite(sulama, HIGH);
    digitalWrite(gubre1, HIGH);
    digitalWrite(gubre2, HIGH);
    digitalWrite(gubre3, HIGH);
    digitalWrite(nemlendirici, HIGH);
    
    digitalWrite(fan, HIGH);
    digitalWrite(LED, HIGH);
    digitalWrite(kapiKilit, LOW);

    HMISerial.begin(9600);
    HMISerial.print("baud=115200");
    HMISerial.write(0xff);
    HMISerial.write(0xff);
    HMISerial.write(0xff);
    HMISerial.end();

    nexInit();

    b0.attachPop(b0PopCallback, &b0); 
    settings.attachPop(settingsPopCallback, &settings);
    ppH.attachPop(ppHPopCallback, &ppH);//ph ölçümü
    option.attachPop(optionPopCallback, &option);//opt ekranını güncelle
    geri1.attachPop(geri1PopCallback, &geri1);//ana ekranını güncelle
    geri2.attachPop(geri2PopCallback, &geri2);//ana ekranını güncelle
    geri3.attachPop(geri3PopCallback, &geri3);//ana ekranını güncelle
    nemler.attachPop(nemlerPopCallback, &nemler);
    templer.attachPop(templerPopCallback, &templer);
    kayalar.attachPop(kayalarPopCallback, &kayalar);
    nemOnay.attachPop(nemOnayPopCallback, &nemOnay);
    ofOnay.attachPop(ofOnayPopCallback, &ofOnay);
    limitK.attachPop(limitKPopCallback, &limitK);//opt ekranını eeproma kaydet
    kilit.attachPop(kilitPopCallback, &kilit);
    activate.attachPop(activatePopCallback, &activate);//ledi aç-kapa tuşu
    tarihAyar.attachPop(tarihAyarPopCallback, &tarihAyar);
    saatAyar.attachPop(saatAyarPopCallback, &saatAyar);//saati ayarla
    phOlc.attachPop(phOlcPopCallback, &phOlc);//ph ölçümü
    wF.attachPop(wFPopCallback, &wF);
    gub1.attachPop(gub1PopCallback, &gub1);
    gub2.attachPop(gub2PopCallback, &gub2);
    gub3.attachPop(gub3PopCallback, &gub3);
   
   
    dht.begin();
   
    Serial.begin(9600);
    HMISerial.begin(115200);
    clock.begin();
   // clock.setDateTime(__DATE__, __TIME__);
    dt=clock.getDateTime();
}

void loop() 
{ 
  
    if(millis() - mTime >= 60000 )
    {
      dt = clock.getDateTime();
      tarihSaat();
      veriOku();
      wifiButon();
      sulaDurum();
      gbrDurum();
      if(wifiDurum == 1)
      {
        wifiYaz();
        wifiOku();
      }
      mTime=millis();
    }
  

  if(millis() - lTime >= 60000)
  {
    ledKontrol();
    depoKontrol(); 

    if (gubreleme == 1)
      {
        gubreKontrol();
      }
    
    

    if (sula == 1)
      {
        suKontrol();
      }

    lTime = millis();
  }

  
    if(kapiDurum==true)
  {
    if (millis() - kT > 3000)
   {
     
      digitalWrite(kapiKilit,LOW);
      kapiDurum=false;
      kT = millis();
    }
  }

    nexLoop(nex_listen_list);
}

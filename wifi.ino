#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>

//#include <SoftwareSerial.h>
//SoftwareSerial wf(4, 5);

#define FIREBASE_HOST "samed-f9120.firebaseio.com"
#define FIREBASE_AUTH "2IEE2e5Q4Sj1Q0F0YelxuO6eIbGzo5OvEfX9qndc"
#define WIFI_SSID "Superonline"
#define WIFI_PASSWORD "hakkibulut.34"


int y=13,m=12;
unsigned long int t=0;
int serialDelay=20;
void setup() 
{
 // Serial.setTimeout(10);
  Serial.begin(9600); 
//  wf.begin(9600);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);

  pinMode(y,OUTPUT);
  pinMode(m,OUTPUT);
  digitalWrite(y,HIGH);
  digitalWrite(m,HIGH);
}
int ee1=0,ee2=0,ee3=0,ee4=0,ee5=0,ee6=0,ee7=0,ee8=0,ee9=0,e10=0,su=0,g1=0,g2=0,g3=0,led=0,sula=0;
int tee1=1,tee2=1,tee3=1,tee4=1,tee5=1,tee6=1,tee7=1,tee8=1,tee9=1,te10=1;
String yaz = "";

float tT1=0,tT2=0,tT3=0,tT4=0;
float tL1=0,tL2=0,tL3=0,tL4=0;
float tH1=0,tH2=0,tH3=0,tH4=0;

float bT1=0,bT2=0,bT3=0,bT4=0;
float bH1=0,bH2=0,bH3=0,bH4=0;

void loop() 
{

  Firebase.setFloat("loop", 1);
  digitalWrite(y,LOW);

  if (Serial.available()) 
  {
    Firebase.setFloat("AKTIF", 1);
    String asd=Serial.readString();
   
    digitalWrite(y,HIGH);
    digitalWrite(m,LOW);
//****** Toprak Temp *******//
    int i=asd.indexOf('=');

    int i1 = asd.indexOf('a');
    tT1 = asd.substring(i+1,i1).toFloat();

    int i2 = asd.indexOf('b');
    tT2 = asd.substring(i1+1,i2).toFloat();

    int i3 = asd.indexOf('c');
    tT3 = asd.substring(i2+1,i3).toFloat();

    int i4 = asd.indexOf('d');
    tT4 = asd.substring(i3+1,i4).toFloat();

//****** LED Sıcaklık *******//   

    int i5 = asd.indexOf('e');
    tL1 = asd.substring(i4+1,i5).toFloat();

    int i6 = asd.indexOf('f');
    tL2 = asd.substring(i5+1,i6).toFloat();

    int i7 = asd.indexOf('g');
    tL3 = asd.substring(i6+1,i7).toFloat();

    int i8 = asd.indexOf('h');
    tL4 = asd.substring(i7+1,i8).toFloat();

//****** Toprak Hum *******//  

    int i9 = asd.indexOf('i');
    tH1 = asd.substring(i8+1,i9).toFloat();

    int i10 = asd.indexOf('j');
    tH2 = asd.substring(i9+1,i10).toFloat();

    int i11 = asd.indexOf('k');
    tH3 = asd.substring(i10+1,i11).toFloat();

    int i12 = asd.indexOf('l');
    tH4 = asd.substring(i11+1,i12).toFloat();

//****** Bölüm Temp *******// 

    int i13 = asd.indexOf('m');
    bT1 = asd.substring(i12+1,i13).toFloat();

    int i14 = asd.indexOf('n');
    bT2 = asd.substring(i13+1,i14).toFloat();

    int i15 = asd.indexOf('o');
    bT3 = asd.substring(i14+1,i15).toFloat();

    int i16 = asd.indexOf('p');
    bT4 = asd.substring(i15+1,i16).toFloat();

//****** Bölüm Hum *******// 

    int i17 = asd.indexOf('q');
    bH1 = asd.substring(i16+1,i17).toFloat();

    int i18 = asd.indexOf('r');
    bH2 = asd.substring(i17+1,i18).toFloat();

    int i19 = asd.indexOf('s');
    bH3 = asd.substring(i18+1,i19).toFloat();

    int i20 = asd.indexOf('t');
    bH4 = asd.substring(i19+1,i20).toFloat();

//******    EEPROM    *******//  

    int i21 = asd.indexOf('u');
    su = asd.substring(i20+1,i21).toFloat();

    int i22 = asd.indexOf('x');
    g1 = asd.substring(i21+1,i22).toFloat();
//
    int i23 = asd.indexOf('v');
    g2= asd.substring(i22+1,i23).toFloat();
//
    int i24 = asd.indexOf('y');
    g3 = asd.substring(i23+1,i24).toFloat();
//
    int i25 = asd.indexOf('z');
    led = asd.substring(i24+1,i25).toFloat();

    int i26 = asd.indexOf('+');
    sula = asd.substring(i25+1,i26).toFloat();
//
//    int i27 = asd.indexOf('-');
//    ee7 = asd.substring(i26+1,i27).toFloat();
//
//    int i28 = asd.indexOf('/');
//    ee8 = asd.substring(i27+1,i28).toFloat();
//
//    int i29 = asd.indexOf('*');
//    ee9 = asd.substring(i28+1,i29).toFloat();
//
//    int i30 = asd.indexOf(',');
//    e10 = asd.substring(i29+1,i30).toFloat();



  }


 //################################ BULUTA YAZ ######################################//
 //*********************** Toprak - LED Sıcaklık - Nem ******************************//

  if(tT1!=0)
    Firebase.setFloat("tT1", tT1);
  if(tT2!=0)
    Firebase.setFloat("tT2", tT2);
  if(tT3!=0)
    Firebase.setFloat("tT3", tT3);
  if(tT4!=0)
    Firebase.setFloat("tT4", tT4);
  
  if(tL1!=0)
    Firebase.setFloat("tL1", tL1);
  if(tL2!=0)
    Firebase.setFloat("tL2", tL2);
  if(tL3!=0)
    Firebase.setFloat("tL3", tL3);
  if(tL4!=0)
    Firebase.setFloat("tL4", tL4);
  
  if(tH1!=0)
    Firebase.setFloat("tH1", tH1);
  if(tH2!=0)
    Firebase.setFloat("tH2", tH2);
  if(tH3!=0)
    Firebase.setFloat("tH3", tH3);
  if(tH4!=0)
    Firebase.setFloat("tH4", tH4);

  
  //*************************** Bölüm Sıcaklık - Nem **********************************//

  if(bH1!=0)
    Firebase.setFloat("bH1", bH1);
  if(bH2!=0)
    Firebase.setFloat("bH2", bH2);
  if(bH3!=0)
    Firebase.setFloat("bH3", bH3);
  if(bH4!=0)
    Firebase.setFloat("bH4", bH4);

  if(bT1!=0)
    Firebase.setFloat("bT1", bT1);
  if(bT2!=0)
    Firebase.setFloat("bT2", bT2);
  if(bT3!=0)
    Firebase.setFloat("bT3", bT3);
  if(bT4!=0)
    Firebase.setFloat("bT4", bT4);

//*************************** LİMİT DEĞERLERİ **********************************//
  if(su!=0)
    Firebase.setFloat("su", su);
  if(g1!=0)
    Firebase.setFloat("g1", g1);
  if(g2!=0)
    Firebase.setFloat("g2", g2);
  if(g3!=0)
    Firebase.setFloat("g3", g3);
  if(led!=0)
    Firebase.setFloat("led", led);
  if(sula!=0)
    Firebase.setFloat("sula", sula);
//  if(ee7!=0)
//    Firebase.setFloat("ee7", ee7);
//  if(ee8!=0)
//    Firebase.setFloat("ee8", ee8);
//  if(ee9!=0)
//    Firebase.setFloat("ee9", ee9);
//  if(e10!=0)
//    Firebase.setFloat("e10", e10);



  ee1=Firebase.getInt("ee1");
  ee2=Firebase.getInt("ee2");
  ee3=Firebase.getInt("ee3");
  ee4=Firebase.getInt("ee4");
  ee5=Firebase.getInt("ee5");
  ee6=Firebase.getInt("ee6");
  ee7=Firebase.getInt("ee7");
  ee8=Firebase.getInt("ee8");
  ee9=Firebase.getInt("ee9");
  e10=Firebase.getInt("e10");

  if(ee1!=0 && ee2!=0 && ee3!=0 && ee4!=0 && ee5!=0 && ee6!=0 && ee7!=0 && ee8!=0 && ee9!=0 && e10!=0)
  {
    Seriyaz();
  }
  
}

void Seriyaz()
{



  yaz = "";
  yaz = "t";

  yaz +=ee1; yaz +="u";
  yaz +=ee2; yaz +="x";
  yaz +=ee3; yaz +="v";
  yaz +=ee4; yaz +="y";
  yaz +=ee5; yaz +="z";
  yaz +=ee6; yaz +="+";
  yaz +=ee7; yaz +="-";
  yaz +=ee8; yaz +="/";
  yaz +=ee9; yaz +="*";
  yaz +=e10; yaz +=",";

  Serial.println(yaz);
  delay(serialDelay);
  
}
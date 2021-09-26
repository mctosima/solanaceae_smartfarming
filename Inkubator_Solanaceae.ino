#include <virtuabotixRTC.h>
#include <SoftwareSerial.h>
#include <DHT.h>
#include <Servo.h>
#define sensorsuhu 2


virtuabotixRTC RTCku(3, 4, 12);
SoftwareSerial SIM800L(13, 9);
DHT dht(sensorsuhu,DHT11);
Servo s1;

const int water_pump = 7;
const int TRIG_PIN_terong = 8;
const int ECHO_PIN_terong = 10;
const int led = 5;
const int led2 = 6;
const int senslembab = A0;
const int LDR = A1;
int value;
int nilai;
int temperatur;
int intensity2;
int tinggi_terong;
unsigned long duration_terong;
int distance_terong;
int prediksi;
int arrPrediksi[8];
int a = 0 && a <=8;
int mean;
int jumlah;

void setup() {
  Serial.begin(9600);
  SIM800L.begin(9600);
  dht.begin(9600);
  s1.attach(11);
  pinMode(TRIG_PIN_terong, OUTPUT);
  pinMode(ECHO_PIN_terong, INPUT);
  pinMode(water_pump, OUTPUT);
  pinMode(led, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(senslembab,INPUT);
  pinMode(sensorsuhu, INPUT);
  pinMode(LDR, INPUT);
  //RTCku.setDS1302Time(20, 34, 14, 7, 16, 3, 2021); //(Hanya dipakai saat setting awal)
  
  while(!Serial);
  SIM800L.write("AT+CMGF=1\r\n");
  Serial.println("Layanan SMS aktif");
  delay(2000);
  SIM800L.write("AT+CMGS=\"082377200227\"\r\n");//Masukan nomor tujuan
  delay(500);
  SIM800L.print("Inkubator ON");

  SIM800L.write((char)26);
  delay(500);
}

void loop() {
  RTCku.updateTime();
  
  String jam, menit, detik, tanggal, bulan, tahun;
  jam = RTCku.hours;
  menit = RTCku.minutes;
  detik = RTCku.seconds;

  tanggal = RTCku.dayofmonth;
  bulan = RTCku.month;
  tahun = RTCku.year;

  Serial.println("Waktu = " + jam +":"+ menit +":"+ detik);
  kelembaban();
  Serial.println("Tingkat Kelembaban tanah : " + (String)nilai + "% atau " + (String)value);
  cahaya();
  Serial.println("intensitas cahaya: " + (String)intensity2 + "%\n");

  //bagian dht11
  int temperatur = dht.readTemperature();
  if(isnan (temperatur)){ temperatur = 0;}
  Serial.println("Suhu Inkubator : " + (String)temperatur + " derajat celcius");
  
  //bagian kondisi servo muter kanopi
  if (temperatur >= 32){
    s1.write(90);
    //Serial.println("Kanopi Terbuka");
  }
  else {
      s1.write(0);
    //Serial.println("Kanopi Tertutup");
    }

  //bagian ultrasonik
  digitalWrite(TRIG_PIN_terong, HIGH);
  delayMicroseconds(5);
  digitalWrite(TRIG_PIN_terong, LOW);
  duration_terong = pulseIn(ECHO_PIN_terong, HIGH);
  distance_terong = duration_terong / 58.2;
  tinggi_terong = 24 - distance_terong ;
  if(tinggi_terong < 0){
    tinggi_terong = 0;
  }
  Serial.println("Tinggi terong = " +(String)tinggi_terong + " cm");
 // delay(500);
  
//  if(RTCku.hours == 15 && RTCku.minutes == 42 && RTCku.seconds == 11){
//    SIM800L.write("AT+CMGS=\"082377200227\"\r\n");//Masukan nomor tujuan
//    delay(500);
//    kirimsms();
//    
//    SIM800L.print("Suhu = " + (String)temperatur + " Derajat Celcius    # ");
//    SIM800L.print("Tinggi Terong = " + (String)tinggi_terong + " cm     # ");
//    //a++;
//    arrPrediksi[0] += tinggi_terong;
//    //delay(1000);
//    SIM800L.write((char)26);
//    delay(500);
//    }
// if(RTCku.hours == 15 && RTCku.minutes == 43 && RTCku.seconds == 10){
//    SIM800L.write("AT+CMGS=\"082377200227\"\r\n");//Masukan nomor tujuan
//    delay(500);
//    kirimsms();
//    
//    SIM800L.print("Suhu = " + (String)temperatur + " Derajat Celcius    # ");
//    SIM800L.print("Tinggi Terong = " + (String)tinggi_terong + " cm      # ");
//    arrPrediksi[1] += tinggi_terong;
//    //delay(1000);
//    SIM800L.write((char)26);
//    delay(500);
//    }
// if(RTCku.hours == 15 && RTCku.minutes == 44 && RTCku.seconds == 10){
//    SIM800L.write("AT+CMGS=\"082377200227\"\r\n");//Masukan nomor tujuan
//    delay(500);
//    kirimsms();
//    
//    SIM800L.print("Suhu = " + (String)temperatur + " Derajat Celcius    # ");
//    SIM800L.print("Tinggi Terong = " + (String)tinggi_terong + " cm      # ");
//    arrPrediksi[2] += tinggi_terong;  
//    //delay(1000);
//    SIM800L.write((char)26);
//    delay(500);
//    }
//  if(RTCku.hours == 15 && RTCku.minutes == 45 && RTCku.seconds == 10){
//    SIM800L.write("AT+CMGS=\"082377200227\"\r\n");//Masukan nomor tujuan
//    delay(500);
//    kirimsms();
//    
//    SIM800L.print("Suhu = " + (String)temperatur + " Derajat Celcius    # ");
//    SIM800L.print("Tinggi Terong = " + (String)tinggi_terong + " cm      # ");
//    arrPrediksi[3] += tinggi_terong;
//    //delay(1000);
//    SIM800L.write((char)26);
//    delay(500);
//    }
// if(RTCku.hours == 15 && RTCku.minutes == 46 && RTCku.seconds == 10){
//    SIM800L.write("AT+CMGS=\"082377200227\"\r\n");//Masukan nomor tujuan
//    delay(500);
//    kirimsms();
//    
//    SIM800L.print("Suhu = " + (String)temperatur + " Derajat Celcius    # ");
//    SIM800L.print("Tinggi Terong = " + (String)tinggi_terong + " cm      # ");
//    arrPrediksi[4] += tinggi_terong;
//    //delay(1000);
//    SIM800L.write((char)26);
//    delay(500);
//    }
//  if(RTCku.hours == 15 && RTCku.minutes == 47 && RTCku.seconds == 10){
//    SIM800L.write("AT+CMGS=\"082377200227\"\r\n");//Masukan nomor tujuan
//    delay(500);
//    kirimsms();
//    
//    SIM800L.print("Suhu = " + (String)temperatur + " Derajat Celcius    # ");
//    SIM800L.print("Tinggi Terong = " + (String)tinggi_terong + " cm      # ");
//    arrPrediksi[5] += tinggi_terong;
//    //delay(1000);
//    SIM800L.write((char)26);
//    delay(500);
//    }
//  if(RTCku.hours == 15 && RTCku.minutes == 48 && RTCku.seconds == 10){
//    SIM800L.write("AT+CMGS=\"082377200227\"\r\n");//Masukan nomor tujuan
//    delay(500);
//    kirimsms();
//    
//    SIM800L.print("Suhu = " + (String)temperatur + " Derajat Celcius    # ");
//    SIM800L.print("Tinggi Terong = " + (String)tinggi_terong + " cm      # ");
//    arrPrediksi[6] += tinggi_terong;
//    //delay(1000);
//    SIM800L.write((char)26);
//    delay(500);
//    }
  if(RTCku.hours == 17 && RTCku.minutes == 18 && RTCku.seconds == 11){
    SIM800L.write("AT+CMGS=\"082377200227\"\r\n");//Masukan nomor tujuan
    delay(500);
    kirimsms();
    
    SIM800L.print("Suhu = " + (String)temperatur + " Derajat Celcius    # ");
    SIM800L.print("Tinggi Terong = " + (String)tinggi_terong + " cm      # ");
    //arrPrediksi[7] += tinggi_terong;
    //delay(1000);
//    
   a++;
   arrPrediksi[a] += tinggi_terong;
   SIM800L.write((char)26);
   delay(1000);
//    delay(1000);
    Serial.println("Pesan kondisi saat ini telah terkirim");
    //delay(500);
    if (arrPrediksi[7] != 0){
      kirimprediksi();
      Serial.println("Pesan prediksi telah terkirim");
    }
    
  }
  delay(1000);
  if (tinggi_terong >= 10){
    if(RTCku.hours == 17 && RTCku.minutes == 20 && RTCku.seconds == 11){
      //kirim ke sim800L
      SIM800L.write("AT+CMGS=\"082377200227\"\r\n");//Masukan nomor tujuan
      delay(500);
      SIM800L.print("Tanaman Siap Dipindahkan");
      SIM800L.write((char)26);
      delay(1000);
      }  
  }
     
}

void kelembaban(){
  //bagian sensor kelembaban tanah
  value = analogRead(senslembab);
  nilai = map(value,1023,0,0,100);
  if (nilai > 100){
    nilai = 100;
  }
  if (nilai >= 70){
    //Serial.println("Waterpump Mati");
    digitalWrite(water_pump, HIGH);
  }
  else if (nilai < 70) {
   // Serial.println("Waterpump Menyala");
    digitalWrite(water_pump, LOW);
  }
}

//void suhu(){
  //bagian sensor kelembaban udara
//  float temperatur = dht.readTemperature();
//  if(isnan (temperatur)){ temperatur = 0;}
//  //bagian kondisi servo muter kanopi
//  
//  Serial.println("Suhu Inkubator : " + (String)temperatur + " derajat celcius");
//  
//  if (temperatur > 31){
//    s1.write(100);
//    //Serial.println("Kanopi Terbuka");
//  }
//  else {
//      s1.write(0);
//    //Serial.println("Kanopi Tertutup");
//    }
//}

void cahaya(){
  //bagian kondisi led berdasar intensitas cahaya
  int intensity = analogRead(LDR);
  intensity2 = map(intensity,0,1023,100,0);
  
  if(intensity2 < 45 ){
  digitalWrite(led, HIGH);
  digitalWrite(led2, HIGH);
  }
  else if(intensity2 >= 45 && intensity2 < 75){
  analogWrite(led, intensity2);
  analogWrite(led2, intensity2);
  }
  else{
 // Serial.println("Kondisi Terang, LED mati");
  digitalWrite(led, LOW);
  digitalWrite(led2, LOW);
  }
}

//void ultrasonik(){
//  digitalWrite(TRIG_PIN_terong, HIGH);
//  delayMicroseconds(5);
//  digitalWrite(TRIG_PIN_terong, LOW);
//  duration_terong = pulseIn(ECHO_PIN_terong, HIGH);
//  distance_terong = duration_terong / 58.2;
//  tinggi_terong = (26-distance_terong);
//  Serial.println("Tinggi terong yang dikirim = " +(String)tinggi_terong + " cm");
  
  //SIM800L.print("Tinggi Terong = " + (String)tinggi_terong + " cm    # ");
//}

//void kirimUltrasonik(){
//  ultrasonik();
//  Serial.println("Tinggi terong yang dikirim = " +(String)tinggi_terong + " cm");
//  SIM800L.print("Tinggi Terong = " + (String)tinggi_terong + " cm    # ");
//  
//}

void kirimsms(){
//    SIM800L.write("AT+CMGS=\"082377200227\"\r\n");//Masukan nomor tujuan
//    delay(500);
    kelembaban();
    SIM800L.print("Kelembaban Tanah = " + (String)nilai + "%    # ");
//    suhu();
//    SIM800L.print("Suhu Inkubator = ");
//    SIM800L.print(temperatur + "Derajat Celcius");
    cahaya();
    SIM800L.print("Intensitas Cahaya = " + (String)intensity2 + "%    # ");
//    kirimUltrasonik();
    //suhu();
    //SIM800L.print("Suhu Inkubator = " + (String)temperatur + "Derajat Celcius    #");
    //ultrasonik();
    
}

void kirimprediksi(){
    for (int i=0; i<=7; i++){
      jumlah = jumlah + arrPrediksi[a];
    }
    Serial.println("Jumlah tinggi = " + (String)jumlah);
    mean = jumlah / 8;
    prediksi = 10 / mean;
    Serial.print("Tanaman akan siap dipindahkan dalam " + (String)prediksi + " hari lagi ");
    SIM800L.print("Tanaman akan siap dipindahkan dalam " + (String)prediksi + " hari lagi ");
    //Serial.println("Pesan prediksi terkirim");

    arrPrediksi[0]=0;
    arrPrediksi[1]=0;
    arrPrediksi[2]=0;
    arrPrediksi[3]=0;
    arrPrediksi[4]=0;
    arrPrediksi[5]=0;
    arrPrediksi[6]=0;
    arrPrediksi[7]=0;
    a = 0;
}

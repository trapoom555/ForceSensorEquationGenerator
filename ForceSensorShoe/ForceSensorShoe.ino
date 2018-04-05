#define pin_F1 A0
#define pin_F2 A1
#define pin_F3 A2
#define pin_Buzzer 8
#define pin_redLED 9
#define pin_greenLED 10
String t = "";
String weight_string = "";
String percent_string = "";
float weight = 0;
float percent = 0;
int x = 0;
float maximum = 0;
float caution = 0;
float Vr1 = 0;
float Vr2 = 0;
float Vr3 = 0;
float Rcon = 1.1E5;
float Vs = 5;
float tot_F = 0;
//equation from calibration Sensor 1/Rf = m(F) + c ; m = 2.721E-6 c = -6.892E-7
float m = 3E-06;
float c = -6E-07;
float F_init = 0;
float F_diff1 = 0;
float F_diff2 = 0;
//declare function

float AverageOfDiff(float F_diff1 , float F_diff2){
  return (F_diff1 + F_diff2) / 2;
}
float VrToF_1(float Vr) { 
  if ( Vr != 0 && Vr != 1024 ) {
  return 1/(5.514E-01*((1024/Vr)-1))-(4.63E-2);
  }
  else {
    return 0;
  }
}

float VrToF_2(float Vr) { 
  if ( Vr != 0 && Vr != 1024 ) {
   return 1/(5.086E-01*((1024/Vr)-1))-(1.095E-1);
}
  else {
    return 0;
  }
}

float VrToF_3(float Vr) { 
  if ( Vr != 0 && Vr != 1024 ) {
  return 1/(4.439E-01*((1024/Vr)-1))-(5.604E-2);
  }
  else {
    return 0;
  }
}

void danger() {
  digitalWrite(pin_Buzzer,HIGH);
}

void noBuzzer() {
  digitalWrite(pin_Buzzer,LOW);
}

//
void setup() {
  Serial.begin(9600);
  pinMode(pin_F1,INPUT);
  pinMode(pin_F2,INPUT);
  pinMode(pin_F3,INPUT);
  pinMode(pin_Buzzer,OUTPUT);
  pinMode(pin_redLED,OUTPUT);
  pinMode(pin_greenLED,OUTPUT);
}

void loop() {
  noBuzzer();
  while(Serial.available()) {
    t += (char)Serial.read();
    for(int i = 0 ; i < t.length() ; i++ ) {
    if(t[i] == '?' ) {
      x = i;
    }
    if(t[i] == 'c' && t[i+1] == 'l' && t[i+2] == 'r') {
      t = "";
      caution = 0;
      maximum = 0;
      F_init = 0;
      F_diff1 = 0;
      F_diff2 = 0;
    }
    if(t[i] == 's' && t[i+1] == 'e' && t[i+2] == 't') {
      t = "";
      caution = 0;
      maximum = 0;
      F_init = VrToF_1(Vr1) + VrToF_2(Vr2) + VrToF_3(Vr3);
    }
    if(t[i] == 'C' && t[i+1] == 'a' && t[i+2] == 'l'&& t[i+3] == '5') {
      t = "";
      caution = 0;
      maximum = 0;
      F_diff1 = 5 - tot_F;
    }
    if(t[i] == 'C' && t[i+1] == 'a' && t[i+2] == 'l'&& t[i+3] == '1' && t[i+4] == '0') {
      t = "";
      caution = 0;
      maximum = 0;
      F_diff2 = 10 - tot_F;
    }
  }
  weight_string = (t.substring(0,x));
  weight = weight_string.toInt();
  percent_string = (t.substring(x+1));
  percent = percent_string.toInt();
  } // separate weight and percent by "?"
  /* Debug zone */
  //Serial.println(t);
  //Serial.println(weight);
  //delay(100);
  //Serial.println(percent);
  
  caution = weight * ( percent / 100 ); // calculate caution value
  maximum = 1.1 * caution; // calculate max value
  Vr1 = analogRead(pin_F1);
  Vr2 = analogRead(pin_F2);
  Vr3 = analogRead(pin_F3);
  tot_F = VrToF_1(Vr1) + VrToF_2(Vr2) + VrToF_3(Vr3) - F_init + AverageOfDiff(F_diff1 , F_diff2) ; 
    /* Debug zone */
//  Serial.println(VrToF_1(Vr1));
//  Serial.println(VrToF_2(Vr2));
//  Serial.println(VrToF_3(Vr3));
  Serial.println(F_diff1);
  Serial.println(F_diff2);
  Serial.println(tot_F);
  Serial.println("-------------");
  delay(500); 
//  // Condition zone
  if( caution !=0 && maximum != 0 && caution > 0 && maximum > 0) { // skip Condition zone when no input
    digitalWrite(pin_greenLED,HIGH);
    digitalWrite(pin_redLED,LOW);
  if( tot_F >= caution && tot_F <= maximum ) {
    for (int i = 0 ; i < 3 ; i++ ){
          danger();
          delay(300);
          noBuzzer();
          delay(300);
    }
  }
  else if( tot_F > maximum ) {
    danger();
    delay(1000);
  }
  else {
    noBuzzer();
  }
  }
  else {
    digitalWrite(pin_greenLED,LOW);
    digitalWrite(pin_redLED,HIGH);
  }

}

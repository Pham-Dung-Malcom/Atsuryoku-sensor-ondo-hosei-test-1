

double Offset = 2.5 ;
double slope = 2.33 ;
//double slope = 1 ;

int sample = 100;


double sensorValue_1 ;
double sensorValue_2 ;
double sensorValue_3 ;
double numberToDisplay_1 ;
double numberToDisplay_2 ;
double numberToDisplay_3 ;
int output_1 = 0,output_2 = 0,output_3 = 0;

double S1_base_line = 0.9 ;
double S2_base_line = 1.5 ;
double S3_base_line = 1.2 ;


double S1_delta = 0;
double S2_delta = 0;
double S3_delta = 0;

double S1_old_delta = 0;
double S2_old_delta = 0;
double S3_old_delta = 0;
int firt_time = 1;
int sokuteichuu = 0;
int ondohosei = 0;
int loop_count = 110;
int comport_hosei = 2;

const double VREF = 4.95; // Điện áp tham chiếu (Vref)
void(* resetFunc) (void) = 0;
void setup() {
  Serial.begin(9600); // Khởi động Serial
  delay(200);
    if (Serial) {
      comport_hosei = 0;
    } else {
     comport_hosei = 3;
    }
  pinMode(3, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  delay(200);
}

void loop() {

  if ( loop_count > 65 && firt_time == 0 )  { loop_count = 0 ;}
  // Đọc giá trị điện áp từ chân Analog A0
   sensorValue_1 = 0;
  for(int i = 0; i < sample ; i++)
  { sensorValue_1 = sensorValue_1 + analogRead(A0); }
    sensorValue_1 = sensorValue_1 / sample;
 
  sensorValue_2 = 0;
  for(int i = 0; i < sample ; i++)
  { sensorValue_2 = sensorValue_2 + analogRead(A1);  delay(1);  }
   sensorValue_2 = sensorValue_2 / sample;

   sensorValue_3 = 0;
  for(int i = 0; i < sample ; i++)
  { sensorValue_3 = sensorValue_3 + analogRead(A2);   delay(1); }
   sensorValue_3 = sensorValue_3 / sample;
   
 

 
  // Chuyển đổi giá trị analog thành giá trị điện áp (tính theo Vref)
   double voltage_1 = sensorValue_1 * (VREF  / 1023.0) ; // 1023: Giá trị analog tối đa
   double voltage_2 = sensorValue_2 * (VREF  / 1023.0) ; // 1023: Giá trị analog tối đa
   double voltage_3 = sensorValue_3 * (VREF / 1023.0) ; // 1023: Giá trị analog tối đa
   if(voltage_1 > 1.7){voltage_1 = voltage_1*0.99;}
   if(voltage_2 > 1.7){voltage_2 = voltage_2*0.99;}
   if(voltage_3 > 1.7){voltage_3 = voltage_3*0.99;}
  
  if (voltage_1 > VREF) {  voltage_1 = VREF; }
  if (voltage_2 > VREF) {  voltage_2 = VREF; }
  if (voltage_3 > VREF) {  voltage_3 = VREF; }

if(voltage_1 < 0) {voltage_1 = 0;}
if(voltage_2 < 0) {voltage_2 = 0;}
if(voltage_3 < 0) {voltage_3 = 0;}


Serial.print("S1: " );Serial.print(voltage_1); Serial.print("   " );
Serial.print("S2: " );Serial.print(voltage_2); Serial.print("   " );
Serial.print("S3: " );Serial.print(voltage_3); Serial.print("   " );
Serial.println("");


if(voltage_1 > 1.49  &&  voltage_2 > 2 && voltage_3 > 1.65)
{sokuteichuu = 1; loop_count  = 55; }
else
{sokuteichuu = 0;}
if(sokuteichuu == 0 && loop_count > 60)
{ 
  if(abs(voltage_1 - S1_base_line) <= 0.3 && abs(voltage_2 - S2_base_line) <= 0.3 && abs(voltage_3 - S3_base_line) <= 0.3 )
     { S1_delta = (voltage_1 - S1_base_line)*1.15 ;
       S2_delta = (voltage_2 - S2_base_line)*1.2 ;
       S3_delta = (voltage_3 - S3_base_line)*0.7 ;
      if(firt_time == 0)
      {
       if((S1_delta - S1_old_delta) > 0.08) {S1_delta = S1_old_delta ; }
       if((S2_delta - S2_old_delta) > 0.08) {S2_delta = S2_old_delta ; }
       if((S3_delta - S3_old_delta) > 0.08) {S3_delta = S3_old_delta ; }
      }
      firt_time = 0;

      Serial.println("Hosei actived");delay(100);
     }
    
  if(abs(voltage_1 - S1_base_line) > 0.3 && abs(voltage_2 - S2_base_line) > 0.3 && abs(voltage_3 - S3_base_line) > 0.3 && firt_time == 1)
  {   int S11_delta = abs(voltage_1 - S1_base_line) ;
      int S22_delta = abs(voltage_2 - S2_base_line) ;
      int S33_delta = abs(voltage_3 - S3_base_line) ;
      if(S11_delta - S1_old_delta < 0.1 && S22_delta - S2_old_delta < 0.1 && S33_delta - S3_old_delta < 0.1)
      {
         while(1){ 
            analogWrite(3,255 ); analogWrite(5,255 ); analogWrite(6,255 ); delay(3000);          
            analogWrite(3,0 ); analogWrite(5,0 );  analogWrite(6,0 );delay(3000);
            Serial.println("Outside operating temperature range");
            /*
           if(abs(voltage_1 - S1_base_line) < 0.3 && abs(voltage_2 - S2_base_line) < 0.3 && abs(voltage_3 - S3_base_line) < 0.3 )
            { resetFunc();
            break;
            }  
            */ 
                }     
       }
  }
loop_count = 0;   
}
///// Abnomrmal_check
   if(((voltage_1 - S1_base_line) < -0.5) || ((voltage_2 - S2_base_line) < -0.5) || ((voltage_3 - S3_base_line) < -0.5) )
    { while(1){ 
           analogWrite(3,255 ); analogWrite(5,255 ); analogWrite(6,255 ); delay(3000);          
           analogWrite(3,0 ); analogWrite(5,0 );  analogWrite(6,0 );delay(3000);

            Serial.println("The sensor is deformed outward");
              }
    }

   if(S1_delta == 0 && S2_delta == 0 && S3_delta == 0  )
   { while(1){ 
        analogWrite(3,255 ); analogWrite(5,255 ); analogWrite(6,255 ); delay(3000);          
        analogWrite(3,0 ); analogWrite(5,0 );  analogWrite(6,0 );delay(3000);

         Serial.println("Out of operating temperature range at start-up time");
              }
      }

    
///////////////////////////////



  // Ondohosei
voltage_1 = voltage_1 - S1_delta;
voltage_2 = voltage_2 - S2_delta;
voltage_3 = voltage_3 - S3_delta;
S1_old_delta = S1_delta;
S2_old_delta = S2_delta;
S3_old_delta = S3_delta;
///////
 Serial.print("S1_hosei: " );Serial.print(voltage_1); Serial.print("   " );
 Serial.print("S2_hosei: " );Serial.print(voltage_2); Serial.print("   " );
 Serial.print("S3_hosei: " );Serial.print(voltage_3); Serial.print("   " );
 Serial.println("");
////////


 Serial.print("S1_delta: " ); Serial.print(S1_delta); Serial.print("   " );
 Serial.print("S2_delta: " ); Serial.print(S2_delta); Serial.print("   " );
 Serial.print("S3_delta: " ); Serial.print(S3_delta); Serial.print("   " );
 Serial.println("");





Serial.print("sokuteijoutai: ");Serial.print(sokuteichuu);
Serial.println("");
Serial.print("ondohoseijoutai: ");Serial.print(ondohosei);
Serial.println("");

//delay(100);



if( voltage_1 < S1_base_line + 0.25 ) {output_1 = 0;}
if( voltage_2 < S2_base_line + 0.25 ) {output_2 = 0;}
if( voltage_3 < S3_base_line + 0.25 ) {output_3 = 0;}


//// 300g
if(voltage_1 >= S1_base_line + 0.2 && voltage_1 < 3.05)
{output_1 =(voltage_1*0.9+0.1)*255/4.70*0.85 + comport_hosei;}
if(voltage_2 >= S2_base_line + 0.2 && voltage_2 < 3.3)
{output_2 =(voltage_2*0.82+0.1)*255/4.70*0.85 + comport_hosei;}
if(voltage_3 >= S3_base_line + 0.2 && voltage_3 < 3)
{output_3 =(voltage_3*0.91+0.1)*255/4.70*0.85 + comport_hosei;}

//// 400g
if(voltage_1 >= 3.05 && voltage_1 <= 3.8)
{output_1 =(voltage_1*0.91+0.1)*255/4.70*0.89 + comport_hosei;}
if(voltage_2 >= 3.3 && voltage_2 <= 3.6)
{output_2 =(voltage_2*0.92+0.1)*255/4.70*0.92 + comport_hosei;}
if(voltage_3 >= 3 && voltage_3 <= 3.6)
{output_3 =(voltage_3*0.9+0.15)*255/4.70*0.94 + comport_hosei + 2;}


/// 500g
if(voltage_1 > 3.8 && voltage_1 <= 3.91)
{output_1 =(voltage_1*0.96+0.1)*255/4.70*0.89 + comport_hosei;}
if(voltage_2 > 3.6 && voltage_2 <= 4.5)
{output_2 =(voltage_2*0.95+0.18)*255/4.70*0.93 + comport_hosei;}
if(voltage_3 >= 3.6 && voltage_3 <= 4.5)
{output_3 =(voltage_3*0.93+0.17)*255/4.70*0.94 + comport_hosei + 2;}

/// 600g
if(voltage_1 > 3.91)
{output_1 =(voltage_1*0.955+0.1)*255/4.70*0.905 + comport_hosei;}
if(voltage_2 > 3.6)
{output_2 =(voltage_2*0.95+0.18)*255/4.70*0.93 + comport_hosei;}
if(voltage_3 > 3.95)
{output_3 =(voltage_3*0.91+0.17)*255/4.70*0.925 + comport_hosei + 2  ;}


if(output_1 > 218) {output_1 = 218 ;}
if(output_2 > 218) {output_2 = 218 ;}
if(output_3 > 218) {output_3 = 218 ;}


Serial.print("output_1: " );
Serial.println(output_1); 
Serial.print("output_2: " );
Serial.println(output_2); 
Serial.print("output_3: " );
Serial.println(output_3);


analogWrite(3,output_1);
analogWrite(5,output_2 );
analogWrite(6,output_3 );




/*
if(sokuteichuu == 0) 
 {int temperature = readInternalTemperature();
  Serial.print("Internal Temperature: ");
  Serial.print(temperature);
  Serial.println(" C");
 }
 */
 loop_count = loop_count + 1;
}

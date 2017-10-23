// program scris de Nicu FLORICA (niq_ro)
// DRL testat de Liviu HINOVEANU

int brightnessPin = 1;
int BatPin = A3;
int FscPin = A2;
int ButtonPin = 2;
int UBat = 0;        
int UFsc = 0;
int brightness = 0;
int brightup = 51; 
int ButtonState = 1; 
unsigned long timp = 0;
unsigned long debounce = 100;          
float KUBat = 4.95/5.;
float KUFsc = 4.95/5.;
int Uprag1 = 417/KUFsc; // U faza scurta 12,0 V
int Uprag2 = 446/KUBat; // U bat 12,8 V FOLOSITA LA FADE
int Uprag3 = 458/KUBat; // U bat 13,1 V

byte motorpornit = 0;
byte fazascurta = 0;

void setup() 
{
pinMode(1, OUTPUT);
pinMode(2, INPUT_PULLUP);
pinMode(A2, INPUT);
pinMode(A3, INPUT);
}

void loop() 
{
UBat = analogRead(BatPin);  // citire tensiune baterie
UFsc  = analogRead(FscPin); // citire tensiune faza scurta
//analogWrite(1, brightness);  // aprindere becuri FL la anumit procent (variabila)
ButtonState = digitalRead(ButtonPin);  // citire stare buton

if (UBat > Uprag3)  // daca a pornit motorul si alternatorul incarca
{
  motorpornit = 1;
}

if (UFsc < Uprag1)  // daca faza scurta e stinsa
{
  fazascurta = 0;
}
else fazascurta = 1;

if (fazascurta == 0)
{
if ((ButtonState == 0) && (millis() - timp > debounce) && (UBat > Uprag3)) // daca se apasa butonul si tensiunea bateriei e mai mare de 13,1V si faza scurta e stinsa
{
brightness = brightness + brightup;  // creste variabila
if (brightness  > 254 )  // daca variabila e maxim o face 0
{
brightness = 0;
}
timp = millis(); // se memoreaza timpul apasarii
} // aici se termina partea de modificare variabila

if (motorpornit == 1)   // daca tensiunea pe baterie mai mare de 13,1V si faza scurta stinsa
{
analogWrite(1, brightness);  // se comanda becul prin PWM
}
}

if ((motorpornit == 1) && (fazascurta == 1))  // daca tensiunea pe baterie e mai mare de 13,1V si pozitia aprinsa
{
analogWrite(1, 0);   // stinge luminile comandate pwm             
}

if ((UBat < Uprag2) && (motorpornit == 1))  // daca tensiunea pe baterie scade sub 12,4V si motorul este sau a fost pornit anterior
{    
for (int fadeValue = 255 ; fadeValue >= 0; fadeValue -= 5)   // stinge lent becurile
{
  
analogWrite(1, fadeValue);
delay(15);
}
motorpornit = 0;  // considera ca motorul e oprit
}

delay(10);  // pauza mica
}

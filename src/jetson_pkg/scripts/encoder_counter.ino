const int pinCanalAM1=32;
const int pinCanalBM1=33;
int contador1=0;
int state1=0;

//Interrupcion de Encoder 1
void IRAM_ATTR Encoder1()
{
  if (state1==1){
    contador1++;
    }
    else{
      contador1--;
      contador1--;
    }
}

//Interrupcion de Encoder 1B
void IRAM_ATTR Encoder1B()
{
  if (digitalRead(pinCanalBM1)==HIGH){
    state1=1;
  }
  else{
    state1=0;
  }
}

void setup() {
  Serial.begin(9600);
  
  pinMode(pinCanalAM1,INPUT);
  pinMode(pinCanalBM1,INPUT);
  attachInterrupt(pinCanalAM1,Encoder1,RISING);
  attachInterrupt(pinCanalBM1,Encoder1B,CHANGE);
  


}

void loop() {
 //Mostrar datos en Serial
  Serial.print("Motor1 = ");
  Serial.println(contador1);

  

}





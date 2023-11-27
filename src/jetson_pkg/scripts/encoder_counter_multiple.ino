const int pinCanalAM1=34;
const int pinCanalBM1=35;
int contador1=0;
int state1=0;

const int pinCanalAM2=32;
const int pinCanalBM2=33;
int contador2=0;
int state2=0;

const int pinCanalAM3=25;
const int pinCanalBM3=26;
int contador3=0;
int state3=0;

const int pinCanalAM4=14;
const int pinCanalBM4=27;
int contador4=0;
int state4=0;

const int pinCanalAM5=18;
const int pinCanalBM5=19;
int contador5=0;
int state5=0;

const int pinCanalAM6=5;
const int pinCanalBM6=17;
int contador6=0;
int state6=0;

const int pinCanalAM7=4;
const int pinCanalBM7=16;
int contador7=0;
int state7=0;

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

//Interrupcion de Encoder 2
void IRAM_ATTR Encoder2()
{
  if (state2==1){
    contador2++;
    }
    else{
      contador2--;
      contador2--;
    }
}

//Interrupcion de Encoder 2B
void IRAM_ATTR Encoder2B()
{
  if (digitalRead(pinCanalBM2)==HIGH){
    state2=1;
  }
  else{
    state2=0;
  }
}


//Interrupcion de Encoder 3
void IRAM_ATTR Encoder3()
{
  if (state3==1){
    contador3++;
    }
    else{
      contador3--;
      contador3--;
    }
}

//Interrupcion de Encoder 3B
void IRAM_ATTR Encoder3B()
{
  if (digitalRead(pinCanalBM3)==HIGH){
    state3=1;
  }
  else{
    state3=0;
  }
}

//Interrupcion de Encoder 4
void IRAM_ATTR Encoder4()
{
  if (state4==1){
    contador4++;
    }
    else{
      contador4--;
      contador4--;
    }
}

//Interrupcion de Encoder 4B
void IRAM_ATTR Encoder4B()
{
  if (digitalRead(pinCanalBM4)==HIGH){
    state4=1;
  }
  else{
    state4=0;
  }
}


//Interrupcion de Encoder 5
void IRAM_ATTR Encoder5()
{
  if (state5==1){
    contador5++;
    }
    else{
      contador5--;
      contador5--;
    }
}

//Interrupcion de Encoder 5B
void IRAM_ATTR Encoder5B()
{
  if (digitalRead(pinCanalBM5)==HIGH){
    state5=1;
  }
  else{
    state5=0;
  }
}

//Interrupcion de Encoder 6
void IRAM_ATTR Encoder6()
{
  if (state6==1){
    contador6++;
    }
    else{
      contador6--;
      contador6--;
    }
}

//Interrupcion de Encoder 6B
void IRAM_ATTR Encoder6B()
{
  if (digitalRead(pinCanalBM6)==HIGH){
    state6=1;
  }
  else{
    state6=0;
  }
}

//Interrupcion de Encoder 7
void IRAM_ATTR Encoder7()
{
  if (state7==1){
    contador7++;
    }
    else{
      contador7--;
      contador7--;
    }
}

//Interrupcion de Encoder 7B
void IRAM_ATTR Encoder7B()
{
  if (digitalRead(pinCanalBM7)==HIGH){
    state7=1;
  }
  else{
    state7=0;
  }
}

void setup() {
  Serial.begin(9600);
  
  pinMode(pinCanalAM1,INPUT);
  pinMode(pinCanalBM1,INPUT);
  attachInterrupt(pinCanalAM1,Encoder1,RISING);
  attachInterrupt(pinCanalBM1,Encoder1B,CHANGE);
  
  pinMode(pinCanalAM2,INPUT);
  pinMode(pinCanalBM2,INPUT);
  attachInterrupt(pinCanalAM2,Encoder2,RISING);
  attachInterrupt(pinCanalBM2,Encoder2B,CHANGE);

  pinMode(pinCanalAM3,INPUT);
  pinMode(pinCanalBM3,INPUT);
  attachInterrupt(pinCanalAM3,Encoder3,RISING);
  attachInterrupt(pinCanalBM3,Encoder3B,CHANGE);
  
  pinMode(pinCanalAM4,INPUT);
  pinMode(pinCanalBM4,INPUT);
  attachInterrupt(pinCanalAM4,Encoder4,RISING);
  attachInterrupt(pinCanalBM4,Encoder4B,CHANGE);

  pinMode(pinCanalAM5,INPUT);
  pinMode(pinCanalBM5,INPUT);
  attachInterrupt(pinCanalAM5,Encoder5,RISING);
  attachInterrupt(pinCanalBM5,Encoder5B,CHANGE);
  
  pinMode(pinCanalAM6,INPUT);
  pinMode(pinCanalBM6,INPUT);
  attachInterrupt(pinCanalAM6,Encoder6,RISING);
  attachInterrupt(pinCanalBM6,Encoder6B,CHANGE);

  pinMode(pinCanalAM7,INPUT);
  pinMode(pinCanalBM7,INPUT);
  attachInterrupt(pinCanalAM7,Encoder7,RISING);
  attachInterrupt(pinCanalBM7,Encoder7B,CHANGE);

}

void loop() {
 //Mostrar datos en Serial
  Serial.print("Motor1 = ");
  Serial.print(contador1);
  Serial.print("   Motor2 = ");
  Serial.print(contador2);
  Serial.print("   Motor3 = ");
  Serial.print(contador3);
  Serial.print("   Motor4 = ");
  Serial.print(contador4);
  Serial.print("   Motor5 = ");
  Serial.print(contador5);
  Serial.print("   Motor6 = ");
  Serial.print(contador6);
  Serial.print("   Motor7 = ");
  Serial.println(contador7);
  

}





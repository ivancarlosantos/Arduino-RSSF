/*
ARDUINO EMULANDO RADIUINO
 */

// variáveis utilizadas em diferentes aplicações do Radiuino
byte PacoteRX[52]; // Pacote de 52 bytes que será recebido pelo nó sensor
byte PacoteTX[52]; // Pacote de 52 bytes que será transmitido pelo nó sensor

float tensao; // Tensão utilizada pelo dispositivo. Para o Arduino é 5 Volts.
float temp;
float passo; // Passo do conversor AD
int vTemp; // variável obtida do conversor analógico digital que vai de 0 a 1023

int ID; 

void setup() {

  Serial.begin(9600);
  tensao = 5;  // tensão de alimentação do Arduino
  passo = tensao/1024;  // passo do conversor analógico digital
  ID = 1;  // Identificação do sensor

  // Liga o LED que está no pino 13 do Arduino
  pinMode (13, OUTPUT);
  digitalWrite (13, HIGH);
}

void loop() {

  // Faz medida da temperatura 
  vTemp = analogRead(A0); 

  // Calcula a temperatura em graus
  temp = (vtemp*passo*100)-50;  // Conversão da tensão em temperatura para o MCP9700.


  // IMPORTANTE: para fazer o debug do código através do Serial Monitor do Arduino tirar o comentário a seguir.
  // Quanto for utilizar o RadiuinoTest ou Python ou ScadaBR. Caso estas linhas não sejam comentadas haverá conflito de COM.
  /*Serial.print(" temp =  ");
   Serial.print(temp);
   Serial.println(vtemp);
   delay(2000); */

  if (Serial.available() ==52)   // recebe pacote que entrou pela USB
  {
    // A transmissão através do pacote do Radiuino é feita por bytes. Para isto deve ser convertido para um número
    // inteiro. Como a temperatura tem duas casas decimais basta multiplicar por 100. Quando receber pelo software
    // Python ou ScadaBR deve ser dividido por 100.
    temp = temp*100;

    // Leitura do buffer da serial e coloca no PacoteRX[] e zera pacote de transmissão PacoteTX[]
    for (int i = 0; i < 52; i++) // PacoteTX[#] é preenchido com zero e PacoteRX[#] recebe os bytes do buffer
    {
      PacoteTX[i] = 0; // Zera o pacote de transmissão
      PacoteRX[i] = Serial.read();  // Faz a leitura dos bytes que estão no buffer da serial
      delay(1);
    }
    // Verifica se o pacote que chegou é para este sensor, que no caso é 1, pois foi definido no void setup ID = 1
    if (PacoteRX[8] == ID){
      // Como a temperatura´tem duas casas decimais é necessário multiplicar por 100

      // Escreve no pacote as informações do ADC 0

      PacoteTX[17] = (byte) ((int)(temp)/256); // Valor inteiro no byte 17
      PacoteTX[18] = (byte) ((int)(temp)%256); // Resto da divisão no byte 18

      // Transmite o pacote
      for (int i = 0; i < 52; i++)
      {
        Serial.write(PacoteTX[i]);
      }

    }
  }    
}



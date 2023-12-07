

char x;
String x_correct;


int Index1,Index2,Index3,Index4;
float lasterror = 0;
float timestep = 0.5;
float CAf = 0;
float integral_error = 0;
float derivative_error = 0;

void setup() {
 
  Serial.begin(9600);
 
}

void loop() {


  
  if (Serial.available()>0){
    //x=Serial.parseFloat();
    x=Serial.read();
    x_correct += x; // basicamente o arduino só lê caracter a caracter , logo somamos todos 1 a 1 para formar a string inteira 
    
    if(x=='\t'){ // sempre que escrevo um numero , escrevo com o labview um espaço para o programa saber que acabou o numero(neste caso vou escrever um conjunto de numeros que é o P, I ,D, SET POINT e PV) que vão estar numa type string
      
    x_correct.replace(",",".");// no labview as virgulas equivalem aos pontos no arduino , logo temos que substituir os pontos por virgulas
    
    
  
       if (x_correct.length()>0) // objetivo : dividir a string "x_correct" exemplo x_correct= "22.5;35,8;98.64;48.12" , e sacar cada um dos números delimitados por ";"(escolhi este no labview)
        {
        Index1 = x_correct.indexOf(';');
        Index2 = x_correct.indexOf(';', Index1+1);
        Index3 = x_correct.indexOf(';', Index2+1);
        Index4 =x_correct.indexOf(';', Index3+1);
          // falta aqui o PV , ou seja encontrar o indice do próximo ";" que ainda não existe porque eu não envio o valor do PV(process value) para o arduino
        
        
        String P=x_correct.substring(0, Index1);
        String I = x_correct.substring(Index1+1, Index2);
        String D = x_correct.substring(Index2+1, Index3);
        String Seat_P=x_correct.substring(Index3+1,Index4); 
        String PValue=x_correct.substring(Index4+1);

    //Depois de conseguir extrair os numeros , eles estão em string , para fazer calculos com eles temos que coloca-los em float OU double
        float Pf=P.toFloat();
        float If=I.toFloat();
        float Df=D.toFloat();
        float SPf=Seat_P.toFloat();
        float Pvaluef=PValue.toFloat();
         //falta o PV(process value que me esqueci)NOTA:no Labview é fazer o mesmo procedimento , colocar o ";" e o valor do process_value . PEQUENO PROBLEMA : na primeira iteração temos que mandar um valor random do CA para o "system" porque ainda não existe um CA calculado pelo arduino
      
      // tendo agora todos em float podemos fazer os calculos de controlo como quisermos e fazer os metodos que aprendemos
                      // Escrever algoritmo e calculos aqui
          

          //calcular o erro
              float error = SPf - Pvaluef;
          //calcular CA
          
              
              integral_error += error * timestep;
              derivative_error = (error-lasterror)/timestep;
              CAf = Pf* error + If * integral_error + Df * derivative_error;


              lasterror = error;

              float erro_relativo= (error/SPf)*100;
        
        // converter em string e colocar virgulas novamente(labview)
        P=String(Pf);
        P.replace('.',',');
        I=String(If);
        I.replace('.',',');
        D=String(Df);
        D.replace('.',',');
        Seat_P=String(SPf);
        Seat_P.replace('.',',');
        String CA=String(CAf);
        CA.replace('.',',');
        PValue=String(Pvaluef);
        PValue.replace('.',',');
        String error_relativo_str=String(erro_relativo);
        error_relativo_str.replace('.',',');
        

        String numbers_string={P+'\t'+I+'\t'+D+'\t'+Seat_P+'\t'+CA+'\t'+PValue+'\t'+error_relativo_str};

    
                      
     // depois quando tivermos os valores todos , temos que enviar o control action de volta ao labview para pôr na função system o Control Action que vai ser calculado aqui         

     // O labview consegue ler estes valores(abrir labview e ver)

        
        Serial.print(numbers_string);

        // Próximo objetivo: o Labview vai receber os valores como uma string "22.5 35,8 98.64 48.12". Agora temos que fazer o processo inverso
                             // Ou seja conseguir extrair o valor 1 a 1 da string em cima e converter em formato "double" (Principalmente o CA) para enviar para o ficheiro system.vi e ver a saída.
        
        }

 
    x_correct=""; // reiniciar a variavel 
    }

    
  }
    
 }
  

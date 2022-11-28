/* 


------------------------------------------ Arquivo com a configuração de locomoção dos robôs 3kg ------------------------------------------

*/

#include <Arduino.h>
#include <pinagem.hpp>


// namespace com as funcoes de locomocao padrao dos motores

namespace locomocaoPadrao{


    #ifdef APOLO

    /**
    * @brief Funcao que inicia os canais, frequencia e resolucao do PWM dos motores
    * @param
    * @return 
    */
    void iniciarMotores(){

        // configurando os canais que geram os sinais de PWM dos motores
        ledcSetup(Apolo.canalDoPinoPWMHMotorEsquerdo, Apolo.frequenciaPWM, Apolo.resolucaoPWM);
        ledcSetup(Apolo.canalDoPinoPWMHMotorDireito, Apolo.frequenciaPWM, Apolo.resolucaoPWM);

        // associando os canais aos pinos de PWM dos motores
        ledcAttachPin(Apolo.motorEsquerdoPWMH, Apolo.canalDoPinoPWMHMotorEsquerdo);
        ledcAttachPin(Apolo.motorDireitoPWMH, Apolo.canalDoPinoPWMHMotorDireito);

    }

    /**
    * @brief Funcao para controle dos motores, os valores de PWM passados correspondem aos valores de velocidade dos motores
    * @param valorPWMEsquerdo: 0 a 4095
    * @param valorPWMDireito: 0 a 4095
    * @return 
    */
    void controlarMotores(int valorPWMEsquerdo, int valorPWMDireito){

        // colocando o valor máximo de PWM nos pinos dos motores
        // Para uma resolução de 12 bits: valorMaximoDePWM = 2¹² - 1 = 4095
        ledcWrite(Apolo.canalDoPinoPWMHMotorEsquerdo, valorPWMEsquerdo);
        ledcWrite(Apolo.canalDoPinoPWMHMotorDireito, valorPWMDireito);


    }

    /**
    * @brief Funcao para frear os motores completamente
    * @param
    * @return 
    */
    void pararMotores(){

        // colocando o valor mínimo de PWM nos pinos dos motores
        ledcWrite(Apolo.canalDoPinoPWMHMotorEsquerdo, 0);
        ledcWrite(Apolo.canalDoPinoPWMHMotorDireito, 0);

    }

    #endif


    #ifdef ATENA

    /**
    * @brief Funcao que inicia os canais, frequencia e resolucao do PWM dos motores
    * @param
    * @return 
    */
    void iniciarMotores(){

        // configurando os canais que geram os sinais de PWM dos motores
        ledcSetup(Atena.canalDoPinoPWMHMotorEsquerdo, Atena.frequenciaPWM, Atena.resolucaoPWM);
        ledcSetup(Atena.canalDoPinoPWMHMotorDireito, Atena.frequenciaPWM, Atena.resolucaoPWM);

        // associando os canais aos pinos de PWM dos motores
        ledcAttachPin(Atena.motorEsquerdoPWMH, Atena.canalDoPinoPWMHMotorEsquerdo);
        ledcAttachPin(Atena.motorDireitoPWMH, Atena.canalDoPinoPWMHMotorDireito);


    }

    /**
    * @brief Funcao para controle dos motores, os valores de PWM passados correspondem aos valores de velocidade dos motores
    * @param valorPWMEsquerdo: 0 a 4095
    * @param valorPWMDireito: 0 a 4095
    * @return 
    */
    void controlarMotores(int valorPWMEsquerdo, int valorPWMDireito){

        // colocando o valor máximo de PWM nos pinos dos motores
        // Para uma resolução de 12 bits: valorMaximoDePWM = 2¹² - 1 = 4095
        ledcWrite(Atena.canalDoPinoPWMHMotorEsquerdo, valorPWMEsquerdo);
        ledcWrite(Atena.canalDoPinoPWMHMotorDireito, valorPWMDireito);

    }

    /**
    * @brief Funcao para frear os motores completamente
    * @param
    * @return 
    */
    void pararMotores(){

        // colocando o valor mínimo de PWM nos pinos dos motores
        ledcWrite(Atena.canalDoPinoPWMHMotorEsquerdo, 0);
        ledcWrite(Atena.canalDoPinoPWMHMotorDireito, 0);

    }

    #endif


    #ifdef VALESKA
    
   /**
    * @brief Funcao que inicia os canais, frequencia e resolucao do PWM dos motores
    * @param
    * @return 
    */ 
    void iniciarMotores(){

        // configurando os canais que geram os sinais de PWM dos motores
        ledcSetup(Valeska.canalDoPinoPWMHMotorEsquerdo, Valeska.frequenciaPWM, Valeska.resolucaoPWM);
        ledcSetup(Valeska.canalDoPinoPWMHMotorDireito, Valeska.frequenciaPWM, Valeska.resolucaoPWM);

        // associando os canais aos pinos de PWM dos motores
        ledcAttachPin(Valeska.motorEsquerdoPWMH, Valeska.canalDoPinoPWMHMotorEsquerdo);
        ledcAttachPin(Valeska.motorDireitoPWMH, Valeska.canalDoPinoPWMHMotorDireito);

    }

    /**
    * @brief Funcao para controle dos motores, os valores de PWM passados correspondem aos valores de velocidade dos motores
    * @param valorPWMEsquerdo: 0 a 4095
    * @param valorPWMDireito: 0 a 4095
    * @return 
    */
    void controlarMotores(int valorPWMEsquerdo, int valorPWMDireito){

        // colocando o valor máximo de PWM nos pinos dos motores
        // Para uma resolução de 12 bits: valorMaximoDePWM = 2¹² - 1 = 4095
        ledcWrite(Valeska.canalDoPinoPWMHMotorEsquerdo, valorPWM);
        ledcWrite(Valeska.canalDoPinoPWMHMotorDireito, valorPWM);

    }

    /**
    * @brief Funcao para frear os motores completamente
    * @param
    * @return 
    */
    void pararMotores(){

        // colocando o valor mínimo de PWM nos pinos dos motores
        ledcWrite(Valeska.canalDoPinoPWMHMotorEsquerdo, 0);
        ledcWrite(Valeska.canalDoPinoPWMHMotorDireito, 0);

    }

    #endif

}
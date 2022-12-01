/* 


------------------------------------------ Arquivo com a configuração de locomoção dos robôs 3kg ------------------------------------------

*/

#include <Arduino.h>
#include <pinagem.hpp>


// namespace com as funcoes de locomocao padrao dos motores

class locomocaoSimples{

    public:

    void configurandoMotores(){

        ledcSetup(Atena.canalDoPinoPWMHMotorEsquerdo, Atena.frequenciaPWM, Atena.resolucaoPWM);
        ledcSetup(Atena.canalDoPinoPWMHMotorDireito, Atena.frequenciaPWM, Atena.resolucaoPWM);

        ledcAttachPin(Atena.motorEsquerdoPWMH, Atena.canalDoPinoPWMHMotorEsquerdo);
        ledcAttachPin(Atena.motorDireitoPWMH, Atena.canalDoPinoPWMHMotorDireito);

        pinMode(Atena.motorEsquerdoPWML, OUTPUT);
        pinMode(Atena.motorEsquerdoPHASE, OUTPUT);
        pinMode(Atena.motorDireitoPWML, OUTPUT);
        pinMode(Atena.motorDireitoPHASE, OUTPUT);

        Atena.setValorMaximoPWM(4095);

        digitalWrite(Atena.motorEsquerdoPWML, HIGH);
        digitalWrite(Atena.motorDireitoPWML, HIGH);   
    
    }

    void pararMotores(){

        ledcWrite(Atena.canalDoPinoPWMHMotorEsquerdo, 0);
        ledcWrite(Atena.canalDoPinoPWMHMotorDireito, 0);

    }


};
/*
Cabeçalho responsável pelo controle do driver de motor A3941.
*/

#pragma once

#include <Arduino.h>

// Classe para controlar motores que utilizam o driver A3941
class A3941 {
    private:

    // Atributo que armazena o pino PHASE do driver de motor
    int _pinoPHASE;

    // Atributo que armazena o canal utilizado pelo pino PWMH do driver de motor
    // Esse canal é responsável por gerar o sinal PWM
    int _canalDoPinoPWMH;

    // Atributo que armazena o valor máximo de potência do motor
    int _valorMaximoDePotencia;

    // Atributo que armazena o valor da potêncial atual do motor
    int _potencia = 0;

    public:

    bool motorInvertido = false;

    /**
     * @brief Método construtor que configura o motor.
     * 
     * @param pinoPWML Número do pino PWML do driver;
     * @param pinoPWMH Número do pino PWMH do driver;
     * @param pinoPHASE Número do pino PHASE do driver;
     * @param canalDoPinoPWMH Canal de PWM do pino PWMH do driver;
     * @param frequenciaDoSinalDePWM Frequência do sinal PWM;
     * @param resolucao Número de bits da resolução do sinal PWM. Por exemplo, uma resolução de 12 bits significa que o valor de PWM varia no intervalo [0, 4095] (2¹² = 4096 níveis de controle).
     */
    A3941(int pinoPWML, int pinoPWMH, int pinoPHASE, int canalDoPinoPWMH, int frequenciaDoSinalDePWM, int resolucao):
        // Atribui os valores passados nos atributos
        _pinoPHASE(pinoPHASE),
        _canalDoPinoPWMH(canalDoPinoPWMH),

        // Calcula o valor máximo de potência do motor a partir da resolução
        _valorMaximoDePotencia(pow(2, resolucao) - 1)


    {
        // Configura o canal que gera o sinal PWM
        ledcSetup(_canalDoPinoPWMH, frequenciaDoSinalDePWM, resolucao);

        // Anexa o canal que gera o sinal PWM no pino PWMH do driver de motor
        ledcAttachPin(pinoPWMH, _canalDoPinoPWMH);

        // Define o modo OUTPUT para os pinos PWML e PHASE dos driver de motor
        // Esses pinos não recebem sinais de PWM
        pinMode(pinoPWML, OUTPUT);
        pinMode(_pinoPHASE, OUTPUT);

        // Coloca o pino PWML do driver de motor com valor HIGH
        digitalWrite(pinoPWML, HIGH);

        // Para o motor
        parar();
    }

    /**
     * @brief Método para alterar a velocidade do motor de acordo com o valor de potência passado.
     * 
     * @param potencia Valor da potência, na qual varia no intervalo [-valorMaximoDePotencia, valorMaximoDePotencia], para aplicar no motor. Para valores positivos o motor gira em um sentido, para valores negativos o motor gira no sentido contrário.
     */
    void setPotencia(int potencia) {

        // Se o motor estiver invertido, inverte o valor da potência
        if (motorInvertido) {
            potencia = -potencia;
        }

        // Se a potência passada for positiva, escreve esse valor no canal do pino PWMH e escreve LOW no pino PHASE do driver de motor
        if (potencia > 0) {
            ledcWrite(_canalDoPinoPWMH, potencia);
            digitalWrite(_pinoPHASE, LOW);
        }
        // Senão, se a potência passada for negativa, escreve o valor absoluto desse valor no canal do pino PWMH e escreve HIGH no pino PHASE do driver de motor
        else if (potencia < 0) {
            ledcWrite(_canalDoPinoPWMH, abs(potencia));
            digitalWrite(_pinoPHASE, HIGH);
        }
        // Senão, se o valor de potência for 0, freia o motor escrevendo o valor 0 no canal do pino PWMH
        else {
            ledcWrite(_canalDoPinoPWMH, 0);
        }

        // Atualiza o atributo da potência do motor com o valor da potência atual
        _potencia = potencia;
    }

    // Método para parar o motor
    void parar() {
        // Freia o motor escrevendo o valor 0 no canal do pino PWMH
        ledcWrite(_canalDoPinoPWMH, 0);

        // Atualiza o atributo da potência do motor com o valor de potência 0
        _potencia = 0;
    }

    /**
     * @brief Método para obter o valor do atributo que armazena o valor máximo de potência do motor.
     * 
     * @return O valor máximo de potência do motor.
     */
    int getValorMaximoDePotencia() {
        return _valorMaximoDePotencia;
    }

    /**
     * @brief Método para obter o valor do atributo que armazena o valor da potêncial atual do motor.
     * 
     * @return O valor da potência atual do motor.
     */
    int getPotencia() {
        return _potencia;
    }
    void setCanal(int novoCanalDoPinoPWMH){
        _canalDoPinoPWMH = novoCanalDoPinoPWMH;
    }
};





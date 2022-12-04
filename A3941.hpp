/*
Cabeçalho responsável pelo controle do driver de motor A3941.
*/

#include <Arduino.h>

#pragma once

// Classe para controlar motores utilizando o A3941
class A3941 {
    private:

    // Atributos que armazenam os pinos do driver do motor
    int _pinoPWMH;
    int _pinoPWML;
    int _pinoPHASE;

    // Atributo que armazena o canal utilizado pelo driver do motor
    // Esse canal é responsável por gerar os sinais de PWM
    int _canalDoPinoPWMH;

    // Atributo que armazena a frequência (em Hz) do sinal de PWM
    int _frequenciaDoSinalDePWM;

    // Atributo que armazena a resolução (em bits) que define o intervalo entre o mínimo e máximo do valor de PWM do motor
    int _resolucao;

    // Atributo que armazena o valor máximo de PWM do motor
    int _valorMaximoDePWM;

    public:

    /**
     * @brief Método construtor que configura o motor.
     * 
     * @param pinoPWMH Número do pino PWMH do driver;
     * @param pinoPWML Número do pino PWML do driver;
     * @param pinoPHASE Número do pino PHASE do driver;
     * @param canalDoPinoPWMH Canal de PWM do pino PWMH do driver;
     * @param frequenciaDoSinalDePWM Frequência do sinal de PWM;
     * @param resolucao Número de bits da resolução do sinal de PWM. Por exemplo, uma resolução de 12 bits significa que o valor de PWM varia no intervalo [0, 4095] (2¹² = 4096 níveis de controle).
     */
    A3941(int pinoPWMH, int pinoPWML, int pinoPHASE, int canalDoPinoPWMH, int frequenciaDoSinalDePWM, int resolucao) {
        // Atribui os valores passados nos atributos
        _pinoPWMH = pinoPWMH;
        _pinoPWML = pinoPWML;
        _pinoPHASE = pinoPHASE;
        _canalDoPinoPWMH = canalDoPinoPWMH;
        _frequenciaDoSinalDePWM = frequenciaDoSinalDePWM;
        _resolucao = resolucao;

        // Configura o canal que gera o sinal de PWM
        ledcSetup(_canalDoPinoPWMH, _frequenciaDoSinalDePWM, _resolucao);

        // Anexa os canais que geram os sinais de PWM nos pinos do driver do motor
        ledcAttachPin(_pinoPWMH, _canalDoPinoPWMH);

        // Define o modo OUTPUT para os pinos PWML e PHASE dos driver do motor
        // OBS.: Esses pinos não recebem valores de PWM
        pinMode(_pinoPWML, OUTPUT);
        pinMode(_pinoPHASE, OUTPUT);

        // Coloca o pino PWML do driver do motor com valor HIGH
        digitalWrite(_pinoPWML, HIGH);

        // Calcula o valor máximo de PWM do motor a partir da resolução
        _valorMaximoDePWM = pow(2, _resolucao) - 1;

        // Para o motor
        parar();
    };

    /**
     * @brief Função para alterar a velocidade do motor de acordo com o valor de PWM passado.
     * 
     * @param PWM Valor de PWM, no qual varia no intervalo [-valorMaximoDePWM, valorMaximoDePWM], para aplicar no motor. Para valores positivos de PWM o motor gira em um sentido, para valores negativos o motor gira no sentido contrário.
     */
    void setPotencia(int PWM) {
        // Caso o valor de PWM seja positivo, coloca esse valor no canal do pino PWMH e desliga o pino PHASE do driver do motor
        if (PWM > 0) {            
            digitalWrite(_pinoPHASE, LOW);
            ledcWrite(_canalDoPinoPWMH, PWM);
        }
        // Caso o valor de PWM seja negativo, coloca o valor absoluto desse valor no canal do pino PWMH e liga o pino PHASE do driver do motor
        else if (PWM < 0) {
            digitalWrite(_pinoPHASE, HIGH);
            ledcWrite(_canalDoPinoPWMH, abs(PWM));
        }
        // Caso o valor de PWM seja 0, freia o motor (coloca o canal do pino PWMH com valor 0 de PWM)
        else {
            ledcWrite(_canalDoPinoPWMH, 0);
        }
    }

    // Função para parar o motor
    void parar() {
        // Freia o motor colocando o canal do pino PWMH do driver do motor com valor 0 de PWM
        ledcWrite(_canalDoPinoPWMH, 0);
    }

    /**
     * @brief Método para obter o valor do atributo que armazena o valor máximo de PWM do motor.
     * 
     * @return O valor máximo de PWM do motor.
     */
    int getValorMaximoDePWM() {
        return _valorMaximoDePWM;
    }
};

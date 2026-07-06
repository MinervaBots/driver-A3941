/*
Cabeçalho responsável pelo controle do driver de motor A3941.
*/

#pragma once

#include "driver/ledc.h"
#include "driver/gpio.h"

// Namespace para as constantes que serão usadas na classe
namespace ConstA3941 {
    // Frequência recomendada para motores de potência para controlar melhor a comutação dos MOSFETs
    constexpr uint32_t FREQUENCIA_SINAL_PWM = 20000;

    // Resolução do PWM no valor convencional
    constexpr ledc_timer_bit_t RESOLUCAO_SINAL_PWM = LEDC_TIMER_8_BIT;
}

// Classe para controlar motores que utilizam o driver A3941
class A3941 {
private:

    // Atributo para armazenar os pinos
    gpio_num_t _pinoPHASE;
    gpio_num_t _pinoPWML;
    gpio_num_t _pinoPWMH;

    // Atributo que armazena o canal utilizado pelo pino PWMH do driver de motor
    // Esse canal é responsável por gerar o sinal PWM
    ledc_channel_t _canalDoPinoPWMH;

    // Atributos para armazenar as configurações do timer e canal de PWM
    ledc_mode_t modoPWM;
    ledc_timer_t timerSelecionado;

    // Atributo que armazena o número de bits da resolução do sinal PWM
    ledc_timer_bit_t resolucao;

    void setCanalPWM(uint8_t canalPinoPWMH) {
        #ifdef SOC_LEDC_SUPPORT_HS_MODE
            if (canalPinoPWMH <= 7) {
                this->_canalDoPinoPWMH = static_cast<ledc_channel_t>(canalPinoPWMH);
                this->modoPWM = LEDC_LOW_SPEED_MODE;
            }

            else if (canalPinoPWMH > 7 && canalPinoPWMH <= 15) {
                // Transforma os canais de 8 a 15 em valores de 0 a 7 para a função de inicialização
                this->_canalDoPinoPWMH = static_cast<ledc_channel_t>(canalPinoPWMH - 8);
                // Os canais de 8 a 15 são do hardware HIGH SPEED
                this->modoPWM = LEDC_HIGH_SPEED_MODE;
            }
        #else
            this->_canalDoPinoPWMH = static_cast<ledc_channel_t>(canalPinoPWMH % 8);
            this->modoPWM = LEDC_LOW_SPEED_MODE;
        #endif
    }

    void initCanalPWM() {
        bool timerAlocado = false;

        // Inicia um loop para verificar os timers disponíveis e escolher um para configurar o canal do driver
        for (uint8_t i = 0; i < LEDC_TIMER_MAX; i++) {
            ledc_timer_t timerTeste = static_cast<ledc_timer_t>(i);

            uint32_t freqAtual = ledc_get_freq(modoPWM, timerTeste); // Salva a frequência do timer de teste

            // Se a frequência do timer de teste já for igual a um timer em funcionamento, então esse timer será reutilizado
            if (freqAtual == ConstA3941::FREQUENCIA_SINAL_PWM) {
                timerSelecionado = timerTeste;
                timerAlocado = true;
                break; // quebra o loop
            }

            // Se a frequência do timer de teste for 0, significa que ele está livre para ser configurado
            else if (freqAtual == 0) {
                timerSelecionado = timerTeste;

                ledc_timer_config_t timerConfig = {
                    .speed_mode = modoPWM, // Escolhe o modo de velocidade de acordo com o canal de entrada na classe
                    .duty_resolution = resolucao,
                    .timer_num = timerSelecionado,
                    .freq_hz = ConstA3941::FREQUENCIA_SINAL_PWM,
                    .clk_cfg = LEDC_AUTO_CLK
                };

                ledc_timer_config(&timerConfig);
                timerAlocado = true;
                break; // quebra o loop
            }
        }

        ledc_channel_config_t canalConfig = {
            .gpio_num = _pinoPWMH, // Pino de saída
            .speed_mode = modoPWM, // Escolhe o modo de velocidade de acordo com o canal de entrada na classe
            .channel = _canalDoPinoPWMH, // Canal já remapeado
            .intr_type = LEDC_INTR_DISABLE, // desativa as interrupções
            .timer_sel = timerSelecionado, // Timer já configurado
            .duty = 0, // Começa com o duty cicle em 0
            .hpoint = 0
        };

        ledc_channel_config(&canalConfig);
    }

    void initGPIO() {
        gpio_config_t gpioConfig = {
            .pin_bit_mask = (1ULL << _pinoPWML) | (1ULL << _pinoPHASE), // configura os pinos juntos
            .mode = GPIO_MODE_OUTPUT, // pinos para saída
            .pull_up_en = GPIO_PULLUP_DISABLE,
            .pull_down_en = GPIO_PULLDOWN_DISABLE,
            .intr_type = GPIO_INTR_DISABLE // interrupções desabilitadas
        };

        gpio_config(&gpioConfig);
    }

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
    A3941(uint8_t pinoPWML, uint8_t pinoPWMH, uint8_t pinoPHASE, uint8_t canalDoPinoPWMH) :
        // Atribui os valores passados nos atributos
        _pinoPWML(static_cast<gpio_num_t>(pinoPWML)),
        _pinoPWMH(pinoPWMH),
        _pinoPHASE(static_cast<gpio_num_t>(pinoPHASE))
    {
        setCanalPWM(canalDoPinoPWMH);
        setResolucao(0); // Para usar a resolução padrão da biblioteca, que é 8 bits
    }

    /**
     * @brief Método para alterar a velocidade do motor de acordo com o valor de potência passado.
     * 
     * @param potencia Valor da potência, na qual varia no intervalo [-valorMaximoDePotencia, valorMaximoDePotencia], para aplicar no motor. Para valores positivos o motor gira em um sentido, para valores negativos o motor gira no sentido contrário.
     */
    void setPotencia(int potencia) {
        potencia = (potencia > (int)getValorMaximoDePotencia()) ? getValorMaximoDePotencia() : potencia; // Limita a potência ao valor máximo permitido
        potencia = (potencia < -(int)getValorMaximoDePotencia()) ? -getValorMaximoDePotencia() : potencia; // Limita a potência ao valor mínimo permitido

        // Se o motor estiver invertido, inverte o valor da potência
        if (motorInvertido) {
            potencia = -potencia;
        }

        // Se a potência passada for positiva, escreve esse valor no canal do pino PWMH e escreve LOW no pino PHASE do driver de motor
        if (potencia > 0) {
            ledc_set_duty(modoPWM, _canalDoPinoPWMH, potencia);
            ledc_update_duty(modoPWM, _canalDoPinoPWMH);
            gpio_set_level(_pinoPHASE, 0);
        }
        // Senão, se a potência passada for negativa, escreve o valor absoluto desse valor no canal do pino PWMH e escreve HIGH no pino PHASE do driver de motor
        else if (potencia < 0) {
            ledc_set_duty(modoPWM, _canalDoPinoPWMH, -potencia);
            ledc_update_duty(modoPWM, _canalDoPinoPWMH);
            gpio_set_level(_pinoPHASE, 1);
        }
        // Senão, se o valor de potência for 0, freia o motor escrevendo o valor 0 no canal do pino PWMH
        else {
            ledc_stop(modoPWM, _canalDoPinoPWMH, 0);
        }
    }

    /// @brief Método para parar o motor com potência 0
    void parar() {
        // Freia o motor escrevendo o valor 0 no canal do pino PWMH
        ledc_stop(modoPWM, _canalDoPinoPWMH, 0);
    }

    /**
     * @brief Método para obter o valor do atributo que armazena o valor máximo de potência do motor.
     * 
     * @return O valor máximo de potência do motor.
     */
    uint32_t getValorMaximoDePotencia() const {
        return ((1 << resolucao) - 1);
    }

    /**
     * @brief Método para obter o valor do atributo que armazena o valor da potêncial atual do motor.
     * 
     * @return O valor da potência atual do motor.
     */
    uint32_t getPotencia() const {
        return ledc_get_duty(modoPWM, _canalDoPinoPWMH);
    }

    /**
     * @brief Método para alterar o canal de PWM do pino PWMH do driver de motor
     * 
     * @param novoCanalDoPinoPWMH Canal de PWM do pino PWMH do driver, valor entre [0, 15] se tiver suporte para HIGH SPEED, se não, [0, 7]
     */
    void setCanal(uint8_t novoCanalDoPinoPWMH){
        setCanalPWM(novoCanalDoPinoPWMH);
    }

    /**
     * @brief Método para alterar a resolução do PWM
     * 
     * @param novaResolucao Número de bits da resolução do sinal PWM valor entre [1, 15]
     */
    void setResolucao(uint8_t novaResolucao) {
        if (novaResolucao > 0 && novaResolucao <= LEDC_TIMER_BIT_MAX) {
            resolucao = static_cast<ledc_timer_bit_t>(novaResolucao); // converte o valor int passado para a macro do enum ledc_timer_bit_t
        }

        else {
            resolucao = ConstA3941::RESOLUCAO_SINAL_PWM; // para qualquer valor fora das resoluções aceita, usará o padrão da biblioteca (8)
        }
    }

    /// @brief Método para inicializar o controle do motor, deve ser chamado no setup()
    void begin() {
        // Funções para inicializar o canal de PWM e os pinos GPIO do driver de motor
        initCanalPWM();
        initGPIO();

        gpio_set_level(_pinoPWML, 1); // Ativa o driver de motor escrevendo HIGH no pino PWML

        parar(); // Garante que o motor comece parado
    }

};

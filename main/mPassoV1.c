#include <stdio.h>								// Acesso as operacoes de entradas/saidas.
#include <string.h>								// Acesso as manipulacoes de strings. 

#include "freertos/FreeRTOS.h"					// Acesso aos termos.
#include "freertos/task.h"						// Acesso as prioridades da TASK.
#include "freertos/queue.h"
#include "freertos/event_groups.h"              // 
#include "driver/gpio.h"														// Acesso ao uso das GPIOs.



//MOTOR DE PASSO//
#define mpSLP__ GPIO_NUM_22      // Seleciona o pino de 'Sleep'. Determina se motor ativo (1).
#define mpSTP__ GPIO_NUM_21     // Seleciona o pino de 'Step'. Pulso para o passo do motor.
#define mpDIR__ GPIO_NUM_19     // Seleciona o pino de 'Direction'. Determina o sentido do motor.

#define CONFIG_FREERTOS_HZ 100

void __mpIniciar (void)
{
    gpio_reset_pin(mpSLP__);        //reseta pino e limpa cfg anteriores
    gpio_reset_pin(mpSTP__);        //reseta pino e limpa cfg anteriores
    gpio_reset_pin(mpDIR__);        //reseta pino e limpa cfg anteriores
    gpio_set_direction(mpSLP__,GPIO_MODE_OUTPUT);       //definido como saida
    gpio_set_direction(mpSTP__,GPIO_MODE_OUTPUT);       //definido como saida
    gpio_set_direction(mpDIR__,GPIO_MODE_OUTPUT);       //definido como saida
    gpio_set_level(mpSLP__,0);                          //limpa o pino
    gpio_set_level(mpDIR__,0);                          //limpa o pino
    gpio_set_level(mpSTP__,0);                          //limpa o pino
}

void __mpPulse(void)      //gera pulso no DRV8825/A4988
{
    gpio_set_level(mpSTP__,1);
    vTaskDelay(10 / portTICK_PERIOD_MS);
    gpio_set_level(mpSTP__,0);
}

void __mpAng (unsigned char ang, unsigned char sentido, unsigned char passo )         //aciona o mP com os parametros fornecidos
{

    
    unsigned int contaPulsos = 0;
    contaPulsos = ang * 0.5556 * passo;                     //calculo do numero de passo  (regra de 3 simples)  360o / 1.8o  = 200passos        y = (200/360)*x (...)y = 0.5556 * x  (FOI USADO ESSES VALORES POIS O MOTOR DE PASSO USADO TEM 1.8 DE GRAUS/PASSOS)
    gpio_set_level(mpSLP__,1);                              // sai do modo 'sleep'/ ativa as saidas
    gpio_set_level(mpDIR__,sentido);                        // define o sentido do motor (horario e antihorario)
    while (contaPulsos!=0)
    {
        __mpPulse();          //gera pulso no DRV8825/A4988
        contaPulsos--;      //Decrementa vslor
        vTaskDelay(1);

    }
    gpio_set_level(mpSLP__,0);      //entra no modo 'sleep'/desliga as saidas
    

}


void app_main(void)
{
    __mpIniciar();


    
    while(1)
    {
        __mpAng(90,0,1);    //90 graus a direita 1:1


    }

}
      
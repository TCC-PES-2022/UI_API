﻿// p2.cpp: define o ponto de entrada para o aplicativo.
//

#include "p2.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <chrono>
#include <unistd.h>
#include "UI_API.h"
#include <iostream>

void* GUI(void* arg)
{
    uint8_t val_teste = 0;
    static st_ui_image img_gui;
    static st_ui_aut aut_gui;
    if (iniciar_UI_interface(&img_gui,&aut_gui)) {
        //img_gui.byte_controle = UI_Carregar_Imagem; // APENAS PARA TESTES
        aut_gui.byte_controle = UI_EnviarLogin; // APENAS PARA TESTES
        val_teste = UI_EnviarLogin;
    }
    pthread_detach(pthread_self());
    sleep(1);
    while (1)
    {
        if ((img_gui.byte_controle != UI_Aguardar) && (img_gui.byte_controle != UI_Null) || (aut_gui.byte_controle != UI_Aguardar) && (aut_gui.byte_controle != UI_Null)) {
            std::cout << "+[GUI] Cmd Enviado " << unsigned(val_teste) << endl;
           //img_gui = *carregar_imagem_GUI(val_teste, &img_gui);
           //img_gui = *transferir_imagem_GUI(val_teste, &img_gui);
           aut_gui = *verificar_autenticacao_GUI(val_teste, &aut_gui);
        }
        uint8_t val = verificarFilas(&img_gui,&aut_gui, UI__CTL);
        if (val == 1) {
            std::cout << "+[GUI] Cmd Recebido " << unsigned(img_gui.byte_controle) << endl;
            std::cout << "+[GUI] Cmd Anterior " << unsigned(img_gui.byte_controle_anterior) << endl;
            usleep(2000);
            //reseta MQ
            std::cout << "+[GUI] Resetando MQ... " << endl;
            img_gui = *carregar_imagem_GUI(UI_Null, &img_gui);
        }
        else if (val == 2) {
            std::cout << "+[GUI] Cmd Recebido " << unsigned(img_gui.byte_controle) << endl;
            std::cout << "+[GUI] Cmd Anterior " << unsigned(img_gui.byte_controle_anterior) << endl;
            usleep(2000);
            //reseta MQ
            std::cout << "+[GUI] Resetando MQ... " << endl;
            img_gui = *carregar_imagem_GUI(UI_Null, &img_gui);
        }
        else if (val == 3)
        {
            std::cout << "+[GUI] Cmd Recebido " << unsigned(aut_gui.byte_controle) << endl;
            std::cout << "+[GUI] Cmd Anterior " << unsigned(aut_gui.byte_controle_anterior) << endl;
            usleep(2000);
            //reseta MQ
            std::cout << "+[GUI] Resetando MQ... " << endl;
            aut_gui = *verificar_autenticacao_GUI(UI_Null, &aut_gui);

        }
        //else
            //std::cout << "+[CTL] Cmd Nao Recebido \r\n";
        sleep(1);
    }

    // exit the current thread
    pthread_exit(NULL);
}


void* CTR(void* arg)
{
    static st_ui_image img_controler = { 0 };
    static st_ui_aut aut_controler = {0};
    uint8_t val = 0;
    uint8_t env_resp = 0;
    uint8_t resposta = UI_Ok;
    pthread_detach(pthread_self());
    sleep(1);
    while (1) {
        val = verificarFilas(&img_controler, &aut_controler, UI__GUI); // se val > 0 -> Atualizou a estrutura toda
        if (val == 1) {
            std::cout << "+[CTL] Cmd Recebido " << unsigned(img_controler.byte_controle) << endl;
            std::cout << "+[CTL] Cmd Processando... " << endl;
            img_controler.byte_controle = UI_Aguardar;
            usleep(2000);
            img_controler.byte_controle = resposta;
            env_resp += val;
        }
        else if (val == 2) {
            std::cout << "+[CTL] Cmd Recebido " << unsigned(img_controler.byte_controle) << endl;
            std::cout << "+[CTL] Cmd Processando... " << endl;
            img_controler.byte_controle = UI_Aguardar;
            usleep(2000);
            img_controler.byte_controle = resposta;
            env_resp += val;
        }
        else if (val == 3) {
            std::cout << "+[CTL] Cmd Recebido " << unsigned(aut_controler.byte_controle) << endl;
            std::cout << "+[CTL] Cmd Processando... " << endl;
            aut_controler.byte_controle = UI_Aguardar;
            usleep(2000);
            aut_controler.byte_controle = resposta;
            env_resp += val;
        }

        if ((img_controler.byte_controle != UI_Aguardar) && (img_controler.byte_controle != UI_Null)) {

            if (env_resp == 1) {
                std::cout << "+[CTL] Cmd Enviado " << unsigned(img_controler.byte_controle) << endl;
                img_controler = *carregar_imagem_Controler(resposta, &img_controler);
                env_resp -= 1;
            }
            if (env_resp == 2)
            {
                std::cout << "+[CTL] Cmd Enviado " << unsigned(img_controler.byte_controle) << endl;
                img_controler = *transferir_imagem_Controler(resposta, &img_controler);
                env_resp -= 1;
            }
            //reseta MQ
            std::cout << "+[CTL] Resetando MQ... " << endl;
            img_controler = *carregar_imagem_Controler(UI_Null, &img_controler);
        }
        if ((aut_controler.byte_controle != UI_Aguardar) && (aut_controler.byte_controle != UI_Null)) {
            if (env_resp == 3)
            {
                std::cout << "+[CTL] Cmd Enviado " << unsigned(aut_controler.byte_controle) << endl;
                aut_controler = *verificar_autenticacao_Controler(resposta, &aut_controler);
                std::cout << "+[CTL] Resetando MQ... " << endl;
                aut_controler = *verificar_autenticacao_Controler(UI_Null, &aut_controler);
                env_resp -= 3;
            }
        
        }
        sleep(1);
    }

    // exit the current thread
    pthread_exit(NULL);
}


void fun()
{
    pthread_t pt_gui,pt_ctr;

    // Creating a new thread
    pthread_create(&pt_gui, NULL, &GUI, NULL);
    pthread_create(&pt_ctr, NULL, &CTR, NULL);

        // Waiting for the created thread to terminate
        pthread_join(pt_gui, NULL);
        pthread_join(pt_ctr, NULL);

        pthread_exit(NULL);
}

// Driver code
int main()
{
    fun();
    return 0;
}
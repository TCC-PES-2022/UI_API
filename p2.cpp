// p2.cpp: define o ponto de entrada para o aplicativo.
//

#include "p2.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <chrono>
#include <unistd.h>
#include "UI_API.h"
#include <iostream>
#include "assert.h"
void* GUI(void* arg)
{
    uint8_t val_teste = 0;
    static st_ui_image img_gui;
    static st_ui_aut aut_gui;
    static st_ui_conexao con_gui;

    if (iniciar_UI_interface(&img_gui,&aut_gui,&con_gui,UI__GUI)) {
        img_gui.byte_controle = UI_Carregar_Imagem; // APENAS PARA TESTES
        //aut_gui.byte_controle = UI_Cadastrar; // APENAS PARA TESTES
        //con_gui.byte_controle = UI_EncerrarConexao;
        val_teste = UI_Carregar_Imagem;
        //        memcpy(aut_gui.autenticao.login,"Afonso",sizeof("Afonso"));
        //        memcpy(aut_gui.autenticao.senha,"123",sizeof("123"));
    }
    sleep(1);
    pthread_detach(pthread_self());
    while (1)
    {
        if ((img_gui.byte_controle != UI_Aguardar) && (img_gui.byte_controle != UI_Null)
                || (aut_gui.byte_controle != UI_Aguardar) && (aut_gui.byte_controle != UI_Null)
                || (con_gui.byte_controle != UI_Aguardar) && (con_gui.byte_controle != UI_Null)) {
            std::cout << "+[GUI] Cmd Enviado " << unsigned(val_teste) << endl;
            img_gui = *carregar_imagem_GUI(val_teste, &img_gui);
            //img_gui = *transferir_imagem_GUI(val_teste, &img_gui);
            //aut_gui = *verificar_autenticacao_GUI(val_teste, &aut_gui);
            // con_gui = *conexoes_GUI(val_teste,&con_gui);
        }
        uint8_t val = verificarFilas(&img_gui,&aut_gui,&con_gui, UI__GUI);
        if (val == RP_fila_imagem) {
            std::cout << "+[GUI] Cmd Recebido " << unsigned(img_gui.byte_controle) << endl;
            std::cout << "+[GUI] Cmd Anterior " << unsigned(img_gui.byte_controle_anterior) << endl;
            usleep(2000);
            //reseta MQ
            std::cout << "+[GUI] Resetando MQ... " << endl;
            img_gui = *carregar_imagem_GUI(UI_Null, &img_gui);
        }
        else if (val == RP_fila_transf_imagem) {
            std::cout << "+[GUI] Cmd Recebido " << unsigned(img_gui.byte_controle) << endl;
            std::cout << "+[GUI] Cmd Anterior " << unsigned(img_gui.byte_controle_anterior) << endl;
            usleep(2000);
            //reseta MQ
            std::cout << "+[GUI] Resetando MQ... " << endl;
            img_gui = *carregar_imagem_GUI(UI_Null, &img_gui);
        }
        else if (val == RP_fila_autenticacao)
        {
            std::cout << "+[GUI] Cmd Recebido " << unsigned(aut_gui.byte_controle) << endl;
            std::cout << "+[GUI] Cmd Anterior " << unsigned(aut_gui.byte_controle_anterior) << endl;
            usleep(2000);
            //reseta MQ
            std::cout << "+[GUI] Resetando MQ... " << endl;
            aut_gui = *verificar_autenticacao_GUI(UI_Null, &aut_gui);

        }
        else if (val == RP_fila_conexao)
        {
            std::cout << "+[GUI] Cmd Recebido " << unsigned(con_gui.byte_controle) << endl;
            std::cout << "+[GUI] Cmd Anterior " << unsigned(con_gui.byte_controle_anterior) << endl;
            //reseta MQ
            std::cout << "+[GUI] Resetando MQ... " << endl;
            con_gui = *conexoes_GUI(UI_Null, &con_gui);
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
    static st_ui_conexao con_controler = {0};
    uint8_t val = 0;
    uint8_t env_resp = 0;
    uint8_t resposta = UI_Ok;
    pthread_detach(pthread_self());

    iniciar_UI_interface(&img_controler,&aut_controler,&con_controler,UI__CTL);
    sleep(1);
    while (1) {
        val = verificarFilas(&img_controler, &aut_controler, &con_controler,UI__CTL); // se val > 0 -> Atualizou a estrutura toda
        if (val == RP_fila_imagem) {
            std::cout << "+[CTL] Cmd Recebido " << unsigned(img_controler.byte_controle) << endl;
            std::cout << "+[CTL] Cmd Processando... " << endl;
            img_controler.byte_controle = UI_Aguardar;
            usleep(2000);
            img_controler.byte_controle = resposta;
            env_resp += val;
        }
        else if (val == RP_fila_transf_imagem) {
            std::cout << "+[CTL] Cmd Recebido " << unsigned(img_controler.byte_controle) << endl;
            std::cout << "+[CTL] Cmd Processando... " << endl;
            img_controler.byte_controle = UI_Aguardar;
            usleep(2000);
            img_controler.byte_controle = resposta;
            env_resp += val;
        }
        else if (val == RP_fila_autenticacao) {
            std::cout << "+[CTL] Cmd Recebido " << unsigned(aut_controler.byte_controle) << endl;
            std::cout << "+[CTL] Cmd Processando... " << endl;
            aut_controler.byte_controle = UI_Aguardar;
            usleep(2000);
            aut_controler.byte_controle = resposta;
            env_resp += val;
        }
        else if (val == RP_fila_conexao)
        {
            std::cout << "+[CTL] Cmd Recebido " << unsigned(con_controler.byte_controle) << endl;
            std::cout << "+[CTL] Cmd Processando... " << endl;
            con_controler.byte_controle = UI_Aguardar;
            usleep(2000);
            con_controler.byte_controle = resposta;
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
        if ((con_controler.byte_controle != UI_Aguardar) && (con_controler.byte_controle !=UI_Null) )
        {
            if (env_resp == 4)
            {
                std::cout << "+[CTL] Cmd Enviado " << unsigned(con_controler.byte_controle) << endl;
                con_controler = *conexoes_Controler(resposta, &con_controler);
                std::cout << "+[CTL] Resetando MQ... " << endl;
                con_controler = *conexoes_Controler(UI_Null, &con_controler);
                env_resp -= 4;
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
    // fun();
    uint8_t val_teste = 0;
    uint8_t rsp = 0;
    static st_ui_image img_gui;
    static st_ui_aut aut_gui;
    static st_ui_conexao con_gui;
    static st_ui_image img_controler = { 0 };
    static st_ui_aut aut_controler = {0};
    static st_ui_conexao con_controler = {0};
    //----Initialization Test----//
    int r = iniciar_UI_interface(&img_gui,&aut_gui,&con_gui,UI__GUI);
    assert(1 == r);
    r = 0;
    r = iniciar_UI_interface(&img_controler, &aut_controler, &con_controler,UI__CTL);
    assert(1 == r);
    //---------------------------//

    //----Send Autenticação Test Resp Ok----//
    for (uint8_t i = UI_EnviarLogin; i<= UI_RemoverUsuario;i++){
        val_teste = i;
        aut_gui = *verificar_autenticacao_GUI(val_teste, &aut_gui);                     // Faz o envio GUI->CTRL
        rsp = verificarFilas(&img_controler, &aut_controler, &con_controler,UI__CTL);   // Verifica Fila CTRL
        assert(RP_fila_autenticacao == rsp);
        aut_controler = *verificar_autenticacao_Controler(UI_Ok,&aut_controler);        // Faz o envio CTRL->GUI
        rsp = verificarFilas(&img_gui, &aut_gui, &con_gui,UI__GUI);                     // Verifica Fila GUI
        assert(RP_fila_autenticacao == rsp);
        assert(aut_gui.byte_controle_anterior == i);
        assert(aut_gui.byte_controle == UI_Ok);

        //----Send Autenticação Test Resp Falha----//
        val_teste = i;
        aut_gui = *verificar_autenticacao_GUI(val_teste, &aut_gui);                     // Faz o envio GUI->CTRL
        rsp = verificarFilas(&img_controler, &aut_controler, &con_controler,UI__CTL);   // Verifica Fila CTRL
        assert(RP_fila_autenticacao == rsp);
        aut_controler = *verificar_autenticacao_Controler(UI_Falha,&aut_controler);     // Faz o envio CTRL->GUI
        rsp = verificarFilas(&img_gui, &aut_gui, &con_gui,UI__GUI);                     // Verifica Fila GUI
        assert(RP_fila_autenticacao == rsp);
        assert(aut_gui.byte_controle_anterior == i);
        assert(aut_gui.byte_controle == UI_Falha);
    }


    //---------------------------------//
    //----Send Conexão Test Resp Ok----//
    for (uint8_t i = UI_ListarConexoes; i<= UI_EncerrarConexao;i++){
        val_teste = i;
        con_gui = *conexoes_GUI(val_teste, &con_gui);                     // Faz o envio GUI->CTRL
        rsp = verificarFilas(&img_controler, &aut_controler, &con_controler,UI__CTL);   // Verifica Fila CTRL
        assert(RP_fila_conexao == rsp);
        con_controler = *conexoes_Controler(UI_Ok,&con_controler);        // Faz o envio CTRL->GUI
        rsp = verificarFilas(&img_gui, &aut_gui, &con_gui,UI__GUI);                     // Verifica Fila GUI
        assert(RP_fila_conexao == rsp);
        assert(con_gui.byte_controle_anterior == i);
        assert(con_gui.byte_controle == UI_Ok);

        //----Send Conexão Test Resp Falha----//
        val_teste = i;
        con_gui = *conexoes_GUI(val_teste, &con_gui);                                   // Faz o envio GUI->CTRL
        rsp = verificarFilas(&img_controler, &aut_controler, &con_controler,UI__CTL);   // Verifica Fila CTRL
        assert(RP_fila_conexao == rsp);
        con_controler = *conexoes_Controler(UI_Falha,&con_controler);                   // Faz o envio CTRL->GUI
        rsp = verificarFilas(&img_gui, &aut_gui, &con_gui,UI__GUI);                     // Verifica Fila GUI
        assert(RP_fila_conexao == rsp);
        assert(con_gui.byte_controle_anterior == i);
        assert(con_gui.byte_controle == UI_Falha);
    }


    //--------------------------------------------------//
    //----Send UI_Iniciar_Transferencia Test Resp Ok----//

    val_teste = UI_Iniciar_Transferencia;
    img_gui = *transferir_imagem_GUI(val_teste, &img_gui);                          // Faz o envio GUI->CTRL
    rsp = verificarFilas(&img_controler, &aut_controler, &con_controler,UI__CTL);   // Verifica Fila CTRL
    assert(RP_fila_transf_imagem == rsp);
    img_controler = *transferir_imagem_Controler(UI_Ok,&img_controler);             // Faz o envio CTRL->GUI
    rsp = verificarFilas(&img_gui, &aut_gui, &con_gui,UI__GUI);                     // Verifica Fila GUI
    assert(RP_fila_transf_imagem == rsp);
    assert(img_gui.byte_controle_anterior == UI_Iniciar_Transferencia);
    assert(img_gui.byte_controle == UI_Ok);
    //-----------------------------------------------------//
    //----Send UI_Iniciar_Transferencia Test Resp Falha----//

    val_teste = UI_Iniciar_Transferencia;
    img_gui = *transferir_imagem_GUI(val_teste, &img_gui);                          // Faz o envio GUI->CTRL
    rsp = verificarFilas(&img_controler, &aut_controler, &con_controler,UI__CTL);   // Verifica Fila CTRL
    assert(RP_fila_transf_imagem == rsp);
    img_controler = *transferir_imagem_Controler(UI_Falha,&img_controler);             // Faz o envio CTRL->GUI
    rsp = verificarFilas(&img_gui, &aut_gui, &con_gui,UI__GUI);                     // Verifica Fila GUI
    assert(RP_fila_transf_imagem == rsp);
    assert(img_gui.byte_controle_anterior == UI_Iniciar_Transferencia);
    assert(img_gui.byte_controle == UI_Falha);
    //--------------------------------------------------//
    //----Send UI_Iniciar_Transferencia Test Resp Ok----//

    val_teste = UI_Andamento_Transferencia;
    img_gui = *transferir_imagem_GUI(val_teste, &img_gui);                          // Faz o envio GUI->CTRL
    rsp = verificarFilas(&img_controler, &aut_controler, &con_controler,UI__CTL);   // Verifica Fila CTRL
    assert(RP_fila_transf_imagem == rsp);
    img_controler = *transferir_imagem_Controler(UI_Ok,&img_controler);             // Faz o envio CTRL->GUI
    rsp = verificarFilas(&img_gui, &aut_gui, &con_gui,UI__GUI);                     // Verifica Fila GUI
    assert(RP_fila_transf_imagem == rsp);
    assert(img_gui.byte_controle_anterior == UI_Andamento_Transferencia);
    assert(img_gui.byte_controle == UI_Ok);
    //-----------------------------------------------------//
    //----Send UI_Iniciar_Transferencia Test Resp Falha----//

    val_teste = UI_Andamento_Transferencia;
    img_gui = *transferir_imagem_GUI(val_teste, &img_gui);                          // Faz o envio GUI->CTRL
    rsp = verificarFilas(&img_controler, &aut_controler, &con_controler,UI__CTL);   // Verifica Fila CTRL
    assert(RP_fila_transf_imagem == rsp);
    img_controler = *transferir_imagem_Controler(UI_Falha,&img_controler);             // Faz o envio CTRL->GUI
    rsp = verificarFilas(&img_gui, &aut_gui, &con_gui,UI__GUI);                     // Verifica Fila GUI
    assert(RP_fila_transf_imagem == rsp);
    assert(img_gui.byte_controle_anterior == UI_Andamento_Transferencia);
    assert(img_gui.byte_controle == UI_Falha);
    //----------------------------------------------//
    //----Send UI_Fim_Transferencia Test Resp Ok----//

    val_teste = UI_Fim_Transferencia;
    img_gui = *transferir_imagem_GUI(val_teste, &img_gui);                          // Faz o envio GUI->CTRL
    rsp = verificarFilas(&img_controler, &aut_controler, &con_controler,UI__CTL);   // Verifica Fila CTRL
    assert(RP_fila_transf_imagem == rsp);
    img_controler = *transferir_imagem_Controler(UI_Ok,&img_controler);             // Faz o envio CTRL->GUI
    rsp = verificarFilas(&img_gui, &aut_gui, &con_gui,UI__GUI);                     // Verifica Fila GUI
    assert(RP_fila_transf_imagem == rsp);
    assert(img_gui.byte_controle_anterior == UI_Fim_Transferencia);
    assert(img_gui.byte_controle == UI_Ok);
    //--------------------------------------------------//
    //----Send UI_Fim_Transferencia Test Resp Falha ----//

    val_teste = UI_Fim_Transferencia;
    img_gui = *transferir_imagem_GUI(val_teste, &img_gui);                          // Faz o envio GUI->CTRL
    rsp = verificarFilas(&img_controler, &aut_controler, &con_controler,UI__CTL);   // Verifica Fila CTRL
    assert(RP_fila_transf_imagem == rsp);
    img_controler = *transferir_imagem_Controler(UI_Falha,&img_controler);             // Faz o envio CTRL->GUI
    rsp = verificarFilas(&img_gui, &aut_gui, &con_gui,UI__GUI);                     // Verifica Fila GUI
    assert(RP_fila_transf_imagem == rsp);
    assert(img_gui.byte_controle_anterior == UI_Fim_Transferencia);
    assert(img_gui.byte_controle == UI_Falha);
    //-------------------------------------//
    //----Send UI_Cancelar Test Resp Ok----//

    val_teste = UI_Cancelar;
    img_gui = *transferir_imagem_GUI(val_teste, &img_gui);                          // Faz o envio GUI->CTRL
    rsp = verificarFilas(&img_controler, &aut_controler, &con_controler,UI__CTL);   // Verifica Fila CTRL
    assert(RP_fila_transf_imagem == rsp);
    img_controler = *transferir_imagem_Controler(UI_Ok,&img_controler);             // Faz o envio CTRL->GUI
    rsp = verificarFilas(&img_gui, &aut_gui, &con_gui,UI__GUI);                     // Verifica Fila GUI
    assert(RP_fila_transf_imagem == rsp);
    assert(img_gui.byte_controle_anterior == UI_Cancelar);
    assert(img_gui.byte_controle == UI_Ok);
    //-----------------------------------------//
    //----Send UI_Cancelar Test Resp Falha ----//

    val_teste = UI_Cancelar;
    img_gui = *transferir_imagem_GUI(val_teste, &img_gui);                          // Faz o envio GUI->CTRL
    rsp = verificarFilas(&img_controler, &aut_controler, &con_controler,UI__CTL);   // Verifica Fila CTRL
    assert(RP_fila_transf_imagem == rsp);
    img_controler = *transferir_imagem_Controler(UI_Falha,&img_controler);          // Faz o envio CTRL->GUI
    rsp = verificarFilas(&img_gui, &aut_gui, &con_gui,UI__GUI);                     // Verifica Fila GUI
    assert(RP_fila_transf_imagem == rsp);
    assert(img_gui.byte_controle_anterior == UI_Cancelar);
    assert(img_gui.byte_controle == UI_Falha);
    //--------------------------------------------//
    //----Send UI_Carregar_Imagem Test Resp Ok----//

    val_teste = UI_Carregar_Imagem;
    img_gui = *carregar_imagem_GUI(val_teste, &img_gui);                          // Faz o envio GUI->CTRL
    rsp = verificarFilas(&img_controler, &aut_controler, &con_controler,UI__CTL);   // Verifica Fila CTRL
    assert(RP_fila_imagem == rsp);
    img_controler = *carregar_imagem_Controler(UI_Ok,&img_controler);             // Faz o envio CTRL->GUI
    rsp = verificarFilas(&img_gui, &aut_gui, &con_gui,UI__GUI);                     // Verifica Fila GUI
    assert(RP_fila_imagem == rsp);
    assert(img_gui.byte_controle_anterior == UI_Carregar_Imagem);
    assert(img_gui.byte_controle == UI_Ok);
    //------------------------------------------------//
    //----Send UI_Carregar_Imagem Test Resp Falha ----//

    val_teste = UI_Carregar_Imagem;
    img_gui = *carregar_imagem_GUI(val_teste, &img_gui);                          // Faz o envio GUI->CTRL
    rsp = verificarFilas(&img_controler, &aut_controler, &con_controler,UI__CTL);   // Verifica Fila CTRL
    assert(RP_fila_imagem == rsp);
    img_controler = *carregar_imagem_Controler(UI_Falha,&img_controler);          // Faz o envio CTRL->GUI
    rsp = verificarFilas(&img_gui, &aut_gui, &con_gui,UI__GUI);                     // Verifica Fila GUI
    assert(RP_fila_imagem == rsp);
    assert(img_gui.byte_controle_anterior == UI_Carregar_Imagem);
    assert(img_gui.byte_controle == UI_Falha);

    //-----------------------------------------------//
    //----Send UI_Informacoes_Imagem Test Resp Ok----//

    val_teste = UI_Informacoes_Imagem;
    img_gui = *carregar_imagem_GUI(val_teste, &img_gui);                          // Faz o envio GUI->CTRL
    rsp = verificarFilas(&img_controler, &aut_controler, &con_controler,UI__CTL);   // Verifica Fila CTRL
    assert(RP_fila_imagem == rsp);
    img_controler = *carregar_imagem_Controler(UI_Ok,&img_controler);             // Faz o envio CTRL->GUI
    rsp = verificarFilas(&img_gui, &aut_gui, &con_gui,UI__GUI);                     // Verifica Fila GUI
    assert(RP_fila_imagem == rsp);
    assert(img_gui.byte_controle_anterior == UI_Informacoes_Imagem);
    assert(img_gui.byte_controle == UI_Ok);
    //---------------------------------------------------//
    //----Send UI_Informacoes_Imagem Test Resp Falha ----//

    val_teste = UI_Informacoes_Imagem;
    img_gui = *carregar_imagem_GUI(val_teste, &img_gui);                          // Faz o envio GUI->CTRL
    rsp = verificarFilas(&img_controler, &aut_controler, &con_controler,UI__CTL);   // Verifica Fila CTRL
    assert(RP_fila_imagem == rsp);
    img_controler = *carregar_imagem_Controler(UI_Falha,&img_controler);          // Faz o envio CTRL->GUI
    rsp = verificarFilas(&img_gui, &aut_gui, &con_gui,UI__GUI);                     // Verifica Fila GUI
    assert(RP_fila_imagem == rsp);
    assert(img_gui.byte_controle_anterior == UI_Informacoes_Imagem);
    assert(img_gui.byte_controle == UI_Falha);
    //-----------------------------------------------//
    //----Send UI_Informacoes_Imagem Test Resp Ok----//

    val_teste = UI_Apagar_Aquivo;
    img_gui = *carregar_imagem_GUI(val_teste, &img_gui);                          // Faz o envio GUI->CTRL
    rsp = verificarFilas(&img_controler, &aut_controler, &con_controler,UI__CTL);   // Verifica Fila CTRL
    assert(RP_fila_imagem == rsp);
    img_controler = *carregar_imagem_Controler(UI_Ok,&img_controler);             // Faz o envio CTRL->GUI
    rsp = verificarFilas(&img_gui, &aut_gui, &con_gui,UI__GUI);                     // Verifica Fila GUI
    assert(RP_fila_imagem == rsp);
    assert(img_gui.byte_controle_anterior == UI_Apagar_Aquivo);
    assert(img_gui.byte_controle == UI_Ok);
    //----------------------------------------------//
    //----Send UI_Apagar_Aquivo Test Resp Falha ----//

    val_teste = UI_Apagar_Aquivo;
    img_gui = *carregar_imagem_GUI(val_teste, &img_gui);                          // Faz o envio GUI->CTRL
    rsp = verificarFilas(&img_controler, &aut_controler, &con_controler,UI__CTL);   // Verifica Fila CTRL
    assert(RP_fila_imagem == rsp);
    img_controler = *carregar_imagem_Controler(UI_Falha,&img_controler);          // Faz o envio CTRL->GUI
    rsp = verificarFilas(&img_gui, &aut_gui, &con_gui,UI__GUI);                     // Verifica Fila GUI
    assert(RP_fila_imagem == rsp);
    assert(img_gui.byte_controle_anterior == UI_Apagar_Aquivo);
    assert(img_gui.byte_controle == UI_Falha);

    return 0;
}

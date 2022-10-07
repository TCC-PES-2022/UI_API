//
// pch.h
//

#pragma once

#include "gtest/gtest.h"
#pragma once

#include <stdio.h>
#include <cstdint>
#include <stdio.h>
#include <iostream>
#include <mutex>
#include <queue>
#include <string>		// Necess�rio para usar strings

#define PORT 8080

#define TAM_DIR 100 // Tamanho m�ximo do diret�rio
#define TIMER_THREAD 500 // 500 ms
#define TAM_LOG 20

#define DEBUG_UI_API_
#define UI__GUI 0
#define UI__CTL 1
using namespace std;
enum UI_RESP {
	RP_fila_imagem = 1,
	RP_fila_transf_imagem = 2,
	RP_fila_autenticacao = 3,
};
enum  UI_INTERFACE {

	UI_Falha = 4,
	UI_Ok = 1,
	UI_Timeout = 2,
	UI_Aguardar = 3,
	UI_Null = 0,
	UI_Iniciar_Transferencia = 5,
	UI_Andamento_Transferencia = 6,
	UI_Cancelar = 7,
	UI_Fim_Transferencia = 8,
	UI_Carregar_Imagem = 9,
	UI_Informacoes_Imagem = 10,
	UI_EnviarLogin = 11,
	//
	UI_TOT_ESTADOS,
};
///\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\ 
///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\

// Estrutura destinada � informa��es sobre o Diret�rio
typedef struct st_dir_img {
	char diretorio_img[TAM_DIR];
}st_img_ty;

// Estrutura destinada ao campo de informa��es da imagem
typedef struct st_info_img {


}st_info_img;

// Estrutura Das informa��es sobre a Imagem
typedef struct st_ui_image {

	uint8_t byte_controle;
	uint8_t byte_controle_anterior;
	st_dir_img dir_img;
	uint16_t status_transferencia;
	st_info_img img_info;
}st_ui_image;

///\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\ 
///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\

// Estrutura Das informa��es sobre a conexao
typedef struct st_info_conexao {


}st_info_conexao_ty;
// Estrutura Geral da Conexao
typedef struct st_ui_conexao {

	uint8_t byte_controle;
	st_info_conexao info_conexao;
}st_ui_conexao_ty;

///\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\ 
///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\

// Estrutura Das informa��es sobre a autentica��o
typedef struct st_info_aut {
	//hash, senha, etc
	uint8_t login[32];
	uint8_t senha[32];
}st_info_aut;
// Estrutura Geral da Autentica��o
typedef struct st_ui_aut {

	uint8_t byte_controle;
	uint8_t byte_controle_anterior;
	st_info_aut autenticao;
}st_ui_aut;

///\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\ 
///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\


//Fun��es referentes a Imagem
st_ui_image* carregar_imagem_GUI(uint8_t cmd_controle, st_ui_image* val);
st_ui_image* carregar_imagem_Controler(uint8_t cmd_controle, st_ui_image* val);

st_ui_image* transferir_imagem_GUI(uint8_t cmd_controle, st_ui_image* val);
st_ui_image* transferir_imagem_Controler(uint8_t cmd_controle, st_ui_image* val);
st_ui_image* informacoes_imagem(uint8_t cmd_controle, st_ui_image* val);
//Fun��es referentes a Conex�o
st_info_conexao* listar_conexoes(uint8_t cmd_controle, st_info_conexao* val);
st_info_conexao* estabelecer_conexao(uint8_t cmd_controle, st_info_conexao* val);
st_info_conexao* encerrar_conexao(uint8_t cmd_controle, st_info_conexao* val);
st_info_conexao* verificar_conexao(uint8_t cmd_controle, st_info_conexao* val);
//Fun��es referentes a Autentica��o
st_ui_aut* verificar_autenticacao_GUI(uint8_t cmd_controle, st_ui_aut* val);
st_ui_aut* verificar_autenticacao_Controler(uint8_t cmd_controle, st_ui_aut* val);

//
int iniciar_UI_interface(st_ui_image* val, st_ui_aut* val2);
int kill_UI_interface(st_ui_image* val);
uint8_t verificarFilas(st_ui_image* val, st_ui_aut* val2, uint8_t thread);
int quad(int v);
//DEBUG

int ui_api_debug(string txt);
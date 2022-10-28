#ifndef UI_API_H
#define UI_API_H

#include <stdio.h>
#include <string.h>
#include <iostream>
#include <queue>
#include <string>


#define UI_API_VERSION 0.0.4

#define TAM_MAX_LIST 10
#define TAM_DIR      100 // Tamanho máximo do diretório

#define DEBUG_UI_API
#define UI__GUI 1
#define UI__CTL 0
#define Addr_status 0
#define Addr_imagem 5
#define Addr_aut 20
#define Addr_con 30
using namespace std;

/****************/
/* ENUMERATIONS */
/****************/

enum UI_RESP {
    //Ao usar verificarFilas(..), tem-se as seguintes possibilidades
    RP_fila_imagem        = 1,  //Resposta da função carregar_imagem_XX(..)
    RP_fila_transf_imagem = 2,  //Resposta da função transferir_imagem_XX(..)
    RP_fila_autenticacao  = 3,  //Resposta da função verificar_autenticacao_XX(..)
    RP_fila_conexao       = 4,  //Resposta da função conexoes_XX(..)
};

enum  UI_INTERFACE {
    UI_Null                    = Addr_status,   // Comando de Reset da Maquina
    UI_Ok                      = Addr_status+1, // Comando OK
    UI_Timeout                 = Addr_status+2, // Comando Timeout
    UI_Aguardar                = Addr_status+3, // Comando Aguardar
    UI_Falha                   = Addr_status+4, // Comando de Falha (não ok)
    //Imagem
    UI_Iniciar_Transferencia   = Addr_imagem,   // Comando para Iniciar a Transferencia de Imagem
    UI_Andamento_Transferencia = Addr_imagem+1, // Comando indicando que a Transferencia está em andamento
    UI_Cancelar                = Addr_imagem+2, // Comando para Cancelar
    UI_Fim_Transferencia       = Addr_imagem+3, // Comando indicando fim da transferencia de imagem
    UI_Carregar_Imagem         = Addr_imagem+4, // Comando para o usuario carregar uma nova imagem para o sistema
    UI_Informacoes_Imagem      = Addr_imagem+5, // Comando para listar as informações das  imagens presentes
    UI_Apagar_Aquivo           = Addr_imagem+6, // Comando para apagar a imagem contida no sistema
    //Autenticacao
    UI_EnviarLogin             = Addr_aut,      // Comando para verificar o login digitado
    UI_Cadastrar               = Addr_aut+1,    // Comando para cadastrar novo usuario
    UI_RemoverUsuario          = Addr_aut+2,    // Comando para Remover usuário especifico
    //Conexao
    UI_ListarConexoes          = Addr_con,      // Comando para Listar as conexoes
    UI_EstabelecerConexao      = Addr_con+1,    // Comando para Estabelecer conexao
    UI_EncerrarConexao         = Addr_con+2,    // Comando para Encerrar conexao
    //
    UI_TOT_ESTADOS, /*TODO*/
};

/***********/
/* STRUCTS */
/***********/

// Estrutura destinada à informações sobre o Diretório
typedef struct st_dir_img {
    char *diretorio_img; // Diretorio da imagem
    char *diretorio_cmp; // Diretorio do arquivo de compatibilidade
    uint16_t tam_dir;
} st_dir_img;

// Estrutura destinada ao campo de informações da imagem
typedef struct st_info_img {
    // tamanho máximo de envio por requisição
    char **tabela_de_img;
    int tam;
} st_info_img;

// Estrutura Das informações sobre a Imagem
typedef struct st_ui_image {
    uint8_t byte_controle;
    uint8_t byte_controle_anterior;

    // diretorio a ser enviado para a pasta
    st_dir_img dir_img;
    // 0 - 1000 (1 CASA DECIMAL DE PRECISÃO)
    uint16_t status_transferencia;
    // ESTRUTURA QUE CONTEM A LISTA DE IMAGENS DO DIRETÓRIO
    st_info_img img_info;
} st_ui_image;

// Estrutura Das informações sobre a conexao
typedef struct st_info_conexao {
    /* TODO */

} st_info_conexao;

// Estrutura Geral da Conexao
typedef struct st_ui_conexao {
    uint8_t byte_controle;
    uint8_t byte_controle_anterior;
    st_info_conexao info_conexao;
} st_ui_conexao;

// Estrutura Das informações sobre a autenticação
typedef struct st_info_aut {
    //usuário e senha
    uint8_t login[32];
    uint8_t senha[32];
} st_info_aut;

// Estrutura Geral da Autenticação
typedef struct st_ui_aut {
    uint8_t byte_controle;
    uint8_t byte_controle_anterior;
    st_info_aut autenticao;
} st_ui_aut;

/*********************/
/* VISIBLE FUNCTIONS */
/*********************/

// Funções referentes a Imagem
st_ui_image* carregar_imagem_GUI(uint8_t cmd_controle, st_ui_image* val);
st_ui_image* carregar_imagem_Controler(uint8_t cmd_controle, st_ui_image* val);
st_ui_image* transferir_imagem_GUI(uint8_t cmd_controle, st_ui_image* val);
st_ui_image* transferir_imagem_Controler(uint8_t cmd_controle, st_ui_image* val);

// Funções referentes a Conexão
st_ui_conexao* conexoes_GUI(uint8_t cmd_controle, st_ui_conexao* val);
st_ui_conexao* conexoes_Controler(uint8_t cmd_controle, st_ui_conexao* val);
// Funções referentes a Autenticação
st_ui_aut* verificar_autenticacao_GUI(uint8_t cmd_controle, st_ui_aut* val);
st_ui_aut* verificar_autenticacao_Controler(uint8_t cmd_controle, st_ui_aut* val);


int iniciar_UI_interface(st_ui_image* val, st_ui_aut* val2,st_ui_conexao *val3, uint8_t thread);
uint8_t verificarFilas(st_ui_image* val, st_ui_aut* val2,st_ui_conexao *val3, uint8_t thread);

/* DEBUG */
int ui_api_debug(string txt);

#endif

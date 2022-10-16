#include <QtTest>
#include "../../UI_API.h"

// add necessary includes here

class Teste_UI_API : public QObject
{
    Q_OBJECT

public:
    Teste_UI_API();
    ~Teste_UI_API();

    st_ui_aut aut_gui;
    st_ui_image img_gui;
    st_ui_conexao con_gui;

    st_ui_image img_controler;
    st_ui_aut aut_controler;
    st_ui_conexao con_controler;
private slots:

    void test_Inicializacao_GUI();
    void test_Inicializacao_CTL();
    //Switch Test for authentication proccess

    void test_envio_autenticacao_ok();
    void test_envio_autenticacao_falha();
    void test_cadastrar_usuario_ok();
    void test_cadastrar_usuario_falha();
    void test_remover_usuario_ok();
    void test_remover_usuario_falha();

    void test_reiniciar_mq_GUI();
    void test_reiniciar_mq_CTL();
    //Switch Test for image directory proccess

    void test_envio_diretorio_imagem_ok();
    void test_envio_diretorio_imagem_falha();
};

Teste_UI_API::Teste_UI_API()
{

}

Teste_UI_API::~Teste_UI_API()
{

}

void Teste_UI_API::test_Inicializacao_GUI()
{
    uint8_t thread = UI__GUI;
    memset(&img_gui,0x1,sizeof(img_gui));
    memset(&aut_gui,0x1,sizeof(img_gui));
    memset(&con_gui,0x1,sizeof(img_gui));
    uint8_t ret = iniciar_UI_interface(&img_gui,&aut_gui,&con_gui, thread);
    QCOMPARE(ret,1);
    QCOMPARE(img_gui.byte_controle,0);
    QCOMPARE(img_gui.byte_controle_anterior,0);

    QCOMPARE(aut_gui.byte_controle,0);
    QCOMPARE(aut_gui.byte_controle_anterior,0);

    QCOMPARE(con_gui.byte_controle,0);
    QCOMPARE(con_gui.byte_controle_anterior,0);
}
void Teste_UI_API::test_Inicializacao_CTL()
{
    uint8_t thread = UI__CTL;
    memset(&img_controler,0x1,sizeof(img_controler));
    memset(&aut_controler,0x1,sizeof(aut_controler));
    memset(&con_controler,0x1,sizeof(con_controler));

    uint8_t ret = iniciar_UI_interface(&img_controler,&aut_controler,&con_controler, thread);
    QCOMPARE(ret,1);

    QCOMPARE(img_controler.byte_controle,0);
    QCOMPARE(img_controler.byte_controle_anterior,0);

    QCOMPARE(aut_controler.byte_controle,0);
    QCOMPARE(aut_controler.byte_controle_anterior,0);

    QCOMPARE(con_controler.byte_controle,0);
    QCOMPARE(con_controler.byte_controle_anterior,0);
}

void Teste_UI_API::test_envio_autenticacao_ok()
{
    aut_gui.byte_controle = UI_EnviarLogin;     //Set Comamand
    memcpy(aut_gui.autenticao.login,"Afonso",sizeof("Afonso"));     //Set Login
    memcpy(aut_gui.autenticao.senha,"123",sizeof("123"));   //Set Password
    aut_gui = *verificar_autenticacao_GUI(aut_gui.byte_controle, &aut_gui); //Send to ctr queue
    QCOMPARE(aut_gui.byte_controle,UI_Aguardar);    //Comparing control byte
    uint8_t val = verificarFilas(&img_controler, &aut_controler,&con_controler, UI__CTL); //Verifica fila do ctr
    QCOMPARE(val,RP_fila_autenticacao); // Verifica o retorno da fila
    QCOMPARE(aut_controler.byte_controle,UI_EnviarLogin); // compara o comando enviado
    aut_controler = *verificar_autenticacao_Controler(UI_Ok,&aut_controler); // envia resposta para a GUI
    QCOMPARE(aut_controler.byte_controle,UI_Aguardar);  // Verifica o estado da maquina de estados
    QCOMPARE(memcmp(&aut_controler.autenticao.login,"Afonso",sizeof("Afonso")),0); // verifica o login enviado
    QCOMPARE(memcmp(&aut_controler.autenticao.senha,"123",sizeof("123")),0);// verifica a senha enviada
    val = verificarFilas(&img_gui, &aut_gui,&con_gui, UI__GUI);  // verifica a fila da gui
    QCOMPARE(val,RP_fila_autenticacao);// Verifica retorno da fila
    QCOMPARE(memcmp(&aut_gui.autenticao.login,"Afonso",sizeof("Afonso")),0); // verifica se houve alteração no login enviado
    QCOMPARE(memcmp(&aut_gui.autenticao.senha,"123",sizeof("123")),0);// verifica se houve alteração na senha enviada
    QCOMPARE(aut_gui.byte_controle_anterior,UI_EnviarLogin); // compara o comando solicitado
    QCOMPARE(aut_gui.byte_controle,UI_Ok); // compara o comando enviado
}

void Teste_UI_API::test_envio_autenticacao_falha()
{
    aut_gui.byte_controle = UI_EnviarLogin;     //Set Comamand
    memcpy(aut_gui.autenticao.login,"Afonso",sizeof("Afonso"));     //Set Login
    memcpy(aut_gui.autenticao.senha,"123",sizeof("123"));   //Set Password
    aut_gui = *verificar_autenticacao_GUI(aut_gui.byte_controle, &aut_gui); //Send to ctr queue
    QCOMPARE(aut_gui.byte_controle,UI_Aguardar);    //Comparing control byte
    uint8_t val = verificarFilas(&img_controler, &aut_controler,&con_controler, UI__CTL); //Verifica fila do ctr
    QCOMPARE(val,RP_fila_autenticacao); // Verifica o retorno da fila
    QCOMPARE(aut_controler.byte_controle,UI_EnviarLogin); // compara o comando enviado
    aut_controler = *verificar_autenticacao_Controler(UI_Falha,&aut_controler); // envia resposta para a GUI
    QCOMPARE(aut_controler.byte_controle,UI_Aguardar);  // Verifica o estado da maquina de estados
    QCOMPARE(memcmp(&aut_controler.autenticao.login,"Afonso",sizeof("Afonso")),0); // verifica o login enviado
    QCOMPARE(memcmp(&aut_controler.autenticao.senha,"123",sizeof("123")),0);// verifica a senha enviada
    val = verificarFilas(&img_gui, &aut_gui,&con_gui, UI__GUI);  // verifica a fila da gui
    QCOMPARE(val,RP_fila_autenticacao);// Verifica retorno da fila
    QCOMPARE(memcmp(&aut_gui.autenticao.login,"Afonso",sizeof("Afonso")),0); // verifica se houve alteração no login enviado
    QCOMPARE(memcmp(&aut_gui.autenticao.senha,"123",sizeof("123")),0);// verifica se houve alteração na senha enviada
    QCOMPARE(aut_gui.byte_controle_anterior,UI_EnviarLogin); // compara o comando solicitado
    QCOMPARE(aut_gui.byte_controle,UI_Falha); // compara o comando enviado
}
void Teste_UI_API::test_cadastrar_usuario_ok()
{
     aut_gui.byte_controle = UI_Cadastrar;
     memcpy(aut_gui.autenticao.login,"Afonso",sizeof("Afonso"));     //Set Login
     memcpy(aut_gui.autenticao.senha,"123",sizeof("123"));   //Set Password

     aut_gui = *verificar_autenticacao_GUI(aut_gui.byte_controle, &aut_gui);
     QCOMPARE(aut_gui.byte_controle,UI_Aguardar);

     uint8_t val = verificarFilas(&img_controler, &aut_controler,&con_controler, UI__CTL); //Verifica fila do ctr
     QCOMPARE(val,RP_fila_autenticacao); // Verifica o retorno da fila
     QCOMPARE(aut_controler.byte_controle,UI_Cadastrar); // compara o comando enviado
     aut_controler = *verificar_autenticacao_Controler(UI_Ok,&aut_controler); // envia resposta para a GUI
     QCOMPARE(aut_controler.byte_controle,UI_Aguardar);  // Verifica o estado da maquina de estados
     QCOMPARE(memcmp(&aut_controler.autenticao.login,"Afonso",sizeof("Afonso")),0); // verifica o login enviado
     QCOMPARE(memcmp(&aut_controler.autenticao.senha,"123",sizeof("123")),0);// verifica a senha enviada
     val = verificarFilas(&img_gui, &aut_gui,&con_gui, UI__GUI);  // verifica a fila da gui
     QCOMPARE(val,RP_fila_autenticacao);// Verifica retorno da fila
     QCOMPARE(memcmp(&aut_gui.autenticao.login,"Afonso",sizeof("Afonso")),0); // verifica se houve alteração no login enviado
     QCOMPARE(memcmp(&aut_gui.autenticao.senha,"123",sizeof("123")),0);// verifica se houve alteração na senha enviada
     QCOMPARE(aut_gui.byte_controle_anterior,UI_Cadastrar); // compara o comando solicitado
     QCOMPARE(aut_gui.byte_controle,UI_Ok); // compara o comando enviado
}

void Teste_UI_API::test_cadastrar_usuario_falha()
{
     aut_gui.byte_controle = UI_Cadastrar;
     memcpy(aut_gui.autenticao.login,"Afonso",sizeof("Afonso"));     //Set Login
     memcpy(aut_gui.autenticao.senha,"123",sizeof("123"));   //Set Password

     aut_gui = *verificar_autenticacao_GUI(aut_gui.byte_controle, &aut_gui);
     QCOMPARE(aut_gui.byte_controle,UI_Aguardar);
     uint8_t val = verificarFilas(&img_controler, &aut_controler,&con_controler, UI__CTL); //Verifica fila do ctr

     QCOMPARE(val,RP_fila_autenticacao); // Verifica o retorno da fila
     QCOMPARE(aut_controler.byte_controle,UI_Cadastrar); // compara o comando enviado
     aut_controler = *verificar_autenticacao_Controler(UI_Falha,&aut_controler); // envia resposta para a GUI
     QCOMPARE(aut_controler.byte_controle,UI_Aguardar);  // Verifica o estado da maquina de estados
     QCOMPARE(memcmp(&aut_controler.autenticao.login,"Afonso",sizeof("Afonso")),0); // verifica o login enviado
     QCOMPARE(memcmp(&aut_controler.autenticao.senha,"123",sizeof("123")),0);// verifica a senha enviada
     val = verificarFilas(&img_gui, &aut_gui,&con_gui, UI__GUI);  // verifica a fila da gui
     QCOMPARE(val,RP_fila_autenticacao);// Verifica retorno da fila
     QCOMPARE(memcmp(&aut_gui.autenticao.login,"Afonso",sizeof("Afonso")),0); // verifica se houve alteração no login enviado
     QCOMPARE(memcmp(&aut_gui.autenticao.senha,"123",sizeof("123")),0);// verifica se houve alteração na senha enviada
     QCOMPARE(aut_gui.byte_controle_anterior,UI_Cadastrar); // compara o comando solicitado
     QCOMPARE(aut_gui.byte_controle,UI_Falha); // compara o comando enviado
}

void Teste_UI_API::test_remover_usuario_ok()
{
    aut_gui.byte_controle = UI_RemoverUsuario;
    memcpy(aut_gui.autenticao.login,"Afonso",sizeof("Afonso"));     //Set Login
    memcpy(aut_gui.autenticao.senha,"123",sizeof("123"));   //Set Password

    aut_gui = *verificar_autenticacao_GUI(aut_gui.byte_controle, &aut_gui);
    QCOMPARE(aut_gui.byte_controle,UI_Aguardar);
    uint8_t val = verificarFilas(&img_controler, &aut_controler,&con_controler, UI__CTL); //Verifica fila do ctr

    QCOMPARE(val,RP_fila_autenticacao); // Verifica o retorno da fila
    QCOMPARE(aut_controler.byte_controle,UI_RemoverUsuario); // compara o comando enviado
    aut_controler = *verificar_autenticacao_Controler(UI_Ok,&aut_controler); // envia resposta para a GUI
    QCOMPARE(aut_controler.byte_controle,UI_Aguardar);  // Verifica o estado da maquina de estados
    QCOMPARE(memcmp(&aut_controler.autenticao.login,"Afonso",sizeof("Afonso")),0); // verifica o login enviado
    QCOMPARE(memcmp(&aut_controler.autenticao.senha,"123",sizeof("123")),0);// verifica a senha enviada
    val = verificarFilas(&img_gui, &aut_gui,&con_gui, UI__GUI);  // verifica a fila da gui
    QCOMPARE(val,RP_fila_autenticacao);// Verifica retorno da fila
    QCOMPARE(memcmp(&aut_gui.autenticao.login,"Afonso",sizeof("Afonso")),0); // verifica se houve alteração no login enviado
    QCOMPARE(memcmp(&aut_gui.autenticao.senha,"123",sizeof("123")),0);// verifica se houve alteração na senha enviada
    QCOMPARE(aut_gui.byte_controle_anterior,UI_RemoverUsuario); // compara o comando solicitado
    QCOMPARE(aut_gui.byte_controle,UI_Ok); // compara o comando enviado
}
void Teste_UI_API::test_remover_usuario_falha()
{
    aut_gui.byte_controle = UI_RemoverUsuario;
    memcpy(aut_gui.autenticao.login,"Afonso",sizeof("Afonso"));     //Set Login
    memcpy(aut_gui.autenticao.senha,"123",sizeof("123"));   //Set Password

    aut_gui = *verificar_autenticacao_GUI(aut_gui.byte_controle, &aut_gui);
    QCOMPARE(aut_gui.byte_controle,UI_Aguardar);
    uint8_t val = verificarFilas(&img_controler, &aut_controler,&con_controler, UI__CTL); //Verifica fila do ctr

    QCOMPARE(val,RP_fila_autenticacao); // Verifica o retorno da fila
    QCOMPARE(aut_controler.byte_controle,UI_RemoverUsuario); // compara o comando enviado
    aut_controler = *verificar_autenticacao_Controler(UI_Falha,&aut_controler); // envia resposta para a GUI
    QCOMPARE(aut_controler.byte_controle,UI_Aguardar);  // Verifica o estado da maquina de estados
    QCOMPARE(memcmp(&aut_controler.autenticao.login,"Afonso",sizeof("Afonso")),0); // verifica o login enviado
    QCOMPARE(memcmp(&aut_controler.autenticao.senha,"123",sizeof("123")),0);// verifica a senha enviada
    val = verificarFilas(&img_gui, &aut_gui,&con_gui, UI__GUI);  // verifica a fila da gui
    QCOMPARE(val,RP_fila_autenticacao);// Verifica retorno da fila
    QCOMPARE(memcmp(&aut_gui.autenticao.login,"Afonso",sizeof("Afonso")),0); // verifica se houve alteração no login enviado
    QCOMPARE(memcmp(&aut_gui.autenticao.senha,"123",sizeof("123")),0);// verifica se houve alteração na senha enviada
    QCOMPARE(aut_gui.byte_controle_anterior,UI_RemoverUsuario); // compara o comando solicitado
    QCOMPARE(aut_gui.byte_controle,UI_Falha); // compara o comando enviado
}

void Teste_UI_API::test_reiniciar_mq_GUI()
{
    img_gui = *carregar_imagem_GUI(UI_Null, &img_gui);
    aut_gui = *verificar_autenticacao_GUI(UI_Null, &aut_gui);
    QCOMPARE(aut_gui.byte_controle_anterior,UI_Null);
    QCOMPARE(aut_gui.byte_controle,UI_Null);
    QCOMPARE(img_gui.byte_controle_anterior,UI_Null);
    QCOMPARE(img_gui.byte_controle,UI_Null);
}

void Teste_UI_API::test_reiniciar_mq_CTL()
{
    img_controler = *carregar_imagem_Controler(UI_Null, &img_controler);
    aut_controler = *verificar_autenticacao_Controler(UI_Null, &aut_controler);
    QCOMPARE(aut_controler.byte_controle_anterior,UI_Null);
    QCOMPARE(aut_controler.byte_controle,UI_Null);
    QCOMPARE(img_controler.byte_controle_anterior,UI_Null);
    QCOMPARE(img_controler.byte_controle,UI_Null);
}

void Teste_UI_API::test_envio_diretorio_imagem_ok()
{
    img_gui.dir_img.diretorio_img = "/csr/afsfd/asdasd/afonso/qqq/892139291.bin"; // Diretório teste
    img_gui.dir_img.tam_dir = sizeof("/csr/afsfd/asdasd/afonso/qqq/892139291.bin"); // tamanho do diretório
    img_gui = *carregar_imagem_GUI(UI_Carregar_Imagem,&img_gui); // comando enviado
    QCOMPARE(img_gui.byte_controle,UI_Aguardar); // status da maquina
    //
    uint8_t val = verificarFilas(&img_controler, &aut_controler,&con_controler, UI__CTL); // verificar a fila
    QCOMPARE(val,RP_fila_imagem); // resposta da funcao acima
    QCOMPARE(img_controler.byte_controle,UI_Carregar_Imagem); // comando recebido
    img_controler = *carregar_imagem_Controler(UI_Ok,&img_controler); // comando de resposta enviado
    QCOMPARE(img_controler.byte_controle,UI_Aguardar); // status da maquina
    QCOMPARE(img_controler.dir_img.diretorio_img,"/csr/afsfd/asdasd/afonso/qqq/892139291.bin"); // diretório recebido
    QCOMPARE(img_controler.dir_img.tam_dir,sizeof("/csr/afsfd/asdasd/afonso/qqq/892139291.bin"));//tamanhao recebido
    // Reset
    img_controler = *carregar_imagem_Controler(UI_Null, &img_controler); // reset da maquina
    QCOMPARE(img_controler.byte_controle_anterior,UI_Null);// status da maquina
    QCOMPARE(img_controler.byte_controle,UI_Null);// status da maquina
    //
    val = verificarFilas(&img_gui, &aut_gui,&con_gui, UI__GUI); // verifica a fila
    QCOMPARE(val,RP_fila_imagem); // resposta da funcao acima
    QCOMPARE(img_gui.byte_controle_anterior,UI_Carregar_Imagem); // comando anterior
    QCOMPARE(img_gui.byte_controle,UI_Ok); // resposta recebida
    // Reset
    img_gui = *carregar_imagem_GUI(UI_Null, &img_gui); // reset da maquina
    QCOMPARE(img_gui.byte_controle_anterior,UI_Null); // status da maquina
    QCOMPARE(img_gui.byte_controle,UI_Null);// status da maquina

}

void Teste_UI_API::test_envio_diretorio_imagem_falha()
{
    img_gui.dir_img.diretorio_img = "/csr/afsfd/asdasd/afonso/qqq/892139291.bin";
    img_gui.dir_img.tam_dir = sizeof("/csr/afsfd/asdasd/afonso/qqq/892139291.bin");
    img_gui = *carregar_imagem_GUI(UI_Carregar_Imagem,&img_gui);
    QCOMPARE(img_gui.byte_controle,UI_Aguardar);
    //
    uint8_t val = verificarFilas(&img_controler, &aut_controler,&con_controler, UI__CTL);
    cout << "Diretorio: " << img_controler.dir_img.diretorio_img <<endl;
    cout << "Tam: " << img_controler.dir_img.tam_dir <<endl;
    QCOMPARE(val,RP_fila_imagem);
    QCOMPARE(img_controler.byte_controle,UI_Carregar_Imagem);
    img_controler = *carregar_imagem_Controler(UI_Falha,&img_controler);
    QCOMPARE(img_controler.byte_controle,UI_Aguardar);
    QCOMPARE(img_controler.dir_img.diretorio_img,"/csr/afsfd/asdasd/afonso/qqq/892139291.bin");
    QCOMPARE(img_controler.dir_img.tam_dir,sizeof("/csr/afsfd/asdasd/afonso/qqq/892139291.bin"));
    // Reset
    img_controler = *carregar_imagem_Controler(UI_Null, &img_controler);
    QCOMPARE(img_controler.byte_controle_anterior,UI_Null);
    QCOMPARE(img_controler.byte_controle,UI_Null);
    //
    val = verificarFilas(&img_gui, &aut_gui,&con_gui, UI__GUI);
    cout << "Diretorio: " << img_gui.dir_img.diretorio_img <<endl;
    cout << "Tam: " << img_gui.dir_img.tam_dir <<endl;
    QCOMPARE(val,RP_fila_imagem);
    QCOMPARE(img_gui.byte_controle_anterior,UI_Carregar_Imagem);
    QCOMPARE(img_gui.byte_controle,UI_Falha);
    // Reset
    img_gui = *carregar_imagem_GUI(UI_Null, &img_gui);
    QCOMPARE(img_gui.byte_controle_anterior,UI_Null);
    QCOMPARE(img_gui.byte_controle,UI_Null);
}

QTEST_APPLESS_MAIN(Teste_UI_API)

#include "tst_teste_ui_api.moc"

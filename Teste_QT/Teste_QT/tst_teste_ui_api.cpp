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
    void test_envio_autenticacao();
    void test_recebimento_autenticacao();
    void test_resposta_autenticacao();
    void test_reiniciar_mq_GUI();
    void test_reiniciar_mq_CTL();
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
void Teste_UI_API::test_envio_autenticacao()
{
    static st_ui_aut aut_gui;
    aut_gui.byte_controle = UI_EnviarLogin;
    memcpy(aut_gui.autenticao.login,"Afonso",sizeof("Afonso"));
    memcpy(aut_gui.autenticao.senha,"123",sizeof("123"));
    aut_gui = *verificar_autenticacao_GUI(aut_gui.byte_controle, &aut_gui);
    QCOMPARE(aut_gui.byte_controle,UI_Aguardar);
}
void Teste_UI_API::test_recebimento_autenticacao()
{

    uint8_t val = verificarFilas(&img_controler, &aut_controler,&con_controler, UI__CTL);
    cout << "Senha: " << aut_controler.autenticao.senha <<endl;
    cout << "Login: " << aut_controler.autenticao.login <<endl;

    QCOMPARE(val,RP_fila_autenticacao);
    QCOMPARE(aut_controler.byte_controle,UI_EnviarLogin);
    aut_controler = *verificar_autenticacao_Controler(UI_Ok,&aut_controler);
    QCOMPARE(aut_controler.byte_controle,UI_Aguardar);
}

void Teste_UI_API::test_resposta_autenticacao()
{
    uint8_t val = verificarFilas(&img_gui, &aut_gui,&con_gui, UI__GUI);
    cout << "Senha: " << aut_gui.autenticao.senha <<endl;
    cout << "Login: " << aut_gui.autenticao.login <<endl;
    QCOMPARE(val,RP_fila_autenticacao);
    QCOMPARE(aut_gui.byte_controle_anterior,UI_EnviarLogin);
    QCOMPARE(aut_gui.byte_controle,UI_Ok);

}

void Teste_UI_API::test_reiniciar_mq_GUI()
{
    std::cout << "+[CTL] Resetando MQ... " << endl;
    img_gui = *carregar_imagem_GUI(UI_Null, &img_gui);
    aut_gui = *verificar_autenticacao_GUI(UI_Null, &aut_gui);
    QCOMPARE(aut_gui.byte_controle_anterior,UI_Null);
    QCOMPARE(aut_gui.byte_controle,UI_Null);
    QCOMPARE(img_gui.byte_controle_anterior,UI_Null);
    QCOMPARE(img_gui.byte_controle,UI_Null);
}

void Teste_UI_API::test_reiniciar_mq_CTL()
{
    std::cout << "+[CTL] Resetando MQ... " << endl;
    img_controler = *carregar_imagem_Controler(UI_Null, &img_controler);
    aut_controler = *verificar_autenticacao_Controler(UI_Null, &aut_controler);
    QCOMPARE(aut_controler.byte_controle_anterior,UI_Null);
    QCOMPARE(aut_controler.byte_controle,UI_Null);
    QCOMPARE(img_controler.byte_controle_anterior,UI_Null);
    QCOMPARE(img_controler.byte_controle,UI_Null);
}
QTEST_APPLESS_MAIN(Teste_UI_API)

#include "tst_teste_ui_api.moc"

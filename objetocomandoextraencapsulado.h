#ifndef OBJETOCOMANDOEXTRAENCAPSULADO_H
#define OBJETOCOMANDOEXTRAENCAPSULADO_H

#include "comandoextra.h"
#include <QLineEdit>
#include <QGroupBox>


class ObjetoComandoExtraEncapsulado
{
public:
    ObjetoComandoExtraEncapsulado();

    QGroupBox* grupo;
    QLineEdit* registroText;
    QLineEdit* valorText;
    ComandoExtra comandoExtra;

};

#endif // OBJETOCOMANDOEXTRAENCAPSULADO_H

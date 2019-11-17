#ifndef DOUBLEVALIDATOR_H
#define DOUBLEVALIDATOR_H

#include <QObject>
#include <QValidator>

class DoubleValidator : public QValidator
{
    Q_OBJECT
public:
    explicit DoubleValidator(QObject* parent = 0) { }
    virtual State validate(QString & input, int & pos) const
    {
        QString valoresValidos = "0123456789,";
        if (input.isEmpty())
            return Acceptable;
        for (int i = 0; i < input.length(); i++) {

            if (!valoresValidos.contains(QString(input.at(i)))) {
                return Invalid;
            }

        }
        return Acceptable;
    }
};

#endif // DOUBLEVALIDATOR_H

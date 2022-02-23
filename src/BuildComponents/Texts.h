#ifndef TEXTS_H
#define TEXTS_H

#include "BuildComponentBase.h"
#include <QObject>
#include <QMap>


class Texts : public BuildComponentBase
{
    // The map that link the identifier of the text with it's actual text
    QMap<QString, QString> m_texts;

public:
    ///
    /// \brief Coordinates Contruct the Coordinates object
    /// \param project The project that we are working on
    ///
    Texts(Project* project);



    QString GetText(const QString& text);

    // BuildComponentBase interface
public:
    void FromJson(const QJsonObject &obj);
    QJsonObject GetJsonTemplateFile();
};
#endif // TEXTS_H

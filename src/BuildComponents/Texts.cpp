#include "Texts.h"

#include <QJsonObject>

Texts::Texts(Project *project)
    :BuildComponentBase(project)
{

}

QString Texts::GetText(const QString &text)
{
    if(m_texts.contains(text)) {
        return m_texts[text];
    }
    return text;
}

void Texts::FromJson(const QJsonObject &obj)
{
    QJsonObject TextsObj = obj["texts"].toObject();

    // Add the integers to the map
    QStringList TextsKeys = TextsObj.keys();
    // Loop throught all the texts in the container object
    for(int i = 0 ; i < TextsKeys.size() ; i++) {
        QString key = TextsKeys[i];
        if(m_texts.contains(key))    qWarning()<< "<Texts::FromJson> There is already a text with the name : " + key + ".";
        else if(!TextsObj[key].isString()) qWarning()<< "<Texts::FromJson> The 'texts' childs must be JsonArray. Key : "+ key;
        else {
            // Get one single quest
            QString texts_js = TextsObj[key].toString();
            m_texts.insert(key , texts_js);
            qDebug()<< "<Texts::FromJson> Registered the text : " << key << ".";
        }
    }
}

QJsonObject Texts::GetJsonTemplateFile()
{
    QJsonObject rootObject;
    QJsonObject textsObject;
    textsObject.insert("quest.castle.description" , "Kill the giant spider.");
    rootObject.insert("texts" , textsObject);
    return rootObject;
}

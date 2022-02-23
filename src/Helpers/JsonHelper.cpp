#include "JsonHelper.h"

#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>

QJsonObject JsonHelper::ParseJsonObject(const QString &value)
{
    QJsonObject obj;
    QJsonDocument doc = QJsonDocument::fromJson(value.toUtf8());
    if(doc.isObject()) {
        return doc.object();
    }
    return obj;
}

QJsonArray JsonHelper::ParseJsonArray(const QString &value)
{
    QJsonArray array;
    QJsonDocument doc = QJsonDocument::fromJson(value.toUtf8());
    if(doc.isArray()) {
        return doc.array();
    }
    return array;
}

QStringList JsonHelper::GetCommandsList(const QJsonObject &sourceObject, const QString &key)
{
    // THe string list that we will return
    QStringList final = QStringList();
    // If the object contains a good array with the good key
    if(sourceObject.contains(key) && sourceObject[key].isArray()){
        QJsonArray array = sourceObject[key].toArray();
        // Loop throught all the items contained intot the array
        for(int i = 0 ; i < array.size() ; i++) {
            // If the item is a string
            if(array[i].isString()) {
                // Add the item to the funal string list
                final.push_back(array[i].toString());
            }
        }
    }
    // Return the final list of string
    return final;
}

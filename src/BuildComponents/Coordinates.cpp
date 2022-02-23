#include "Coordinates.h"

#include "QJsonObject"
#include "QJsonArray"


Coordinates::Coordinates(Project *project)
    :BuildComponentBase(project)
{

}

QString Coordinates::GetCoordinates(const QString &key)
{
    if(m_coordinates.contains(key)) {
        Coordinate coord = m_coordinates[key];
        return QString::number(coord.x) + " " + QString::number(coord.y) + " " +QString::number(coord.z);
    }
    else {
        qWarning()<<"<Coordinates::GetCoordinates> No coordinates were found for the key : "<< key;
    }
    return "0 0 0";
}

QString Coordinates::GetCoordinatesRot(const QString &key)
{
    if(m_coordinates.contains(key)) {
        Coordinate coord = m_coordinates[key];
        return QString::number(coord.x) + " " + QString::number(coord.y) + " " +QString::number(coord.z) + " " + QString::number(coord.xRot) + " " + QString::number(coord.yRot);
    }
    else {
        qWarning()<<"<Coordinates::GetCoordinates> No coordinates were found for the key : "<< key;
    }
    return "0 0 0 0 0";
}

QString Coordinates::GetCoordintatesTarget(const QString &key)
{
    if(m_coordinates.contains(key)) {
        Coordinate coord = m_coordinates[key];
        return "x=" + QString::number(coord.x) + ",y=" + QString::number(coord.y) + ",z=" +QString::number(coord.z);
    }
    else {
        qWarning()<<"<Coordinates::GetCoordinates> No coordinates were found for the key : "<< key;
    }
    return "x=0,y=0,z=0";
}



void Coordinates::FromJson(const QJsonObject &obj)
{
    QJsonObject CoordinatesObj = obj["coordinates"].toObject();



    // Add the integers to the map
    QStringList CoordinatesKeys = CoordinatesObj.keys();
    // Loop throught all the cinematics in the container object
    for(int i = 0 ; i < CoordinatesKeys.size() ; i++) {
        QString key = CoordinatesKeys[i];
        if(m_coordinates.contains(key))    qWarning()<< "<Coordinates::FromJson> There is already a coordinate with the name : " + key + ".";
        else if(!CoordinatesObj[key].isArray()) qWarning()<< "<Coordinates::FromJson> The 'coordinates' childs must be JsonArray. Key : "+ key;
        else {
            // Get one single quest
            QJsonArray coordinate_js = CoordinatesObj[key].toArray();
            // Create this quest from json
            Coordinate coord;
            if(coord.FromJson(coordinate_js)) {
                m_coordinates.insert(key, coord);
                qDebug()<< "<Coordinates::FromJson> Registered the coordinate: " << key << ".";
            }
        }
    }
}

QJsonObject Coordinates::GetJsonTemplateFile()
{
    QJsonObject rootObject;
    QJsonObject CoordinatesObject;
    QJsonArray CoordinateArray;
    CoordinateArray.push_back(30);
    CoordinateArray.push_back(90);
    CoordinateArray.push_back(12);
    CoordinateArray.push_back(45.12);
    CoordinateArray.push_back(-10.3);
    CoordinatesObject.insert("post_office", CoordinateArray);
    rootObject.insert("coordinates" , CoordinatesObject);
    return rootObject;
}

bool Coordinate::FromJson(const QJsonArray &array)
{
    x = array[0].toDouble(0);
    y = array[1].toDouble(0);
    z = array[2].toDouble(0);
    xRot = array[3].toDouble(0);
    yRot = array[4].toDouble(0);

    return true;
}

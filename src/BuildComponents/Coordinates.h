#ifndef COORDINATES_H
#define COORDINATES_H

#include "BuildComponentBase.h"
#include <QObject>
#include <QMap>


class Coordinate {
public:
    float x = 0;
    float y = 0;
    float z = 0;
    float xRot = 0;
    float yRot = 0;

    bool FromJson(const QJsonArray& array);

};


class Coordinates : public BuildComponentBase
{
    // The map that link the identifier of the coordinates with their coordinates
    QMap<QString, Coordinate> m_coordinates;

public:
    ///
    /// \brief Coordinates Contruct the Coordinates object
    /// \param project The project that we are working on
    ///
    Coordinates(Project* project);



    QString GetCoordinates(const QString& key);
    QString GetCoordinatesRot(const QString& key);
    QString GetCoordintatesTarget(const QString& key);

    // BuildComponentBase interface
public:
    void FromJson(const QJsonObject &obj);
    QJsonObject GetJsonTemplateFile();
};

#endif // COORDINATES_H

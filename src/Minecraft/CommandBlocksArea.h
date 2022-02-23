#ifndef COMMANDBLOCKSAREA_H
#define COMMANDBLOCKSAREA_H

#include <QObject>
#include <QMap>

///
/// \brief The CommandBlocksArea class
///
/// Palette :
///     0 : air
///     1 : stone
///     2 : redstone_block
///     3 : lapis
///     4 : command_block
///     5 : repeating_command_block
///

class CommandBlockData {
public:
    QString Command;
    QString CustomName;
    int DelayInTick;
    __int16 ExecuteOnFirstTick = 0;
    __int16 conditionMet = 0;
};


const int ChunkBlocksNumber = 4096;

class CommandBlocksArea
{

    QMap<QString, CommandBlockData> m_commandsBlocksData;

    int m_blocksId[ChunkBlocksNumber];


public:
    CommandBlocksArea();

    void SetBlockAt(int x, int y , int z , QString identifier);
    void SetCommandBlockAt(int x, int y, int z, QString Command, int delay_in_ticks = 0);
    void SetTickingCommandBlockAt(int x, int y, int z , QString Command, int delay_in_ticks = 0);

    // Save as a mcstructure
    void Save(const QString& path);


    int BlockIdToPaletteIndex(QString identifier);

};

#endif // COMMANDBLOCKSAREA_H

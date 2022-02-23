#include "CommandBlocksArea.h"

#include "Minecraft/QtNbt.h"

#include <QDebug>

NbtCompound GetBlockPalette(const QString& identifer) {
    NbtCompound compound;
    compound.insert("name" , new NbtString(identifer));
    compound.insert("version" , new NbtInt(17959425));
    compound.insert("states" , new NbtCompound());
    return compound;
}


NbtCompound GetCommandBlockBlockEntityData(const CommandBlockData& data) {
    NbtCompound compound;
    NbtCompound* block_entity_data = new NbtCompound();
    compound.insert("block_entity_data" , block_entity_data);

    block_entity_data->insert("Command", new NbtString(data.Command));
    block_entity_data->insert("CustomName", new NbtString(data.CustomName));
    block_entity_data->insert("ExecuteOnFirstTick" , new NbtByte(data.ExecuteOnFirstTick));
    block_entity_data->insert("LPCommandMode" , new NbtInt(0));
    block_entity_data->insert("LPCondionalMode" , new NbtByte(0));
    block_entity_data->insert("LPRedstoneMode" , new NbtByte(0));
    block_entity_data->insert("LastExecution", new NbtLong(0));
    block_entity_data->insert("LastOutput", new NbtString(""));
    block_entity_data->insert("LastOutputParams", new NbtList(TAG_String));
    block_entity_data->insert("SuccessCount", new NbtInt(0));
    block_entity_data->insert("TickDelay", new NbtInt(data.DelayInTick));
    block_entity_data->insert("TrackOutput", new NbtByte(0));
    block_entity_data->insert("Version", new NbtInt(18));
    block_entity_data->insert("Auto", new NbtByte(0));
    block_entity_data->insert("conditionMet", new NbtByte(0));
    block_entity_data->insert("conditionalMode", new NbtByte(0));
    block_entity_data->insert("id", new NbtString("CommandBlock"));
    block_entity_data->insert("isMovable", new NbtByte(1));
    block_entity_data->insert("powered", new NbtByte(0));
    block_entity_data->insert("x", new NbtInt(0));
    block_entity_data->insert("y", new NbtInt(0));
    block_entity_data->insert("z", new NbtInt(0));


    return compound;
}



CommandBlocksArea::CommandBlocksArea()
{
    // Initialise the blocks array
    for(int i = 0 ; i < ChunkBlocksNumber; i++) {
        m_blocksId[i] = 0;
    }
}

void CommandBlocksArea::SetBlockAt(int x, int y, int z, QString identifier)
{
    // Get the place of the block on the array of blocks
    int index = x + y * 16 + z * 16 * 16;
    int paletteIndex= BlockIdToPaletteIndex(identifier);
    if(paletteIndex != -1) {
        m_blocksId[index] = paletteIndex;
    }
}

void CommandBlocksArea::SetCommandBlockAt(int x, int y, int z, QString Command, int delay_in_ticks)
{
    CommandBlockData data;
    data.Command = Command;
    data.CustomName = "";
    data.DelayInTick = delay_in_ticks;
    data.ExecuteOnFirstTick = 0;
    data.conditionMet = 0;

    // Get the place of the block on the array of blocks
    int index = x + y * 16 + z * 16 * 16;
    // Add the blocks data to the map of command blocks ( for the block entity data)
    m_commandsBlocksData.insert(QString::number(index), data);

    // Set the block to command block
    m_blocksId[index] = 4;
}

void CommandBlocksArea::SetTickingCommandBlockAt(int x, int y, int z, QString Command, int delay_in_ticks)
{
    CommandBlockData data;
    data.Command = Command;
    data.CustomName = "";
    data.DelayInTick = delay_in_ticks;
    data.ExecuteOnFirstTick = 1;
    data.conditionMet = 1;

    // Get the place of the block on the array of blocks
    int index = x + y * 16 + z * 16 * 16;
    // Add the blocks data to the map of command blocks ( for the block entity data)
    m_commandsBlocksData.insert(QString::number(index), data);

    // Set the block to command block
    m_blocksId[index] = 5;


}



void CommandBlocksArea::Save(const QString &path)
{
    NbtCompound rootCompound;

    // Add the format version
    rootCompound.insert("format_version" , new NbtInt(1));

    // Add the size
    NbtList size(NbtType::TAG_Int);
    size.pushBack(new NbtInt(16));
    size.pushBack(new NbtInt(16));
    size.pushBack(new NbtInt(16));
    rootCompound.insert("size", &size);

    // Add the structure_world_origin
    NbtList structure_world_origin(NbtType::TAG_Int);
    structure_world_origin.pushBack(new NbtInt(0));
    structure_world_origin.pushBack(new NbtInt(0));
    structure_world_origin.pushBack(new NbtInt(0));
    rootCompound.insert("structure_world_origin", &structure_world_origin);

    // Add the structure
    NbtCompound structure;
    rootCompound.insert("structure" , &structure);

    // First, create the pallette
    NbtCompound palette;
    structure.insert("palette" , &palette);
    NbtCompound default_palette;
    palette.insert("default", &default_palette);

    //
    // Block palette
    //
    NbtList block_palette(NbtType::TAG_Compound);
    default_palette.insert("block_palette" , &block_palette);

    // Stone (index 0)
    NbtCompound air = GetBlockPalette("minecraft:air");
    block_palette.pushBack(&air);

    // Stone (index 1)
    NbtCompound stone = GetBlockPalette("minecraft:stone");
    stone.at("states")->toCoumpound()->insert("stone_type", new NbtString("stone"));
    block_palette.pushBack(&stone);

    // Stone (index 2)
    NbtCompound redstone_block = GetBlockPalette("minecraft:redstone_block");
    block_palette.pushBack(&redstone_block);

    // Stone (index 3)
    NbtCompound lapis_block = GetBlockPalette("minecraft:lapis_block");
    block_palette.pushBack(&lapis_block);

    // Stone (index 4)
    NbtCompound command_block = GetBlockPalette("minecraft:command_block");
    command_block.at("states")->toCoumpound()->insert("conditional_bit", new NbtByte(0));
    command_block.at("states")->toCoumpound()->insert("facing_direction", new NbtInt(1));
    block_palette.pushBack(&command_block);

    // Stone (index 5)
    NbtCompound repeating_command_block = GetBlockPalette("minecraft:repeating_command_block");
    repeating_command_block.at("states")->toCoumpound()->insert("conditional_bit", new NbtByte(0));
    repeating_command_block.at("states")->toCoumpound()->insert("facing_direction", new NbtInt(1));
    block_palette.pushBack(&repeating_command_block);





    // Block Position Data
    NbtCompound block_position_data;
    default_palette.insert("block_position_data", &block_position_data);


    QStringList commandsBlocksDataKeys = m_commandsBlocksData.keys();
    for(int i = 0 ; i < commandsBlocksDataKeys.size() ; i++) {
        QString key = commandsBlocksDataKeys[i];
        CommandBlockData data = m_commandsBlocksData[key];
        NbtCompound* compound = new NbtCompound(GetCommandBlockBlockEntityData(data));
        block_position_data.insert(key, compound);
    }


    // Add the entities
    NbtList entities(TAG_Compound);
    structure.insert("entities", &entities);

    NbtList block_indices(NbtType::TAG_List);
    structure.insert("block_indices", &block_indices);

    NbtList block_indices_0(NbtType::TAG_Int);
    NbtList block_indices_1(NbtType::TAG_Int);
    block_indices.pushBack(&block_indices_0);
    block_indices.pushBack(&block_indices_1);

    for(int i = 0 ; i < ChunkBlocksNumber ; i ++ ) {
        block_indices_0.pushBack(new NbtInt(m_blocksId[i]));
        block_indices_1.pushBack(new NbtInt(-1));
    }



    // Create a document and save it into the path
    NbtDocument doc(&rootCompound);
    doc.SaveAs(path);

}

int CommandBlocksArea::BlockIdToPaletteIndex(QString identifier)
{
    if(identifier == "air") {
        return 0;
    }
    if(identifier == "stone") {
        return 1;
    }
    if(identifier == "redstone_block") {
        return 2;
    }
    if(identifier == "lapis_lazuli") {
        return 3;
    }


    qDebug()<< "Impossible to set the block "<< identifier;
    return -1;
}










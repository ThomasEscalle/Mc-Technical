
# Table of content

* [Concepts](#concepts)
    * [Variables](#variables)
    * [Ticks](#ticks)
    * [Delayed Events](#delayed-events)
* [Commands](#commands)
    * [Minecraft commands](#minecraft-commands)
    * [Comments](#comments)
    * [Variables modifications commands](#variables-modifications-commands)
    * [Conditional commands](#conditional-commands)
    * [Ticking commands](#ticking-commands)
    * [Delayed commands](#delayed-commands)
    * [Cinematics commands](#cinematics-commands)
    * [Quests commands](#quests-commands)
* [Commands preprocessing](#commands-preprocessing)
    * [Coordinates](#coordinates)
    * [Coordinates with rotations](#coordinates-with-rotations)
    * [Coordinates from target](#coordinates-from-target)
    * [Texts](#texts)
* [Hierarchy](#hierarchy)


<br> <br> <br> <br> <br>

# Concepts

## Variables
Variables are used to store some persistant data. We can modify them throught commands and they can be used to conditionaly trigger some others commands.

There are two types of variables :
* Boolean variables, they can be equal to true or false.
They are represented as a block in the redstone block chunk that is equal to stone (false) or lapis lazuly (true)
* Integers varibles, they can be equal to any integer like 0, 35 or 15566
They are represented in minecraft as a scoreboard applied to the game controller entity.

You can apply the variables a default value when you define them from the variables file. This default value will be reset ingame when you reset the map.

For technical reasons, there is a maximum of 256 booleans variables defined and 256 integers that can be defined (not including the reserved ones).

There are some variables names already reserved that you are not allowed to use:
* ``cinematic`` (represent the current tick of a running cinematic).

***

## Ticks
Ticks is a list of commands that is running on a repeating command block.
You can start them and stop them by calling their name in a command.

You can add a delay between every tick.

***

## Delayed Events
Delayed commands are a list of commands that are executed after a certain number of ticks.

<br> <br> <br> <br> <br>

# Commands
Commands are used to trigger something. They are represented by a string in a Json array.
There are different types of commands.
The type of command is defined by the first character or by the first keyword of the string.

<br>

## Minecraft Commands:
Minecraft commands start with a ``/``. 
The ``/`` character will be removed when creating the function.
Example :
```json
{
    "on_selected" : [
        "/say hello" // Say helo in the chat
        "kill @a"    // Command without '/' also works
    ]
}
```

***
## Comments:
Comments start with a ``#``. Example:
```json
{
    "on_selected" : [
        "# This is a comment"
    ]
}
```



***
## Variables modifications commands:
Variables modification starts with a ``$`` or with the ``var`` keyword.
Example :
```json
{
    "on_selected" : [
        "$myvar = 3",   // Set myvar to 3
        "$mySecondVar += 2",  // Add 2 to mySecondVar
        "$myThirdVar = myvar", // Set the value of myThirdVar to the value of myvar
        "var myBooleanVar = false", // Set the value of the boolean myBooleanVar to false 
        "var var3 ++" // Increment the var3 varible by one
    ]
}
```

***
## Conditional commands:
Conditional commands starts with a ```?``` or with the ```if``` keyword
Example :
```json
{
    "on_selected" : [
        "?myvar == 3 /say hello", // trigger a minecraft command
        "?myBool != false $myVar = 4" // set the variable myVar to 4,
        "if myvar == 3 if myBool == false /kill @a" // test for two conditions (equal to a && in c++).
    ]
}
```

There are multiple types of conditional operators:

Booleans operator :
* ```==``` equal to another variable or to a constant
* ```!=``` different from a variable or from a constant

Integers operators :

***
## Ticking commands:
Ticking commands starts with a ```*``` or with the ```tick``` keyword.
Example :
```json
{
    "on_selected" : [
        "*start myTickingEvents", // Start a ticking event.
        "tick finish mySecondTickingEvent" // Finish a ticking event.
    ]
}
```

Command format : ``` tick 'action' 'tick_identifier'```

You are allowed to use two types of actions for the ticks :
* Start the ticks : By using the ```start``` keyword.
* Finish the ticks : By using the ```finish``` or ``end`` keyword.



***
## Delayed commands:
Delayed commands start with a ```-``` or with the keyword ```delayed```.
Example :
```json
{
    "on_selected" : [
        "-my_delayed_command",   // Trigger the corresponding ticking command.
        "delayed delayedCommand" // Also start the ticking command
    ]
}
```

You can only start a delayed command, and not stop it in while waiting.

The format of the command is : ```delayed 'delayed_identifier'```



***
## Cinematics commands:
Cinematics commands start with the keyword ```cine```. Example :
```json
{
    "on_selected" : [
        "cine start myCinematic", // Start the cinematic named : myCinematic
    ]
}
```

For now, you can only start a cinematic and not stop it while running.
The format of the command is : ```cine 'action' 'cinematic_identifier'```

The possible actions are :
* Start : Start a cinematic by using the keyword ```start```

***
## Quests commands:
Quests commands starts with the keyword ```quest```
```json
{
    "on_selected" : [
        "quest unlock myquest", // Unlock the quest named 'myquest'
        "quest select mySecondQuest", // Select a quest named 'mySecondQuest'
        "quest next", // Triggers the next sub quest from the selected quest
        "quest debug" // Debug the selected quest and it's subquest
    ]
}
```
The format of the command is : ```quest 'action' 'quest_identifier'```

The possible actions are :
* Unlock : Unlock a quest by using the keyword ```unlock```.
* Select : Select a quest by using the keyword ```select```.
* Next : Select the next subquest from the selected quest ```next```.
* Debug : Debug the selected quest and it's subquest ```debug```.


<br><br><br><br>


# Commands preprocessing:
You can ask the tool to preprocess some part of your commands.

For that, your need to put your preprocessing arguments inside a ```{ }```.

There are multiple preprocessing commands that you can use:

***
## Coordinates
You can get the coordinates from a coordinate argument : ```{coord:'coord_identifier'}```.
The returned format will be ```'CoordX' 'CoordY' 'CoordZ'```.

Example :
```json
{
    "on_selected" : [
        "setblock {coord:commands} redstone_block 0", // Set a block of redstone at the coordinates named 'commands'.
    ]
}
```

***
## Coordinates with rotations
You can get the coordinates including the head rotation by using the preprocessing argument : ```{coord_rot:'coord_identifier'}```.
The returned format will be ```'CoordX' 'CoordY' 'CoordZ' 'RotationX' 'RotationY'```.

Example :
```json
{
    "on_selected" : [
        "tp @a {coord_rot:spawn}", // Teleport everyone to the spawn
        // This will return something like : tp @a 0 30 50 90 0
    ]
}
```

***
## Coordinates from target
You can get the coordinates in a target format by using the preprocessing argument : ```{coord_target:'coord_identifier'}```.
The returned format will be ```x='CoordX',y='CoordY',z='CoordZ'```.

Example :
```json
{
    "on_selected" : [
        "execute @a[{coord_target:house},r=4] ~ ~ ~ say I'm in the house", 
        // This will return something like : execute @a[x=3,y=25,z=150,r=4] ~ ~ ~ say I'm in the house
    ]
}
```

***
## Texts
You can get a piece of text by using the proprocessing argument : ```{text:'text_identifier'}```.
This will return the text linked to the identifier.

Example :
```json
{
    "on_selected" : [
        "say {text:text.helloWorld}", 
        // This will say in the chat the text linked to 'text.helloWorld'
    ]
}
```






<br><br><br><br>
# Hierarchy
This program automaticaly create functions files in your behavior pack.
This is how the hierarchy is working.

```
<Behavior Pack>
    <structures>
        commands_blocks_area.mcstructure
    <functions>
        <tec>
            - init.mcfunction
            - reset.mcfunction
            - informations.mcfunction
        <ticks>
            - 'tick_id'.mcfunction
        <cinematics>
            - start.mcfunction
            - finish.mcfunction
            <'cinematic_id'>
                - start.mcfunction
                - finish.mcfunction
                - exec.mcfunction
                <exec>
                    - 'file_number'.mcfunction
        <quests>
            - next.mcfunction
            - reset.mcfunction
            <'quest_id'>
                - next.mcfunction
                - select.mcfunction
                - unlock.mcfunction
                - cancel.mcfunction
                <'subquest_id'>
                    - 

```     
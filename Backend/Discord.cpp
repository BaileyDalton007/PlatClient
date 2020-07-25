#include "Discord.h"
#include <iostream>
#include <ctime>
#include <string>


void Discord::Initialize()
{
    DiscordEventHandlers Handle;
	memset(&Handle, 0, sizeof(Handle));
	Discord_Initialize("732805057007058964", &Handle, 1, NULL); //Discord Client ID
}

void Discord::Update()
{
    //Time since epoch
    std::time_t result = std::time(nullptr);

	// Discord Visual Generated code
    DiscordRichPresence discordPresence;
    memset(&discordPresence, 0, sizeof(discordPresence));
    discordPresence.state = "Bedrock Edition";
    discordPresence.details = "Minecraft";
    discordPresence.startTimestamp = result;
    discordPresence.largeImageKey = "pfp";
    discordPresence.largeImageText = "PlatClient";
    discordPresence.smallImageKey = "bedrock";
    discordPresence.smallImageText = "ign";
    Discord_UpdatePresence(&discordPresence);
}
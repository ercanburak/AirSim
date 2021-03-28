#include "airsim_settings_parser.h"

AirSimSettingsParser::AirSimSettingsParser(const std::string& host_ip)
    : host_ip_(host_ip)
{
    success_ = initializeSettings();
}

bool AirSimSettingsParser::success()
{
    return success_;
}

// bool AirSimSettingsParser::readSettingsTextFromFile(std::string settingsFilepath, std::string& settingsText) 
// {
//     // check if path exists
//     bool found = std::ifstream(settingsFilepath.c_str()).good(); 
//     if (found)
//     {
//         std::ifstream ifs(settingsFilepath);
//         std::stringstream buffer;
//         buffer << ifs.rdbuf();
//         // todo airsim's simhud.cpp does error checking here
//         settingsText = buffer.str(); // todo convert to utf8 as done in simhud.cpp?
//     }

//     return found;
// }

bool AirSimSettingsParser::getSettingsText(std::string& settings_text) const
{
    msr::airlib::RpcLibClientBase airsim_client(host_ip_);
    airsim_client.confirmConnection();

    settings_text = airsim_client.getSettingsString();

    // bool success = readSettingsTextFromFile(msr::airlib::Settings::Settings::getUserDirectoryFullPath("settings.json"), settingsText);
    return !settings_text.empty();
}

std::string AirSimSettingsParser::getSimMode()
{
    Settings& settings_json = Settings::loadJSonString(settings_text_);
    return settings_json.getString("SimMode", "");
}

// mimics void ASimHUD::initializeSettings()
bool AirSimSettingsParser::initializeSettings()
{
    if (getSettingsText(settings_text_))
    {
        AirSimSettings::initializeSettings(settings_text_);

        // not sure where settings_json initialized in AirSimSettings::initializeSettings() is actually used
        // Settings& settings_json = Settings::loadJSonString(settings_text_);
        // std::string simmode_name = settings_json.getString("SimMode", "");
        // std::cout << "simmode_name: " << simmode_name << std::endl;

        AirSimSettings::singleton().load(std::bind(&AirSimSettingsParser::getSimMode, this));
        std::cout << "SimMode: " << AirSimSettings::singleton().simmode_name << std::endl;

        return true;
    }
    
    return false;
}
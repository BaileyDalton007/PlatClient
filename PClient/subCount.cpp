#include <cpr/cpr.h>
#include <exception>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include "nlohmann/json.hpp"
#include "PClient.h"
#include "configParser.h"

#include <iostream>

namespace pt = boost::property_tree;

Config config{};

std::string channelData(){
    config = getConfig(config);
    if (!config.ytChannelId.empty() || !config.ytAPIkey.empty())
    {
        std::string channelid = config.ytChannelId;
        std::string apiKey = config.ytAPIkey;
        std::stringstream s;
        try
        {
            cpr::Response r = cpr::Get(cpr::Url{ "https://www.googleapis.com/youtube/v3/channels?part=snippet%2Cstatistics&id=" + channelid + "&key=" + apiKey });
            r.status_code;                  // 200
            r.header["content-type"];       // application/json; charset=utf-8
            r.text;                         // JSON text string

            s << r.text;
            pt::ptree root;      // Creates a root
            pt::read_json(s, root);  // Loads the json file in this ptree

            std::string subCount = root.get<std::string>("items..statistics.subscriberCount");
            std::string channelName = root.get<std::string>("items..snippet.title");

            if (subCount.length() > 3 && subCount.length() < 7)
            {
                subCount.insert(subCount.begin() + (subCount.length() - 3), '.');
                subCount.erase(4, subCount.length() - 3);
                subCount.insert(subCount.begin() + subCount.length(), 'K');
            }
            else if (subCount.length() > 7 && subCount.length() < 10)
            {
                subCount.insert(subCount.begin() + (subCount.length() - 6), '.');
                subCount.erase(4, subCount.length() - 6);
                subCount.insert(subCount.begin() + subCount.length(), 'M');
            }
            std::string data(channelName + " : " + subCount);

            return data;
        }
        catch (...)
        {
            return "";
        }
    }
    else
    {
        return "";
    }
}

std::string youtubeData = channelData();
const char* charData = youtubeData.c_str();


const char* getYoutubeData()
{
    return charData;
}
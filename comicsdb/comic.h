#pragma once

#include <drogon/drogon.h>
#include <string>

namespace comicsdb
{
    struct Id
    {
        std::string oid;
    };
    struct Comic
    {
        comicsdb::Id id;
        enum
        {
            DELETED_ISSUE = -1
        };
        std::string title;
        int64_t issue{DELETED_ISSUE};
        std::string writer;
        std::string penciler;
        std::string inker;
        std::string letterer;
        std::string colorist;
    };

    // Json::Value toJson(const Comic &comic);
    // std::string toJsonString(const Comic &comic);
    // Comic fromJsonString(const std::string &jsonString);
    // Comic fromJson(const Json::Value &comicJson);

} // namespace comicsdb

// namespace drogon
// {
//     template <>
//     inline comicsdb::Comic fromRequest(const HttpRequest &req)
//     {
//         auto json = req.getJsonObject();
//         comicsdb::Comic comic;
//         if (json)
//         {
//             comic = comicsdb::fromJson(*json);
//         }
//         return comic;
//     }

// } // namespace drogon
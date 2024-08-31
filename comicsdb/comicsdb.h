#pragma once

#include <string>
#include <vector>

#include <mongocxx/client.hpp>
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/json.hpp>
#include <mongocxx/uri.hpp>
#include <mongocxx/pool.hpp>

#include "comic.h"

namespace comicsdb
{
    using ComicCollection = mongocxx::collection;

    class MongoDB
    {
    public:
        ComicCollection getCollection(mongocxx::pool* pool, std::string &dbName, std::string &collectionName);
        // Get all the data bases name in the given client.
        std::vector<std::string> getDatabases(mongocxx::client &client);
        // Find a comic by key & value.
        std::vector<std::string> findComic(ComicCollection &comicCollection, const std::string &key, const std::string &value);
        // Create a new comic document in the given collection.
        // std::string createComic(ComicCollection &comicCollection, Comic &&comic);

    private:
        std::string mongoURIStr;
        // Create a new db in the given client.
        void createDB(mongocxx::client &client, const std::string &dbName);
        // Create a new collection in the given database.
        void createCollection(mongocxx::database &db, const std::string &collectionName);
        // Create a document from the given json.
        // bsoncxx::document::value createDocument(const json &jsonData);
        // Insert a document into the given collection.
        // std::string insertDocument(ComicCollection &comicCollection, const bsoncxx::document::value &document);
    };
} // namespace comicsdb
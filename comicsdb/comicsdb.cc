#include "comicsdb.h"

namespace comicsdb
{
    ComicCollection MongoDB::getCollection(mongocxx::pool* pool, std::string &dbName, std::string &collectionName)
    {
        if (dbName.empty())
        {
            throw std::runtime_error("mongo db name is empty!");
        }
        if (collectionName.empty())
        {
            throw std::runtime_error("mongo collection name is empty!");
        }
        
        auto mongoClient = (*pool).acquire(); // Acquire a client from the pool

        auto dbs = getDatabases(*mongoClient);
        if (!(std::find(dbs.begin(), dbs.end(), dbName) != dbs.end()))
        {
            // Create a new database & collection.
            createDB(*mongoClient, dbName);
        }
        auto comicDB = (*mongoClient).database(dbName);
        auto allCollections = comicDB.list_collection_names();
        // Check if collection already exists.
        if (!(std::find(allCollections.begin(), allCollections.end(), collectionName) != allCollections.end()))
        {
            createCollection(comicDB, collectionName);
        }

        return comicDB.collection(collectionName);
    }

    // Get all the data bases name in the given client.
    std::vector<std::string> MongoDB::getDatabases(mongocxx::client &client)
    {
        return client.list_database_names();
    }
    // Create a new db in the given client.
    void MongoDB::createDB(mongocxx::client &client, const std::string &dbName)
    {
        client[dbName];
    }
    // Create a new collection in the given database.
    void MongoDB::createCollection(mongocxx::database &db, const std::string &collectionName)
    {
        db.create_collection(collectionName);
    }

    // Find a comic by key & value.
    std::vector<std::string> MongoDB::findComic(ComicCollection &comicCollection, const std::string &key, const std::string &value)
    {
        std::vector<std::string> comics;
        // Create the query filter
        bsoncxx::builder::stream::document document{};
        if (key == "_id")
        {
            document << key << bsoncxx::oid(value);
        }
        else
        {
            document << key << value;
        }
        auto filter = document << bsoncxx::builder::stream::finalize;
        
        // Add query filter argument in find
        auto cursor = comicCollection.find({filter});
        for (auto &&doc : cursor)
        {
            std::string comicJsonString = bsoncxx::to_json(doc);
            // std::cout << comicJsonString << std::endl;
            comics.push_back(comicJsonString);
        }
        std::cout << "Found " << comics.size() << " records with key:" << key << " and value:" << value << "\n";
        return comics;
    }
} // namespace comicsdb

#include "comicService_v1_Service.h"

namespace comicService
{
    namespace v1
    {
        // Add definition of your processing function here

        Service::Service(std::string dbName, std::string collectionName)
            : dbName(dbName),
              collectionName(collectionName),
              mongoDB(comicsdb::MongoDB())
        {
            run();
        }

        void Service::run()
        {
            pool = drogon::app().getPlugin<MongoDBPlugin>()->getPool();
        }

        mongocxx::collection Service::getCollection()
        {
            return mongoDB.getCollection(pool, dbName, collectionName);
        }

        void Service::getComic(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback, std::string id)
        {
            try
            {
                LOG_DEBUG << "User " << id << " get his information";

                // Verify the validity of the token, etc.
                // Read the database or cache to get user information
                // Json::Value ret;
                // ret["result"] = "ok";
                // ret["user_name"] = "Juel";
                // ret["user_id"] = id;
                // ret["gender"] = 1;
                // auto resp = HttpResponse::newHttpJsonResponse(ret);
                // callback(resp);

                std::cout << "Finding data for id:" << id << "\n";
                mongocxx::collection comicCollection = getCollection();
                std::string filterKey = "_id";
                std::vector<std::string> comics = mongoDB.findComic(comicCollection, filterKey, id);

                auto resp = HttpResponse::newHttpResponse();
                resp->setStatusCode(k200OK);
                resp->setContentTypeCode(CT_TEXT_PLAIN);
                resp->setBody(comics.size() > 0 ? comics[0] : "Empty");
                callback(resp);
            }
            catch (const std::runtime_error &bang)
            {
                LOG_ERROR << "Runtime error: " << bang.what();

                auto resp = HttpResponse::newHttpResponse();
                resp->setStatusCode(k500InternalServerError);
                resp->setContentTypeCode(CT_TEXT_PLAIN);
                resp->setBody(bang.what());
                callback(resp);
            }
            catch (...)
            {
                LOG_ERROR << "Some error occured!";

                auto resp = HttpResponse::newHttpResponse();
                resp->setStatusCode(k500InternalServerError);
                resp->setContentTypeCode(CT_TEXT_PLAIN);
                resp->setBody("Something went wrong!");
                callback(resp);
            }
        }
    } // namespace v1
} // namespace comicService

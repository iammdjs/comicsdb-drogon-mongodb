#pragma once

#include <drogon/HttpController.h>

#include "comicsdb/comicsdb.h"
#include "plugins/mongodb_plugin.cc"

using namespace drogon;

namespace comicService
{
    namespace v1
    {
        class Service : public drogon::HttpController<Service>
        {
        public:
            Service(std::string dbName = "comicsdb", 
            std::string collectionName = "comics");

            void run();
            
            METHOD_LIST_BEGIN
            // use METHOD_ADD to add your custom processing function here;
            // METHOD_ADD(Service::get, "/{2}/{1}", Get); // path is /comicService/v1/Service/{arg2}/{arg1}
            // METHOD_ADD(Service::your_method_name, "/{1}/{2}/list", Get); // path is /comicService/v1/Service/{arg1}/{arg2}/list
            // ADD_METHOD_TO(Service::your_method_name, "/absolute/path/{1}/{2}/list", Get); // path is /absolute/path/{arg1}/{arg2}/list
            ADD_METHOD_TO(Service::getComic, "/api/comicservice/v1/comic/{1:id}", Get);

            METHOD_LIST_END
            // your declaration of processing function maybe like this:
            // void get(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback, int p1, std::string p2);
            // void your_method_name(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback, double p1, int p2) const;
            void getComic(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback, std::string id);

        private:
            mongocxx::collection getCollection();
            
            mongocxx::pool* pool{nullptr};
            comicsdb::MongoDB mongoDB;
            mongocxx::client mongoClient;
            std::string dbName;
            std::string collectionName;
        };
    } // namespace v1
} // namespace comicService

#include <drogon/plugins/Plugin.h>
#include <mongocxx/pool.hpp>
#include <mongocxx/instance.hpp>

class MongoDBPlugin : public drogon::Plugin<MongoDBPlugin> {
public:
    MongoDBPlugin() = default;

    void initAndStart(const Json::Value &config) override {
        instance = std::make_unique<mongocxx::instance>();
        pool = std::make_unique<mongocxx::pool>(mongocxx::uri{"mongodb://localhost:27017"});
    }

    void shutdown() override {
        pool.reset();
        instance.reset();
    }

    mongocxx::pool* getPool() const {
        return pool.get();
    }

private:
    std::unique_ptr<mongocxx::instance> instance;
    std::unique_ptr<mongocxx::pool> pool;
};

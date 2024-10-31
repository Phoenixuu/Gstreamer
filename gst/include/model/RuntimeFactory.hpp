#ifndef RuntimeFactory_hpp
#define RuntimeFactory_hpp

#include "nlohmann/json.hpp"

#include "AbstractRuntime.hpp"
#include "define/JsonTypes.hpp"

class RuntimeFactory
{
    public:
        RuntimeFactory();
        ~RuntimeFactory();
        static AbstractRuntime* createRuntime(std::string& strModel, std::string& strFrameWork, nlohmann::json& jConfigModel);
};

#endif // RuntimeFactory_hpp
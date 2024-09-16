#include <cstddef>
#include <odb/nullable.hxx>
#include <odb/core.hxx>

namespace chat_im {
    #pragma db object table("relation")
    class Relation {
    public:
    private:
        friend class odb::access ;
        #pragma db id auto 
        unsigned long _id;
        #pragma db type("varchar(64)") index 
        std::string _user_id;
        #pragma db type("varchar(64)")
        std::string _peer_id;
    };
}
// odb -d mysql --std c++11 --generate-query --generate-schema --profile boost/date-time person.hxx
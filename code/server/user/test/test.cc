#include <iostream>
#include <sstream>
#include <vector>
#include <string>

int main()
{
    std::vector<std::string> id_list = {"id1", "id2", "id3"};

    std::stringstream condition;
    condition << "user_id in (";
    for (const auto &id : id_list)
    {
        condition << "'" << id << "',";
    }
    std::string sql = condition.str();
    sql.pop_back();
    sql += ")";

    std::cout << sql << std::endl;
    return 0;
}
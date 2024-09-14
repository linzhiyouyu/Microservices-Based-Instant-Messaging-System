#pragma once
#include <boost/date_time/posix_time/posix_time.hpp>
#include <odb/nullable.hxx>
#include <odb/core.hxx>

//odb -d mysql --std c++11 --generate-query --generate-schema --profile boost/date-time student.hxx
#pragma db object
class Student {
public:
    Student() {}
    Student(unsigned long sn, const std::string& name, unsigned short age, unsigned long cid)
        :_sn(sn), _name(name), _age(age), _classes_id(cid) {}
    void sn(unsigned long sn) { _sn = sn; }
    unsigned long sn() { return _sn; }
    void name(const std::string& name) { _name = name; }
    std::string name() { return _name; }
    void age(unsigned short age) { _age = age; }
    odb::nullable<unsigned short> age() { return _age; }
    void class_id(unsigned long cid) { _classes_id = cid; }
    unsigned long class_id() { return _classes_id; }

private:
    friend class odb::access;
    #pragma db id auto
    unsigned long _id;
    #pragma db unique
    unsigned long _sn;
    std::string _name;
    odb::nullable<unsigned short> _age;
    #pragma db index
    unsigned long _classes_id;
};
#pragma db object
class Classes {
public:
    Classes() {}
    Classes(const std::string& name): _name(name) {}
    void name(const std::string& name) { _name = name; }
    std::string name() { return _name; }
private:
    friend class odb::access;
    #pragma db id auto
    unsigned long _id;
    std::string _name;
};

#pragma db view object(Student = s)\
                object(Classes = c : s::_classes_id == c::_id) \
                query((?))
struct class_student {
    #pragma db column(s::_id)
    unsigned long _id;
    #pragma db column(s::_sn)
    unsigned long _sn;
    #pragma db column(s::_name)
    std::string _name;
    #pragma db column(s::_age)
    odb::nullable<unsigned short> _age;
    #pragma db column(c::_name)
    std::string _class_name;

};
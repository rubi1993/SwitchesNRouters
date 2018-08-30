//
// Created by igor.a on 8/30/18.
//

#ifndef SRA_RULES_H
#define SRA_RULES_H

#include <string>

class Rule{
public:
    std::string source_address;
    std::string destination_address;
    std::string source_port;
    std::string destination_port;
    std::string protocol;
    std::string rule_name;

    Rule(std::string sa, std::string da, std::string sp, std::string dp, std::string p, std::string name):source_address(sa),
                                                                                               destination_address(da),
                                                                                               source_port(sp),
                                                                                               destination_port(dp),
                                                                                               protocol(p),
                                                                                               rule_name(name)
    {}

};

class PacketHeader{
public:
    std::string source_address;
    std::string destination_address;
    std::string source_port;
    std::string destination_port;
    std::string protocol;

    PacketHeader(std::string sa, std::string da, std::string sp, std::string dp, std::string p, std::string name):source_address(sa),
                                                                                                          destination_address(da),
                                                                                                          source_port(sp),
                                                                                                          destination_port(dp),
                                                                                                          protocol(p)
    {}

};

#endif //SRA_RULES_H

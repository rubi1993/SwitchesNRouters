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
    int source_adress_start,source_adress_end;
    int destination_adress_start,destination_adress_end;
    int source_port_start, source_port_end;
    int destination_port_start, destination_port_end;
    std::string protocol;
    int priority;
    std::string rule_name;
    int num_of_partitions_colliding(int partSize);
    Rule(std::string sa, std::string da, int sp_s, int sp_e, int dp_s, int dp_e, std::string p, int pri, std::string name,int sas=0,int sae=0,int das=0,int dae=0):

                    source_address(sa),
                    destination_address(da),
                    source_port_start(sp_s),
                    source_port_end(sp_e),
                    destination_port_start(dp_s),
                    destination_port_end(dp_e),
                    protocol(p),
                    priority(pri),
                    rule_name(name),
                    source_adress_start(sas),
                    source_adress_end(sae),
                    destination_adress_start(das),
                    destination_adress_end(dae)

    {}


};

class PacketHeader{
public:
    std::string source_address;
    std::string destination_address;
    int source_port;
    int destination_port;
    std::string protocol;

    PacketHeader(std::string sa, std::string da, int sp, int dp, std::string p):source_address(sa),
                                                                                  destination_address(da),
                                                                                  source_port(sp),
                                                                                  destination_port(dp),
                                                                                  protocol(p)
    {}

};

#endif //SRA_RULES_H

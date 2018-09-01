//
// Created by rubi1993 on 8/30/18.
//
#include "rules.h"

int Rule::num_of_partitions_colliding(int partSize) {
}

bool Rule::operator!=(const Rule &a)
{
    return !( source_address==a.source_address &&
            destination_address==a.destination_address &&
            source_port_start==a.source_port_start &&
            source_port_end== a.source_port_end &&
            destination_port_start== a.destination_adress_start &&
            destination_port_end==a.destination_port_end &&
            protocol==a.protocol &&
            priority==a.priority &&
            rule_name==a.rule_name &&
            source_adress_start==a.source_adress_start &&
            source_adress_end==a.source_adress_end &&
            destination_adress_start==a.destination_adress_start &&
            destination_adress_end==a.destination_adress_end);
}

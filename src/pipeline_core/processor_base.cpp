#include "./include/processor_base.hpp"
#include <iostream>

using namespace std;
using namespace vc;

void core::ProcessorBase::define_inport(string port_name) {
        if(inports.size() == 0 && processor_type != ProcessorType::Ingress) {
            inports.push_back(port_name);
        } else {
            if (processor_type == ProcessorType::Ingress) {
                throw std::runtime_error("[ERROR] Adding inports to an Ingress element");
            } else {
                throw std::runtime_error("[ERROR] Unimplemented feature: Multiple named inports");
            }
        }
}


void core::ProcessorBase::define_outport(string port_name) {
        if(outports.size() == 0 && processor_type != ProcessorType::Egress) {
            outports.push_back(port_name);
        } else {
            if (processor_type == ProcessorType::Egress) {
                throw std::runtime_error("[ERROR] Adding outports to an Egress element");
            } else {
                throw std::runtime_error("[ERROR] Unimplemented feature: Multiple named outports");
            }
        }
}


#include <cassert>
#include <string>
#include <iostream>
#include "../src/vc_pipeline/include/processor_type.hpp"
#include "../src/vc_pipeline/include/processor_base.hpp" 
#include "../src/vc_pipeline/include/processor_context.hpp"
#include "../src/vc_pipeline/include/processor_instance.hpp"

using namespace vc;
using namespace std;


class MockIngress : public ProcessorBase {

public:
    explicit MockIngress()
        : ProcessorBase(ProcessorType::Ingress) {
        define_outport("outport");
    }
private:
    void processor_function(ProcessorContext*) override { }
};

class MockTransform : public ProcessorBase {


public:
    explicit MockTransform()
        : ProcessorBase(ProcessorType::Transform) {
        define_inport("inport");
        define_outport("outport");
    }
private:
    void processor_function(ProcessorContext*) override { }

};


void test_parent_child_flow_graph() {

    ProcessorBase* ingress = new MockIngress();
    ProcessorBase* transform = new MockTransform();

    ProcessorInstance* ingress_ins = new ProcessorInstance(ingress, nullptr);
    ProcessorInstance* transform_ins = new ProcessorInstance(transform, nullptr);

    ingress_ins->attachChildProcessor(transform_ins);

    assert(ingress_ins->children().size() == 1);
    assert(ingress_ins->children()[0] == transform_ins);

    assert(transform_ins->parents().size() == 1);
    assert(transform_ins->parents()[0] == ingress_ins);

    delete ingress_ins;
    delete transform_ins;
}

int main() {
    test_parent_child_flow_graph();
}


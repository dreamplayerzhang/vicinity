#include "./include/pipeline.hpp"
#include "./include/utils.hpp"
#include <boost/asio/thread_pool.hpp>
#include <boost/asio/post.hpp>
#include <boost/uuid/uuid.hpp>
#include <memory>
#include <functional>

using namespace vc;

using std::function;
using std::make_unique;
using boost::uuids::uuid;
using boost::asio::post;
using boost::asio::thread_pool;

Pipeline::Pipeline(size_t num_of_threads)
    : worker_pool{num_of_threads},
      pipeline_context(make_unique<PipelineContext>()) { }


void Pipeline::attachProcessorChain(unique_ptr<ProcessorInstance> root) {
    uuid root_uuid = root->getUUID();
    roots.push_back(root_uuid);
    _attach_processors(std::move(root));
}

void Pipeline::attachProcessor(unique_ptr<ProcessorInstance> processor, bool is_root) {
    if (is_root) {
        roots.push_back(processor->getUUID());
    }
    processors[processor->getUUID()] = std::move(processor);
}

void Pipeline::runPipeline() {
    // Phase 1 implementation 
    
    // submit all roots processors into the thread pool
    for (const uuid& processor_uuid : roots) {
        ProcessorInstance* root = processors[processor_uuid].get();
        pipeline_context->submitJob(root);
    }

    // update pipeline state
    pipeline_context->setPipelineState(PipelineState::RUNNING);

    // keep polling the job queue as long as the pipeline is in runnable state
    // TODO: handle repeating Back-off restart state
    while (_is_pipeline_still_in_runnable_state()) {
        if (pipeline_context->queueSize() != 0) {
            ProcessorInstance* next_job = pipeline_context->nextJob();
            function<void()> runnable_job = [next_job] () -> void {
                next_job->runProcessor();
            };
            post(worker_pool, runnable_job);
        }
    }

    switch (pipeline_context->getPipelineState()) {
        case PipelineState::CANCELLED:
            // TODO: handle cancel
            break;
        case PipelineState::ERROR:
            // TODO: handle error
            break;
        case PipelineState::COMPLETED:
            // TODO: handle complete
            break;
        default:
            throw std::runtime_error("[ERROR]: pipeline terminated with invalid state");
    }
}

bool Pipeline::_is_pipeline_still_in_runnable_state() {
    PipelineState state = pipeline_context->getPipelineState();
    return state == PipelineState::RUNNING;
}

void Pipeline::_attach_processors(unique_ptr<ProcessorInstance> root) {
    for (auto [child_processor, port_connections] : root->children()) {
        _attach_processors(unique_ptr<ProcessorInstance>(child_processor));
    }

    processors[root->getUUID()] = std::move(root);
}


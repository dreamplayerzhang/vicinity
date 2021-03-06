#pragma once

#include <map>
#include <string>
#include <queue>
#include <memory>
#include <boost/any.hpp>
#include "./pipeline_state.hpp"

namespace vc {

namespace core {

using std::map;
using std::string;
using std::queue;
using std::unique_ptr;
using std::atomic;
using std::make_unique;
using boost::any;
using boost::any_cast;

class ProcessorInstance;

class PipelineSignal;

class PipelineContext {

    PipelineState pipeline_state;
    atomic<PipelineSignal*> pipeline_signal;
    string last_error_message;
    queue<ProcessorInstance*> job_queue;
    map<string, unique_ptr<any>> pipeline_cache;

public: 

    explicit PipelineContext();

    PipelineState getPipelineState() const {
        return pipeline_state;
    }

    void setPipelineState(PipelineState newState) {
        pipeline_state = newState;
    }

    const string& getLastErrorMessage() const {
        return last_error_message;
    }

    void setLastErrorMessage(string message) {
        last_error_message = message;
    }

    size_t queueSize() const {
        return job_queue.size();
    }

    void submitJob(ProcessorInstance* job);

    void sendSignal(PipelineSignal*);

    PipelineSignal* getSignal() const;

    ProcessorInstance* nextJob();

    // TODO: add locking
    template <typename T>
    void store(string, unique_ptr<T> value);

    template <typename T>
    T* retrieve(string);

    ~PipelineContext();

};

template <typename T>
void PipelineContext::store(string key, unique_ptr<T> value) {
    unique_ptr<any> value_ptr = make_unique<any>(value.get());
    value.release();
    pipeline_cache[key] = std::move(value_ptr);
}

template <typename T>
T* PipelineContext::retrieve(string key) {
    return any_cast<T*>(*pipeline_cache[key]);
}

};

};

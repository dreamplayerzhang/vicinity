#pragma once

#include <map>
#include <string>
#include <memory>
#include <opencv2/opencv.hpp>
#include <boost/uuid/uuid.hpp>
#include "./pipeline_context.hpp"

namespace vc {

using cv::Mat;
using std::map;
using std::string;
using std::unique_ptr;
using boost::uuids::uuid;

class ProcessorInstance;

class ProcessorContext {

    const ProcessorInstance* processor_instance;
    const PipelineContext* pipeline_context;

    map<string, unique_ptr<Mat>> inports;
    map<string, unique_ptr<Mat>> outports;

public:

    explicit ProcessorContext(ProcessorInstance*, const PipelineContext*);


};

};

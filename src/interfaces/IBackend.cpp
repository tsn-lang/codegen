#include <codegen/interfaces/IBackend.h>
#include <codegen/CodeHolder.h>
#include <codegen/FunctionBuilder.h>
#include <utils/Array.hpp>

namespace codegen {
    IBackend::IBackend() {
    }

    IBackend::~IBackend() {
    }

    void IBackend::addPostProcess(IPostProcessStep* process) {
        m_postProcesses.push(process);
    }

    bool IBackend::process(FunctionBuilder* input, u32 postProcessMask) {
        CodeHolder ch(input->getCode());
        ch.owner = input;
        ch.rebuildAll();

        if (!onBeforePostProcessing(&ch)) return false;
        
        for (IPostProcessStep* step : m_postProcesses) {
            for (BasicBlock& b : ch.cfg.blocks) {
                while (step->execute(&ch, &b, postProcessMask));
            }

            while (step->execute(&ch, postProcessMask));
        }

        if (!onAfterPostProcessing(&ch)) return false;

        return transform(&ch);
    }

    bool IBackend::onBeforePostProcessing(CodeHolder* ch) { return true; }
    bool IBackend::onAfterPostProcessing(CodeHolder* ch) { return true; }
};
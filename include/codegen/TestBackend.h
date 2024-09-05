#pragma once
#include <codegen/interfaces/IBackend.h>
#include <codegen/Execute.h>

namespace codegen {
    class TestBackend : public IBackend {
        public:
            TestBackend();
            virtual ~TestBackend();

            virtual void transform(CodeHolder* processedCode);
        
        protected:
            Array<TestExecuterCallHandler*> m_callHandlers;
    };
};
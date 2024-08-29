#include <codegen/FunctionBuilder.h>
#include <bind/bind.h>
using namespace codegen;
using namespace bind;
using namespace utils;

struct test_struct {
    i32 a, b, c;
};

int main(int argc, const char** argv) {
    Registry::Create();
    
    {
        build<void>("void");
        build<bool>("bool");
        build<i8>("i8");
        build<i16>("i16");
        build<i32>("i32");
        build<i64>("i64");
        build<u8>("u8");
        build<u16>("u16");
        build<u32>("u32");
        build<u64>("u64");
        build<f32>("f32");
        build<f64>("f64");
        auto ts = build<test_struct>("ivec3");
        ts.prop("a", &test_struct::a);
        ts.prop("b", &test_struct::b);
        ts.prop("c", &test_struct::c);

        Function fn = Function("test", Registry::Signature<i32, i32, i32>(), Registry::GlobalNamespace());
        FunctionBuilder fb = FunctionBuilder(&fn);

        fb.enableValidation();
        Value arg1 = fb.getArg(0);
        Value arg2 = fb.getArg(1);
        Value something = arg1 + arg2;

        arg1.setName("arg1");
        arg2.setName("arg2");
        something.setName("something");
        
        fb.generateIf(something > fb.val(10), [&](){
            something += arg2;

            Value i = fb.val(Registry::GetType<u32>());
            i.setName("i");
            i = fb.val(u32(0));
            fb.generateFor([&]() -> Value{
                return i < fb.val(10);
            }, [&](){
                i++;
            }, [&](){
                something += i;
            });

            Value v = fb.val(ts.getType());
            v.setName("vec");
            fb.store(fb.val(0), v, offsetof(test_struct, a));
            fb.store(fb.val(1), v, offsetof(test_struct, b));
            fb.store(fb.val(2), v, offsetof(test_struct, c));

            something -= fb.generateCall(&fb, { arg1, arg2 });
        });

        fb.ret(something);

        auto code = fb.getCode();
        for (u32 i = 0;i < code.size();i++) {
            printf("0x%0.3X | %s\n", i, code[i].toString().c_str());
        }
        fflush(stdout);
    }

    Registry::Destroy();
    return 0;
}
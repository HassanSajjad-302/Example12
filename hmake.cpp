
#include "Configure.hpp"

void buildSpecification()
{
    DSC<CppSourceTarget> &std = GetCppObjectDSC("std");
    std.getSourceTarget().MODULE_FILES("std.cpp").ASSIGN(CxxSTD::V_20);

    DSC<CppSourceTarget> &cat = GetCppObjectDSC("cat");
    cat.getSourceTarget().MODULE_FILES("cat.cpp").ASSIGN(CxxSTD::V_20).PRIVATE_COMPILE_DEFINITION("INCLUDE_IOSTREAM");

    cat.PRIVATE_LIBRARIES(&std);

    DSC<CppSourceTarget> &app = GetCppExeDSC("app");
    app.getSourceTarget().MODULE_FILES("main.cpp");

/*    CppSourceTarget &std1 = GetCppObject("std1-cpp");
    CppSourceTarget &cat1 = GetCppObject("cat1-cpp");
    std.pushAndInitialize(&std1);
    cat.pushAndInitialize(&cat1).assignObjectFileProducerDeps(Dependency::PRIVATE, &std);*/

    app.PRIVATE_LIBRARIES(&cat, &std);
}

MAIN_FUNCTION
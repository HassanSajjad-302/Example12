
#include "Configure.hpp"

void buildSpecification()
{
    // DSC constructor has this line         prebuiltBasic->objectFileProducers.emplace(objectFileProducer);
    // which specifies std CppSourceTarget (and transitively its object-files) as dependency of std LinkOrArchiveTarget.
    DSC<CppSourceTarget> &std = GetCppObjectDSC("std");
    std.getSourceTarget().INTERFACE_FILES("std/std.cpp").ASSIGN(CxxSTD::V_20);

    DSC<CppSourceTarget> &cat = GetCppObjectDSC("cat");
    cat.getSourceTarget()
        .INTERFACE_FILES("cat/cat.ixx")
        .MODULE_FILES("cat/cat.cpp")
        .ASSIGN(CxxSTD::V_20)
        .PUBLIC_HU_INCLUDES("cat");

    cat.PRIVATE_LIBRARIES(&std);

    DSC<CppSourceTarget> &app = GetCppExeDSC("app");
    app.getSourceTarget().MODULE_FILES("main.cpp");

    // This will define two new CppSourceTargets. saveAndReplace will save the older CppSourceTarget in the
    // DSC<CppSourceTarget> while replacing it with these newer one to be used in PRIVATE_LIBRARIES function. This new
    // target is compiled with the default value /std:c++latest same as the main.cpp. saveAndReplace will also populate
    // the module files of these newer targets with the similar value of the older targets. assignObjectFileProducerDeps
    // is used instead of PRIVATE_LIBRARIES because besides adding std CppSourceTarget as dependency of cat
    // CppSourceTarget, PRIVATE_LIBRARIES also adds std LinkOrArchiveTarget as dependency of cat LinkOrArchiveTarget
    // which has already been done. Please notice that the older CppSourceTargets that we are replacing in the
    // following, we had already specified them (and transitively their object files) as the dependency of the
    // respective LinkOrArchiveTargets in the DSC constructor. The newer, following declared, CppSourceTargets are not
    // specified as dependnecy of any LinkOrArchiveTaget, hence the object files of these are wasted but the ifc files
    // are used while compiling their dependents.

    // If the following 4 lines are commented out, two warnings of incompatible ifcs  are printed as
    // there is bmi incompatibility introduced because of difference in language versions.
    CppSourceTarget &std1 = GetCppObject("std1-cpp");
    CppSourceTarget &cat1 = GetCppObject("cat1-cpp");
    std.saveAndReplace(&std1);
    cat.saveAndReplace(&cat1).assignObjectFileProducerDeps(Dependency::PRIVATE, &std);

    app.PRIVATE_LIBRARIES(&cat, &std);

    // Please notice that saveAndReplace() function stores the older pointer in that respective DSC and populates the
    // module files of the newer target with only the interface-file of the  older target. This means that cat.ixx and
    // cat.hpp will be compiled in cat1-cpp but not cat.cpp. saveAndReplace also copies other properties such as
    // compiler-definitions and include-directories. It, however, does not copy the compiler-flags. It also does not
    // copy the build-system features such as CxxStd. The reason is that such feature values can result to bmi sensitive
    // values to the compile-command.

    // The CPP20 definition is provided to include <iostream> in cat.cpp. This is provided in the end so it is not
    // copied to cat1.
    (cat.stored ? cat.stored : cat.getSourceTargetPointer())->PRIVATE_COMPILE_DEFINITION("CPP20");
}

MAIN_FUNCTION
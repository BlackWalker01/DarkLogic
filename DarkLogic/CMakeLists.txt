cmake_minimum_required(VERSION 3.5)

project(DarkLogic LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(pybind11)

set(SRCS
    #Sources
    logic.cpp
    Utils/utils.cpp
    Utils/state.cpp
    Formula/Arithmetic/aarithmeticformula.cpp
    Formula/Arithmetic/inontermarithmetic.cpp
    Formula/Boolean/asubimpurerule.cpp
    Formula/Boolean/asubimpuretheorem.cpp
    Formula/Boolean/asubpurerule.cpp
    Formula/Boolean/asubpuretheorem.cpp
    Formula/Boolean/axiom.cpp
    Formula/Boolean/iproposition.cpp
    Formula/formula.cpp
    Formula/iisubtheoremformula.cpp
    Formula/iisubruleformula.cpp
    Formula/valuetypeobject.cpp
    Operator/ioperator.cpp
    Formula/Boolean/asubrule.cpp
    Formula/Boolean/asubtheorem.cpp
    Formula/Boolean/dbrule.cpp
    Formula/dbvarprop.cpp
    Formula/Boolean/rule.cpp
    Formula/Boolean/subrule.cpp
    Formula/Boolean/subtheorem.cpp
    Formula/Boolean/theorem.cpp
    Formula/parser.cpp
    Set/booleanset.cpp
    Set/naturalintegerset.cpp
    Set/set.cpp
    Variable/Boolean/boolean.cpp
    Variable/Boolean/constboolean.cpp
    Variable/NaturalInteger/constnaturalinteger.cpp
    Variable/NaturalInteger/naturalinteger.cpp
    Variable/Set/constset.cpp
    Variable/Set/varset.cpp    
    Variable/dbvar.cpp
    Variable/term.cpp
    Variable/avariable.cpp
    Variable/Boolean/hypboolean.cpp
    Logger/log.cpp
    Utils/action.cpp
    Variable/variableexception.cpp
    Formula/evaluater.cpp
    darklogic.cpp
    Formula/Boolean/dbtheorem.cpp
    Utils/rulecontent.cpp)

set(HEADER
    logic_global.h
    logic.h
    Utils/utils.h
    Utils/state.h
    Formula/Arithmetic/ConstantFormula/constantruleformula.h
    Formula/Arithmetic/ConstantFormula/constanttheoremformula.h
    Formula/Arithmetic/PlusFormula/plusruleformula.h
    Formula/Arithmetic/PlusFormula/plustheoremformula.h
    Formula/Arithmetic/SetEqualFormula/setequalruleformula.h
    Formula/Arithmetic/SetEqualFormula/setequaltheoremformula.h
    Formula/Arithmetic/VariableFormula/variableruleformula.h
    Formula/Arithmetic/VariableFormula/variabletheoremformula.h
    Formula/Arithmetic/aarithmeticformula.h
    Formula/Arithmetic/allarithformula.h
    Formula/Arithmetic/asubarithmeticrule.h
    Formula/Arithmetic/asubarithmetictheorem.h
    Formula/Arithmetic/inontermarithmetic.h
    Formula/Arithmetic/subarithmeticrule.h
    Formula/Arithmetic/subarithmetictheorem.h
    Formula/Boolean/BelongsTo/subrulebelongsto.h
    Formula/Boolean/BelongsTo/subtheorembelongsto.h
    Formula/Boolean/BelongsTo/theorembelongsto.h
    Formula/Boolean/Boolean/subruleboolean.h
    Formula/Boolean/Boolean/subtheoremboolean.h
    Formula/Boolean/Boolean/theoremboolean.h
    Formula/Boolean/ConstBoolean/subruleconstboolean.h
    Formula/Boolean/ConstBoolean/subtheoremconstboolean.h
    Formula/Boolean/ConstBoolean/theoremconstboolean.h
    Formula/Boolean/Equal/subruleequal.h
    Formula/Boolean/Equal/subtheoremequal.h
    Formula/Boolean/Equal/theoremequal.h
    Formula/Boolean/Generic/subrulegeneric.h
    Formula/Boolean/Generic/subtheoremgeneric.h
    Formula/Boolean/Generic/theoremgeneric.h
    Formula/Boolean/Hyp/subrulehyp.h
    Formula/Boolean/Hyp/subtheoremhyp.h
    Formula/Boolean/Hyp/theoremhyp.h
    Formula/Boolean/Let/subrulelet.h
    Formula/Boolean/Let/subtheoremlet.h
    Formula/Boolean/Let/theoremlet.h
    Formula/Boolean/Not/subrulenot.h
    Formula/Boolean/Not/subtheoremnot.h
    Formula/Boolean/Not/theoremnot.h
    Formula/Boolean/allproposition.h
    Formula/Boolean/allsubtheorem.h
    Formula/Boolean/asubimpurerule.h
    Formula/Boolean/asubimpuretheorem.h
    Formula/Boolean/asubpurerule.h
    Formula/Boolean/asubpuretheorem.h
    Formula/Boolean/axiom.h
    Formula/Boolean/axiomset.h
    Formula/Boolean/iproposition.h
    Formula/Boolean/allrule.h
    Formula/iisubruleformula.h
    Formula/iisubtheoremformula.h
    Formula/isubruleformula.h
    Formula/isubtheoremformula.h
    Formula/valuetypeobject.h
    Operator/Arithmetic/allarithmeticoperator.h
    Operator/Arithmetic/plus.h
    Operator/Arithmetic/setequal.h
    Operator/Boolean/allbooleanoperator.h
    Operator/Boolean/belongsto.h
    Operator/Boolean/equal.h
    Operator/Boolean/let.h
    Operator/alloperator.h
    Operator/Boolean/and.h
    Operator/Boolean/and.hxx
    Operator/Boolean/equivalent.h
    Operator/Boolean/equivalent.hxx
    Operator/Boolean/hyp.h
    Operator/Boolean/hyp.hxx
    Operator/Boolean/implication.h
    Operator/Boolean/implication.hxx
    Operator/ioperator.h
    Operator/Boolean/not.h
    Operator/Boolean/not.hxx
    Operator/operator.h
    Operator/operator.hxx
    Operator/Boolean/or.h
    Operator/Boolean/or.hxx
    Formula/Boolean/asubrule.h
    Formula/Boolean/asubtheorem.h
    Formula/Boolean/dbrule.h
    Formula/dbvarprop.h
    Formula/formula.h
    Formula/formula.hxx
    Formula/Boolean/rule.h
    Formula/Boolean/subrule.h
    Formula/Boolean/subtheorem.h
    Formula/Boolean/theorem.h
    Formula/parser.h
    Set/allset.h
    Set/naturalintegerset.h
    Set/omniset.h
    Variable/Boolean/boolean.h
    Variable/NaturalInteger/constnaturalinteger.h
    Variable/NaturalInteger/naturalinteger.h
    Variable/Set/constset.h
    Variable/Set/varset.h
    Variable/allvariable.h
    Variable/constante.h
    Variable/constante.hxx
    Variable/Boolean/constboolean.h
    Variable/dbvar.h
    Variable/term.h
    Variable/avariable.h
    Variable/variable.h
    Variable/variable.hxx
    Variable/Boolean/hypboolean.h
    Logger/log.h
    Utils/action.h
    Utils/exception.h
    Variable/variableexception.h
    Formula/evaluater.h
    darklogic.h
    )



add_compile_definitions(_SILENCE_CLANG_CONCEPTS_MESSAGE)
if(UNIX)
    # darklogic library
    add_library(darklogic SHARED ${SRCS} ${HEADER})
    target_compile_definitions(darklogic PRIVATE DARK_LOGIC_LIBRARY)
    target_link_libraries(darklogic PUBLIC stdc++ pybind11::module)
    # DarkLogic library
    add_library(DarkLogic SHARED ${SRCS} ${HEADER})
    target_compile_definitions(DarkLogic PRIVATE DARK_LOGIC_LIBRARY)
    target_link_libraries(DarkLogic PUBLIC stdc++ pybind11::module)
    set_target_properties(DarkLogic PROPERTIES PREFIX "${PYTHON_MODULE_PREFIX}"
                                         SUFFIX "${PYTHON_MODULE_EXTENSION}")
    install(TARGETS darklogic DESTINATION ${CMAKE_SOURCE_DIR}/../Lib)
    install(TARGETS DarkLogic DESTINATION ${CMAKE_SOURCE_DIR}/../Lib)
else()
    add_library(DarkLogic SHARED ${SRCS} ${HEADER})
    target_compile_definitions(DarkLogic PRIVATE DARK_LOGIC_LIBRARY)
    target_link_libraries(DarkLogic PRIVATE pybind11::module)
    set_target_properties(DarkLogic PROPERTIES PREFIX "${PYTHON_MODULE_PREFIX}"
                                         SUFFIX "${PYTHON_MODULE_EXTENSION}")    

endif()

install(TARGETS DarkLogic DESTINATION ${CMAKE_SOURCE_DIR}/../Lib)

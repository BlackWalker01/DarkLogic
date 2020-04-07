#ifndef UTILS_H
#define UTILS_H
#include <string>
#include <memory>
#include <complex>
#include <concepts>

namespace  N_Logic
{
    //Main pointer for Logic
    template<typename T>
    using ptr=std::shared_ptr<const T>;

    /**---------------------------------------------------------------
      * Define basic types for Logic
      *---------------------------------------------------------------
      */
    using UInt = size_t; //Natural Integer
    using Int = long long int; //Integer
    //using Rational = rational<Int>; //Rational
    using Real = double; //Real
    using Complex = std::complex<Real>;

    template<typename T>
    concept BoolType = std::is_same_v<T, bool>;
    template<typename T>
    concept BasicArithmeticType = std::is_same_v<T, void> || std::is_same_v<T, UInt> || std::is_same_v<T, Int> || std::is_same_v<T, Real>
        || std::is_same_v<T, Complex>;
    template<typename T>
    concept BasicType = BoolType<T> || BasicArithmeticType<T>;


    /**---------------------------------------------------------------
      * Define basic Sets for Logic
      *---------------------------------------------------------------
      */
    template<typename T>
    class Set;

    template<typename T>
    concept SetDerived = std::is_base_of_v<Set<T>, T>;
    template<typename T>
    concept BasicSetType = SetDerived<T> && BasicType<typename T::Type>;
    template<typename T>
    concept ArithmeticType = BasicArithmeticType<T> || BasicSetType<T>;
    template<typename T>
    concept Type = BoolType<T> || ArithmeticType<T>;


    /**---------------------------------------------------------------
      * Define Variable for Logic
      ---------------------------------------------------------------
      */
    template<SetDerived SetType>
    class Variable;
    class Boolean;
    template<typename T>
    concept VariableType = std::is_base_of_v<Variable<typename T::SetType>, T>;
    template<typename T>
    concept ArithVariableType = VariableType<T> && ArithmeticType<typename T::ValueType>;


    /**---------------------------------------------------------------
      * Define Constant for Logic
      *---------------------------------------------------------------
      */
    template<SetDerived SetType>
    class Constante;
    class ConstBoolean;
    template<typename T>
    concept ConstantType = std::is_base_of_v<Constante<typename T::SetType>, T>;
    template<typename T>
    concept ArithConstantType = ConstantType<T> && ArithmeticType<typename T::ValueType>;



    /**---------------------------------------------------------------
      * Define Formulas for Logic
      *---------------------------------------------------------------
    */
    template<Type ValueType_>
    class AbstractFormula;

    template<typename T>
    concept Formula = std::is_base_of_v<AbstractFormula<typename T::ValueType>, T>;

    //Proposition case
    class ASubTheorem;
    class ASubRule;

    template<typename T>
    concept RuleProposition = std::is_base_of_v<ASubRule, T>;
    template<typename T>
    concept TheoremProposition = std::is_base_of_v<ASubTheorem, T>;
    template<typename T>
    concept Proposition = RuleProposition<T> || TheoremProposition<T>;


    //Arithmetic Formula case
    template<ArithmeticType ValueType>
    class ASubArithmeticRule;
    template<ArithmeticType ValueType>
    class ASubArithmeticTheorem;

    template<typename T>
    concept RuleArithFormula = std::is_base_of_v<ASubArithmeticRule<typename T::ValueType>, T>;
    template<typename T>
    concept TheoremArithFormula = std::is_base_of_v<ASubArithmeticTheorem<typename T::ValueType>, T>;
    template<typename T>
    concept ArithFormula = RuleArithFormula<T> || TheoremArithFormula<T>;

    //Rule Formula
    template<typename T>
    concept RuleFormula = RuleProposition<T> || RuleArithFormula<T>;    
    template<Formula Fml>
    struct ToRuleStruct{using Type = Fml; };    
    template<Formula Fml>
    using ToRule = typename ToRuleStruct<Fml>::Type; 

    //Theorem Formula
    template<typename T>
    concept TheoremFormula = TheoremProposition<T> || TheoremArithFormula<T>;
    template<Formula Fml>
    struct ToTheoremStruct { using Type = Fml; };
    template<Formula Fml>
    using ToTheorem = typename ToTheoremStruct<Fml>::Type;

    /**---------------------------------------------------------------
      * Define Operators for Logic
      *---------------------------------------------------------------
    */
    enum Name
    {
        NONE,
        AND,
        EQUIVALENT,
        IMPLICATION,
        NOT,
        OR,
        HYP,
        EQUAL,
        SETEQUAL,
        LET,
        BELONGSTO,
        PLUS,
    };

    using Arity = size_t;

    enum class Associativity
    {
        RIGHT,
        LEFT
    };

    enum NbArgBefore
    {
        E_ZERO,
        E_ONE
    };
    template<typename T>
    concept FunOpe = requires(T t, unsigned short priorityParent)
    {
        Type<typename T::ValueType>;        
        Formula<typename T::SubFormulaType>;
        Name(T::name());
        Arity(t.arity());
        uint16_t(T::priority());
        Associativity(T::associativity());
        std::string(T::symbol());
        NbArgBefore(T::nbArgBefore());
        std::string(t.toString(priorityParent));
    };

    template<FunOpe FunType_>
    class Operator;
    template<typename T>
    concept OperatorType = std::is_base_of_v<Operator<typename T::FunType>, T> && requires
    {
        typename T::RuleOpe;
        typename T::TheoremOpe;
    };

    template<typename T>
    concept BooleanOpeType = OperatorType<T> && std::is_same_v<bool, typename T::ValueType>;
    template<typename T>
    concept ArithOpeType = OperatorType<T> && ArithmeticType<typename T::ValueType>;

    template<typename T>
    concept RuleOperator = OperatorType< T> && RuleFormula<typename T::SubFormulaType>;
    template<typename T>
    concept TheoremOperator = OperatorType<T> && TheoremFormula<typename T::SubFormulaType>;
    template<OperatorType OpeType>
    using ToRuleOpe = typename OpeType::RuleOpe;

    template<typename T>
    concept SubRuleProperty = (RuleOperator<T> && BooleanOpeType<T>) || std::is_same_v<T, Boolean> || std::is_same_v<T, ConstBoolean>;
    template<typename T>
    concept SubTheoremProperty = (TheoremOperator<T> && BooleanOpeType<T>) || std::is_same_v<T, Boolean> || std::is_same_v<T, ConstBoolean>;
    template<OperatorType OpeType>
    using ToTheoremOpe = typename OpeType::TheoremOpe;

    template<typename T>
    concept SubRuleFormula = (RuleOperator<T> && ArithOpeType<T>) || ArithVariableType<T> || ArithConstantType<T>;
    template<typename T>
    concept SubTheoremFormula = (TheoremOperator<T> && ArithOpeType<T>) || ArithVariableType<T> || ArithConstantType<T>;
}

/**
 * @brief isCharInVar
 * @param c
 * @return true if c can be in a name of a variable
 */
bool isCharInVar(char c);
bool isSyntaxChar(char c);
std::string getVarName(const std::string& content, size_t &i);
std::string uintToString(unsigned int x);
std::string sizeToString(const size_t& x);
#endif // UTILS_H

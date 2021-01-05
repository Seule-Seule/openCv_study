#ifndef TYPESEXT_HPP
#define TYPESEXT_HPP

#define SYNTHESIZE_VAR(varType, varName, funName)\
protected: varType varName;\
public: virtual varType get##funName(void) const { return varName; }\
public: virtual void set##funName(varType var){ varName = var; }


class TypesExt
{
public:
  TypesExt();
};


#endif // TYPESEXT_HPP

// Copyright (C) 2022 Satya Das and CppParser contributors
// SPDX-License-Identifier: MIT

#ifndef AB11A5E0_FDCE_4B20_B3D4_8B8B91501356
#define AB11A5E0_FDCE_4B20_B3D4_8B8B91501356

#include "cppast/cpp_entity.h"
#include "cppast/cpp_templatable_entity.h"
#include "cppast/cpp_var_list.h"

namespace cppast {

class CppTypedefName : public CppEntity
{
public:
  static constexpr auto EntityType()
  {
    return CppEntityType::TYPEDEF_DECL;
  }

public:
  CppTypedefName(std::unique_ptr<CppVar> var)
    : CppEntity(EntityType())
    , var_(std::move(var))
  {
  }

public:
  const CppVar* var() const
  {
    return var_.get();
  }

private:
  std::unique_ptr<CppVar> var_;
};

class CppTypedefList : public CppEntity
{
public:
  static constexpr auto EntityType()
  {
    return CppEntityType::TYPEDEF_DECL_LIST;
  }

public:
  CppTypedefList(std::unique_ptr<CppVarList> varList)
    : CppEntity(EntityType())
    , varList_(std::move(varList))
  {
  }

public:
  const CppVarList& varList() const
  {
    return *varList_;
  }

private:
  std::unique_ptr<CppVarList> varList_;
};

class CppUsingDecl : public CppEntity, public CppTemplatableEntity
{
public:
  static constexpr auto EntityType()
  {
    return CppEntityType::USING_DECL;
  }

  using DeclData = std::variant<std::unique_ptr<CppVarType>,
                                std::unique_ptr<CppFunctionPointer>,
                                std::unique_ptr<CppCompound>>;

public:
  CppUsingDecl(std::string name, DeclData declData)
    : CppEntity(EntityType())
    , name_(std::move(name))
    , declData_(std::move(declData))
  {
  }

  CppUsingDecl(std::string name)
    : CppEntity(EntityType())
    , name_(std::move(name))
  {
  }

public:
  const std::string& name() const
  {
    return name_;
  }

  const DeclData& definition() const
  {
    return declData_;
  }

private:
  std::string name_;
  DeclData    declData_;
};

} // namespace cppast

#endif /* AB11A5E0_FDCE_4B20_B3D4_8B8B91501356 */

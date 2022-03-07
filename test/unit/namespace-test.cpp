#include <catch/catch.hpp>

#include "cppparser.h"

#include "embedded-snippet-test-base.h"

class NamespaceTest : public EmbeddedSnippetTestBase
{
protected:
  NamespaceTest()
    : EmbeddedSnippetTestBase(__FILE__)
  {
  }
};

#if TEST_CASE_SNIPPET_STARTS_FROM_NEXT_LINE
#  if __cplusplus > 201703L
namespace my::ns1 {
auto p = new char*[5];
}
#  endif
#endif

TEST_CASE_METHOD(NamespaceTest, " C++17 style nested namespace")
{
  auto testSnippet = getTestSnippetParseStream(__LINE__ - 5);

  CppParser  parser;
  const auto ast = parser.parseStream(testSnippet.data(), testSnippet.size());
  REQUIRE(ast != nullptr);

  const auto& members = ast->members();
  REQUIRE(members.size() == 3);

  CppCompoundEPtr ns = members[1];
  REQUIRE(ns);
  CHECK(ns->name() == "my::ns1");
  const auto& nsMembers = ns->members();
  REQUIRE(nsMembers.size() == 1);

  CppVarEPtr var = nsMembers[0];
  REQUIRE(var);

  CHECK(var->assignValue() != nullptr);
}

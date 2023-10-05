
/// @file BnCover_test.cc
/// @brief BnCover_test の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include <gtest/gtest.h>
#include "ym/BnCover.h"


BEGIN_NAMESPACE_YM

TEST(BnCoverTest, null_cover_test1)
{
  auto cov = BnCover{0, {}};

  EXPECT_EQ( 0, cov.input_num() );
  EXPECT_EQ( 0, cov.cube_num() );
  EXPECT_EQ( '1', cov.output_pat() );

  auto& sop = cov.input_cover();
  EXPECT_EQ( 0, sop.variable_num() );
  EXPECT_EQ( 0, sop.cube_num() );

  auto expr = cov.expr();
  auto rep_str = expr.rep_string();
  EXPECT_EQ( "C0", rep_str );
}

TEST(BnCoverTest, null_cover_test2)
{
  auto cov = BnCover{0, {}, '0'};

  EXPECT_EQ( 0, cov.input_num() );
  EXPECT_EQ( 0, cov.cube_num() );
  EXPECT_EQ( '0', cov.output_pat() );

  auto sop = cov.input_cover();
  EXPECT_EQ( 0, sop.variable_num() );
  EXPECT_EQ( 0, sop.cube_num() );

  auto expr = cov.expr();
  auto rep_str = expr.rep_string();
  EXPECT_EQ( "C1", rep_str );
}

TEST(BnCoverTest, null_cube_test1)
{
  auto cov = BnCover{0, {{}}};

  EXPECT_EQ( 0, cov.input_num() );
  EXPECT_EQ( 1, cov.cube_num() );
  EXPECT_EQ( '1', cov.output_pat() );

  auto sop = cov.input_cover();
  EXPECT_EQ( 0, sop.variable_num() );
  EXPECT_EQ( 1, sop.cube_num() );

  auto expr = cov.expr();
  auto rep_str = expr.rep_string();
  EXPECT_EQ( "C1", rep_str );
}

TEST(BnCoverTest, null_cube_test2)
{
  auto cov = BnCover{0, {{}}, '0'};

  EXPECT_EQ( 0, cov.input_num() );
  EXPECT_EQ( 1, cov.cube_num() );
  EXPECT_EQ( '0', cov.output_pat() );

  auto sop = cov.input_cover();
  EXPECT_EQ( 0, sop.variable_num() );
  EXPECT_EQ( 1, sop.cube_num() );

  auto expr = cov.expr();
  auto rep_str = expr.rep_string();
  EXPECT_EQ( "C0", rep_str );
}

TEST(BnCoverTest, test3)
{
  Literal lit0{0, false};
  auto cov = BnCover{1, {{lit0}}, '1'};

  auto sop = cov.input_cover();
  EXPECT_EQ( 1, sop.variable_num() );
  EXPECT_EQ( 1, sop.cube_num() );
  EXPECT_EQ( SopPat::_1, sop.get_pat(0, 0) );

  EXPECT_EQ( '1', cov.output_pat() );

  auto expr = cov.expr();
  auto rep_str = expr.rep_string();
  EXPECT_EQ( "P0", rep_str );
}

TEST(BnCoverTest, test4)
{
  Literal lit0{0, false};
  Literal lit1{1, false};
  auto cov = BnCover{2, {{lit0, lit1}}, '0'};

  auto sop = cov.input_cover();
  ASSERT_EQ( 2, sop.variable_num() );
  ASSERT_EQ( 1, sop.cube_num() );
  EXPECT_EQ( SopPat::_1, sop.get_pat(0, 0) );
  EXPECT_EQ( SopPat::_1, sop.get_pat(0, 1) );

  EXPECT_EQ( '0', cov.output_pat() );

  auto expr = cov.expr();
  auto rep_str = expr.rep_string();
  EXPECT_EQ( "O2N0N1", rep_str );
}

TEST(BnCoverTest, test5)
{
  Literal lit0{0, false};
  Literal lit1{1, false};
  Literal lit2{2, false};
  auto cov = BnCover{3, {{~lit0}, {~lit1}, {~lit2}}, '1'};

  auto sop = cov.input_cover();
  ASSERT_EQ( 3, sop.variable_num() );
  ASSERT_EQ( 3, sop.cube_num() );
  EXPECT_EQ( SopPat::_0, sop.get_pat(0, 0) );
  EXPECT_EQ( SopPat::_X, sop.get_pat(0, 1) );
  EXPECT_EQ( SopPat::_X, sop.get_pat(0, 2) );
  EXPECT_EQ( SopPat::_X, sop.get_pat(1, 0) );
  EXPECT_EQ( SopPat::_0, sop.get_pat(1, 1) );
  EXPECT_EQ( SopPat::_X, sop.get_pat(1, 2) );
  EXPECT_EQ( SopPat::_X, sop.get_pat(2, 0) );
  EXPECT_EQ( SopPat::_X, sop.get_pat(2, 1) );
  EXPECT_EQ( SopPat::_0, sop.get_pat(2, 2) );

  EXPECT_EQ( '1', cov.output_pat() );

  auto expr = cov.expr();
  auto rep_str = expr.rep_string();
  EXPECT_EQ( "O3N0N1N2", rep_str );
}

TEST( BnCoverTest, construct1 )
{
  const SizeType input_num = 2;
  const vector<vector<Literal>> icover{
    {Literal{0, false}},
    {Literal{1, true}}
  };
  BnCover cov{input_num, icover};

  EXPECT_EQ( input_num, cov.input_num() );
  EXPECT_EQ( icover.size(), cov.cube_num() );
  EXPECT_EQ( SopPat::_1, cov.input_pat(0, 0) );
  EXPECT_EQ( SopPat::_X, cov.input_pat(0, 1) );
  EXPECT_EQ( SopPat::_X, cov.input_pat(1, 0) );
  EXPECT_EQ( SopPat::_0, cov.input_pat(1, 1) );
  EXPECT_EQ( '1', cov.output_pat() );

  EXPECT_EQ( "O2P0N1", cov.expr().rep_string() );
}

TEST( BnCoverTest, construct2 )
{
  const SizeType input_num = 2;
  const vector<vector<Literal>> icover{
    {Literal{0, false}, Literal{1, true}},
    {Literal{0, true}, Literal{1, false}}
  };
  BnCover cov{input_num, icover, '0'};

  // SopCover のキューブは常にソートされている．
  EXPECT_EQ( input_num, cov.input_num() );
  EXPECT_EQ( icover.size(), cov.cube_num() );
  EXPECT_EQ( SopPat::_0, cov.input_pat(0, 0) );
  EXPECT_EQ( SopPat::_1, cov.input_pat(0, 1) );
  EXPECT_EQ( SopPat::_1, cov.input_pat(1, 0) );
  EXPECT_EQ( SopPat::_0, cov.input_pat(1, 1) );
  EXPECT_EQ( '0', cov.output_pat() );

  EXPECT_EQ( "A2O2P0N1O2N0P1", cov.expr().rep_string() );
}

END_NAMESPACE_YM

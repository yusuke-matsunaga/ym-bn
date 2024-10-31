
/// @file FuncImpl_test.cc
/// @brief FuncImpl_test の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include <gtest/gtest.h>
#include "FuncImpl.h"
#include "ym/SopCover.h"
#include "ym/Expr.h"
#include "ym/TvFunc.h"
#include "ym/Bdd.h"
#include "ym/BddVar.h"
#include "ym/BddMgr.h"


BEGIN_NAMESPACE_YM_BN

TEST(FuncImpl_test, null_cover)
{
  auto func = FuncImpl::new_cover(0, {}, '1');

  EXPECT_EQ( BnFuncType::COVER, func->type() );
  EXPECT_TRUE( func->is_cover() );
  EXPECT_FALSE( func->is_expr() );
  EXPECT_FALSE( func->is_tvfunc() );
  EXPECT_FALSE( func->is_bdd() );
  EXPECT_EQ( 0, func->input_num() );
  EXPECT_EQ( 0, func->input_cover().cube_num() );
  EXPECT_EQ( '1', func->output_pat() );
  EXPECT_EQ( "C0", func->expr().rep_string() );
  EXPECT_THROW( {func->tvfunc();}, std::invalid_argument );
  EXPECT_THROW( {func->bdd();}, std::invalid_argument );
}

TEST(FuncImpl_test, null_cube_test)
{
  auto func = FuncImpl::new_cover(0, {{}}, '1');

  EXPECT_EQ( BnFuncType::COVER, func->type() );
  EXPECT_TRUE( func->is_cover() );
  EXPECT_FALSE( func->is_expr() );
  EXPECT_FALSE( func->is_tvfunc() );
  EXPECT_FALSE( func->is_bdd() );
  EXPECT_EQ( 0, func->input_num() );
  EXPECT_EQ( 1, func->input_cover().cube_num() );
  EXPECT_EQ( '1', func->output_pat() );
  EXPECT_EQ( "C1", func->expr().rep_string() );
  EXPECT_THROW( {func->tvfunc();}, std::invalid_argument );
  EXPECT_THROW( {func->bdd();}, std::invalid_argument );
}

TEST(FuncImpl_test, cover1)
{
  Literal lit0{0, false};
  Literal lit1{1, false};
  auto func = FuncImpl::new_cover(2, {{lit0, lit1}}, '0');

  EXPECT_EQ( BnFuncType::COVER, func->type() );
  EXPECT_TRUE( func->is_cover() );
  EXPECT_FALSE( func->is_expr() );
  EXPECT_FALSE( func->is_tvfunc() );
  EXPECT_FALSE( func->is_bdd() );
  EXPECT_EQ( 2, func->input_num() );

  auto& sop = func->input_cover();
  ASSERT_EQ( 2, sop.variable_num() );
  ASSERT_EQ( 1, sop.cube_num() );
  EXPECT_EQ( SopPat::_1, sop.get_pat(0, 0) );
  EXPECT_EQ( SopPat::_1, sop.get_pat(0, 1) );

  EXPECT_EQ( '0', func->output_pat() );

  auto expr = func->expr();
  auto rep_str = expr.rep_string();
  EXPECT_EQ( "O2N0N1", rep_str );
}

TEST(FuncImpl_test, cover2)
{
  Literal lit0{0, false};
  Literal lit1{1, true};
  auto func = FuncImpl::new_cover(2, {{lit0}, {lit1}}, '1');

  EXPECT_EQ( BnFuncType::COVER, func->type() );
  EXPECT_TRUE( func->is_cover() );
  EXPECT_FALSE( func->is_expr() );
  EXPECT_FALSE( func->is_tvfunc() );
  EXPECT_FALSE( func->is_bdd() );
  EXPECT_EQ( 2, func->input_num() );

  auto& sop = func->input_cover();
  ASSERT_EQ( 2, sop.variable_num() );
  ASSERT_EQ( 2, sop.cube_num() );
  EXPECT_EQ( SopPat::_1, sop.get_pat(0, 0) );
  EXPECT_EQ( SopPat::_X, sop.get_pat(0, 1) );
  EXPECT_EQ( SopPat::_X, sop.get_pat(1, 0) );
  EXPECT_EQ( SopPat::_0, sop.get_pat(1, 1) );

  EXPECT_EQ( '1', func->output_pat() );

  auto expr = func->expr();
  auto rep_str = expr.rep_string();
  EXPECT_EQ( "O2P0N1", rep_str );
}

TEST(FuncImpl_test, cover_bad1)
{
  Literal lit0{0, false};
  Literal lit1{1, true};
  // input_num よりも大きなリテラルがある．
  EXPECT_THROW( {FuncImpl::new_cover(1, {{lit0}, {lit1}}, '1');}, std::invalid_argument );
}

TEST(FuncImpl_test, cover_bad2)
{
  Literal lit0{0, false};
  Literal lit1{1, true};
  // output_pat が不正
  EXPECT_THROW( {FuncImpl::new_cover(2, {{lit0}, {lit1}}, 'a');}, std::invalid_argument );
}

TEST(FuncImpl_test, expr_zero)
{
  auto expr = Expr::zero();
  auto func = FuncImpl::new_expr(expr);

  EXPECT_EQ( BnFuncType::EXPR, func->type() );
  EXPECT_FALSE( func->is_cover() );
  EXPECT_TRUE( func->is_expr() );
  EXPECT_FALSE( func->is_tvfunc() );
  EXPECT_FALSE( func->is_bdd() );
  EXPECT_EQ( 0, func->input_num() );
  EXPECT_THROW( {func->input_cover();}, std::invalid_argument );
  EXPECT_THROW( {func->output_pat();}, std::invalid_argument );
  EXPECT_EQ( "C0", func->expr().rep_string() );
  EXPECT_THROW( {func->tvfunc();}, std::invalid_argument );
  EXPECT_THROW( {func->bdd();}, std::invalid_argument );
}

TEST(FuncImpl_test, expr_one)
{
  auto expr = Expr::one();
  auto func = FuncImpl::new_expr(expr);

  EXPECT_EQ( BnFuncType::EXPR, func->type() );
  EXPECT_FALSE( func->is_cover() );
  EXPECT_TRUE( func->is_expr() );
  EXPECT_FALSE( func->is_tvfunc() );
  EXPECT_FALSE( func->is_bdd() );
  EXPECT_EQ( 0, func->input_num() );
  EXPECT_THROW( {func->input_cover();}, std::invalid_argument );
  EXPECT_THROW( {func->output_pat();}, std::invalid_argument );
  EXPECT_EQ( "C1", func->expr().rep_string() );
  EXPECT_THROW( {func->tvfunc();}, std::invalid_argument );
  EXPECT_THROW( {func->bdd();}, std::invalid_argument );
}

TEST(FuncImpl_test, expr1)
{
  auto v0 = Expr::literal(0);
  auto v1 = Expr::literal(1);
  auto expr = v0 | ~v1;
  auto func = FuncImpl::new_expr(expr);

  EXPECT_EQ( BnFuncType::EXPR, func->type() );
  EXPECT_FALSE( func->is_cover() );
  EXPECT_TRUE( func->is_expr() );
  EXPECT_FALSE( func->is_tvfunc() );
  EXPECT_FALSE( func->is_bdd() );
  EXPECT_EQ( 2, func->input_num() );
  EXPECT_THROW( {func->input_cover();}, std::invalid_argument );
  EXPECT_THROW( {func->output_pat();}, std::invalid_argument );
  EXPECT_EQ( "O2P0N1", func->expr().rep_string() );
  EXPECT_THROW( {func->tvfunc();}, std::invalid_argument );
  EXPECT_THROW( {func->bdd();}, std::invalid_argument );
}

TEST(FuncImpl_test, expr2)
{
  // 1変数だが2入力関数とみなす．
  auto v1 = Expr::literal(1);
  auto expr = v1;
  auto func = FuncImpl::new_expr(expr);

  EXPECT_EQ( BnFuncType::EXPR, func->type() );
  EXPECT_FALSE( func->is_cover() );
  EXPECT_TRUE( func->is_expr() );
  EXPECT_FALSE( func->is_tvfunc() );
  EXPECT_FALSE( func->is_bdd() );
  EXPECT_EQ( 2, func->input_num() );
  EXPECT_THROW( {func->input_cover();}, std::invalid_argument );
  EXPECT_THROW( {func->output_pat();}, std::invalid_argument );
  EXPECT_EQ( "P1", func->expr().rep_string() );
  EXPECT_THROW( {func->tvfunc();}, std::invalid_argument );
  EXPECT_THROW( {func->bdd();}, std::invalid_argument );
}

TEST(FuncImpl_test, tvfunc_zero)
{
  auto tvfunc = TvFunc::zero(4);
  auto func = FuncImpl::new_tvfunc(tvfunc);

  EXPECT_EQ( BnFuncType::TVFUNC, func->type() );
  EXPECT_FALSE( func->is_cover() );
  EXPECT_FALSE( func->is_expr() );
  EXPECT_TRUE( func->is_tvfunc() );
  EXPECT_FALSE( func->is_bdd() );
  EXPECT_EQ( 4, func->input_num() );
  EXPECT_THROW( {func->input_cover();}, std::invalid_argument );
  EXPECT_THROW( {func->output_pat();}, std::invalid_argument );
  EXPECT_THROW( {func->expr();}, std::invalid_argument );
  EXPECT_EQ( "0000000000000000", func->tvfunc().str() );
  EXPECT_THROW( {func->bdd();}, std::invalid_argument );
}

TEST(FuncImpl_test, tvfunc_one)
{
  auto tvfunc = TvFunc::one(4);
  auto func = FuncImpl::new_tvfunc(tvfunc);

  EXPECT_EQ( BnFuncType::TVFUNC, func->type() );
  EXPECT_FALSE( func->is_cover() );
  EXPECT_FALSE( func->is_expr() );
  EXPECT_TRUE( func->is_tvfunc() );
  EXPECT_FALSE( func->is_bdd() );
  EXPECT_EQ( 4, func->input_num() );
  EXPECT_THROW( {func->input_cover();}, std::invalid_argument );
  EXPECT_THROW( {func->output_pat();}, std::invalid_argument );
  EXPECT_THROW( {func->expr();}, std::invalid_argument );
  EXPECT_EQ( "1111111111111111", func->tvfunc().str() );
  EXPECT_THROW( {func->bdd();}, std::invalid_argument );
}

TEST(FuncImpl_test, tvfunc1)
{
  auto v0 = TvFunc::posi_literal(3, 0);
  auto v1 = TvFunc::posi_literal(3, 1);
  auto v2 = TvFunc::posi_literal(3, 2);
  auto tvfunc = v0 | v1 | v2;
  auto func = FuncImpl::new_tvfunc(tvfunc);

  EXPECT_EQ( BnFuncType::TVFUNC, func->type() );
  EXPECT_FALSE( func->is_cover() );
  EXPECT_FALSE( func->is_expr() );
  EXPECT_TRUE( func->is_tvfunc() );
  EXPECT_FALSE( func->is_bdd() );
  EXPECT_EQ( 3, func->input_num() );
  EXPECT_THROW( {func->input_cover();}, std::invalid_argument );
  EXPECT_THROW( {func->output_pat();}, std::invalid_argument );
  EXPECT_THROW( {func->expr();}, std::invalid_argument );
  EXPECT_EQ( "11111110", func->tvfunc().str() );
  EXPECT_THROW( {func->bdd();}, std::invalid_argument );
}

TEST(FuncImpl_test, bdd_zero)
{
  BddMgr mgr;
  auto bdd = mgr.zero();
  auto func = FuncImpl::new_bdd(bdd);

  EXPECT_EQ( BnFuncType::BDD, func->type() );
  EXPECT_FALSE( func->is_cover() );
  EXPECT_FALSE( func->is_expr() );
  EXPECT_FALSE( func->is_tvfunc() );
  EXPECT_TRUE( func->is_bdd() );
  EXPECT_EQ( 0, func->input_num() );
  EXPECT_THROW( {func->input_cover();}, std::invalid_argument );
  EXPECT_THROW( {func->output_pat();}, std::invalid_argument );
  EXPECT_THROW( {func->expr();}, std::invalid_argument );
  EXPECT_THROW( {func->tvfunc();}, std::invalid_argument );
  EXPECT_EQ( bdd, func->bdd() );
}

TEST(FuncImpl_test, bdd_one)
{
  BddMgr mgr;
  auto bdd = mgr.one();
  auto func = FuncImpl::new_bdd(bdd);

  EXPECT_EQ( BnFuncType::BDD, func->type() );
  EXPECT_FALSE( func->is_cover() );
  EXPECT_FALSE( func->is_expr() );
  EXPECT_FALSE( func->is_tvfunc() );
  EXPECT_TRUE( func->is_bdd() );
  EXPECT_EQ( 0, func->input_num() );
  EXPECT_THROW( {func->input_cover();}, std::invalid_argument );
  EXPECT_THROW( {func->output_pat();}, std::invalid_argument );
  EXPECT_THROW( {func->expr();}, std::invalid_argument );
  EXPECT_THROW( {func->tvfunc();}, std::invalid_argument );
  EXPECT_EQ( bdd, func->bdd() );
}

TEST(FuncImpl_test, bdd1)
{
  BddMgr mgr;
  auto var0 = mgr.new_variable();
  auto var1 = mgr.new_variable();
  auto v0 = mgr.posi_literal(var0);
  auto v1 = mgr.posi_literal(var1);
  auto bdd = v0 & ~v1;
  auto func = FuncImpl::new_bdd(bdd);

  EXPECT_EQ( BnFuncType::BDD, func->type() );
  EXPECT_FALSE( func->is_cover() );
  EXPECT_FALSE( func->is_expr() );
  EXPECT_FALSE( func->is_tvfunc() );
  EXPECT_TRUE( func->is_bdd() );
  EXPECT_EQ( 2, func->input_num() );
  EXPECT_THROW( {func->input_cover();}, std::invalid_argument );
  EXPECT_THROW( {func->output_pat();}, std::invalid_argument );
  EXPECT_THROW( {func->expr();}, std::invalid_argument );
  EXPECT_THROW( {func->tvfunc();}, std::invalid_argument );
  EXPECT_EQ( bdd, func->bdd() );
}

TEST(FuncImpl_test, bdd2)
{
  BddMgr mgr;
  auto var0 = mgr.new_variable();
  auto v1 = mgr.posi_literal(var0);
  auto bdd = ~v1;
  auto func = FuncImpl::new_bdd(bdd);

  EXPECT_EQ( BnFuncType::BDD, func->type() );
  EXPECT_FALSE( func->is_cover() );
  EXPECT_FALSE( func->is_expr() );
  EXPECT_FALSE( func->is_tvfunc() );
  EXPECT_TRUE( func->is_bdd() );
  EXPECT_EQ( 1, func->input_num() );
  EXPECT_THROW( {func->input_cover();}, std::invalid_argument );
  EXPECT_THROW( {func->output_pat();}, std::invalid_argument );
  EXPECT_THROW( {func->expr();}, std::invalid_argument );
  EXPECT_THROW( {func->tvfunc();}, std::invalid_argument );
  EXPECT_EQ( bdd, func->bdd() );
}

END_NAMESPACE_YM_BN

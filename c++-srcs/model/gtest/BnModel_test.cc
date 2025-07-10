
/// @file BnModel_test.cc
/// @brief BnModel_test の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include <gtest/gtest.h>
#include "ym/BnModel.h"
#include "ym/BnNode.h"
#include "ym/BnFunc.h"
#include "ym/SopCover.h"
#include "ym/TvFunc.h"
#include "ym/Bdd.h"
#include "ym/BddVar.h"
#include "ym/BddMgr.h"


BEGIN_NAMESPACE_YM_BN

TEST( BnModelTest, constructor1 )
{
  BnModel model;

  EXPECT_EQ( std::string{}, model.name() );
  EXPECT_EQ( std::vector<std::string>{}, model.comment_list() );
  EXPECT_EQ( 0, model.node_num() );
  EXPECT_EQ( 0, model.input_num() );
  EXPECT_EQ( 0, model.input_list().size() );
  EXPECT_EQ( 0, model.output_num() );
  EXPECT_EQ( 0, model.output_list().size() );
  EXPECT_EQ( 0, model.logic_num() );
  EXPECT_EQ( 0, model.logic_list().size() );
  EXPECT_EQ( 0, model.dff_num() );
  EXPECT_EQ( 0, model.func_num() );
}

TEST( BnModelTest, input_bad )
{
  BnModel model;

  EXPECT_THROW( {model.input(0);}, std::out_of_range );
}

TEST( BnModelTest, input_name_bad )
{
  BnModel model;

  EXPECT_THROW( {model.input_name(0);}, std::out_of_range );
}

TEST( BnModelTest, output_bad )
{
  BnModel model;

  EXPECT_THROW( {model.output(0);}, std::out_of_range );
}

TEST( BnModelTest, output_name_bad )
{
  BnModel model;

  EXPECT_THROW( {model.output_name(0);}, std::out_of_range );
}

TEST( BnModelTest, logic_bad )
{
  BnModel model;

  EXPECT_THROW( {model.logic(0);}, std::out_of_range );
}

TEST( BnModelTest, dff_output_bad )
{
  BnModel model;

  EXPECT_THROW( {model.dff_output(0);}, std::out_of_range );
}

TEST( BnModelTest, func_bad )
{
  BnModel model;

  EXPECT_THROW( {model.func(0);}, std::out_of_range );
}

TEST( BnModelTest, new_primitive )
{
  BnModel model;

  auto type = PrimType::And;

  auto input1 = model.new_input();
  auto input2 = model.new_input();
  auto fanin_list = std::vector<BnNode>{input1, input2};
  auto input_num = fanin_list.size();

  auto node = model.new_primitive(type, fanin_list);

  EXPECT_EQ( BnNode::LOGIC, node.type() );
  EXPECT_FALSE( node.is_input() );
  EXPECT_TRUE( node.is_logic() );

  auto func = node.func();
  EXPECT_EQ( BnFunc::PRIMITIVE, func.type() );
  EXPECT_EQ( type, func.primitive_type() );

  EXPECT_EQ( fanin_list.size(), node.fanin_num() );
  for ( SizeType i = 0; i < fanin_list.size(); ++ i ) {
    auto node1 = fanin_list[i];
    EXPECT_EQ( node1, node.fanin(i) );
  }
  EXPECT_EQ( fanin_list, node.fanin_list() );

  EXPECT_FALSE( node.is_primary_input() );
  EXPECT_FALSE( node.is_dff_output() );
  EXPECT_THROW( node.input_id(), std::invalid_argument );
  EXPECT_THROW( node.dff_id(), std::invalid_argument );
}

TEST( BnModelTest, reg_cover )
{
  BnModel model;

  auto input1 = model.new_input();
  auto input2 = model.new_input();
  auto fanin_list = std::vector<BnNode>{input1, input2};
  auto input_num = fanin_list.size();

  auto lit0 = Literal{0, false};
  auto lit1 = Literal{1, false};
  auto input_cover = SopCover(input_num, {{lit0, lit1}});
  auto output_inv = true;

  auto node = model.new_cover(input_cover, output_inv, fanin_list);

  EXPECT_EQ( BnNode::LOGIC, node.type() );
  EXPECT_FALSE( node.is_input() );
  EXPECT_TRUE( node.is_logic() );

  auto func = node.func();
  EXPECT_EQ( BnFunc::COVER, func.type() );
  EXPECT_FALSE( func.is_primitive() );
  EXPECT_TRUE( func.is_cover() );
  EXPECT_FALSE( func.is_expr() );
  EXPECT_FALSE( func.is_tvfunc() );
  EXPECT_FALSE( func.is_bdd() );
  EXPECT_EQ( input_cover, func.input_cover() );
  ASSERT_EQ( output_inv, func.output_inv() );
}

TEST( BnModelTest, new_expr )
{
  BnModel model;

  auto v0 = Expr::literal(0);
  auto v1 = Expr::literal(1);
  auto expr = v0 | ~v1;

  auto input1 = model.new_input();
  auto input2 = model.new_input();
  auto fanin_list = std::vector<BnNode>{input1, input2};
  auto input_num = fanin_list.size();

  auto node = model.new_expr(expr, fanin_list);

  EXPECT_EQ( BnNode::LOGIC, node.type() );
  EXPECT_FALSE( node.is_input() );
  EXPECT_TRUE( node.is_logic() );

  auto func = node.func();
  EXPECT_EQ( BnFunc::EXPR, func.type() );
  EXPECT_FALSE( func.is_primitive() );
  EXPECT_FALSE( func.is_cover() );
  EXPECT_TRUE( func.is_expr() );
  EXPECT_FALSE( func.is_tvfunc() );
  EXPECT_FALSE( func.is_bdd() );
  EXPECT_EQ( "O2P0N1", func.expr().rep_string() );
}

TEST( BnModelTest, new_tvfunc )
{
  BnModel model;

  auto v0 = TvFunc::posi_literal(3, 0);
  auto v1 = TvFunc::posi_literal(3, 1);
  auto v2 = TvFunc::posi_literal(3, 2);
  auto tvfunc = v0 | v1 | v2;

  auto input1 = model.new_input();
  auto input2 = model.new_input();
  auto fanin_list = std::vector<BnNode>{input1, input2};
  auto input_num = fanin_list.size();

  auto node = model.new_tvfunc(tvfunc, fanin_list);

  EXPECT_EQ( BnNode::LOGIC, node.type() );
  EXPECT_FALSE( node.is_input() );
  EXPECT_TRUE( node.is_logic() );

  auto func = node.func();
  EXPECT_EQ( BnFunc::TVFUNC, func.type() );
  EXPECT_FALSE( func.is_primitive() );
  EXPECT_FALSE( func.is_cover() );
  EXPECT_FALSE( func.is_expr() );
  EXPECT_TRUE( func.is_tvfunc() );
  EXPECT_FALSE( func.is_bdd() );
  EXPECT_EQ( 3, func.input_num() );
  EXPECT_EQ( "11111110", func.tvfunc().str() );
}

TEST( BnModelTest, new_bdd )
{
  BnModel model;

  BddMgr mgr;
  auto var0 = mgr.variable(0);
  auto var1 = mgr.variable(1);
  auto bdd = var0 & ~var1;

  auto input1 = model.new_input();
  auto input2 = model.new_input();
  auto fanin_list = std::vector<BnNode>{input1, input2};
  auto input_num = fanin_list.size();

  auto node = model.new_bdd(bdd, fanin_list);

  EXPECT_EQ( BnNode::LOGIC, node.type() );
  EXPECT_FALSE( node.is_input() );
  EXPECT_TRUE( node.is_logic() );

  auto func = node.func();
  EXPECT_EQ( BnFunc::BDD, func.type() );
  EXPECT_FALSE( func.is_primitive() );
  EXPECT_FALSE( func.is_cover() );
  EXPECT_FALSE( func.is_expr() );
  EXPECT_FALSE( func.is_tvfunc() );
  EXPECT_TRUE( func.is_bdd() );
  EXPECT_EQ( 2, func.input_num() );
  EXPECT_EQ( bdd, mgr.copy(func.bdd()) );
}

TEST( BnModelTest, new_input )
{
  BnModel model;

  auto node = model.new_input();

  EXPECT_TRUE( node.is_valid() );
  EXPECT_TRUE( node.is_input() );
}

TEST( BnModelTest, new_dff_output)
{
  BnModel model;

  auto node = model.new_dff_output();

  EXPECT_TRUE( node.is_valid() );
  EXPECT_EQ( BnNode::INPUT, node.type() );
  EXPECT_TRUE( node.is_input() );
  EXPECT_FALSE( node.is_logic() );

  EXPECT_FALSE( node.is_primary_input() );
  EXPECT_TRUE( node.is_dff_output() );
  EXPECT_THROW( node.input_id(), std::invalid_argument );
  EXPECT_EQ( 0, node.dff_id() );

  EXPECT_THROW( node.func(), std::invalid_argument );
  EXPECT_EQ( 0, node.fanin_num() );
  EXPECT_THROW( node.fanin(0), std::out_of_range );
  EXPECT_EQ( std::vector<BnNode>{}, node.fanin_list() );
}

TEST( BnModelTest, clear )
{
  BnModel model;

  auto input1 = model.new_input();
  auto input2 = model.new_input();
  auto fanin_list = std::vector<BnNode>{input1, input2};
  auto type = PrimType::And;
  auto node = model.new_primitive(type, fanin_list);
  auto output = model.new_output(node);

  model.wrap_up();

  EXPECT_EQ( 2, model.input_num() );
  EXPECT_EQ( 1, model.output_num() );
  EXPECT_EQ( 1, model.logic_num() );

  model.clear();

  EXPECT_EQ( 0, model.input_num() );
  EXPECT_EQ( 0, model.output_num() );
  EXPECT_EQ( 0, model.logic_num() );
}

END_NAMESPACE_YM_BN

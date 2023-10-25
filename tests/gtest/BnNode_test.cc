
/// @file BnNode_test.cc
/// @brief BnNode_test の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include <gtest/gtest.h>
#include "ym/BnNode.h"
#include "ym/BnModel.h"
#include "ModelImpl.h"


BEGIN_NAMESPACE_YM_BN

TEST(BnNodeTest, constructor1)
{
  BnNode node;

  EXPECT_FALSE( node.is_valid() );
  EXPECT_THROW( {node.parent_model(); }, std::invalid_argument );
  EXPECT_THROW( {node.name(); }, std::invalid_argument );
  EXPECT_THROW( {node.type(); }, std::invalid_argument );
  EXPECT_THROW( {node.is_input(); }, std::invalid_argument );
  EXPECT_THROW( {node.is_logic(); }, std::invalid_argument );
  EXPECT_THROW( {node.is_primitive(); }, std::invalid_argument );
  EXPECT_THROW( {node.is_aig(); }, std::invalid_argument );
  EXPECT_THROW( {node.is_cover(); }, std::invalid_argument );
  EXPECT_THROW( {node.is_expr(); }, std::invalid_argument );
  EXPECT_THROW( {node.is_cell(); }, std::invalid_argument );
  EXPECT_THROW( {node.is_func(); }, std::invalid_argument );
  EXPECT_THROW( {node.is_bdd(); }, std::invalid_argument );
  EXPECT_THROW( {node.input_id(); }, std::invalid_argument );
  EXPECT_THROW( {node.fanin_num(); }, std::invalid_argument );
  EXPECT_THROW( {node.fanin(0); }, std::invalid_argument );
  EXPECT_THROW( {node.fanin_list(); }, std::invalid_argument );
  EXPECT_THROW( {node.primitive_type(); }, std::invalid_argument );
  EXPECT_THROW( {node.fanin_inv(0); }, std::invalid_argument );
  EXPECT_THROW( {node.cover_id(); }, std::invalid_argument );
  EXPECT_THROW( {node.cover(); }, std::invalid_argument );
  EXPECT_THROW( {node.expr_id(); }, std::invalid_argument );
  EXPECT_THROW( {node.expr(); }, std::invalid_argument );
  EXPECT_THROW( {node.cell_id(); }, std::invalid_argument );
  EXPECT_THROW( {node.cell(); }, std::invalid_argument );
  EXPECT_THROW( {node.func_id(); }, std::invalid_argument );
  EXPECT_THROW( {node.func(); }, std::invalid_argument );
  EXPECT_THROW( {node.bdd_id(); }, std::invalid_argument );
  EXPECT_THROW( {node.bdd(); }, std::invalid_argument );
}

TEST(BnNodeTest, bad_id)
{
  shared_ptr<ModelImpl> model{new ModelImpl};

  BnNode node{model, BAD_ID};

  EXPECT_FALSE( node.is_valid() );
}

TEST(BnNodeTest, constructor2)
{
  shared_ptr<ModelImpl> model{new ModelImpl};

  string name{"abc"};
  auto id = model->new_node(name);

  BnNode node{model, id};

  EXPECT_TRUE( node.is_valid() );
  EXPECT_EQ( name, node.name() );
  EXPECT_EQ( BnNodeType::NONE, node.type() );
  EXPECT_FALSE( node.is_input() );
  EXPECT_FALSE( node.is_logic() );
  EXPECT_FALSE( node.is_primitive() );
  EXPECT_FALSE( node.is_aig() );
  EXPECT_FALSE( node.is_cover() );
  EXPECT_FALSE( node.is_expr() );
  EXPECT_FALSE( node.is_cell() );
  EXPECT_FALSE( node.is_func() );
  EXPECT_FALSE( node.is_bdd() );
  EXPECT_THROW( {node.input_id(); }, std::invalid_argument );
  EXPECT_THROW( {node.fanin_num(); }, std::invalid_argument );
  EXPECT_THROW( {node.fanin(0); }, std::invalid_argument );
  EXPECT_THROW( {node.fanin_list(); }, std::invalid_argument );
  EXPECT_THROW( {node.primitive_type(); }, std::invalid_argument );
  EXPECT_THROW( {node.fanin_inv(0); }, std::invalid_argument );
  EXPECT_THROW( {node.cover_id(); }, std::invalid_argument );
  EXPECT_THROW( {node.cover(); }, std::invalid_argument );
  EXPECT_THROW( {node.expr_id(); }, std::invalid_argument );
  EXPECT_THROW( {node.expr(); }, std::invalid_argument );
  EXPECT_THROW( {node.cell_id(); }, std::invalid_argument );
  EXPECT_THROW( {node.func_id(); }, std::invalid_argument );
  EXPECT_THROW( {node.func(); }, std::invalid_argument );
  EXPECT_THROW( {node.bdd_id(); }, std::invalid_argument );
  EXPECT_THROW( {node.bdd(); }, std::invalid_argument );
}

TEST( BnNodeTest, input)
{
  BnModel model;

  auto node = model.new_input();

  EXPECT_TRUE( node.is_valid() );
  EXPECT_EQ( BnNodeType::INPUT, node.type() );
  EXPECT_TRUE( node.is_input() );
  EXPECT_FALSE( node.is_logic() );
  EXPECT_FALSE( node.is_primitive() );
  EXPECT_FALSE( node.is_aig() );
  EXPECT_FALSE( node.is_cover() );
  EXPECT_FALSE( node.is_expr() );
  EXPECT_FALSE( node.is_cell() );
  EXPECT_FALSE( node.is_func() );
  EXPECT_FALSE( node.is_bdd() );
  EXPECT_EQ( 0, node.input_id() );
  EXPECT_THROW( {node.fanin_num(); }, std::invalid_argument );
  EXPECT_THROW( {node.fanin(0); }, std::invalid_argument );
  EXPECT_THROW( {node.fanin_list(); }, std::invalid_argument );
  EXPECT_THROW( {node.primitive_type(); }, std::invalid_argument );
  EXPECT_THROW( {node.fanin_inv(0); }, std::invalid_argument );
  EXPECT_THROW( {node.cover_id(); }, std::invalid_argument );
  EXPECT_THROW( {node.cover(); }, std::invalid_argument );
  EXPECT_THROW( {node.expr_id(); }, std::invalid_argument );
  EXPECT_THROW( {node.expr(); }, std::invalid_argument );
  EXPECT_THROW( {node.cell_id(); }, std::invalid_argument );
  EXPECT_THROW( {node.func_id(); }, std::invalid_argument );
  EXPECT_THROW( {node.func(); }, std::invalid_argument );
  EXPECT_THROW( {node.bdd_id(); }, std::invalid_argument );
  EXPECT_THROW( {node.bdd(); }, std::invalid_argument );
}

TEST( BnNodeTest, primitive)
{
  BnModel model;

  auto input1 = model.new_input();
  auto input2 = model.new_input();
  vector<BnNode> fanin_list{input1, input2};
  PrimType type = PrimType::And;
  auto node = model.new_primitive(fanin_list, type);

  EXPECT_EQ( BnNodeType::PRIMITIVE, node.type() );
  EXPECT_FALSE( node.is_input() );
  EXPECT_TRUE( node.is_logic() );
  EXPECT_TRUE( node.is_primitive() );
  EXPECT_FALSE( node.is_aig() );
  EXPECT_FALSE( node.is_cover() );
  EXPECT_FALSE( node.is_expr() );
  EXPECT_FALSE( node.is_cell() );
  EXPECT_FALSE( node.is_func() );
  EXPECT_FALSE( node.is_bdd() );
  EXPECT_EQ( fanin_list.size(), node.fanin_num() );
  for ( SizeType i = 0; i < fanin_list.size(); ++ i ) {
    auto node1 = fanin_list[i];
    EXPECT_EQ( node1, node.fanin(i) );
  }
  EXPECT_EQ( fanin_list, node.fanin_list() );
  EXPECT_THROW( {node.input_id();}, std::invalid_argument );
  EXPECT_EQ( type, node.primitive_type() );
  EXPECT_THROW( {node.fanin_inv(0);}, std::invalid_argument );
  EXPECT_THROW( {node.fanin_inv(1);}, std::invalid_argument );
  EXPECT_THROW( {node.cover_id();}, std::invalid_argument );
  EXPECT_THROW( {node.expr_id();}, std::invalid_argument );
  EXPECT_THROW( {node.cell_id();}, std::invalid_argument );
  EXPECT_THROW( {node.func_id(); }, std::invalid_argument );
  EXPECT_THROW( {node.func(); }, std::invalid_argument );
  EXPECT_THROW( {node.bdd_id(); }, std::invalid_argument );
  EXPECT_THROW( {node.bdd(); }, std::invalid_argument );
}

TEST( BnNodeTest, aig )
{
  BnModel model;

  auto input1 = model.new_input();
  auto input2 = model.new_input();
  bool inv0 = false;
  bool inv1 = true;
  auto node = model.new_aig(input1, input2, inv0, inv1);

  EXPECT_EQ( BnNodeType::AIG, node.type() );
  EXPECT_FALSE( node.is_input() );
  EXPECT_TRUE( node.is_logic() );
  EXPECT_FALSE( node.is_primitive() );
  EXPECT_TRUE( node.is_aig() );
  EXPECT_FALSE( node.is_cover() );
  EXPECT_FALSE( node.is_expr() );
  EXPECT_FALSE( node.is_cell() );
  EXPECT_FALSE( node.is_func() );
  EXPECT_FALSE( node.is_bdd() );
  EXPECT_THROW( {node.input_id();}, std::invalid_argument );
  EXPECT_EQ( 2, node.fanin_num() );
  EXPECT_EQ( input1, node.fanin(0) );
  EXPECT_EQ( input2, node.fanin(1) );
  vector<BnNode> fanin_node_list{input1, input2};
  EXPECT_EQ( fanin_node_list, node.fanin_list() );
  EXPECT_THROW( {node.primitive_type();}, std::invalid_argument );
  EXPECT_EQ( inv0, node.fanin_inv(0) );
  EXPECT_EQ( inv1, node.fanin_inv(1) );
  EXPECT_THROW( {node.cover_id();}, std::invalid_argument );
  EXPECT_THROW( {node.cover();}, std::invalid_argument );
  EXPECT_THROW( {node.expr_id();}, std::invalid_argument );
  EXPECT_THROW( {node.expr();}, std::invalid_argument );
  EXPECT_THROW( {node.cell_id();}, std::invalid_argument );
  EXPECT_THROW( {node.cell();}, std::invalid_argument );
  EXPECT_THROW( {node.func_id(); }, std::invalid_argument );
  EXPECT_THROW( {node.func(); }, std::invalid_argument );
  EXPECT_THROW( {node.bdd_id(); }, std::invalid_argument );
  EXPECT_THROW( {node.bdd(); }, std::invalid_argument );
}

TEST( BnNodeTest, cover )
{
  BnModel model;

  auto input1 = model.new_input();
  auto input2 = model.new_input();
  vector<BnNode> fanin_list{input1, input2};
  Literal lit1{0, false};
  Literal lit2{1, false};
  vector<vector<Literal>> cube_list{{lit1}, {lit2}};
  SizeType cover_id = model.add_cover(2, cube_list, '0');
  auto node = model.new_cover(fanin_list, cover_id);

  EXPECT_EQ( BnNodeType::COVER, node.type() );
  EXPECT_FALSE( node.is_input() );
  EXPECT_TRUE( node.is_logic() );
  EXPECT_FALSE( node.is_primitive() );
  EXPECT_FALSE( node.is_aig() );
  EXPECT_TRUE( node.is_cover() );
  EXPECT_FALSE( node.is_expr() );
  EXPECT_FALSE( node.is_cell() );
  EXPECT_FALSE( node.is_func() );
  EXPECT_FALSE( node.is_bdd() );
  EXPECT_THROW( {node.input_id();}, std::invalid_argument );
  EXPECT_EQ( fanin_list.size(), node.fanin_num() );
  for ( SizeType i = 0; i < node.fanin_num(); ++ i ) {
    EXPECT_EQ( fanin_list[i], node.fanin(i) );
  }
  EXPECT_EQ( fanin_list, node.fanin_list() );
  EXPECT_THROW( {node.primitive_type();}, std::invalid_argument );
  EXPECT_THROW( {node.fanin_inv(0);}, std::invalid_argument );
  EXPECT_THROW( {node.fanin_inv(1);}, std::invalid_argument );
  EXPECT_EQ( cover_id, node.cover_id() );
  EXPECT_THROW( {node.expr_id();}, std::invalid_argument );
  EXPECT_THROW( {node.expr();}, std::invalid_argument );
  EXPECT_THROW( {node.cell_id();}, std::invalid_argument );
  EXPECT_THROW( {node.cell();}, std::invalid_argument );
  EXPECT_THROW( {node.func_id(); }, std::invalid_argument );
  EXPECT_THROW( {node.func(); }, std::invalid_argument );
  EXPECT_THROW( {node.bdd_id(); }, std::invalid_argument );
  EXPECT_THROW( {node.bdd(); }, std::invalid_argument );
}

TEST( BnNodeTest, expr )
{
  BnModel model;

  auto input1 = model.new_input();
  auto input2 = model.new_input();
  vector<BnNode> fanin_list{input1, input2};
  auto lit1 = Expr::make_posi_literal(0);
  auto lit2 = Expr::make_posi_literal(1);
  auto expr = lit1 | lit2;
  SizeType expr_id = model.add_expr(expr);
  auto node = model.new_expr(fanin_list, expr_id);

  EXPECT_EQ( BnNodeType::EXPR, node.type() );
  EXPECT_FALSE( node.is_input() );
  EXPECT_TRUE( node.is_logic() );
  EXPECT_FALSE( node.is_primitive() );
  EXPECT_FALSE( node.is_aig() );
  EXPECT_FALSE( node.is_cover() );
  EXPECT_TRUE( node.is_expr() );
  EXPECT_FALSE( node.is_cell() );
  EXPECT_FALSE( node.is_func() );
  EXPECT_FALSE( node.is_bdd() );
  EXPECT_THROW( {node.input_id();}, std::invalid_argument );
  EXPECT_EQ( fanin_list.size(), node.fanin_num() );
  for ( SizeType i = 0; i < node.fanin_num(); ++ i ) {
    EXPECT_EQ( fanin_list[i], node.fanin(i) );
  }
  EXPECT_EQ( fanin_list, node.fanin_list() );
  EXPECT_THROW( {node.primitive_type();}, std::invalid_argument );
  EXPECT_THROW( {node.fanin_inv(0);}, std::invalid_argument );
  EXPECT_THROW( {node.fanin_inv(1);}, std::invalid_argument );
  EXPECT_THROW( {node.cover_id();}, std::invalid_argument );
  EXPECT_EQ( expr_id, node.expr_id() );
  EXPECT_EQ( expr, node.expr() );
  EXPECT_THROW( {node.cell_id();}, std::invalid_argument );
  EXPECT_THROW( {node.cell();}, std::invalid_argument );
  EXPECT_THROW( {node.func_id(); }, std::invalid_argument );
  EXPECT_THROW( {node.func(); }, std::invalid_argument );
  EXPECT_THROW( {node.bdd_id(); }, std::invalid_argument );
  EXPECT_THROW( {node.bdd(); }, std::invalid_argument );
}

TEST( BnNodeTest, cell )
{
  auto FILENAME = string{DATAPATH} + string{"/HIT018.typ.snp"};
  auto library = ClibCellLibrary::read_liberty(FILENAME);
  auto CELL_NAME = "HIT18NAND2P005";
  auto cell = library.cell(CELL_NAME);

  BnModel model;

  auto input1 = model.new_input();
  auto input2 = model.new_input();
  vector<BnNode> fanin_list{input1, input2};
  auto node = model.new_cell(fanin_list, cell);

  EXPECT_EQ( BnNodeType::CELL, node.type() );
  EXPECT_FALSE( node.is_input() );
  EXPECT_TRUE( node.is_logic() );
  EXPECT_FALSE( node.is_primitive() );
  EXPECT_FALSE( node.is_aig() );
  EXPECT_FALSE( node.is_cover() );
  EXPECT_FALSE( node.is_expr() );
  EXPECT_FALSE( node.is_func() );
  EXPECT_FALSE( node.is_bdd() );
  EXPECT_TRUE( node.is_cell() );
  EXPECT_EQ( fanin_list.size(), node.fanin_num() );
  for ( SizeType i = 0; i < node.fanin_num(); ++ i ) {
    EXPECT_EQ( fanin_list[i], node.fanin(i) );
  }
  EXPECT_EQ( fanin_list, node.fanin_list() );
  EXPECT_THROW( {node.primitive_type();}, std::invalid_argument );
  EXPECT_THROW( {node.fanin_inv(0);}, std::invalid_argument );
  EXPECT_THROW( {node.fanin_inv(1);}, std::invalid_argument );
  EXPECT_THROW( {node.cover_id();}, std::invalid_argument );
  EXPECT_THROW( {node.expr_id();}, std::invalid_argument );
  EXPECT_EQ( cell.id(), node.cell_id() );
  EXPECT_EQ( cell, node.cell() );
  EXPECT_THROW( {node.func_id(); }, std::invalid_argument );
  EXPECT_THROW( {node.func(); }, std::invalid_argument );
  EXPECT_THROW( {node.bdd_id(); }, std::invalid_argument );
  EXPECT_THROW( {node.bdd(); }, std::invalid_argument );
}

END_NAMESPACE_YM_BN

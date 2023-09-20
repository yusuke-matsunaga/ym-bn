
/// @file BfNode_test.cc
/// @brief BfNode_test の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include <gtest/gtest.h>
#include "ym/BfNode.h"
#include "ym/BfModel.h"
#include "ModelImpl.h"


BEGIN_NAMESPACE_YM_BNFE

TEST(BfNodeTest, constructor1)
{
  BfNode node;

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
  EXPECT_THROW( {node.is_dff(); }, std::invalid_argument );
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
  EXPECT_THROW( {node.dff_src(); }, std::invalid_argument );
  EXPECT_THROW( {node.dff_rval(); }, std::invalid_argument );
}

TEST(BfNodeTest, constructor2)
{
  shared_ptr<ModelImpl> model{new ModelImpl};

  string name{"abc"};
  auto id = model->new_node(name);

  BfNode node{model, id};

  EXPECT_TRUE( node.is_valid() );
  EXPECT_EQ( name, node.name() );
  EXPECT_EQ( BfNodeType::None, node.type() );
  EXPECT_FALSE( node.is_input() );
  EXPECT_FALSE( node.is_logic() );
  EXPECT_FALSE( node.is_primitive() );
  EXPECT_FALSE( node.is_aig() );
  EXPECT_FALSE( node.is_cover() );
  EXPECT_FALSE( node.is_expr() );
  EXPECT_FALSE( node.is_cell() );
  EXPECT_FALSE( node.is_dff() );
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
  EXPECT_THROW( {node.dff_src(); }, std::invalid_argument );
  EXPECT_THROW( {node.dff_rval(); }, std::invalid_argument );
}

TEST( BfNodeTest, input)
{
  shared_ptr<ModelImpl> model{new ModelImpl};

  auto id = model->new_node();
  model->set_input(id);

  BfNode node{model, id};

  EXPECT_TRUE( node.is_valid() );
  EXPECT_EQ( BfNodeType::Input, node.type() );
  EXPECT_TRUE( node.is_input() );
  EXPECT_FALSE( node.is_logic() );
  EXPECT_FALSE( node.is_primitive() );
  EXPECT_FALSE( node.is_aig() );
  EXPECT_FALSE( node.is_cover() );
  EXPECT_FALSE( node.is_expr() );
  EXPECT_FALSE( node.is_cell() );
  EXPECT_FALSE( node.is_dff() );
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
  EXPECT_THROW( {node.dff_src(); }, std::invalid_argument );
  EXPECT_THROW( {node.dff_rval(); }, std::invalid_argument );
}

TEST( BfNodeTest, primitive)
{
  shared_ptr<ModelImpl> model{new ModelImpl};

  auto id1 = model->new_node();
  model->set_input(id1);

  auto id2 = model->new_node();
  model->set_input(id2);

  auto id3 = model->new_node();
  vector<SizeType> fanin_list{id1, id2};
  PrimType type = PrimType::And;
  model->set_primitive(id3, fanin_list, type);

  BfNode node{model, id3};

  EXPECT_EQ( BfNodeType::Prim, node.type() );
  EXPECT_FALSE( node.is_input() );
  EXPECT_TRUE( node.is_logic() );
  EXPECT_TRUE( node.is_primitive() );
  EXPECT_FALSE( node.is_aig() );
  EXPECT_FALSE( node.is_cover() );
  EXPECT_FALSE( node.is_expr() );
  EXPECT_FALSE( node.is_cell() );
  EXPECT_FALSE( node.is_dff() );
  EXPECT_EQ( fanin_list.size(), node.fanin_num() );
  for ( SizeType i = 0; i < fanin_list.size(); ++ i ) {
    BfNode node1{model, fanin_list[i]};
    EXPECT_EQ( node1, node.fanin(i) );
  }
  vector<BfNode> fanin_node_list;
  for ( SizeType i = 0; i < fanin_list.size(); ++ i ) {
    fanin_node_list.push_back(BfNode{model, fanin_list[i]});
  }
  EXPECT_EQ( fanin_node_list, node.fanin_list() );
  EXPECT_EQ( type, node.primitive_type() );
  EXPECT_THROW( {node.fanin_inv(0);}, std::invalid_argument );
  EXPECT_THROW( {node.fanin_inv(1);}, std::invalid_argument );
  EXPECT_THROW( {node.cover_id();}, std::invalid_argument );
  EXPECT_THROW( {node.expr_id();}, std::invalid_argument );
  EXPECT_THROW( {node.cell_id();}, std::invalid_argument );
  EXPECT_THROW( {node.dff_src();}, std::invalid_argument );
  EXPECT_THROW( {node.dff_rval();}, std::invalid_argument );
}

TEST( BfNodeTest, aig )
{
  shared_ptr<ModelImpl> model{new ModelImpl};

  auto id1 = model->new_node();
  model->set_input(id1);

  auto id2 = model->new_node();
  model->set_input(id2);

  auto id3 = model->new_node();
  bool inv0 = false;
  bool inv1 = true;
  model->set_aig(id3, id1, id2, inv0, inv1);

  BfNode node{model, id3};

  EXPECT_EQ( BfNodeType::Aig, node.type() );
  EXPECT_FALSE( node.is_input() );
  EXPECT_TRUE( node.is_logic() );
  EXPECT_FALSE( node.is_primitive() );
  EXPECT_TRUE( node.is_aig() );
  EXPECT_FALSE( node.is_cover() );
  EXPECT_FALSE( node.is_expr() );
  EXPECT_FALSE( node.is_cell() );
  EXPECT_FALSE( node.is_dff() );
  EXPECT_EQ( 2, node.fanin_num() );
  BfNode src0{model, id1};
  EXPECT_EQ( src0, node.fanin(0) );
  BfNode src1{model, id2};
  EXPECT_EQ( src1, node.fanin(1) );
  vector<BfNode> fanin_node_list{src0, src1};
  EXPECT_EQ( fanin_node_list, node.fanin_list() );
  EXPECT_THROW( {node.primitive_type();}, std::invalid_argument );
  EXPECT_EQ( inv0, node.fanin_inv(0) );
  EXPECT_EQ( inv1, node.fanin_inv(1) );
  EXPECT_THROW( {node.cover_id();}, std::invalid_argument );
  EXPECT_THROW( {node.expr_id();}, std::invalid_argument );
  EXPECT_THROW( {node.cell_id();}, std::invalid_argument );
  EXPECT_THROW( {node.dff_src();}, std::invalid_argument );
  EXPECT_THROW( {node.dff_rval();}, std::invalid_argument );
}

TEST( BfNodeTest, cover )
{
  shared_ptr<ModelImpl> model{new ModelImpl};

  auto id1 = model->new_node();
  model->set_input(id1);

  auto id2 = model->new_node();
  model->set_input(id2);

  auto id3 = model->new_node();
  vector<SizeType> fanin_list{id1, id2};
  SizeType cover_id = 3;
  model->set_cover(id3, fanin_list, cover_id);

  BfNode node{model, id3};

  EXPECT_EQ( BfNodeType::Cover, node.type() );
  EXPECT_FALSE( node.is_input() );
  EXPECT_TRUE( node.is_logic() );
  EXPECT_FALSE( node.is_primitive() );
  EXPECT_FALSE( node.is_aig() );
  EXPECT_TRUE( node.is_cover() );
  EXPECT_FALSE( node.is_expr() );
  EXPECT_FALSE( node.is_cell() );
  EXPECT_FALSE( node.is_dff() );
  EXPECT_EQ( fanin_list.size(), node.fanin_num() );
  vector<BfNode> fanin_node_list(fanin_list.size());
  for ( SizeType i = 0; i < fanin_list.size(); ++ i ) {
    BfNode node1{model, fanin_list[i]};
    fanin_node_list[i] = node1;
    EXPECT_EQ( node1, node.fanin(i) );
  }
  EXPECT_EQ( fanin_node_list, node.fanin_list() );
  EXPECT_THROW( {node.primitive_type();}, std::invalid_argument );
  EXPECT_THROW( {node.fanin_inv(0);}, std::invalid_argument );
  EXPECT_THROW( {node.fanin_inv(1);}, std::invalid_argument );
  EXPECT_EQ( cover_id, node.cover_id() );
  EXPECT_THROW( {node.expr_id();}, std::invalid_argument );
  EXPECT_THROW( {node.cell_id();}, std::invalid_argument );
  EXPECT_THROW( {node.dff_src();}, std::invalid_argument );
  EXPECT_THROW( {node.dff_rval();}, std::invalid_argument );
}

TEST( BfNodeTest, expr )
{
  shared_ptr<ModelImpl> model{new ModelImpl};

  auto id1 = model->new_node();
  model->set_input(id1);

  auto id2 = model->new_node();
  model->set_input(id2);

  auto id3 = model->new_node();
  vector<SizeType> fanin_list{id1, id2};
  SizeType expr_id = 1;
  model->set_expr(id3, fanin_list, expr_id);

  BfNode node{model, id3};

  EXPECT_EQ( BfNodeType::Expr, node.type() );
  EXPECT_FALSE( node.is_input() );
  EXPECT_TRUE( node.is_logic() );
  EXPECT_FALSE( node.is_primitive() );
  EXPECT_FALSE( node.is_aig() );
  EXPECT_FALSE( node.is_cover() );
  EXPECT_TRUE( node.is_expr() );
  EXPECT_FALSE( node.is_cell() );
  EXPECT_FALSE( node.is_dff() );
  EXPECT_EQ( fanin_list.size(), node.fanin_num() );
  vector<BfNode> fanin_node_list(fanin_list.size());
  for ( SizeType i = 0; i < fanin_list.size(); ++ i ) {
    BfNode node1{model, fanin_list[i]};
    fanin_node_list[i] = node1;
    EXPECT_EQ( node1, node.fanin(i) );
  }
  EXPECT_EQ( fanin_node_list, node.fanin_list() );
  EXPECT_THROW( {node.primitive_type();}, std::invalid_argument );
  EXPECT_THROW( {node.fanin_inv(0);}, std::invalid_argument );
  EXPECT_THROW( {node.fanin_inv(1);}, std::invalid_argument );
  EXPECT_THROW( {node.cover_id();}, std::invalid_argument );
  EXPECT_EQ( expr_id, node.expr_id() );
  EXPECT_THROW( {node.cell_id();}, std::invalid_argument );
  EXPECT_THROW( {node.dff_src();}, std::invalid_argument );
  EXPECT_THROW( {node.dff_rval();}, std::invalid_argument );
}

TEST( BfNodeTest, cell )
{
  shared_ptr<ModelImpl> model{new ModelImpl};

  auto id1 = model->new_node();
  model->set_input(id1);

  auto id2 = model->new_node();
  model->set_input(id2);

  auto id3 = model->new_node();
  vector<SizeType> fanin_list{id1, id2};
  SizeType cell_id = 5;
  model->set_cell(id3, fanin_list, cell_id);

  BfNode node{model, id3};

  EXPECT_EQ( BfNodeType::Cell, node.type() );
  EXPECT_FALSE( node.is_input() );
  EXPECT_TRUE( node.is_logic() );
  EXPECT_FALSE( node.is_primitive() );
  EXPECT_FALSE( node.is_aig() );
  EXPECT_FALSE( node.is_cover() );
  EXPECT_FALSE( node.is_expr() );
  EXPECT_TRUE( node.is_cell() );
  EXPECT_FALSE( node.is_dff() );
  EXPECT_EQ( fanin_list.size(), node.fanin_num() );
  vector<BfNode> fanin_node_list(fanin_list.size());
  for ( SizeType i = 0; i < fanin_list.size(); ++ i ) {
    BfNode node1{model, fanin_list[i]};
    fanin_node_list[i] = node1;
    EXPECT_EQ( node1, node.fanin(i) );
  }
  EXPECT_EQ( fanin_node_list, node.fanin_list() );
  EXPECT_THROW( {node.primitive_type();}, std::invalid_argument );
  EXPECT_THROW( {node.fanin_inv(0);}, std::invalid_argument );
  EXPECT_THROW( {node.fanin_inv(1);}, std::invalid_argument );
  EXPECT_THROW( {node.cover_id();}, std::invalid_argument );
  EXPECT_THROW( {node.expr_id();}, std::invalid_argument );
  EXPECT_EQ( cell_id, node.cell_id() );
  EXPECT_THROW( {node.dff_src();}, std::invalid_argument );
  EXPECT_THROW( {node.dff_rval();}, std::invalid_argument );
}

TEST( BfNodeTest, dff )
{
  shared_ptr<ModelImpl> model{new ModelImpl};

  auto id1 = model->new_node();
  model->set_input(id1);

  auto id3 = model->new_node();
  char rval = '1';
  model->set_dff(id3, id1, rval);

  BfNode node{model, id3};

  EXPECT_EQ( BfNodeType::Dff, node.type() );
  EXPECT_FALSE( node.is_input() );
  EXPECT_FALSE( node.is_logic() );
  EXPECT_FALSE( node.is_primitive() );
  EXPECT_FALSE( node.is_aig() );
  EXPECT_FALSE( node.is_cover() );
  EXPECT_FALSE( node.is_expr() );
  EXPECT_FALSE( node.is_cell() );
  EXPECT_TRUE( node.is_dff() );
  EXPECT_THROW( {node.fanin_num();}, std::invalid_argument );
  EXPECT_THROW( {node.fanin_list();}, std::invalid_argument );
  EXPECT_THROW( {node.primitive_type();}, std::invalid_argument );
  EXPECT_THROW( {node.fanin_inv(0);}, std::invalid_argument );
  EXPECT_THROW( {node.fanin_inv(1);}, std::invalid_argument );
  EXPECT_THROW( {node.cover_id();}, std::invalid_argument );
  EXPECT_THROW( {node.expr_id();}, std::invalid_argument );
  EXPECT_THROW( {node.cell_id();}, std::invalid_argument );
  BfNode src{model, id1};
  EXPECT_EQ( src, node.dff_src() );
  EXPECT_EQ( rval, node.dff_rval() );
}

END_NAMESPACE_YM_BNFE

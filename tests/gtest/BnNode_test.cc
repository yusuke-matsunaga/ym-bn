
/// @file BnNode_test.cc
/// @brief BnNode_test の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include <gtest/gtest.h>
#include "ym/BnNode.h"
#include "ym/BnModel.h"
#include "ym/BnSeq.h"
#include "ModelImpl.h"


BEGIN_NAMESPACE_YM_BN

TEST(BnNodeTest, constructor1)
{
  BnNode node;

  EXPECT_FALSE( node.is_valid() );
  EXPECT_THROW( {node.parent_model(); }, std::invalid_argument );
  EXPECT_THROW( {node.type(); }, std::invalid_argument );
  EXPECT_THROW( {node.is_input(); }, std::invalid_argument );
  EXPECT_THROW( {node.is_seq_output(); }, std::invalid_argument );
  EXPECT_THROW( {node.is_logic(); }, std::invalid_argument );
  EXPECT_THROW( {node.is_primitive(); }, std::invalid_argument );
  EXPECT_THROW( {node.is_aig(); }, std::invalid_argument );
  EXPECT_THROW( {node.is_cover(); }, std::invalid_argument );
  EXPECT_THROW( {node.is_expr(); }, std::invalid_argument );
  EXPECT_THROW( {node.is_cell(); }, std::invalid_argument );
  EXPECT_THROW( {node.is_func(); }, std::invalid_argument );
  EXPECT_THROW( {node.is_bdd(); }, std::invalid_argument );
  EXPECT_THROW( {node.input_id(); }, std::invalid_argument );
  EXPECT_THROW( {node.seq_node(); }, std::invalid_argument );
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

  auto id = model->new_node();

  BnNode node{model, id};

  EXPECT_TRUE( node.is_valid() );
  EXPECT_EQ( BnNodeType::NONE, node.type() );
  EXPECT_FALSE( node.is_input() );
  EXPECT_FALSE( node.is_seq_output() );
  EXPECT_FALSE( node.is_logic() );
  EXPECT_FALSE( node.is_primitive() );
  EXPECT_FALSE( node.is_aig() );
  EXPECT_FALSE( node.is_cover() );
  EXPECT_FALSE( node.is_expr() );
  EXPECT_FALSE( node.is_cell() );
  EXPECT_FALSE( node.is_func() );
  EXPECT_FALSE( node.is_bdd() );
  EXPECT_THROW( {node.input_id(); }, std::invalid_argument );
  EXPECT_THROW( {node.seq_node(); }, std::invalid_argument );
  EXPECT_EQ( 0, node.fanin_num() );
  EXPECT_THROW( {node.fanin(0); }, std::invalid_argument );
  EXPECT_EQ( vector<BnNode>{}, node.fanin_list() );
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

TEST( BnNodeTest, set_input)
{
  shared_ptr<ModelImpl> model{new ModelImpl};

  auto id = model->new_node();
  model->set_input(id);

  BnNode node{model, id};

  EXPECT_TRUE( node.is_valid() );
  EXPECT_EQ( BnNodeType::INPUT, node.type() );
  EXPECT_TRUE( node.is_input() );
  EXPECT_FALSE( node.is_seq_output() );
  EXPECT_FALSE( node.is_logic() );
  EXPECT_FALSE( node.is_primitive() );
  EXPECT_FALSE( node.is_aig() );
  EXPECT_FALSE( node.is_cover() );
  EXPECT_FALSE( node.is_expr() );
  EXPECT_FALSE( node.is_cell() );
  EXPECT_FALSE( node.is_func() );
  EXPECT_FALSE( node.is_bdd() );
  EXPECT_EQ( 0, node.input_id() );
  EXPECT_THROW( {node.seq_node(); }, std::invalid_argument );
  EXPECT_EQ( 0, node.fanin_num());
  EXPECT_THROW( {node.fanin(0); }, std::invalid_argument );
  EXPECT_EQ( vector<BnNode>{}, node.fanin_list() );
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

END_NAMESPACE_YM_BN

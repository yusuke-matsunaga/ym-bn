
/// @file NodeImpl_test.cc
/// @brief NodeImpl_test の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include <gtest/gtest.h>
#include "NodeImpl.h"
#include "NodeImpl_sub.h"
#include "ym/ClibCellLibrary.h"
#include "ym/ClibCell.h"


BEGIN_NAMESPACE_YM_BN

TEST( NodeImplTest, input )
{

  SizeType iid = 10;
  NodeImpl_Input input{iid};
  NodeImpl* node = &input;

  EXPECT_EQ( BnNodeType::INPUT, node->type() );
  EXPECT_TRUE( node->is_input() );
  EXPECT_FALSE( node->is_seq_output() );
  EXPECT_FALSE( node->is_logic() );
  EXPECT_FALSE( node->is_primitive() );
  EXPECT_FALSE( node->is_aig() );
  EXPECT_FALSE( node->is_func() );
  EXPECT_FALSE( node->is_cell() );
  EXPECT_EQ( iid, node->input_id() );
  EXPECT_THROW( {node->seq_id();}, std::invalid_argument );
  EXPECT_EQ( 0, node->fanin_num() );
  EXPECT_EQ( vector<SizeType>{}, node->fanin_list() );
  EXPECT_THROW( {node->primitive_type();}, std::invalid_argument );
  EXPECT_THROW( {node->fanin_inv(0);}, std::invalid_argument );
  EXPECT_THROW( {node->fanin_inv(1);}, std::invalid_argument );
  EXPECT_THROW( {node->local_func_id();}, std::invalid_argument );
  EXPECT_THROW( {node->cell();}, std::invalid_argument );
}

TEST( NodeImplTest, seq_output )
{
  SizeType seq_id = 10;
  NodeImpl_SeqOutput seq_output(seq_id);
  NodeImpl* node = &seq_output;

  EXPECT_EQ( BnNodeType::SEQ_OUTPUT, node->type() );
  EXPECT_FALSE( node->is_input() );
  EXPECT_TRUE( node->is_seq_output() );
  EXPECT_FALSE( node->is_logic() );
  EXPECT_FALSE( node->is_primitive() );
  EXPECT_FALSE( node->is_aig() );
  EXPECT_FALSE( node->is_func() );
  EXPECT_FALSE( node->is_cell() );
  EXPECT_THROW( {node->input_id();}, std::invalid_argument );
  EXPECT_EQ( seq_id, node->seq_id() );
  EXPECT_EQ( 0, node->fanin_num() );
  EXPECT_EQ( vector<SizeType>{}, node->fanin_list() );
  EXPECT_THROW( {node->primitive_type();}, std::invalid_argument );
  EXPECT_THROW( {node->fanin_inv(0);}, std::invalid_argument );
  EXPECT_THROW( {node->fanin_inv(1);}, std::invalid_argument );
  EXPECT_THROW( {node->local_func_id();}, std::invalid_argument );
  EXPECT_THROW( {node->cell();}, std::invalid_argument );
}

TEST( NodeImplTest, primitive )
{
  vector<SizeType> fanin_list{0, 1, 4};
  PrimType type = PrimType::Nand;
  NodeImpl_Primitive primitive{type, fanin_list};
  NodeImpl* node = &primitive;

  EXPECT_EQ( BnNodeType::PRIMITIVE, node->type() );
  EXPECT_FALSE( node->is_input() );
  EXPECT_FALSE( node->is_seq_output() );
  EXPECT_TRUE( node->is_logic() );
  EXPECT_TRUE( node->is_primitive() );
  EXPECT_FALSE( node->is_aig() );
  EXPECT_FALSE( node->is_func() );
  EXPECT_FALSE( node->is_cell() );
  EXPECT_THROW( {node->input_id();}, std::invalid_argument );
  EXPECT_THROW( {node->seq_id();}, std::invalid_argument );
  EXPECT_EQ( fanin_list.size(), node->fanin_num() );
  for ( SizeType i = 0; i < fanin_list.size(); ++ i ) {
    EXPECT_EQ( fanin_list[i], node->fanin(i) );
  }
  EXPECT_EQ( fanin_list, node->fanin_list() );
  EXPECT_EQ( type, node->primitive_type() );
  EXPECT_THROW( {node->fanin_inv(0);}, std::invalid_argument );
  EXPECT_THROW( {node->fanin_inv(1);}, std::invalid_argument );
  EXPECT_THROW( {node->local_func_id();}, std::invalid_argument );
  EXPECT_THROW( {node->cell();}, std::invalid_argument );
}

TEST( NodeImplTest, aig )
{
  SizeType src0 = 2;
  SizeType src1 = 10;
  bool inv0 = false;
  bool inv1 = true;
  NodeImpl_Aig aig{src0, inv0, src1, inv1};
  NodeImpl* node = &aig;

  EXPECT_EQ( BnNodeType::AIG, node->type() );
  EXPECT_FALSE( node->is_input() );
  EXPECT_FALSE( node->is_seq_output() );
  EXPECT_TRUE( node->is_logic() );
  EXPECT_FALSE( node->is_primitive() );
  EXPECT_TRUE( node->is_aig() );
  EXPECT_FALSE( node->is_func() );
  EXPECT_FALSE( node->is_cell() );
  EXPECT_THROW( {node->input_id();}, std::invalid_argument );
  EXPECT_THROW( {node->seq_id();}, std::invalid_argument );
  EXPECT_EQ( 2, node->fanin_num() );
  EXPECT_EQ( src0, node->fanin(0) );
  EXPECT_EQ( src1, node->fanin(1) );
  vector<SizeType> fanin_list{src0, src1};
  EXPECT_EQ( fanin_list, node->fanin_list() );
  EXPECT_THROW( {node->primitive_type();}, std::invalid_argument );
  EXPECT_EQ( inv0, node->fanin_inv(0) );
  EXPECT_EQ( inv1, node->fanin_inv(1) );
  EXPECT_THROW( {node->local_func_id();}, std::invalid_argument );
  EXPECT_THROW( {node->cell();}, std::invalid_argument );
}

TEST( NodeImplTest, func )
{
  vector<SizeType> fanin_list{2, 6, 8, 10};
  SizeType func_id = 9;
  NodeImpl_Func func{func_id, fanin_list};
  NodeImpl* node = &func;

  EXPECT_EQ( BnNodeType::FUNC, node->type() );
  EXPECT_FALSE( node->is_input() );
  EXPECT_FALSE( node->is_seq_output() );
  EXPECT_TRUE( node->is_logic() );
  EXPECT_FALSE( node->is_primitive() );
  EXPECT_FALSE( node->is_aig() );
  EXPECT_TRUE( node->is_func() );
  EXPECT_FALSE( node->is_cell() );
  EXPECT_THROW( {node->input_id();}, std::invalid_argument );
  EXPECT_THROW( {node->seq_id();}, std::invalid_argument );
  EXPECT_EQ( fanin_list.size(), node->fanin_num() );
  for ( SizeType i = 0; i < fanin_list.size(); ++ i ) {
    EXPECT_EQ( fanin_list[i], node->fanin(i) );
  }
  EXPECT_EQ( fanin_list, node->fanin_list() );
  EXPECT_THROW( {node->primitive_type();}, std::invalid_argument );
  EXPECT_THROW( {node->fanin_inv(0);}, std::invalid_argument );
  EXPECT_THROW( {node->fanin_inv(1);}, std::invalid_argument );
  EXPECT_EQ( func_id, node->local_func_id() );
  EXPECT_THROW( {node->cell();}, std::invalid_argument );
}

TEST( NodeImplTest, cell )
{
  vector<SizeType> fanin_list{2, 6, 8, 10};
  ClibCell cell;
  NodeImpl_Cell node_cell{cell, fanin_list};
  NodeImpl* node = &node_cell;

  EXPECT_EQ( BnNodeType::CELL, node->type() );
  EXPECT_FALSE( node->is_input() );
  EXPECT_FALSE( node->is_seq_output() );
  EXPECT_TRUE( node->is_logic() );
  EXPECT_FALSE( node->is_primitive() );
  EXPECT_FALSE( node->is_aig() );
  EXPECT_FALSE( node->is_func() );
  EXPECT_TRUE( node->is_cell() );
  EXPECT_THROW( {node->input_id();}, std::invalid_argument );
  EXPECT_THROW( {node->seq_id();}, std::invalid_argument );
  EXPECT_EQ( fanin_list.size(), node->fanin_num() );
  for ( SizeType i = 0; i < fanin_list.size(); ++ i ) {
    EXPECT_EQ( fanin_list[i], node->fanin(i) );
  }
  EXPECT_EQ( fanin_list, node->fanin_list() );
  EXPECT_THROW( {node->primitive_type();}, std::invalid_argument );
  EXPECT_THROW( {node->fanin_inv(0);}, std::invalid_argument );
  EXPECT_THROW( {node->fanin_inv(1);}, std::invalid_argument );
  EXPECT_THROW( {node->local_func_id();}, std::invalid_argument );
  EXPECT_EQ( cell, node->cell() );
}

END_NAMESPACE_YM_BN

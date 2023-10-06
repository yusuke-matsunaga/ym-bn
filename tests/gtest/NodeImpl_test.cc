
/// @file NodeImpl_test.cc
/// @brief NodeImpl_test の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include <gtest/gtest.h>
#include "NodeImpl.h"


BEGIN_NAMESPACE_YM_BN

TEST( NodeImplTest, constructor1 )
{
  string name{"abcd"};
  NodeImpl node{name};

  EXPECT_EQ( BnNodeType::None, node.type() );
  EXPECT_FALSE( node.is_input() );
  EXPECT_FALSE( node.is_logic() );
  EXPECT_FALSE( node.is_primitive() );
  EXPECT_FALSE( node.is_aig() );
  EXPECT_FALSE( node.is_cover() );
  EXPECT_FALSE( node.is_expr() );
  EXPECT_FALSE( node.is_cell() );
  EXPECT_FALSE( node.is_dff() );
  EXPECT_FALSE( node.is_latch() );
  EXPECT_EQ( name, node.name() );
  EXPECT_THROW( {node.input_id();}, std::invalid_argument );
  EXPECT_THROW( {node.fanin_num();}, std::invalid_argument );
  EXPECT_THROW( {node.fanin_list();}, std::invalid_argument );
  EXPECT_THROW( {node.primitive_type();}, std::invalid_argument );
  EXPECT_THROW( {node.fanin_inv(0);}, std::invalid_argument );
  EXPECT_THROW( {node.fanin_inv(1);}, std::invalid_argument );
  EXPECT_THROW( {node.cover_id();}, std::invalid_argument );
  EXPECT_THROW( {node.expr_id();}, std::invalid_argument );
  EXPECT_THROW( {node.cell_id();}, std::invalid_argument );
  EXPECT_THROW( {node.dff_src();}, std::invalid_argument );
  EXPECT_THROW( {node.dff_rsval();}, std::invalid_argument );
}

TEST( NodeImplTest, set_name )
{
  string name{"xyz"};
  NodeImpl node{""};

  node.set_name(name);

  EXPECT_EQ( name, node.name() );
}

TEST( NodeImplTest, set_input )
{
  NodeImpl node{""};

  SizeType iid = 10;
  node.set_input(iid);

  EXPECT_EQ( BnNodeType::Input, node.type() );
  EXPECT_TRUE( node.is_input() );
  EXPECT_FALSE( node.is_logic() );
  EXPECT_FALSE( node.is_primitive() );
  EXPECT_FALSE( node.is_aig() );
  EXPECT_FALSE( node.is_cover() );
  EXPECT_FALSE( node.is_expr() );
  EXPECT_FALSE( node.is_cell() );
  EXPECT_FALSE( node.is_dff() );
  EXPECT_FALSE( node.is_latch() );
  EXPECT_EQ( iid, node.input_id() );
  EXPECT_THROW( {node.fanin_num();}, std::invalid_argument );
  EXPECT_THROW( {node.fanin_list();}, std::invalid_argument );
  EXPECT_THROW( {node.primitive_type();}, std::invalid_argument );
  EXPECT_THROW( {node.fanin_inv(0);}, std::invalid_argument );
  EXPECT_THROW( {node.fanin_inv(1);}, std::invalid_argument );
  EXPECT_THROW( {node.cover_id();}, std::invalid_argument );
  EXPECT_THROW( {node.expr_id();}, std::invalid_argument );
  EXPECT_THROW( {node.cell_id();}, std::invalid_argument );
  EXPECT_THROW( {node.dff_src();}, std::invalid_argument );
  EXPECT_THROW( {node.dff_rsval();}, std::invalid_argument );
}

TEST( NodeImplTest, set_primitive )
{
  NodeImpl node{""};

  vector<SizeType> fanin_list{0, 1, 4};
  PrimType type = PrimType::Nand;
  node.set_primitive(fanin_list, type);

  EXPECT_EQ( BnNodeType::Prim, node.type() );
  EXPECT_FALSE( node.is_input() );
  EXPECT_TRUE( node.is_logic() );
  EXPECT_TRUE( node.is_primitive() );
  EXPECT_FALSE( node.is_aig() );
  EXPECT_FALSE( node.is_cover() );
  EXPECT_FALSE( node.is_expr() );
  EXPECT_FALSE( node.is_cell() );
  EXPECT_FALSE( node.is_dff() );
  EXPECT_FALSE( node.is_latch() );
  EXPECT_THROW( {node.input_id();}, std::invalid_argument );
  EXPECT_EQ( fanin_list.size(), node.fanin_num() );
  for ( SizeType i = 0; i < fanin_list.size(); ++ i ) {
    EXPECT_EQ( fanin_list[i], node.fanin(i) );
  }
  EXPECT_EQ( fanin_list, node.fanin_list() );
  EXPECT_EQ( type, node.primitive_type() );
  EXPECT_THROW( {node.fanin_inv(0);}, std::invalid_argument );
  EXPECT_THROW( {node.fanin_inv(1);}, std::invalid_argument );
  EXPECT_THROW( {node.cover_id();}, std::invalid_argument );
  EXPECT_THROW( {node.expr_id();}, std::invalid_argument );
  EXPECT_THROW( {node.cell_id();}, std::invalid_argument );
  EXPECT_THROW( {node.dff_src();}, std::invalid_argument );
  EXPECT_THROW( {node.dff_rsval();}, std::invalid_argument );
}

TEST( NodeImplTest, set_aig )
{
  NodeImpl node{""};

  SizeType src0 = 2;
  SizeType src1 = 10;
  bool inv0 = false;
  bool inv1 = true;
  node.set_aig(src0, src1, inv0, inv1);

  EXPECT_EQ( BnNodeType::Aig, node.type() );
  EXPECT_FALSE( node.is_input() );
  EXPECT_TRUE( node.is_logic() );
  EXPECT_FALSE( node.is_primitive() );
  EXPECT_TRUE( node.is_aig() );
  EXPECT_FALSE( node.is_cover() );
  EXPECT_FALSE( node.is_expr() );
  EXPECT_FALSE( node.is_cell() );
  EXPECT_FALSE( node.is_dff() );
  EXPECT_FALSE( node.is_latch() );
  EXPECT_THROW( {node.input_id();}, std::invalid_argument );
  EXPECT_EQ( 2, node.fanin_num() );
  EXPECT_EQ( src0, node.fanin(0) );
  EXPECT_EQ( src1, node.fanin(1) );
  vector<SizeType> fanin_list{src0, src1};
  EXPECT_EQ( fanin_list, node.fanin_list() );
  EXPECT_THROW( {node.primitive_type();}, std::invalid_argument );
  EXPECT_EQ( inv0, node.fanin_inv(0) );
  EXPECT_EQ( inv1, node.fanin_inv(1) );
  EXPECT_THROW( {node.cover_id();}, std::invalid_argument );
  EXPECT_THROW( {node.expr_id();}, std::invalid_argument );
  EXPECT_THROW( {node.cell_id();}, std::invalid_argument );
  EXPECT_THROW( {node.dff_src();}, std::invalid_argument );
  EXPECT_THROW( {node.dff_rsval();}, std::invalid_argument );
}

TEST( NodeImplTest, set_cover )
{
  NodeImpl node{""};

  vector<SizeType> fanin_list{3, 5};
  SizeType cover_id = 15;
  node.set_cover(fanin_list, cover_id);

  EXPECT_EQ( BnNodeType::Cover, node.type() );
  EXPECT_FALSE( node.is_input() );
  EXPECT_TRUE( node.is_logic() );
  EXPECT_FALSE( node.is_primitive() );
  EXPECT_FALSE( node.is_aig() );
  EXPECT_TRUE( node.is_cover() );
  EXPECT_FALSE( node.is_expr() );
  EXPECT_FALSE( node.is_cell() );
  EXPECT_FALSE( node.is_dff() );
  EXPECT_FALSE( node.is_latch() );
  EXPECT_THROW( {node.input_id();}, std::invalid_argument );
  EXPECT_EQ( fanin_list.size(), node.fanin_num() );
  for ( SizeType i = 0; i < fanin_list.size(); ++ i ) {
    EXPECT_EQ( fanin_list[i], node.fanin(i) );
  }
  EXPECT_EQ( fanin_list, node.fanin_list() );
  EXPECT_THROW( {node.primitive_type();}, std::invalid_argument );
  EXPECT_THROW( {node.fanin_inv(0);}, std::invalid_argument );
  EXPECT_THROW( {node.fanin_inv(1);}, std::invalid_argument );
  EXPECT_EQ( cover_id, node.cover_id() );
  EXPECT_THROW( {node.expr_id();}, std::invalid_argument );
  EXPECT_THROW( {node.cell_id();}, std::invalid_argument );
  EXPECT_THROW( {node.dff_src();}, std::invalid_argument );
  EXPECT_THROW( {node.dff_rsval();}, std::invalid_argument );
}

TEST( NodeImplTest, set_expr )
{
  NodeImpl node{""};

  vector<SizeType> fanin_list{2, 6, 8, 10};
  SizeType expr_id = 9;
  node.set_expr(fanin_list, expr_id);

  EXPECT_EQ( BnNodeType::Expr, node.type() );
  EXPECT_FALSE( node.is_input() );
  EXPECT_TRUE( node.is_logic() );
  EXPECT_FALSE( node.is_primitive() );
  EXPECT_FALSE( node.is_aig() );
  EXPECT_FALSE( node.is_cover() );
  EXPECT_TRUE( node.is_expr() );
  EXPECT_FALSE( node.is_cell() );
  EXPECT_FALSE( node.is_dff() );
  EXPECT_FALSE( node.is_latch() );
  EXPECT_THROW( {node.input_id();}, std::invalid_argument );
  EXPECT_EQ( fanin_list.size(), node.fanin_num() );
  for ( SizeType i = 0; i < fanin_list.size(); ++ i ) {
    EXPECT_EQ( fanin_list[i], node.fanin(i) );
  }
  EXPECT_EQ( fanin_list, node.fanin_list() );
  EXPECT_THROW( {node.primitive_type();}, std::invalid_argument );
  EXPECT_THROW( {node.fanin_inv(0);}, std::invalid_argument );
  EXPECT_THROW( {node.fanin_inv(1);}, std::invalid_argument );
  EXPECT_THROW( {node.cover_id();}, std::invalid_argument );
  EXPECT_EQ( expr_id, node.expr_id() );
  EXPECT_THROW( {node.cell_id();}, std::invalid_argument );
  EXPECT_THROW( {node.dff_src();}, std::invalid_argument );
  EXPECT_THROW( {node.dff_rsval();}, std::invalid_argument );
}

TEST( NodeImplTest, set_cell )
{
  NodeImpl node{""};

  vector<SizeType> fanin_list{2, 6, 8, 10};
  SizeType cell_id = 19;
  node.set_cell(fanin_list, cell_id);

  EXPECT_EQ( BnNodeType::Cell, node.type() );
  EXPECT_FALSE( node.is_input() );
  EXPECT_TRUE( node.is_logic() );
  EXPECT_FALSE( node.is_primitive() );
  EXPECT_FALSE( node.is_aig() );
  EXPECT_FALSE( node.is_cover() );
  EXPECT_FALSE( node.is_expr() );
  EXPECT_TRUE( node.is_cell() );
  EXPECT_FALSE( node.is_dff() );
  EXPECT_FALSE( node.is_latch() );
  EXPECT_THROW( {node.input_id();}, std::invalid_argument );
  EXPECT_EQ( fanin_list.size(), node.fanin_num() );
  for ( SizeType i = 0; i < fanin_list.size(); ++ i ) {
    EXPECT_EQ( fanin_list[i], node.fanin(i) );
  }
  EXPECT_EQ( fanin_list, node.fanin_list() );
  EXPECT_THROW( {node.primitive_type();}, std::invalid_argument );
  EXPECT_THROW( {node.fanin_inv(0);}, std::invalid_argument );
  EXPECT_THROW( {node.fanin_inv(1);}, std::invalid_argument );
  EXPECT_THROW( {node.cover_id();}, std::invalid_argument );
  EXPECT_THROW( {node.expr_id();}, std::invalid_argument );
  EXPECT_EQ( cell_id, node.cell_id() );
  EXPECT_THROW( {node.dff_src();}, std::invalid_argument );
  EXPECT_THROW( {node.dff_rsval();}, std::invalid_argument );
}

TEST( NodeImplTest, set_dff )
{
  NodeImpl node{""};

  SizeType src = 99;
  SizeType clock = 100;
  SizeType reset = 101;
  SizeType preset = 102;
  char rsval = '0';
  node.set_dff(src, clock, reset, preset, rsval);

  EXPECT_EQ( BnNodeType::Dff, node.type() );
  EXPECT_FALSE( node.is_input() );
  EXPECT_FALSE( node.is_logic() );
  EXPECT_FALSE( node.is_primitive() );
  EXPECT_FALSE( node.is_aig() );
  EXPECT_FALSE( node.is_cover() );
  EXPECT_FALSE( node.is_expr() );
  EXPECT_FALSE( node.is_cell() );
  EXPECT_TRUE( node.is_dff() );
  EXPECT_FALSE( node.is_latch() );
  EXPECT_FALSE( node.is_latch() );
  EXPECT_THROW( {node.input_id();}, std::invalid_argument );
  EXPECT_THROW( {node.fanin_num();}, std::invalid_argument );
  EXPECT_THROW( {node.fanin_list();}, std::invalid_argument );
  EXPECT_THROW( {node.primitive_type();}, std::invalid_argument );
  EXPECT_THROW( {node.fanin_inv(0);}, std::invalid_argument );
  EXPECT_THROW( {node.fanin_inv(1);}, std::invalid_argument );
  EXPECT_THROW( {node.cover_id();}, std::invalid_argument );
  EXPECT_THROW( {node.expr_id();}, std::invalid_argument );
  EXPECT_THROW( {node.cell_id();}, std::invalid_argument );
  EXPECT_EQ( src, node.dff_src() );
  EXPECT_EQ( clock, node.dff_clock() );
  EXPECT_EQ( reset, node.dff_reset() );
  EXPECT_EQ( preset, node.dff_preset() );
  EXPECT_EQ( rsval, node.dff_rsval() );
}

TEST( NodeImplTest, set_latch )
{
  NodeImpl node{""};

  SizeType src = 99;
  SizeType enable = 100;
  SizeType reset = 101;
  SizeType preset = 102;
  char rsval = '0';
  node.set_latch(src, enable, reset, preset, rsval);

  EXPECT_EQ( BnNodeType::Latch, node.type() );
  EXPECT_FALSE( node.is_input() );
  EXPECT_FALSE( node.is_logic() );
  EXPECT_FALSE( node.is_primitive() );
  EXPECT_FALSE( node.is_aig() );
  EXPECT_FALSE( node.is_cover() );
  EXPECT_FALSE( node.is_expr() );
  EXPECT_FALSE( node.is_cell() );
  EXPECT_FALSE( node.is_dff() );
  EXPECT_TRUE( node.is_latch() );
  EXPECT_THROW( {node.input_id();}, std::invalid_argument );
  EXPECT_THROW( {node.fanin_num();}, std::invalid_argument );
  EXPECT_THROW( {node.fanin_list();}, std::invalid_argument );
  EXPECT_THROW( {node.primitive_type();}, std::invalid_argument );
  EXPECT_THROW( {node.fanin_inv(0);}, std::invalid_argument );
  EXPECT_THROW( {node.fanin_inv(1);}, std::invalid_argument );
  EXPECT_THROW( {node.cover_id();}, std::invalid_argument );
  EXPECT_THROW( {node.expr_id();}, std::invalid_argument );
  EXPECT_THROW( {node.cell_id();}, std::invalid_argument );
  EXPECT_EQ( src, node.latch_src() );
  EXPECT_EQ( enable, node.latch_enable() );
  EXPECT_EQ( reset, node.latch_reset() );
  EXPECT_EQ( preset, node.latch_preset() );
  EXPECT_EQ( rsval, node.latch_rsval() );
}

END_NAMESPACE_YM_BN

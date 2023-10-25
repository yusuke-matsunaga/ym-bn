
/// @file DffImpl_test.cc
/// @brief DffImpl_test の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include <gtest/gtest.h>
#include "DffImpl.h"


BEGIN_NAMESPACE_YM_BN

TEST( DffImplTest, constructor1 )
{
  string name{"abcd"};
  DffImpl node{name};

  EXPECT_EQ( BnDffType::NONE, node.type() );
  EXPECT_FALSE( node.is_dff() );
  EXPECT_FALSE( node.is_latch() );
  EXPECT_FALSE( node.is_cell() );
  EXPECT_EQ( name, node.name() );
  EXPECT_THROW( {node.data_src();}, std::invalid_argument );
  EXPECT_THROW( {node.clock();}, std::invalid_argument );
  EXPECT_THROW( {node.enable();}, std::invalid_argument );
  EXPECT_THROW( {node.clear();}, std::invalid_argument );
  EXPECT_THROW( {node.preset();}, std::invalid_argument );
  EXPECT_THROW( {node.rsval();}, std::invalid_argument );
  EXPECT_THROW( {node.data_output();}, std::invalid_argument );
  EXPECT_THROW( {node.cell_id();}, std::invalid_argument );
}

TEST( DffImplTest, set_name )
{
  string name{"xyz"};
  DffImpl node{""};

  node.set_name(name);

  EXPECT_EQ( name, node.name() );
}

TEST( DffImplTest, set_dff )
{
  DffImpl node{""};

  char rsval = '0';
  SizeType oid = 10;
  node.set_dff(rsval, oid);

  EXPECT_EQ( BnDffType::DFF, node.type() );
  EXPECT_TRUE( node.is_dff() );
  EXPECT_FALSE( node.is_latch() );
  EXPECT_FALSE( node.is_cell() );
  EXPECT_EQ( string{}, node.name() );
  EXPECT_EQ( BAD_ID, node.data_src() );
  EXPECT_EQ( BAD_ID, node.clock() );
  EXPECT_THROW( {node.enable();}, std::invalid_argument );
  EXPECT_EQ( BAD_ID, node.clear() );
  EXPECT_EQ( BAD_ID, node.preset() );
  EXPECT_EQ( rsval, node.rsval() );
  EXPECT_EQ( oid, node.data_output() );
  EXPECT_THROW( {node.cell_id();}, std::invalid_argument );
}

TEST( DffImplTest, set_latch )
{
  DffImpl node{""};

  char rsval = '1';
  SizeType oid = 10;
  node.set_latch(rsval, oid);

  EXPECT_EQ( BnDffType::LATCH, node.type() );
  EXPECT_FALSE( node.is_dff() );
  EXPECT_TRUE( node.is_latch() );
  EXPECT_FALSE( node.is_cell() );
  EXPECT_EQ( string{}, node.name() );
  EXPECT_EQ( BAD_ID, node.data_src() );
  EXPECT_THROW( {node.clock();}, std::invalid_argument );
  EXPECT_EQ( BAD_ID, node.enable() );
  EXPECT_EQ( BAD_ID, node.clear() );
  EXPECT_EQ( BAD_ID, node.preset() );
  EXPECT_EQ( rsval, node.rsval() );
  EXPECT_EQ( oid, node.data_output() );
  EXPECT_THROW( {node.cell_id();}, std::invalid_argument );
}

TEST( DffImplTest, set_cell )
{
  DffImpl node{""};

  SizeType cell_id = 21;
  SizeType pin_num = 3;
  node.set_cell(cell_id, pin_num);

  EXPECT_EQ( BnDffType::CELL, node.type() );
  EXPECT_FALSE( node.is_dff() );
  EXPECT_FALSE( node.is_latch() );
  EXPECT_TRUE( node.is_cell() );
  EXPECT_EQ( string{}, node.name() );
  EXPECT_THROW( {node.data_src();}, std::invalid_argument );
  EXPECT_THROW( {node.clock();}, std::invalid_argument );
  EXPECT_THROW( {node.enable();}, std::invalid_argument );
  EXPECT_THROW( {node.clear();}, std::invalid_argument );
  EXPECT_THROW( {node.preset();}, std::invalid_argument );
  EXPECT_THROW( {node.rsval();}, std::invalid_argument );
  EXPECT_THROW( {node.data_output();}, std::invalid_argument );
  EXPECT_EQ( cell_id, node.cell_id() );
  for ( SizeType i = 0; i < pin_num; ++ i ) {
    EXPECT_EQ( BAD_ID, node.cell_pin(i) );
  }
}

TEST( DffImplTest, set_data_src )
{
  DffImpl node{""};

  char rsval = '0';
  SizeType oid = 10;
  node.set_dff(rsval, oid);
  SizeType src_id = 20;
  node.set_data_src(src_id);

  EXPECT_EQ( BnDffType::DFF, node.type() );
  EXPECT_TRUE( node.is_dff() );
  EXPECT_FALSE( node.is_latch() );
  EXPECT_FALSE( node.is_cell() );
  EXPECT_EQ( string{}, node.name() );
  EXPECT_EQ( src_id, node.data_src() );
  EXPECT_EQ( BAD_ID, node.clock() );
  EXPECT_THROW( {node.enable();}, std::invalid_argument );
  EXPECT_EQ( BAD_ID, node.clear() );
  EXPECT_EQ( BAD_ID, node.preset() );
  EXPECT_EQ( rsval, node.rsval() );
  EXPECT_EQ( oid, node.data_output() );
  EXPECT_THROW( {node.cell_id();}, std::invalid_argument );
}

TEST( DffImplTest, set_clock )
{
  DffImpl node{""};

  char rsval = '0';
  SizeType oid = 10;
  node.set_dff(rsval, oid);
  SizeType clock_id = 30;
  node.set_clock(clock_id);

  EXPECT_EQ( BnDffType::DFF, node.type() );
  EXPECT_TRUE( node.is_dff() );
  EXPECT_FALSE( node.is_latch() );
  EXPECT_FALSE( node.is_cell() );
  EXPECT_EQ( string{}, node.name() );
  EXPECT_EQ( BAD_ID, node.data_src() );
  EXPECT_EQ( clock_id, node.clock() );
  EXPECT_THROW( {node.enable();}, std::invalid_argument );
  EXPECT_EQ( BAD_ID, node.clear() );
  EXPECT_EQ( BAD_ID, node.preset() );
  EXPECT_EQ( rsval, node.rsval() );
  EXPECT_EQ( oid, node.data_output() );
  EXPECT_THROW( {node.cell_id();}, std::invalid_argument );
}

TEST( DffImplTest, set_enable )
{
  DffImpl node{""};

  char rsval = '1';
  SizeType oid = 10;
  node.set_latch(rsval, oid);
  SizeType enable_id = 11;
  node.set_enable(enable_id);

  EXPECT_EQ( BnDffType::LATCH, node.type() );
  EXPECT_FALSE( node.is_dff() );
  EXPECT_TRUE( node.is_latch() );
  EXPECT_FALSE( node.is_cell() );
  EXPECT_EQ( string{}, node.name() );
  EXPECT_EQ( BAD_ID, node.data_src() );
  EXPECT_THROW( {node.clock();}, std::invalid_argument );
  EXPECT_EQ( enable_id, node.enable() );
  EXPECT_EQ( BAD_ID, node.clear() );
  EXPECT_EQ( BAD_ID, node.preset() );
  EXPECT_EQ( rsval, node.rsval() );
  EXPECT_EQ( oid, node.data_output() );
  EXPECT_THROW( {node.cell_id();}, std::invalid_argument );
}

TEST( DffImplTest, set_clear )
{
  DffImpl node{""};

  char rsval = '0';
  SizeType oid = 10;
  node.set_dff(rsval, oid);
  SizeType clear_id = 30;
  node.set_clear(clear_id);

  EXPECT_EQ( BnDffType::DFF, node.type() );
  EXPECT_TRUE( node.is_dff() );
  EXPECT_FALSE( node.is_latch() );
  EXPECT_FALSE( node.is_cell() );
  EXPECT_EQ( string{}, node.name() );
  EXPECT_EQ( BAD_ID, node.data_src() );
  EXPECT_EQ( BAD_ID, node.clock() );
  EXPECT_THROW( {node.enable();}, std::invalid_argument );
  EXPECT_EQ( clear_id, node.clear() );
  EXPECT_EQ( BAD_ID, node.preset() );
  EXPECT_EQ( rsval, node.rsval() );
  EXPECT_EQ( oid, node.data_output() );
  EXPECT_THROW( {node.cell_id();}, std::invalid_argument );
}

TEST( DffImplTest, set_preset )
{
  DffImpl node{""};

  char rsval = '1';
  SizeType oid = 10;
  node.set_latch(rsval, oid);
  SizeType preset_id = 11;
  node.set_preset(preset_id);

  EXPECT_EQ( BnDffType::LATCH, node.type() );
  EXPECT_FALSE( node.is_dff() );
  EXPECT_TRUE( node.is_latch() );
  EXPECT_FALSE( node.is_cell() );
  EXPECT_EQ( string{}, node.name() );
  EXPECT_EQ( BAD_ID, node.data_src() );
  EXPECT_THROW( {node.clock();}, std::invalid_argument );
  EXPECT_EQ( BAD_ID, node.enable() );
  EXPECT_EQ( BAD_ID, node.clear() );
  EXPECT_EQ( preset_id, node.preset() );
  EXPECT_EQ( rsval, node.rsval() );
  EXPECT_EQ( oid, node.data_output() );
  EXPECT_THROW( {node.cell_id();}, std::invalid_argument );
}

END_NAMESPACE_YM_BN

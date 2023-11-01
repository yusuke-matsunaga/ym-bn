
/// @file SeqImpl_test.cc
/// @brief SeqImpl_test の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include <gtest/gtest.h>
#include "SeqImpl.h"


BEGIN_NAMESPACE_YM_BN

TEST( SeqImplTest, constructor1 )
{
  SeqImpl node;

  EXPECT_EQ( BnSeqType::NONE, node.type() );
  EXPECT_FALSE( node.is_dff() );
  EXPECT_FALSE( node.is_latch() );
  EXPECT_FALSE( node.is_cell() );
  EXPECT_THROW( {node.data_src();}, std::invalid_argument );
  EXPECT_THROW( {node.clock();}, std::invalid_argument );
  EXPECT_THROW( {node.enable();}, std::invalid_argument );
  EXPECT_THROW( {node.clear();}, std::invalid_argument );
  EXPECT_THROW( {node.preset();}, std::invalid_argument );
  EXPECT_THROW( {node.rsval();}, std::invalid_argument );
  EXPECT_THROW( {node.data_output();}, std::invalid_argument );
  EXPECT_THROW( {node.cell_id();}, std::invalid_argument );
}

TEST( SeqImplTest, set_dff )
{
  SeqImpl node;

  char rsval = '0';
  SizeType oid = 10;
  node.set_dff(rsval, oid);

  EXPECT_EQ( BnSeqType::DFF, node.type() );
  EXPECT_TRUE( node.is_dff() );
  EXPECT_FALSE( node.is_latch() );
  EXPECT_FALSE( node.is_cell() );
  EXPECT_EQ( BAD_ID, node.data_src() );
  EXPECT_EQ( BAD_ID, node.clock() );
  EXPECT_THROW( {node.enable();}, std::invalid_argument );
  EXPECT_EQ( BAD_ID, node.clear() );
  EXPECT_EQ( BAD_ID, node.preset() );
  EXPECT_EQ( rsval, node.rsval() );
  EXPECT_EQ( oid, node.data_output() );
  EXPECT_THROW( {node.cell_id();}, std::invalid_argument );
}

TEST( SeqImplTest, set_latch )
{
  SeqImpl node;

  char rsval = '1';
  SizeType oid = 10;
  node.set_latch(rsval, oid);

  EXPECT_EQ( BnSeqType::LATCH, node.type() );
  EXPECT_FALSE( node.is_dff() );
  EXPECT_TRUE( node.is_latch() );
  EXPECT_FALSE( node.is_cell() );
  EXPECT_EQ( BAD_ID, node.data_src() );
  EXPECT_THROW( {node.clock();}, std::invalid_argument );
  EXPECT_EQ( BAD_ID, node.enable() );
  EXPECT_EQ( BAD_ID, node.clear() );
  EXPECT_EQ( BAD_ID, node.preset() );
  EXPECT_EQ( rsval, node.rsval() );
  EXPECT_EQ( oid, node.data_output() );
  EXPECT_THROW( {node.cell_id();}, std::invalid_argument );
}

TEST( SeqImplTest, set_cell )
{
  SeqImpl node;

  SizeType cell_id = 21;
  SizeType pin_num = 3;
  node.set_cell(cell_id, pin_num);

  EXPECT_EQ( BnSeqType::CELL, node.type() );
  EXPECT_FALSE( node.is_dff() );
  EXPECT_FALSE( node.is_latch() );
  EXPECT_TRUE( node.is_cell() );
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

TEST( SeqImplTest, set_data_src )
{
  SeqImpl node;

  char rsval = '0';
  SizeType oid = 10;
  node.set_dff(rsval, oid);
  SizeType src_id = 20;
  node.set_data_src(src_id);

  EXPECT_EQ( BnSeqType::DFF, node.type() );
  EXPECT_TRUE( node.is_dff() );
  EXPECT_FALSE( node.is_latch() );
  EXPECT_FALSE( node.is_cell() );
  EXPECT_EQ( src_id, node.data_src() );
  EXPECT_EQ( BAD_ID, node.clock() );
  EXPECT_THROW( {node.enable();}, std::invalid_argument );
  EXPECT_EQ( BAD_ID, node.clear() );
  EXPECT_EQ( BAD_ID, node.preset() );
  EXPECT_EQ( rsval, node.rsval() );
  EXPECT_EQ( oid, node.data_output() );
  EXPECT_THROW( {node.cell_id();}, std::invalid_argument );
}

TEST( SeqImplTest, set_clock )
{
  SeqImpl node;

  char rsval = '0';
  SizeType oid = 10;
  node.set_dff(rsval, oid);
  SizeType clock_id = 30;
  node.set_clock(clock_id);

  EXPECT_EQ( BnSeqType::DFF, node.type() );
  EXPECT_TRUE( node.is_dff() );
  EXPECT_FALSE( node.is_latch() );
  EXPECT_FALSE( node.is_cell() );
  EXPECT_EQ( BAD_ID, node.data_src() );
  EXPECT_EQ( clock_id, node.clock() );
  EXPECT_THROW( {node.enable();}, std::invalid_argument );
  EXPECT_EQ( BAD_ID, node.clear() );
  EXPECT_EQ( BAD_ID, node.preset() );
  EXPECT_EQ( rsval, node.rsval() );
  EXPECT_EQ( oid, node.data_output() );
  EXPECT_THROW( {node.cell_id();}, std::invalid_argument );
}

TEST( SeqImplTest, set_enable )
{
  SeqImpl node;

  char rsval = '1';
  SizeType oid = 10;
  node.set_latch(rsval, oid);
  SizeType enable_id = 11;
  node.set_enable(enable_id);

  EXPECT_EQ( BnSeqType::LATCH, node.type() );
  EXPECT_FALSE( node.is_dff() );
  EXPECT_TRUE( node.is_latch() );
  EXPECT_FALSE( node.is_cell() );
  EXPECT_EQ( BAD_ID, node.data_src() );
  EXPECT_THROW( {node.clock();}, std::invalid_argument );
  EXPECT_EQ( enable_id, node.enable() );
  EXPECT_EQ( BAD_ID, node.clear() );
  EXPECT_EQ( BAD_ID, node.preset() );
  EXPECT_EQ( rsval, node.rsval() );
  EXPECT_EQ( oid, node.data_output() );
  EXPECT_THROW( {node.cell_id();}, std::invalid_argument );
}

TEST( SeqImplTest, set_clear )
{
  SeqImpl node;

  char rsval = '0';
  SizeType oid = 10;
  node.set_dff(rsval, oid);
  SizeType clear_id = 30;
  node.set_clear(clear_id);

  EXPECT_EQ( BnSeqType::DFF, node.type() );
  EXPECT_TRUE( node.is_dff() );
  EXPECT_FALSE( node.is_latch() );
  EXPECT_FALSE( node.is_cell() );
  EXPECT_EQ( BAD_ID, node.data_src() );
  EXPECT_EQ( BAD_ID, node.clock() );
  EXPECT_THROW( {node.enable();}, std::invalid_argument );
  EXPECT_EQ( clear_id, node.clear() );
  EXPECT_EQ( BAD_ID, node.preset() );
  EXPECT_EQ( rsval, node.rsval() );
  EXPECT_EQ( oid, node.data_output() );
  EXPECT_THROW( {node.cell_id();}, std::invalid_argument );
}

TEST( SeqImplTest, set_preset )
{
  SeqImpl node;

  char rsval = '1';
  SizeType oid = 10;
  node.set_latch(rsval, oid);
  SizeType preset_id = 11;
  node.set_preset(preset_id);

  EXPECT_EQ( BnSeqType::LATCH, node.type() );
  EXPECT_FALSE( node.is_dff() );
  EXPECT_TRUE( node.is_latch() );
  EXPECT_FALSE( node.is_cell() );
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

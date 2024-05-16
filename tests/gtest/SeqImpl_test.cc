
/// @file SeqImpl_test.cc
/// @brief SeqImpl_test の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include <gtest/gtest.h>
#include "SeqImpl.h"
#include "SeqImpl_sub.h"


BEGIN_NAMESPACE_YM_BN

TEST( SeqImplTest, dff )
{
  SizeType oid = 10;
  char rsval = '0';
  SeqImpl_DFF seq{oid, rsval};
  SeqImpl* node = &seq;

  EXPECT_EQ( BnSeqType::DFF, node->type() );
  EXPECT_TRUE( node->is_dff() );
  EXPECT_FALSE( node->is_latch() );
  EXPECT_FALSE( node->is_cell() );
  EXPECT_EQ( BAD_ID, node->data_src() );
  EXPECT_EQ( BAD_ID, node->clock() );
  EXPECT_THROW( {node->enable();}, std::invalid_argument );
  EXPECT_EQ( BAD_ID, node->clear() );
  EXPECT_EQ( BAD_ID, node->preset() );
  EXPECT_EQ( rsval, node->rsval() );
  EXPECT_EQ( oid, node->data_output() );
  EXPECT_THROW( {node->cell();}, std::invalid_argument );
}

TEST( SeqImplTest, latch )
{
  SizeType oid = 10;
  char rsval = '1';
  SeqImpl_Latch seq{oid, rsval};
  SeqImpl* node = &seq;

  EXPECT_EQ( BnSeqType::LATCH, node->type() );
  EXPECT_FALSE( node->is_dff() );
  EXPECT_TRUE( node->is_latch() );
  EXPECT_FALSE( node->is_cell() );
  EXPECT_EQ( BAD_ID, node->data_src() );
  EXPECT_THROW( {node->clock();}, std::invalid_argument );
  EXPECT_EQ( BAD_ID, node->enable() );
  EXPECT_EQ( BAD_ID, node->clear() );
  EXPECT_EQ( BAD_ID, node->preset() );
  EXPECT_EQ( rsval, node->rsval() );
  EXPECT_EQ( oid, node->data_output() );
  EXPECT_THROW( {node->cell();}, std::invalid_argument );
}

TEST( SeqImplTest, cell )
{
  ClibCell cell;
  SeqImpl_Cell seq{cell};
  SeqImpl* node = &seq;

  EXPECT_EQ( BnSeqType::CELL, node->type() );
  EXPECT_FALSE( node->is_dff() );
  EXPECT_FALSE( node->is_latch() );
  EXPECT_TRUE( node->is_cell() );
  EXPECT_THROW( {node->data_src();}, std::invalid_argument );
  EXPECT_THROW( {node->clock();}, std::invalid_argument );
  EXPECT_THROW( {node->enable();}, std::invalid_argument );
  EXPECT_THROW( {node->clear();}, std::invalid_argument );
  EXPECT_THROW( {node->preset();}, std::invalid_argument );
  EXPECT_THROW( {node->rsval();}, std::invalid_argument );
  EXPECT_THROW( {node->data_output();}, std::invalid_argument );
  EXPECT_EQ( cell, node->cell() );
}

TEST( SeqImplTest, set_data_src )
{
  char rsval = '0';
  SizeType oid = 10;
  SeqImpl_DFF seq{oid, rsval};

  SeqImpl* node = &seq;
  SizeType src_id = 20;
  node->set_data_src(src_id);

  EXPECT_EQ( BnSeqType::DFF, node->type() );
  EXPECT_TRUE( node->is_dff() );
  EXPECT_FALSE( node->is_latch() );
  EXPECT_FALSE( node->is_cell() );
  EXPECT_EQ( src_id, node->data_src() );
  EXPECT_EQ( BAD_ID, node->clock() );
  EXPECT_THROW( {node->enable();}, std::invalid_argument );
  EXPECT_EQ( BAD_ID, node->clear() );
  EXPECT_EQ( BAD_ID, node->preset() );
  EXPECT_EQ( rsval, node->rsval() );
  EXPECT_EQ( oid, node->data_output() );
  EXPECT_THROW( {node->cell();}, std::invalid_argument );
}

#if 0
TEST( SeqImplTest, set_clock )
{
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
#endif

END_NAMESPACE_YM_BN

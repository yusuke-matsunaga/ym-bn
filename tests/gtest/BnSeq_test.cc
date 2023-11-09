
/// @file BnSeq_test.cc
/// @brief BnSeq_test の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include <gtest/gtest.h>
#include "ym/BnSeq.h"
#include "ym/BnNode.h"
#include "ym/BnModel.h"
#include "ModelImpl.h"


BEGIN_NAMESPACE_YM_BN

TEST(BnSeqTest, constructor1)
{
  BnSeq node;

  EXPECT_FALSE( node.is_valid() );
  EXPECT_EQ( BAD_ID, node.id() );
  EXPECT_THROW( {node.type(); }, std::invalid_argument );
  EXPECT_THROW( {node.is_dff(); }, std::invalid_argument );
  EXPECT_THROW( {node.is_latch(); }, std::invalid_argument );
  EXPECT_THROW( {node.is_cell(); }, std::invalid_argument );
  EXPECT_THROW( {node.data_src(); }, std::invalid_argument );
  EXPECT_THROW( {node.clock(); }, std::invalid_argument );
  EXPECT_THROW( {node.enable(); }, std::invalid_argument );
  EXPECT_THROW( {node.clear(); }, std::invalid_argument );
  EXPECT_THROW( {node.preset(); }, std::invalid_argument );
  EXPECT_THROW( {node.rsval(); }, std::invalid_argument );
  EXPECT_THROW( {node.data_output(); }, std::invalid_argument );
  EXPECT_THROW( {node.cell_id(); }, std::invalid_argument );
  EXPECT_THROW( {node.cell(); }, std::invalid_argument );
}

END_NAMESPACE_YM_BN

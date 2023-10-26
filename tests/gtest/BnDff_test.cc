
/// @file BnDff_test.cc
/// @brief BnDff_test の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include <gtest/gtest.h>
#include "ym/BnDff.h"
#include "ym/BnNode.h"
#include "ym/BnModel.h"
#include "ModelImpl.h"


BEGIN_NAMESPACE_YM_BN

TEST(BnDffTest, constructor1)
{
  BnDff node;

  EXPECT_FALSE( node.is_valid() );
  EXPECT_THROW( {node.parent_model(); }, std::invalid_argument );
  EXPECT_EQ( BAD_ID, node.id() );
  EXPECT_THROW( {node.name(); }, std::invalid_argument );
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

TEST(BnDffTest, bad_id)
{
  shared_ptr<ModelImpl> model{new ModelImpl};

  BnDff node{model, BAD_ID};

  EXPECT_FALSE( node.is_valid() );
}

TEST(BnDffTest, constructor2)
{
  shared_ptr<ModelImpl> model{new ModelImpl};

  auto id = model->new_dff();

  BnDff node{model, id};

  EXPECT_TRUE( node.is_valid() );
  EXPECT_EQ( string{}, node.name() );
  EXPECT_EQ( BnDffType::DFF, node.type() );
  EXPECT_TRUE( node.is_dff() );
  EXPECT_FALSE( node.is_latch() );
  EXPECT_FALSE( node.is_cell() );
  EXPECT_FALSE( node.data_src().is_valid() );
  EXPECT_FALSE( node.clock().is_valid() );
  EXPECT_THROW( {node.enable(); }, std::invalid_argument );
  EXPECT_FALSE( node.clear().is_valid() );
  EXPECT_FALSE( node.preset().is_valid() );
  EXPECT_EQ( ' ', node.rsval() );
  EXPECT_TRUE( node.data_output().is_valid() );
  EXPECT_THROW( {node.cell_id(); }, std::invalid_argument );
  EXPECT_THROW( {node.cell(); }, std::invalid_argument );
}

END_NAMESPACE_YM_BN

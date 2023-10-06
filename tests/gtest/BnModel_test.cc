
/// @file BnModel_test.cc
/// @brief BnModel_test の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include <gtest/gtest.h>
#include "ym/BnModel.h"
#include "ym/BnNode.h"
#include "ModelImpl.h"


BEGIN_NAMESPACE_YM_BN

TEST( BnModelTest, constructor1 )
{
  BnModel model;

  EXPECT_EQ( string{}, model.name() );
  EXPECT_EQ( string{}, model.comment() );
  EXPECT_EQ( 0, model.input_num() );
  EXPECT_EQ( vector<BnNode>{}, model.input_list() );
}

TEST( BnModelTest, new_input )
{
  BnModel model;

  string name1{"input1"};
  auto node = model.new_input(name1);

  EXPECT_TRUE( node.is_valid() );
  EXPECT_TRUE( node.is_input() );
  EXPECT_EQ( model, node.parent_model() );
  EXPECT_EQ( name1, node.name() );
}

END_NAMESPACE_YM_BN

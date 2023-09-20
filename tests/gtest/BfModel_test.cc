
/// @file BfModel_test.cc
/// @brief BfModel_test の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include <gtest/gtest.h>
#include "ym/BfModel.h"
#include "ym/BfNode.h"
#include "ModelImpl.h"


BEGIN_NAMESPACE_YM_BNFE

TEST( BfModelTest, constructor1 )
{
  shared_ptr<ModelImpl> impl{new ModelImpl};
  BfModel model{impl};

  EXPECT_EQ( string{}, model.name() );
  EXPECT_EQ( string{}, model.comment() );
  EXPECT_EQ( 0, model.input_num() );
  EXPECT_EQ( vector<BfNode>{}, model.input_list() );

}

END_NAMESPACE_YM_BNFE

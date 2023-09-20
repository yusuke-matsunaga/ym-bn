
/// @file read_aig_test.cc
/// @brief read_aig_test の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include <gtest/gtest.h>
#include "ym/BfModel.h"


BEGIN_NAMESPACE_YM

TEST( BfModelTest, read_aag1)
{
  // 普通のファイルの読み込みテスト
  string filename{"test1.aag"};
  string path{DATAPATH + filename};

  auto bnet = BfModel::read_aag(path);

  const SizeType ni = 3;
  const SizeType no = 1;
  const SizeType nd = 0;
  const SizeType ng = 2;

  EXPECT_EQ( ni, bnet.input_num() );
  EXPECT_EQ( no, bnet.output_num() );
  EXPECT_EQ( nd, bnet.dff_num() );
  EXPECT_EQ( ng, bnet.logic_num() );
}

END_NAMESPACE_YM

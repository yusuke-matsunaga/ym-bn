
/// @file read_truth_test.cc
/// @brief read_truth_test の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include <gtest/gtest.h>
#include "ym/BnModel.h"


BEGIN_NAMESPACE_YM

TEST( BnModelTest, read_truth )
{
  auto filename = std::string{"ex61.truth"};
  auto path = DATAPATH + filename;
  auto model = BnModel::read_truth(path);
  int ni = 12;
  int no = 11;
  int nd = 0;
  int ng = 11;
  EXPECT_EQ( ni, model.input_num() );
  EXPECT_EQ( no, model.output_num() );
  EXPECT_EQ( nd, model.dff_num() );
  EXPECT_EQ( ng, model.logic_num() );

  // 出力結果の回帰テスト
  std::ostringstream s1;
  model.print(s1);

  auto ref_path = DATAPATH + std::string{"ex61.bn"};
  std::ifstream s2{ref_path};
  ASSERT_TRUE( s2 );
  std::string ref_contents;
  std::string buff;
  while ( getline(s2, buff) ) {
    ref_contents += buff + '\n';
  }

  EXPECT_EQ( ref_contents, s1.str() );
}

END_NAMESPACE_YM

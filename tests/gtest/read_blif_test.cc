
/// @file read_blif_test.cc
/// @brief read_blif_test の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include <gtest/gtest.h>
#include "ym/BfModel.h"


BEGIN_NAMESPACE_YM

TEST( BfModelTest, read_blif1)
{
  // 普通のファイルの読み込みテスト
  string filename{"s5378.blif"};
  string path{DATAPATH + filename};

  auto bnet = BfModel::read_blif(path);

  const SizeType ni = 35;
  const SizeType no = 49;
  const SizeType nd = 179;
  const SizeType ng = 2779;

  EXPECT_EQ( ni, bnet.input_num() );
  EXPECT_EQ( no, bnet.output_num() );
  EXPECT_EQ( nd, bnet.dff_num() );
  EXPECT_EQ( ng, bnet.logic_num() );

  ostringstream s;
  bnet.print(s);

  string exp_filename{"s5378.bnfe"};
  string exp_path{DATAPATH + exp_filename};
  ifstream is1{exp_path};
  ASSERT_TRUE( is1 );

  istringstream is2{s.str()};
  string buff1;
  string buff2;
  while ( getline(is1, buff1) ) {
    getline(is2, buff2);
    EXPECT_TRUE( is2 );
    EXPECT_EQ( buff1, buff2 );
  }
  getline(is2, buff2);
  EXPECT_FALSE( is2 );
}

TEST( BfModelTest, read_blif_file_not_found)
{
  // 存在しないファイルの場合の例外送出テスト
  EXPECT_THROW( {
      auto _ = BfModel::read_blif("not_exist_file");
    }, std::invalid_argument );
}

TEST( BfModelTest, read_blif_wrong_data)
{
  // 誤った内容のファイルの場合の例外送出テスト
  string filename{"broken.blif"};
  string path{DATAPATH + filename};
  EXPECT_THROW( {
      auto _ = BfModel::read_blif(path);
    }, std::invalid_argument );
}

END_NAMESPACE_YM
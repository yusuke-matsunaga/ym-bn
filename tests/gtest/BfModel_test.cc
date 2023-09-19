
/// @file BfModel_test.cc
/// @brief BfModel_test の実装ファイル
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

  string exp_filename{"s5378.print"};
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

TEST( BfModeTest, read_iscas1 )
{
  string filename = "b10.bench";
  string path = DATAPATH + filename;
  auto model = BfModel::read_iscas89(path);
  int ni = 11;
  int no = 6;
  int nd = 17;
  int ng = 172;
  EXPECT_EQ( ni + nd + 1, model.input_num() );
  EXPECT_EQ( no + nd + nd, model.output_num() );
  EXPECT_EQ( ng, model.logic_num() );
  EXPECT_EQ( nd, model.dff_num() );

  // 出力結果の回帰テスト
  ostringstream s1;
  model.print(s1);

  string ref_path = DATAPATH + string{"b10.print"};
  ifstream s2{ref_path};
  ASSERT_TRUE( s2 );
  string ref_contents;
  string buff;
  while ( getline(s2, buff) ) {
    ref_contents += buff + '\n';
  }

  EXPECT_EQ( ref_contents, s1.str() );
}

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

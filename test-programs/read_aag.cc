
/// @file read_aag.cc
/// @brief read_aag の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/BnModel.h"


BEGIN_NAMESPACE_YM

int
read_aag(
  int argc,
  char** argv
)
{
  for ( SizeType i = 1; i < argc; ++ i ) {
    string filename = argv[i];
    auto aig = BnModel::read_aag(filename);
    aig.print(cout);
  }
  return 0;
}

END_NAMESPACE_YM


int
main(
  int argc,
  char** argv
)
{
  return YM_NAMESPACE::read_aag(argc, argv);
}

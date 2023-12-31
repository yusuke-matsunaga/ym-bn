﻿
/// @file read_truth.cc
/// @brief BnModel::read_truth() のテストプログラム
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/BnModel.h"
#include "ym/MsgMgr.h"
#include "ym/StreamMsgHandler.h"


void
usage(
  const char* argv0
)
{
  using namespace std;

  cerr << "USAGE : " << argv0 << " truth-file" << endl;
}

int
main(
  int argc,
  char** argv
)
{
  using namespace std;
  using namespace nsYm;

  int base = 1;

  if ( base + 1 != argc ) {
    usage(argv[0]);
    return 2;
  }

  string filename = argv[base];

  StreamMsgHandler msg_handler(cerr);
  MsgMgr::attach_handler(&msg_handler);

  try {
    auto model = BnModel::read_truth(filename);
    model.print(cout);
  }
  catch ( std::invalid_argument err ) {
    cout << err.what() << endl;
  }

  return 0;
}

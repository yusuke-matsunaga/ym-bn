#ifndef PYM_YMBN_H
#define PYM_YMBN_H

/// @file ymbn.h
/// @brief ymbn の初期化関数のエクスポート
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym_config.h"


BEGIN_NAMESPACE_YM

/// @brief ymbn モジュールの初期化関数
extern "C" PyObject* PyInit_ymbn();

END_NAMESPACE_YM

#endif // PYM_YMBN_H

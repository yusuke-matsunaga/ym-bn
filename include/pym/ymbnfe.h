#ifndef PYM_YMBNFE_H
#define PYM_YMBNFE_H

/// @file ymbnfe.h
/// @brief ymbnfe の初期化関数のエクスポート
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym_config.h"


BEGIN_NAMESPACE_YM

/// @brief ymbnfe モジュールの初期化関数
extern "C" PyObject* PyInit_ymbnfe();

END_NAMESPACE_YM

#endif // PYM_YMBNFE_H

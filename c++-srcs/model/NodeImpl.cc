
/// @file NodeImpl.cc
/// @brief NodeImpl の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2024 Yusuke Matsunaga
/// All rights reserved.

#include "NodeImpl.h"
#include "NodeImpl_sub.h"


BEGIN_NAMESPACE_YM_BN

//////////////////////////////////////////////////////////////////////
// クラス NodeImpl
//////////////////////////////////////////////////////////////////////

// @brief 入力番号を返す．
SizeType
NodeImpl::input_id() const
{
  throw std::invalid_argument{"not an input."};
  return BAD_ID;
}

// @brief SEQ 番号を返す．
SizeType
NodeImpl::seq_id() const
{
  throw std::invalid_argument{"not a SEQ output."};
  return BAD_ID;
}

// @brief 論理ノードの時 true を返す．
bool
NodeImpl::is_logic() const
{
  return false;
}

// @brief ファンイン数を返す．
SizeType
NodeImpl::fanin_num() const
{
  return 0;
}

// @brief ファンインのノード番号を返す．
SizeType
NodeImpl::fanin(
  SizeType pos
) const
{
  throw std::out_of_range{"index out of range"};
  return BAD_ID;
}

// @brief ファンイン番号のリストを返す．
const vector<SizeType>&
NodeImpl::fanin_list() const
{
  static vector<SizeType> dummy;
  return dummy;
}

// @brief プリミティブタイプを得る．
PrimType
NodeImpl::primitive_type() const
{
  throw std::invalid_argument{"not a primitve-type node."};
  return PrimType::None;
}

// @brief ファンインの反転属性を返す．
bool
NodeImpl::fanin_inv(
  SizeType pos
) const
{
  throw std::invalid_argument{"not an AIG node."};
  return false;
}

// @brief 関数番号を返す．
SizeType
NodeImpl::local_func_id() const
{
  throw std::invalid_argument{"not a func-type node."};
  return BAD_ID;
}

// @brief セルを得る．
ClibCell
NodeImpl::cell() const
{
  throw std::invalid_argument{"not a cell-type node."};
  return BAD_ID;
}


//////////////////////////////////////////////////////////////////////
// クラス NodeImpl_Input
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
NodeImpl_Input::NodeImpl_Input(
  SizeType iid
) : mInputId{iid}
{
}

// @brief デストラクタ
NodeImpl_Input::~NodeImpl_Input()
{
}

// @brief ノードの種類を返す．
BnNodeType
NodeImpl_Input::type() const
{
  return BnNodeType::INPUT;
}

// @brief 入力番号を返す．
SizeType
NodeImpl_Input::input_id() const
{
  return mInputId;
}

// @brief 複製を作る．
unique_ptr<NodeImpl>
NodeImpl_Input::copy() const
{
  return unique_ptr<NodeImpl>{new NodeImpl_Input{mInputId}};
}


//////////////////////////////////////////////////////////////////////
// クラス NodeImpl_SeqOutput
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
NodeImpl_SeqOutput::NodeImpl_SeqOutput(
  SizeType seq_id
) : mSeqId{seq_id}
{
}

// @brief デストラクタ
NodeImpl_SeqOutput::~NodeImpl_SeqOutput()
{
}

// @brief ノードの種類を返す．
BnNodeType
NodeImpl_SeqOutput::type() const
{
  return BnNodeType::SEQ_OUTPUT;
}

// @brief SEQ 番号を返す．
SizeType
NodeImpl_SeqOutput::seq_id() const
{
  return mSeqId;
}

// @brief 複製を作る．
unique_ptr<NodeImpl>
NodeImpl_SeqOutput::copy() const
{
  return unique_ptr<NodeImpl>{new NodeImpl_SeqOutput{mSeqId}};
}


//////////////////////////////////////////////////////////////////////
// クラス NodeImpl_Logic
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
NodeImpl_Logic::NodeImpl_Logic(
  const vector<SizeType>& fanin_list
) : mFaninList{fanin_list}
{
}

// @brief デストラクタ
NodeImpl_Logic::~NodeImpl_Logic()
{
}

// @brief 論理ノードの時 true を返す．
bool
NodeImpl_Logic::is_logic() const
{
  return true;
}

// @brief ファンイン数を返す．
SizeType
NodeImpl_Logic::fanin_num() const
{
  return mFaninList.size();
}

// @brief ファンインのノード番号を返す．
SizeType
NodeImpl_Logic::fanin(
  SizeType pos
) const
{
  if ( pos < 0 || fanin_num() <= pos ) {
    throw std::out_of_range{"index out of range"};
  }
  return mFaninList[pos];
}

// @brief ファンイン番号のリストを返す．
const vector<SizeType>&
NodeImpl_Logic::fanin_list() const
{
  return mFaninList;
}


//////////////////////////////////////////////////////////////////////
// クラス NodeImpl_Primitive
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
NodeImpl_Primitive::NodeImpl_Primitive(
  PrimType prim_type,
  const vector<SizeType>& fanin_list
) : NodeImpl_Logic{fanin_list},
    mPrimType{prim_type}
{
}

// @brief デストラクタ
NodeImpl_Primitive::~NodeImpl_Primitive()
{
}

// @brief ノードの種類を返す．
BnNodeType
NodeImpl_Primitive::type() const
{
  return BnNodeType::PRIMITIVE;
}

// @brief プリミティブタイプを得る．
PrimType
NodeImpl_Primitive::primitive_type() const
{
  return mPrimType;
}

// @brief 複製を作る．
unique_ptr<NodeImpl>
NodeImpl_Primitive::copy() const
{
  return unique_ptr<NodeImpl>{new NodeImpl_Primitive{mPrimType, fanin_list()}};
}


//////////////////////////////////////////////////////////////////////
// クラス NodeImpl_Aig
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
NodeImpl_Aig::NodeImpl_Aig(
  SizeType fanin0,
  bool inv0,
  SizeType fanin1,
  bool inv1
) : NodeImpl_Logic{{fanin0, fanin1}},
    mInvFlags{inv0 | inv1 << 1}
{
}

// @brief デストラクタ
NodeImpl_Aig::~NodeImpl_Aig()
{
}

// @brief ノードの種類を返す．
BnNodeType
NodeImpl_Aig::type() const
{
  return BnNodeType::AIG;
}

// @brief ファンインの反転属性を返す．
bool
NodeImpl_Aig::fanin_inv(
  SizeType pos
) const
{
  return mInvFlags[pos % 2];
}

// @brief 複製を作る．
unique_ptr<NodeImpl>
NodeImpl_Aig::copy() const
{
  return unique_ptr<NodeImpl>{new NodeImpl_Aig{fanin(0), fanin_inv(0),
					       fanin(1), fanin_inv(1)}};
}


//////////////////////////////////////////////////////////////////////
// クラス NodeImpl_Func
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
NodeImpl_Func::NodeImpl_Func(
  SizeType func_id,
  const vector<SizeType>& fanin_list
) : NodeImpl_Logic{fanin_list},
    mFuncId{func_id}
{
}

// @brief デストラクタ
NodeImpl_Func::~NodeImpl_Func()
{
}

// @brief ノードの種類を返す．
BnNodeType
NodeImpl_Func::type() const
{
  return BnNodeType::FUNC;
}

// @brief 関数番号を返す．
SizeType
NodeImpl_Func::local_func_id() const
{
  return mFuncId;
}

// @brief 複製を作る．
unique_ptr<NodeImpl>
NodeImpl_Func::copy() const
{
  return unique_tr<NodeImpl>{new NodeImpl_Func{mFuncId, fanin_list()}};
}


//////////////////////////////////////////////////////////////////////
// NodeImpl_Cell
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
NodeImpl_Cell::NodeImpl_Cell(
  ClibCell cell,
  const vector<SizeType>& fanin_list
) : NodeImpl_Logic{fanin_list},
    mCell{cell}
{
}

// @brief デストラクタ
NodeImpl_Cell::~NodeImpl_Cell()
{
}

// @brief ノードの種類を返す．
BnNodeType
NodeImpl_Cell::type() const
{
  return BnNodeType::CELL;
}

// @brief セルを得る．
ClibCell
NodeImpl_Cell::cell() const
{
  return mCell;
}

// @brief 複製を作る．
unique_ptr<NodeImpl>
NodeImpl_Cell::copy() const
{
  return unique_ptr<NodeImpl>{new NodeImpl_Cell{mCell, fanin_list()}};
}

END_NAMESPACE_YM_BN


/// @file BnModel.cc
/// @brief BnModel の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/BnModel.h"
#include "ym/BnNode.h"
#include "ym/BnSeq.h"
#include "ym/BnFunc.h"
#include "ModelImpl.h"


BEGIN_NAMESPACE_YM_BN

//////////////////////////////////////////////////////////////////////
// クラス BnModel
//////////////////////////////////////////////////////////////////////

// @brief 内容をクリアする．
void
BnModel::clear()
{
  mImpl->clear();
}

// @brief 設定情報を確定する．
void
BnModel::wrap_up()
{
  mImpl->make_logic_list();
}

// @brief セルライブラリを設定する．
void
BnModel::set_celllibrary(
  ClibCellLibrary lib
)
{
  if ( library().is_valid() && library() != lib ) {
    throw std::invalid_argument{"library has already been set."};
  }
  mImpl->set_library(lib);
}

// @brief オプション情報をセットする．
void
BnModel::set_option(
  const JsonValue& option
)
{
  mImpl->set_option(option);
}

// @brief 入力ノードを作る．
BnNode
BnModel::new_input(
  const string& name
)
{
  auto id = mImpl->new_input();
  auto node = ModelImpl::new_node(mImpl, id);
  if ( name != string{} ) {
    auto iid = node.input_id();
    mImpl->set_input_name(iid, name);
  }
  return node;
}

// @brief 出力ノードを作る．
SizeType
BnModel::new_output(
  BnNode src,
  const string& name
)
{
  _check_node(src);
  auto oid = mImpl->new_output(src.id());
  if ( name != string{} ) {
    mImpl->set_output_name(oid, name);
  }
  return oid;
}

// @brief 新しいプリミティブ型の論理ノードを作る．
BnNode
BnModel::new_primitive(
  PrimType type,
  const vector<BnNode>& input_list
)
{
  auto input_id_list = to_id_list(input_list);
  auto id = mImpl->new_primitive(input_id_list, type);
  return ModelImpl::new_node(mImpl, id);
}

// @brief 新しいAIG型の論理ノードを作る．
BnNode
BnModel::new_aig(
  BnNode src0,
  BnNode src1,
  bool inv0,
  bool inv1
)
{
  _check_node(src0);
  _check_node(src1);
  auto id = mImpl->new_aig(src0.id(), src1.id(), inv0, inv1);
  return ModelImpl::new_node(mImpl, id);
}

// @brief 関数型の論理ノードを作る．
BnNode
BnModel::new_func(
  BnFunc func,
  const vector<BnNode>& input_list
)
{
  _check_func(func);
  if ( func.input_num() > input_list.size() ) {
    throw std::invalid_argument{"'input_list' is not compatible with 'func'."};
  }
  auto input_id_list = to_id_list(input_list);
  auto id = mImpl->new_func(input_id_list, func.id());
  return ModelImpl::new_node(mImpl, id);
}

// @brief セル型の論理ノードを作る．
BnNode
BnModel::new_cell(
  ClibCell cell,
  const vector<BnNode>& input_list
)
{
  set_celllibrary(cell.library());
  auto ni = cell.input_num();
  if ( input_list.size() != ni ) {
    throw std::invalid_argument{"the size of input_list differs from the cell's input size."};
  }
  auto input_id_list = to_id_list(input_list);
  auto id = mImpl->new_cell(input_id_list, cell);
  return ModelImpl::new_node(mImpl, id);
}

// @brief DFFを作る．
BnSeq
BnModel::new_dff(
  char rs_val,
  const string& name
)
{
  _check_rsval(rs_val);
  auto id = mImpl->new_dff(rs_val, name);
  return ModelImpl::new_seq(mImpl, id);
}

// @brief ラッチを作る．
BnSeq
BnModel::new_latch(
  char rs_val,
  const string& name
)
{
  _check_rsval(rs_val);
  auto id = mImpl->new_latch(rs_val, name);
  return ModelImpl::new_seq(mImpl, id);
}

// @brief セルタイプのSEQノードを作る．
BnSeq
BnModel::new_seq_cell(
  ClibCell cell,
  const string& name
)
{
  set_celllibrary(cell.library());
  auto id = mImpl->new_seq_cell(cell, name);
  return ModelImpl::new_seq(mImpl, id);
}

// @brief DFFのソースノードをセットする．
void
BnModel::set_data_src(
  BnSeq dff,
  BnNode src
)
{
  _check_seq(dff);
  _check_node(src);
  mImpl->set_data_src(dff.id(), src.id());
}

// @brief DFFのクロック入力をセットする．
void
BnModel::set_clock(
  BnSeq dff,
  BnNode clock
)
{
  _check_seq(dff);
  _check_node(clock);
  mImpl->set_clock(dff.id(), clock.id());
}

// @brief ラッチのイネーブル入力をセットする．
void
BnModel::set_enable(
  BnSeq latch,
  BnNode enable
)
{
  _check_seq(latch);
  _check_node(enable);
  mImpl->set_enable(latch.id(), enable.id());
}

// @brief DFFのクリア入力をセットする．
void
BnModel::set_clear(
  BnSeq dff,
  BnNode clear
)
{
  _check_seq(dff);
  _check_node(clear);
  mImpl->set_clear(dff.id(), clear.id());
}

// @brief DFFのプリセット入力をセットする．
void
BnModel::set_preset(
  BnSeq dff,
  BnNode preset
)
{
  _check_seq(dff);
  _check_node(preset);
  mImpl->set_preset(dff.id(), preset.id());
}

// @brief ピンに対応するノードをセットする．
void
BnModel::set_seq_pin(
  BnSeq dff,
  SizeType pos,
  BnNode node
)
{
  _check_seq(dff);
  _check_node(node);
  mImpl->set_seq_pin(dff.id(), pos, node.id());
}

// @brief カバーを追加する．
BnFunc
BnModel::reg_cover(
  SizeType input_num,
  const vector<vector<Literal>>& cube_list,
  char opat
)
{
  auto id = mImpl->reg_cover(input_num, cube_list, opat);
  return ModelImpl::new_func(mImpl, id);
}

// @brief 論理式を追加する．
BnFunc
BnModel::reg_expr(
  const Expr& expr
)
{
  auto id = mImpl->reg_expr(expr);
  return ModelImpl::new_func(mImpl, id);
}

// @brief 真理値表を追加する．
BnFunc
BnModel::reg_tvfunc(
  const TvFunc& tvfunc
)
{
  auto id = mImpl->reg_tvfunc(tvfunc);
  return ModelImpl::new_func(mImpl, id);
}

// @brief BDDを追加する．
BnFunc
BnModel::reg_bdd(
  const Bdd& bdd
)
{
  auto id = mImpl->reg_bdd(bdd);
  return ModelImpl::new_func(mImpl, id);
}

END_NAMESPACE_YM_BN

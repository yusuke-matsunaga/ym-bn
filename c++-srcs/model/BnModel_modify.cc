
/// @file BnModel.cc
/// @brief BnModel の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/BnModel.h"
#include "ym/BnNode.h"
#include "ym/BnSeq.h"
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

// @brief セルライブラリを設定する．
void
BnModel::set_celllibrary(
  ClibCellLibrary lib
)
{
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
  BnNode node{mImpl, id};
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
  auto oid = mImpl->new_output(src.id());
  if ( name != string{} ) {
    mImpl->set_output_name(oid, name);
  }
  return oid;
}

// @brief 新しいプリミティブ型の論理ノードを作る．
BnNode
BnModel::new_primitive(
  const vector<BnNode>& input_list,
  PrimType type
)
{
  auto input_id_list = to_id_list(input_list, "new_primitive");
  return BnNode{mImpl, mImpl->new_primitive(input_id_list, type)};
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
  auto src0_id = to_id(src0, "new_aig");
  auto src1_id = to_id(src1, "new_aig");
  return BnNode{mImpl, mImpl->new_aig(src0_id, src1_id, inv0, inv1)};
}

// @brief 新しいカバー型の論理ノードを作る．
BnNode
BnModel::new_cover(
  const vector<BnNode>& input_list,
  SizeType cover_id
)
{
  auto input_id_list = to_id_list(input_list, "new_cover");
  _check_cover(cover_id, "new_cover");
  _check_cover_input(cover_id, input_list.size(), "new_cover");
  return BnNode{mImpl, mImpl->new_cover(input_id_list, cover_id)};
}

// @brief 論理式型の論理ノードを作る．
BnNode
BnModel::new_expr(
  const vector<BnNode>& input_list,
  SizeType expr_id
)
{
  auto input_id_list = to_id_list(input_list, "new_expr");
  _check_expr(expr_id, "new_expr");
  _check_expr_input(expr_id, input_list.size(), "new_expr");
  return BnNode{mImpl, mImpl->new_expr(input_id_list, expr_id)};
}

// @brief セル型の論理ノードを作る．
BnNode
BnModel::new_cell(
  const vector<BnNode>& input_list,
  ClibCell cell
)
{
  auto input_id_list = to_id_list(input_list, "new_cell");
  return BnNode{mImpl, mImpl->new_cell(input_id_list, cell)};
}

// @brief 真理値表型の論理ノードを作る．
BnNode
BnModel::new_func(
  const vector<BnNode>& input_list,
  SizeType func_id
)
{
  auto input_id_list = to_id_list(input_list, "new_func");
  _check_func(func_id, "new_func");
  _check_func_input(func_id, input_list.size(), "new_func");
  return BnNode{mImpl, mImpl->new_func(input_id_list, func_id)};
}

// @brief BDD型の論理ノードを作る．
BnNode
BnModel::new_bdd(
  const vector<BnNode>& input_list,
  SizeType bdd_id
)
{
  auto input_id_list = to_id_list(input_list, "new_bdd");
  _check_bdd(bdd_id, "new_bdd");
  _check_bdd_input(bdd_id, input_list.size(), "new_bdd");
  return BnNode{mImpl, mImpl->new_bdd(input_id_list, bdd_id)};
}

// @brief DFFを作る．
BnSeq
BnModel::new_dff(
  char rs_val,
  const string& name
)
{
  auto id = mImpl->new_dff(rs_val, BAD_ID);
  if ( name != string{} ) {
    mImpl->set_seq_name(id, name);
  }
  return BnSeq{mImpl, id};
}

// @brief ラッチを作る．
BnSeq
BnModel::new_latch(
  char rs_val,
  const string& name
)
{
  auto id = mImpl->new_latch(rs_val, BAD_ID);
  if ( name != string{} ) {
    mImpl->set_seq_name(id, name);
  }
  return BnSeq{mImpl, id};
}

// @brief セルタイプのSEQノードを作る．
BnSeq
BnModel::new_seq_cell(
  ClibCell cell,
  const string& name
)
{
  auto id = mImpl->new_seq_cell(cell);
  if ( name != string{} ) {
    mImpl->set_seq_name(id, name);
  }
  return BnSeq{mImpl, id};
}

// @brief DFFのソースノードをセットする．
void
BnModel::set_data_src(
  BnSeq dff,
  BnNode src
)
{
  mImpl->set_data_src(dff.id(), src.id());
}

// @brief DFFのクロック入力をセットする．
void
BnModel::set_clock(
  BnSeq dff,
  BnNode clock
)
{
  mImpl->set_clock(dff.id(), clock.id());
}

// @brief ラッチのイネーブル入力をセットする．
void
BnModel::set_enable(
  BnSeq latch,
  BnNode enable
)
{
  mImpl->set_enable(latch.id(), enable.id());
}

// @brief DFFのクリア入力をセットする．
void
BnModel::set_clear(
  BnSeq dff,
  BnNode clear
)
{
  mImpl->set_clear(dff.id(), clear.id());
}

// @brief DFFのプリセット入力をセットする．
void
BnModel::set_preset(
  BnSeq dff,
  BnNode preset
)
{
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
  mImpl->set_seq_pin(dff.id(), pos, node.id());
}

// @brief カバーを追加する．
SizeType
BnModel::add_cover(
  SizeType input_num,
  const vector<vector<Literal>>& cube_list,
  char opat
)
{
  return mImpl->add_cover(input_num, cube_list, opat);
}

// @brief 論理式を追加する．
SizeType
BnModel::add_expr(
  const Expr& expr
)
{
  return mImpl->add_expr(expr);
}

// @brief 真理値表を追加する．
SizeType
BnModel::add_func(
  const TvFunc& func
)
{
  return mImpl->add_func(func);
}

// @brief BDDを追加する．
SizeType
BnModel::add_bdd(
  const Bdd& bdd
)
{
  return mImpl->add_bdd(bdd);
}

END_NAMESPACE_YM_BN

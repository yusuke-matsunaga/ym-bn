
/// @file BnModel.cc
/// @brief BnModel の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/BnModel.h"
#include "ym/BnNode.h"
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
  _model_impl().clear();
}

// @brief 設定情報を確定する．
void
BnModel::wrap_up()
{
  _model_impl().make_logic_list();
}

// @brief オプション情報をセットする．
void
BnModel::set_option(
  const JsonValue& option
)
{
  _model_impl().set_option(option);
}

// @brief 入力ノードを作る．
BnNode
BnModel::new_input(
  const std::string& name
)
{
  auto id = _model_impl().new_input();
  auto node = _id2node(id);
  if ( name != string{} ) {
    auto iid = node.input_id();
    _model_impl().set_input_name(iid, name);
  }
  return node;
}

// @brief DFF出力ノードを作る．
BnNode
BnModel::new_dff_output(
  const std::string& name
)
{
  auto id = _model_impl().new_dff_output();
  auto node = _id2node(id);
  if ( name != std::string{} ) {
    auto dff_id = node.dff_id();
    _model_impl().set_dff_name(dff_id, name);
  }
  return node;
}

// @brief DFFのソースノードをセットする．
void
BnModel::set_dff_src(
  SizeType dff_id,
  BnNode src
)
{
  auto id = _model_impl().dff_output_id(dff_id);
  _model_impl().set_dff_src(id, src.id());
}

// @brief 出力ノードを作る．
SizeType
BnModel::new_output(
  BnNode src,
  const string& name
)
{
  _check_node(src);
  auto oid = _model_impl().new_output(src.id());
  if ( name != string{} ) {
    _model_impl().set_output_name(oid, name);
  }
  return oid;
}

// @brief プリミティブ型の論理ノードを作る．
BnNode
BnModel::new_primitive(
  PrimType primitive_type,
  const std::vector<BnNode>& fanin_list
)
{
  auto input_num = fanin_list.size();
  auto func_id = _model_impl().reg_primitive(input_num, primitive_type);
  auto fanin_id_list = _node2id_list(fanin_list);
  auto id = _model_impl().new_logic(func_id, fanin_id_list);
  return _id2node(id);
}

// @brief カバー型の論理ノードを登録する．
BnNode
BnModel::new_cover(
  const SopCover& input_cover,
  bool output_inv,
  const std::vector<BnNode>& fanin_list
)
{
  auto func_id = _model_impl().reg_cover(input_cover, output_inv);
  auto fanin_id_list = _node2id_list(fanin_list);
  auto id = _model_impl().new_logic(func_id, fanin_id_list);
  return _id2node(id);
}

// @brief 論理式型の論理ノードを登録する．
BnNode
BnModel::new_expr(
  const Expr& expr,
  const std::vector<BnNode>& fanin_list
)
{
  auto func_id = _model_impl().reg_expr(expr);
  auto fanin_id_list = _node2id_list(fanin_list);
  auto id = _model_impl().new_logic(func_id, fanin_id_list);
  return _id2node(id);
}

// @brief 真理値表型の論理ノードを登録する．
BnNode
BnModel::new_tvfunc(
  const TvFunc& func,
  const std::vector<BnNode>& fanin_list
)
{
  auto func_id = _model_impl().reg_tvfunc(func);
  auto fanin_id_list = _node2id_list(fanin_list);
  auto id = _model_impl().new_logic(func_id, fanin_id_list);
  return _id2node(id);
}

// @brief BDD型の論理ノードを登録する．
BnNode
BnModel::new_bdd(
  const Bdd& bdd,
  const std::vector<BnNode>& fanin_list
)
{
  auto func_id = _model_impl().reg_bdd(bdd);
  auto fanin_id_list = _node2id_list(fanin_list);
  auto id = _model_impl().new_logic(func_id, fanin_id_list);
  return _id2node(id);
}

END_NAMESPACE_YM_BN

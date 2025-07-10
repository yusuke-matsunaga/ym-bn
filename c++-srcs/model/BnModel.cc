
/// @file BnModel.cc
/// @brief BnModel の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/BnModel.h"
#include "ym/BnNode.h"
#include "ModelImpl.h"


BEGIN_NAMESPACE_YM_BN

//////////////////////////////////////////////////////////////////////
// クラス BnModel
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
BnModel::BnModel(
) : BnModel(new ModelImpl)
{
}

// @brief 内容を指定したコンストラクタ
BnModel::BnModel(
  ModelImpl* impl
) : BnBase(std::shared_ptr<ModelImpl>{impl})
{
}

// @brief デストラクタ
BnModel::~BnModel()
{
}

// @brief '深い'コピーを作る．
BnModel
BnModel::copy() const
{
  auto impl = _model_impl().copy();
  return BnModel(impl);
}

// @brief DFF数を返す．
SizeType
BnModel::dff_num() const
{
  return _model_impl().dff_num();
}

// @brief DFFを返す．
BnDff
BnModel::dff(
  SizeType dff_id
) const
{
  return _id2dff(dff_id);
}

// @brief ノード数を返す．
SizeType
BnModel::node_num() const
{
  return _model_impl().node_num();
}

// @brief ノードを返す．
BnNode
BnModel::node(
  SizeType id
) const
{
  return _id2node(id);
}

// @brief 入力数を返す．
SizeType
BnModel::input_num() const
{
  return _model_impl().input_num();
}

// @brief 入力のノードを返す．
BnNode
BnModel::input(
  SizeType input_id
) const
{
  _check_input_id(input_id);
  auto id = _model_impl().input_id(input_id);
  return _id2node(id);
}

// @brief 入力のノードのリストを返す．
std::vector<BnNode>
BnModel::input_list() const
{
  return _id2node_list(_model_impl().input_id_list());
}

// @brief 出力数を返す．
SizeType
BnModel::output_num() const
{
  return _model_impl().output_num();
}

// @brief 入力のノードを返す．
BnNode
BnModel::output(
  SizeType output_id
) const
{
  _check_output_id(output_id);
  auto id = _model_impl().output_id(output_id);
  return _id2node(id);
}

// @brief 出力のノードのリストを返す．
std::vector<BnNode>
BnModel::output_list() const
{
  return _id2node_list(_model_impl().output_id_list());
}

// @brief 論理ノード数を返す．
SizeType
BnModel::logic_num() const
{
  return _model_impl().logic_num();
}

// @brief 論理ノードを返す．
BnNode
BnModel::logic(
  SizeType pos
) const
{
  _check_logic_id(pos);
  auto id = _model_impl().logic_id(pos);
  return _id2node(id);
}

// @brief 論理ノードのリストを返す．
std::vector<BnNode>
BnModel::logic_list() const
{
  return _id2node_list(_model_impl().logic_id_list());
}

// @brief 関数情報の数を返す．
SizeType
BnModel::func_num() const
{
  return _model_impl().func_num();
}

// @brief 関数情報を取り出す．
BnFunc
BnModel::func(
  SizeType func_id
) const
{
  if ( func_id < 0 || func_num() <= func_id ) {
    ostringstream buf;
    buf << "'func_id'(" << func_id << ") is out of range";
    throw std::out_of_range{buf.str()};
  }
  return _id2func(func_id);
}

// @brief オプション情報を表す JSON オブジェクトを返す．
JsonValue
BnModel::option() const
{
  return _model_impl().option();
}

// @brief 名前を返す．
std::string
BnModel::name() const
{
  return _model_impl().name();
}

// @brief コメントを返す．
const std::vector<std::string>&
BnModel::comment_list() const
{
  return _model_impl().comment_list();
}

// @brief 入力名を返す．
string
BnModel::input_name(
  SizeType input_id
) const
{
  _check_input_id(input_id);
  return _model_impl().input_name(input_id);
}

// @brief 出力名を返す．
string
BnModel::output_name(
  SizeType output_id
) const
{
  _check_output_id(output_id);
  return _model_impl().output_name(output_id);
}

// @brief DFF名を返す．
string
BnModel::dff_name(
  SizeType dff_id
) const
{
  _check_dff_id(dff_id);
  return _model_impl().dff_name(dff_id);
}

// @brief 内容を出力する．
void
BnModel::print(
  ostream& s
) const
{
  _model_impl().print(s);
}


//////////////////////////////////////////////////////////////////////
// クラス BnDff
//////////////////////////////////////////////////////////////////////

// @brief 内容を指定したコンストラクタ
BnDff::BnDff(
  const shared_ptr<ModelImpl>& model,
  SizeType id
) : BnBase(model),
    mId{id}
{
  if ( is_invalid() ) {
    mId = BAD_ID;
  }
}

// @brief デストラクタ
BnDff::~BnDff()
{
}

// @brief 名前を返す．
const std::string&
BnDff::name() const
{
  auto& dff = _dff_impl();
  return dff.name;
}

// @brief 出力ノードを返す．
BnNode
BnDff::output() const
{
  auto& dff = _dff_impl();
  return _id2node(dff.id);
}

// @brief 入力ノードを返す．
BnNode
BnDff::input() const
{
  auto& dff = _dff_impl();
  return _id2node(dff.src_id);
}

// @brief リセット値
char
BnDff::reset_val() const
{
  auto& dff = _dff_impl();
  return dff.reset_val;
}

// @brief DFFの実体を返す．
const DffImpl&
BnDff::_dff_impl() const
{
  if ( !is_valid() ) {
    throw std::logic_error{"BnDff: invalid data"};
  }
  return _model_impl().dff_impl(mId);
}

END_NAMESPACE_YM_BN


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

// @brief ノード数を返す．
SizeType
BnModel::node_num() const
{
  return _model_impl().node_num();
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

// @brief DFF数を返す．
SizeType
BnModel::dff_num() const
{
  return _model_impl().dff_num();
}

// @brief DFFの出力ノードを返す．
BnNode
BnModel::dff_output(
  SizeType dff_id
) const
{
  _check_dff_id(dff_id);
  auto id = _model_impl().dff_output_id(dff_id);
  return _id2node(id);
}

// @brief DFFの入力ノードを返す．
BnNode
BnModel::dff_src(
  SizeType dff_id
) const
{
  _check_dff_id(dff_id);
  auto dff_output_id = _model_impl().dff_output_id(dff_id);
  auto& dff_output = _model_impl().node_impl(dff_output_id);
  auto id = dff_output.dff_src_id();
  return _id2node(id);
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

BEGIN_NONAMESPACE

inline
string
node_name(
  const BnNode& node
)
{
  ostringstream buf;
  buf << "Node#" << node.id();
  return buf.str();
}

END_NONAMESPACE

// @brief 内容を出力する．
void
BnModel::print(
  ostream& s
) const
{
  if ( name() != string{} ) {
    s << "Name: " << name() << endl;
  }
  for ( auto& comment: comment_list() ) {
    s << "Comment: " << comment << endl;
  }
  for ( SizeType i = 0;i < input_num(); ++ i ) {
    auto node = input(i);
    s << "INPUT#" << i << "[" << input_name(i) << "]"
      << " = " << node_name(node) << endl;
  }
  for ( SizeType i = 0; i < output_num(); ++ i ) {
    s << "OUTPUT#" << i << "[" << output_name(i) << "]"
      << " = " << node_name(output(i)) << endl;
  }
  for ( SizeType i = 0; i < dff_num(); ++ i ) {
    auto node = dff_output(i);
    s << "DFF#" << i << "[" << dff_name(i) << "]:"
      << " output = " << node_name(node)
      << " src = " << node_name(node.dff_src())
      << endl;
  }
  for ( auto node: logic_list() ) {
    s << node_name(node)
      << " = "
      << "Func#" << node.func().id()
      << " (";
    for ( auto inode: node.fanin_list() ) {
      s << " " << node_name(inode);
    }
    s << ")" << endl;
  }
  if ( func_num() > 0 ) {
    s << endl;
    for ( SizeType id = 0; id < func_num(); ++ id ) {
      s << "Func#" << id << ":" << endl;
      func(id).print(s);
      s << endl;
    }
  }
}

END_NAMESPACE_YM_BN

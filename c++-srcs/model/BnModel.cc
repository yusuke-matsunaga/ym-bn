
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

// @brief コピーコンストラクタ
BnModel::BnModel(
  const BnModel& src
) : mImpl{src.mImpl->copy()}
{
}

// @brief ムーブコンストラクタ
BnModel::BnModel(
  BnModel&& src
) : mImpl{std::move(src.mImpl)}
{
  src.mImpl = nullptr;
}

// @brief コンストラクタ
BnModel::BnModel(
) : mImpl{new ModelImpl}
{
}

// @brief デストラクタ
BnModel::~BnModel()
{
}

// @brief セルライブラリを返す．
ClibCellLibrary
BnModel::library() const
{
  return mImpl->library();
}

// @brief ノード数を返す．
SizeType
BnModel::node_num() const
{
  return mImpl->node_num();
}

// @brief 入力数を返す．
SizeType
BnModel::input_num() const
{
  return mImpl->input_num();
}

// @brief 入力のノードを返す．
BnNode
BnModel::input(
  SizeType pos
) const
{
  _check_input_id(pos);
  auto id = mImpl->input_id(pos);
  return ModelImpl::new_node(mImpl, id);
}

// @brief 入力のノードのリストを返す．
BnNodeList
BnModel::input_list() const
{
  auto& id_list = mImpl->input_list();
  return ModelImpl::new_node_list(mImpl, id_list);
}

// @brief 入力数を返す．
SizeType
BnModel::output_num() const
{
  return mImpl->output_num();
}

// @brief 入力のノードを返す．
BnNode
BnModel::output(
  SizeType pos
) const
{
  _check_output_id(pos);
  auto id = mImpl->output_id(pos);
  return ModelImpl::new_node(mImpl, id);
}

// @brief 出力のノードのリストを返す．
BnNodeList
BnModel::output_list() const
{
  auto& id_list = mImpl->output_list();
  return ModelImpl::new_node_list(mImpl, id_list);
}

// @brief 論理ノード数を返す．
SizeType
BnModel::logic_num() const
{
  return mImpl->logic_num();
}

// @brief 論理ノードを返す．
BnNode
BnModel::logic(
  SizeType pos
) const
{
  _check_logic_id(pos);
  auto id = mImpl->logic_id(pos);
  return ModelImpl::new_node(mImpl, id);
}

// @brief 論理ノードのリストを返す．
BnNodeList
BnModel::logic_list() const
{
  auto& id_list = mImpl->logic_list();
  return ModelImpl::new_node_list(mImpl, id_list);
}

// @brief SEQノード数を返す．
SizeType
BnModel::seq_num() const
{
  return mImpl->seq_num();
}

// @brief SEQノードを返す．
BnSeq
BnModel::seq(
  SizeType pos
) const
{
  _check_seq_id(pos);
  return ModelImpl::new_seq(mImpl, pos);
}

// @brief 関数情報の数を返す．
SizeType
BnModel::func_num() const
{
  return mImpl->func_num();
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
  return ModelImpl::new_func(mImpl, func_id);
}

// @brief オプション情報を表す JSON オブジェクトを返す．
JsonValue
BnModel::option() const
{
  return mImpl->option();
}

// @brief 名前を返す．
string
BnModel::name() const
{
  return mImpl->name();
}

// @brief コメントを返す．
string
BnModel::comment() const
{
  return mImpl->comment();
}

// @brief 入力名を返す．
string
BnModel::input_name(
  SizeType input_id
) const
{
  _check_input_id(input_id);
  return mImpl->input_name(input_id);
}

// @brief 出力名を返す．
string
BnModel::output_name(
  SizeType output_id
) const
{
  _check_output_id(output_id);
  return mImpl->output_name(output_id);
}

// @brief ラッチ名を返す．
string
BnModel::seq_name(
  SizeType seq_id
) const
{
  _check_seq_id(seq_id);
  return mImpl->seq_name(seq_id);
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
  if ( comment() != string{} ) {
    s << "Comment: " << comment() << endl;
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
  for ( SizeType i = 0; i < seq_num(); ++ i ) {
    auto seq = this->seq(i);
    s << "SEQ#" << i << "[" << seq_name(i) << "]:"
      << " type = " << seq.type()
      << " output = " << node_name(seq.data_output())
      << " src = " << node_name(seq.data_src())
      << " clock = " << node_name(seq.clock());
    if ( seq.clear().is_valid() ) {
      s << " clear = " << node_name(seq.clear());
    }
    if ( seq.preset().is_valid() ) {
      s << " preset = " << node_name(seq.preset());
    }
    if ( seq.rsval() != ' ' ) {
      s << ", rsval = " << seq.rsval();
    }
    s << endl;
  }
  for ( auto node: logic_list() ) {
    s << node_name(node)
      << " = ";
    if ( node.is_primitive() ) {
      s << node.primitive_type();
    }
    else if ( node.is_aig() ) {
      s << "AIG[" << node.fanin_inv(0) << node.fanin_inv(1) << "]";
    }
    else if ( node.is_func() ) {
      s << "Func#" << node.local_func().id();
    }
    else if ( node.is_cell() ) {
      s << "Cell: " << node.cell().name();
    }
    else {
      ASSERT_NOT_REACHED;
    }
    s << " (";
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
    }
  }
}

// @brief vector<BnNode> から ID番号のリストを取り出す．
vector<SizeType>
BnModel::to_id_list(
  const vector<BnNode>& node_list
) const
{
  vector<SizeType> id_list;
  id_list.reserve(node_list.size());
  for ( auto& node: node_list ) {
    _check_node(node);
    auto id = node.id();
    id_list.push_back(id);
  }
  return id_list;
}

END_NAMESPACE_YM_BN

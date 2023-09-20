
/// @file BfModel.cc
/// @brief BfModel の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/BfModel.h"
#include "ym/BfNode.h"
#include "ModelImpl.h"


BEGIN_NAMESPACE_YM_BNFE

//////////////////////////////////////////////////////////////////////
// クラス BfModel
//////////////////////////////////////////////////////////////////////

// @brief ModelImpl からのキャストコンストラクタ
BfModel::BfModel(
  ModelImpl* impl
) : mImpl{impl}
{
}

// @brief shared_ptr<ModelImpl> からのキャストコンストラクタ
BfModel::BfModel(
  const shared_ptr<ModelImpl>& impl
) : mImpl{impl}
{
}

// @brief コピーコンストラクタ
BfModel::BfModel(
  const BfModel& src
) : mImpl{src.mImpl}
{
}

// @brief ムーブコンストラクタ
BfModel::BfModel(
  BfModel&& src
) : mImpl{std::move(src.mImpl)}
{
  src.mImpl = nullptr;
}

// @brief コピー代入演算子
BfModel&
BfModel::operator=(
  const BfModel& src
)
{
  if ( this != &src ) {
    mImpl = src.mImpl;
  }
  return *this;
}

// @brief ムーブ代入演算子
BfModel&
BfModel::operator=(
  BfModel&& src
)
{
  std::swap(mImpl, src.mImpl);
  src.mImpl = nullptr;
  return *this;
}

// @brief コンストラクタ
BfModel::BfModel(
) : mImpl{shared_ptr<ModelImpl>{new ModelImpl}}
{
}

// @brief デストラクタ
BfModel::~BfModel()
{
}

// @brief 深いコピーを行う．
BfModel
BfModel::copy() const
{
  return BfModel{new ModelImpl{*mImpl}};
}

// @brief 名前を返す．
string
BfModel::name() const
{
  return mImpl->name();
}

// @brief コメントを返す．
string
BfModel::comment() const
{
  return mImpl->comment();
}

// @brief 入力数を返す．
SizeType
BfModel::input_num() const
{
  return mImpl->input_num();
}

// @brief 入力のノードを返す．
BfNode
BfModel::input(
  SizeType pos
) const
{
  return from_id(mImpl->input(pos));
}

// @brief 入力のノードのリストを返す．
vector<BfNode>
BfModel::input_list() const
{
  return from_id_list(mImpl->input_list());
}

// @brief 入力名を返す．
string
BfModel::input_name(
  SizeType pos
) const
{
  return mImpl->input_name(pos);
}

// @brief 入力名のリストを返す．
vector<string>
BfModel::input_name_list() const
{
  return mImpl->input_name_list();
}

// @brief 入力数を返す．
SizeType
BfModel::output_num() const
{
  return mImpl->output_num();
}

// @brief 入力のノードを返す．
BfNode
BfModel::output(
  SizeType pos
) const
{
  return from_id(mImpl->output(pos));
}

// @brief 出力のノードのリストを返す．
vector<BfNode>
BfModel::output_list() const
{
  return from_id_list(mImpl->output_list());
}

// @brief 出力名を返す．
string
BfModel::output_name(
  SizeType pos
) const
{
  return mImpl->output_name(pos);
}

// @brief 出力名のリストを返す．
vector<string>
BfModel::output_name_list() const
{
  return mImpl->output_name_list();
}

// @brief DFF数を返す．
SizeType
BfModel::dff_num() const
{
  return mImpl->dff_num();
}

// @brief DFFのノードを返す．
BfNode
BfModel::dff(
  SizeType pos
) const
{
  return from_id(mImpl->dff(pos));
}

// @brief DFFのノードのリストを返す．
vector<BfNode>
BfModel::dff_list() const
{
  return from_id_list(mImpl->dff_list());
}

// @brief 論理ノード数を返す．
SizeType
BfModel::logic_num() const
{
  return mImpl->logic_num();
}

// @brief 論理ノードを返す．
BfNode
BfModel::logic(
  SizeType pos
) const
{
  return from_id(mImpl->logic(pos));
}

// @brief 論理ノードのリストを返す．
vector<BfNode>
BfModel::logic_list() const
{
  return from_id_list(mImpl->logic_list());
}

// @brief カバーの種類の数を返す．
SizeType
BfModel::cover_num() const
{
  return mImpl->cover_num();
}

// @brief カバーを取り出す．
const BfCover&
BfModel::cover(
  SizeType cover_id
) const
{
  return mImpl->cover(cover_id);
}

// @brief 論理式の数を返す．
SizeType
BfModel::expr_num() const
{
  return mImpl->expr_num();
}

// @brief 論理式を返す．
const Expr&
BfModel::expr(
  SizeType expr_id
) const
{
  return mImpl->expr(expr_id);
}

BEGIN_NONAMESPACE

inline
string
node_name(
  const BfNode& node
)
{
  ostringstream buf;
  buf << "Node#" << node.id();
  if ( node.name() != string{} ) {
    buf << "(" << node.name() << ")";
  }
  return buf.str();
}

END_NONAMESPACE

// @brief 内容を出力する．
void
BfModel::print(
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
    s << node_name(node)
      << ": INPUT#" << node.input_id() << "[" << input_name(i) << "]" << endl;
  }
  for ( SizeType i = 0; i < output_num(); ++ i ) {
    s << "OUTPUT#" << i << "[" << output_name(i) << "]"
      << " = " << "Node#" << output(i).id() << endl;
  }
  for ( SizeType i = 0; i < dff_num(); ++ i ) {
    auto node = dff(i);
    s << node_name(node);
    s << ": DFF#" << i << "[" << node.name() << "]"
      << " src = " << "Node#" << node.dff_src().id();
    if ( node.dff_rval() != ' ' ) {
      s << ", rval = " << node.dff_rval();
    }
    s << endl;
  }
  for ( auto node: logic_list() ) {
    s << node_name(node)
      << ": ";
    if ( node.is_primitive() ) {
      s << node.primitive_type();
    }
    else if ( node.is_aig() ) {
      s << "AIG[" << node.fanin_inv(0) << node.fanin_inv(1) << "]";
    }
    else if ( node.is_cover() ) {
      s << "Cover#" << node.cover_id();
    }
    else if ( node.is_expr() ) {
      s << "Expr#" << node.expr_id();
    }
    else if ( node.is_cell() ) {
      s << "Cell#" << node.cell_id();
    }
    else {
      ASSERT_NOT_REACHED;
    }
    s << " (";
    for ( auto inode: node.fanin_list() ) {
      s << " " << inode.id();
    }
    s << ")" << endl;
  }
  if ( cover_num() > 0 ) {
    s << endl;
    for ( SizeType id = 0; id < cover_num(); ++ id ) {
      s << "Cover#" << id << ":" << endl;
      auto& cover = this->cover(id);
      cover.print(s);
    }
  }
  if ( expr_num() > 0 ) {
    s << endl;
    for ( SizeType id = 0; id < expr_num(); ++ id ) {
      s << "Expr#" << id << ":" << endl
	<< expr(id) << endl;
    }
  }
}

// @brief ID 番号から BfNode を作る．
BfNode
BfModel::from_id(
  SizeType id
) const
{
  return BfNode{mImpl, id};
}

// @brief ID 番号のリストから vector<BfNode> を作る．
vector<BfNode>
BfModel::from_id_list(
  const vector<SizeType>& id_list
) const
{
  vector<BfNode> node_list;
  for ( auto id: id_list ) {
    node_list.push_back(from_id(id));
  }
  return node_list;
}

END_NAMESPACE_YM_BNFE

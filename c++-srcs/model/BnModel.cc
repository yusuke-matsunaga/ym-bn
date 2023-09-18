
/// @file BnModel.cc
/// @brief BnModel の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/BnModel.h"
#include "ym/BnNode.h"
#include "ModelImpl.h"


BEGIN_NAMESPACE_YM_BNET

//////////////////////////////////////////////////////////////////////
// クラス BnModel
//////////////////////////////////////////////////////////////////////

// @brief コピーコンストラクタ
BnModel::BnModel(
  const BnModel& src
) : mImpl{new ModelImpl{*src.mImpl}}
{
}

// @brief ムーブコンストラクタ
BnModel::BnModel(
  BnModel&& src
) : mImpl{src.mImpl}
{
  src.mImpl = nullptr;
}

// @brief コピー代入演算子
BnModel&
BnModel::operator=(
  const BnModel& src
)
{
  if ( this != &src ) {
    delete mImpl;
    mImpl = new ModelImpl{*src.mImpl};
  }
  return *this;
}

// @brief ムーブ代入演算子
BnModel&
BnModel::operator=(
  BnModel&& src
)
{
  mImpl = src.mImpl;
  src.mImpl = nullptr;
  return *this;
}

// @brief コンストラクタ
BnModel::BnModel()
{
  mImpl = new ModelImpl;
}

// @brief デストラクタ
BnModel::~BnModel()
{
  delete mImpl;
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
  return from_id(mImpl->input(pos));
}

// @brief 入力のノードのリストを返す．
vector<BnNode>
BnModel::input_list() const
{
  return from_id_list(mImpl->input_list());
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
  return from_id(mImpl->output(pos));
}

// @brief 出力のノードのリストを返す．
vector<BnNode>
BnModel::output_list() const
{
  return from_id_list(mImpl->output_list());
}

// @brief DFF数を返す．
SizeType
BnModel::dff_num() const
{
  return mImpl->dff_num();
}

// @brief DFFのノードを返す．
BnNode
BnModel::dff(
  SizeType pos
) const
{
  return from_id(mImpl->dff(pos));
}

// @brief DFFのノードのリストを返す．
vector<BnNode>
BnModel::dff_list() const
{
  return from_id_list(mImpl->dff_list());
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
  return from_id(mImpl->logic(pos));
}

// @brief 論理ノードのリストを返す．
vector<BnNode>
BnModel::logic_list() const
{
  return from_id_list(mImpl->logic_list());
}

// @brief カバーの種類の数を返す．
SizeType
BnModel::cover_num() const
{
  return mImpl->cover_num();
}

// @brief カバーを取り出す．
const BnCover&
BnModel::cover(
  SizeType cover_id
) const
{
  return mImpl->cover(cover_id);
}

// @brief 論理式の数を返す．
SizeType
BnModel::expr_num() const
{
  return mImpl->expr_num();
}

// @brief 論理式を返す．
const Expr&
BnModel::expr(
  SizeType expr_id
) const
{
  return mImpl->expr(expr_id);
}

// @brief 内容を出力する．
void
BnModel::print(
  ostream& s
) const
{
  s << "Name: " << name() << endl;
  s << "Input: ";
  for ( auto node: input_list() ) {
    s << " " << node.id();
  }
  s << endl;
  s << "Output: ";
  for ( auto node: output_list() ) {
    s << " " << node.id();
  }
  s << endl;
  for ( auto node: dff_list() ) {
    s << " " << node.id() << " = DFF(" << node.dff_src().id() << ")"
      << ": rval = " << node.dff_rval() << endl;
  }

  for ( auto node: logic_list() ) {
    s << " " << node.id() << " = Logic(";
    for ( auto inode: node.fanin_list() ) {
      s << " " << inode.id();
    }
    s << "): ";
    if ( node.is_primitive() ) {
      s << "Primitive(" << node.primitive_type() << ")";
    }
    else if ( node.is_aig() ) {
      s << "AIG(" << node.fanin_inv(0) << node.fanin_inv(1) << ")";
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
    s << endl;
  }
  s << endl;
  for ( SizeType id = 0; id < cover_num(); ++ id ) {
    s << "Cover#" << id << ":" << endl;
    auto& cover = this->cover(id);
    cover.print(s);
  }
  s << endl;
  for ( SizeType id = 0; id < expr_num(); ++ id ) {
    s << "Expr#" << id << ":" << endl
      << expr(id) << endl;
  }
}

// @brief ID 番号から BnNode を作る．
BnNode
BnModel::from_id(
  SizeType id
) const
{
  return BnNode{mImpl, id};
}

// @brief ID 番号のリストから vector<BnNode> を作る．
vector<BnNode>
BnModel::from_id_list(
  const vector<SizeType>& id_list
) const
{
  vector<BnNode> node_list;
  for ( auto id: id_list ) {
    node_list.push_back(from_id(id));
  }
  return node_list;
}

END_NAMESPACE_YM_BNET

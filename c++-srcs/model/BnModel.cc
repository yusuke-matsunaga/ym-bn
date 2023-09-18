
/// @file BnModel.cc
/// @brief BnModel の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/BnModel.h"
#include "ModelImpl.h"


BEGIN_NAMESPACE_YM_BLIF

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
const string&
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
  return from_id(mImpl->logic());
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
const BlifCover&
BnModel::cover(
  SizeType cover_id
) const
{
  return mImpl->cover(cover_id);
}

// @brief 内容を出力する．
void
BnModel::print(
  ostream& s
) const
{
  s << "Name: " << name() << endl;
  s << "Input: ";
  for ( auto id: input_list() ) {
    s << " " << id;
  }
  s << endl;
  s << "Output: ";
  for ( auto id: output_list() ) {
    s << " " << id;
  }
  s << endl;
  for ( auto id: dff_list() ) {
    s << " " << id << " = DFF(" << node_input(id) << ")"
      << ": rval = " << node_rval(id) << endl;
  }

  SizeType max_cover = 0;
  for ( auto id: logic_list() ) {
    s << " " << id << " = Logic(";
    for ( auto iid: node_fanin_list(id) ) {
      s << " " << iid;
    }
    s << "): ";
    if ( node_type(id) == Cover ) {
      SizeType cover_id = node_cover_id(id);
      s << "cover = " << cover_id;
      max_cover = std::max(max_cover, cover_id);
    }
    else if ( node_type(id) == Cell ) {
      s << "cell = " << node_cell_id(id);
    }
    else {
      ASSERT_NOT_REACHED;
    }
    s << endl;
  }

  s << endl;
  for ( SizeType id = 0; id <= max_cover; ++ id ) {
    s << "Cover#" << id << ":" << endl;
    auto& cover = this->cover(id);
    cover.print(s);
  }
}

END_NAMESPACE_YM_BLIF

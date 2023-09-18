
/// @file ModelImpl.cc
/// @brief ModelImpl の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ModelImpl.h"


BEGIN_NAMESPACE_YM_BNET

// @brief 名前を設定する．
void
ModelImpl::set_name(
  const string& name
)
{
  mName = name;
}

// @brief コメントを設定する．
void
ModelImpl::set_comment(
  const string& comment
)
{
  mComment = comment;
}

// @brief 出力の名前を設定する．
void
ModelImpl::set_output_name(
  SizeType pos,
  const string& name
)
{
  ASSERT_COND( 0 <= pos && pos < output_num() );
  mOutputNameList[pos] = name;
}

// @brief 新しいノードを作る．
SizeType
ModelImpl::new_node(
  const string& name
)
{
  auto id = mNodeArray.size();
  mNodeArray.push_back({name});
  return id;
}

// @brief ノードに名前をつける．
void
ModelImpl::set_node_name(
  SizeType id,
  const string& name
)
{
  auto& node = _node(id);
  node.set_name(name);
}

// @brief 対応するID番号に入力用の印を付ける．
void
ModelImpl::set_input(
  SizeType id
)
{
  auto& node = _node(id);
  node.set_input();
  mInputList.push_back(id);
}

// @brief 対応するID番号に出力用の印をつける．
void
ModelImpl::set_output(
  SizeType id
)
{
  mOutputList.push_back(id);
}

// @brief プリミティブ型のノードの情報をセットする．
void
ModelImpl::set_primitive(
  SizeType id,
  const vector<SizeType>& input_list,
  PrimType type
)
{
  auto& node = _node(id);
  node.set_primitive(input_list, type);
}

// @brief AIG型のノードの情報をセットする．
void
ModelImpl::set_aig(
  SizeType id,
  SizeType src0,
  SizeType src1,
  bool inv0,
  bool inv1
)
{
  auto& node = _node(id);
  node.set_aig(src0, src1, inv0, inv1);
}

// @brief カバー型のノードの情報をセットする．
void
ModelImpl::set_cover(
  SizeType id,
  const vector<SizeType>& input_list,
  SizeType cover_id
)
{
  auto& node = _node(id);
  node.set_cover(input_list, cover_id);
}

// @brief セル型のノードの情報をセットする．
void
ModelImpl::set_cell(
  SizeType id,
  const vector<SizeType>& input_list,
  SizeType cell_id
)
{
  auto& node = _node(id);
  node.set_cell(input_list, cell_id);
}

// @brief DFF型のノードの情報をセットする．
void
ModelImpl::set_dff(
  SizeType id,
  SizeType src_id,
  char rval
)
{
  auto& node = _node(id);
  node.set_dff(src_id, rval);
  mDffList.push_back(id);
}

// @brief 論理ノードのリストを作る．
void
ModelImpl::make_logic_list()
{
  unordered_set<SizeType> mark;

  // 入力ノードに印をつける．
  for ( auto id: mInputList ) {
    mark.emplace(id);
  }

  // ラッチノードに印を作る．
  for ( auto id: mDffList ) {
    mark.emplace(id);
  }

  // 出力ノードからファンインをたどり
  // post-order で番号をつける．
  // 結果としてノードは入力からのトポロジカル順
  // に整列される．
  for ( auto id: mOutputList ) {
    order_node(id, mark);
  }

  // ラッチノードのファンインに番号をつける．
  for ( auto id: mDffList ) {
    auto& node = _node(id);
    order_node(node.dff_src(), mark);
  }

}

// @brief トポロジカルソートを行い mLogicList にセットする．
void
ModelImpl::order_node(
  SizeType id,
  unordered_set<SizeType>& mark
)
{
  if ( mark.count(id) > 0 ) {
    return;
  }
  auto& node = _node(id);
  ASSERT_COND( node.is_logic() );
  for ( auto iid: node.fanin_list() ) {
    order_node(iid, mark);
  }
  mLogicList.push_back(id);
  mark.emplace(id);
}

// @brief カバーを追加する．
SizeType
ModelImpl::add_cover(
  SizeType input_num,
  const vector<vector<Literal>>& cube_list,
  char opat
)
{
  auto id = cover_num();
  mCoverArray.push_back(BnCover{input_num, cube_list, opat});

  return id;
}

END_NAMESPACE_YM_BNET

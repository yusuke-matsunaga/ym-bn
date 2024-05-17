
/// @file ModelImpl.cc
/// @brief ModelImpl の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ModelImpl.h"
#include "NodeImpl_sub.h"
#include "SeqImpl_sub.h"
#include "ym/Bdd.h"


BEGIN_NAMESPACE_YM_BN

// @brief コンストラクタ
ModelImpl::ModelImpl()
{
}

// @brief コピーコンストラクタ
ModelImpl::ModelImpl(
  const ModelImpl& src
) : mLibrary{src.mLibrary},
    mName{src.mName},
    mComment{src.mComment},
    mSymbolDict{src.mSymbolDict},
    mInputList{src.mInputList},
    mOutputList{src.mOutputList},
    mLogicList{src.mLogicList},
    mNodeArray(src.mNodeArray.size()),
    mSeqArray(src.mSeqArray.size()),
    mFuncArray(src.mFuncArray.size())
{
  for ( SizeType i = 0; i < src.mNodeArray.size(); ++ i ) {
    mNodeArray[i] = src.mNodeArray[i]->copy();
  }
  for ( SizeType i = 0; i < src.mSeqArray.size(); ++ i ) {
    mSeqArray[i] = src.mSeqArray[i]->copy();
  }
  for ( SizeType i = 0; i < src.mFuncArray.size(); ++ i ) {
    mFuncArray[i] = src.mFuncArray[i]->copy(mBddMgr);
  }
}

// @brief デストラクタ
ModelImpl::~ModelImpl()
{
}

// @brief オプション情報を返す．
JsonValue
ModelImpl::option() const
{
  unordered_map<string, JsonValue> src_dict;
  if ( name() != string{} ) {
    src_dict.emplace("name", JsonValue{name()});
  }
  if ( comment() != string{} ) {
    src_dict.emplace("comment", JsonValue{comment()});
  }
  if ( !mSymbolDict.empty() ) {
    unordered_map<string, JsonValue> symbol_dict;
    for ( auto& p: mSymbolDict ) {
      auto key = p.first;
      auto value = p.second;
      symbol_dict.emplace(key, JsonValue{value});
    }
    src_dict.emplace("symbol_dict", JsonValue{symbol_dict});
  }
  return JsonValue{src_dict};
}

// @brief 内容をクリアする．
void
ModelImpl::clear()
{
  mLibrary = ClibCellLibrary{};
  mName = string{};
  mComment = string{};
  mSymbolDict.clear();
  mInputList.clear();
  mOutputList.clear();
  mLogicList.clear();
  mNodeArray.clear();
  mSeqArray.clear();
  mFuncArray.clear();
  // BddMgr に関しては完全な初期化は行えない．
}

// @brief セルライブラリを設定する．
void
ModelImpl::set_library(
  ClibCellLibrary library
)
{
  mLibrary = library;
}

// @brief オプション情報をセットする．
void
ModelImpl::set_option(
  const JsonValue& option
)
{
  if ( option.has_key("name") ) {
    set_name(option.at("name").get_string());
  }
  if ( option.has_key("comment") ) {
    set_comment(option.at("comment").get_string());
  }
  if ( option.has_key("symbol_dict") ) {
    for ( auto& p: option.item_list() ) {
      auto key = p.first;
      auto value = p.second.get_string();
      mSymbolDict.emplace(key, value);
    }
  }
}

// @brief 対応するID番号に入力用の印を付ける．
void
ModelImpl::set_input(
  SizeType id
)
{
  auto iid = mInputList.size();
  auto node = new NodeImpl_Input{iid};
  mNodeArray[id] = unique_ptr<NodeImpl>{node};
  mInputList.push_back(id);
}

// @brief 対応するID番号にDFFの出力用の印を付ける．
void
ModelImpl::set_seq_output(
  SizeType id,            ///< [in] ID番号
  SizeType seq_id         ///< [in] DFF/Latch番号
)
{
  auto node = new NodeImpl_SeqOutput{seq_id};
  mNodeArray[id] = unique_ptr<NodeImpl>{node};
}

// @brief プリミティブ型のノードの情報をセットする．
void
ModelImpl::set_primitive(
  SizeType id,
  const vector<SizeType>& input_list,
  PrimType type
)
{
  auto node = new NodeImpl_Primitive{type, input_list};
  mNodeArray[id] = unique_ptr<NodeImpl>{node};
  mLogicList.push_back(id);
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
  auto node = new NodeImpl_Aig{src0, inv0, src1, inv1};
  mNodeArray[id] = unique_ptr<NodeImpl>{node};
  mLogicList.push_back(id);
}

// @brief 関数型のノードの情報をセットする．
void
ModelImpl::set_func(
  SizeType id,
  const vector<SizeType>& input_list,
  SizeType func_id
)
{
  auto node = new NodeImpl_Func{func_id, input_list};
  mNodeArray[id] = unique_ptr<NodeImpl>{node};
  mLogicList.push_back(id);
}

// @brief セル型のノードの情報をセットする．
void
ModelImpl::set_cell(
  SizeType id,
  const vector<SizeType>& input_list,
  ClibCell cell
)
{
  auto node = new NodeImpl_Cell{cell, input_list};
  mNodeArray[id] = unique_ptr<NodeImpl>{node};
  mLogicList.push_back(id);
}

#if 0
// @brief 新しい入力ノードを作る．
SizeType
ModelImpl::new_input()
{
  auto id = mNodeArray.size();
  mNodeArray.push_back({});
  set_input(id);
  return id;
}
#endif

// @brief 新しい出力ノードを作る．
SizeType
ModelImpl::new_output(
  SizeType src_id
)
{
  auto oid = mOutputList.size();
  mOutputList.push_back(src_id);
  return oid;
}

#if 0
// @brief 新しいプリミティブ型の論理ノードを作る．
SizeType
ModelImpl::new_primitive(
  const vector<SizeType>& input_list,
  PrimType type
)
{
  auto node = new NodeImpl_Primitive{type, input_list};
  auto id = reg_node(node);
  mLogicList.push_back(id);
  return id;
}

// @brief 新しいAIG型の論理ノードを作る．
SizeType
ModelImpl::new_aig(
  SizeType src0,
  SizeType src1,
  bool inv0,
  bool inv1
)
{
  auto node = new NodeImpl_Aig{src0, inv0, src1, inv1};
  auto id = reg_node();
  mLogicList.push_back(id);
  return id;
}

// @brief 関数型のノードの情報をセットする．
SizeType
ModelImpl::new_func(
  const vector<SizeType>& input_list,
  SizeType func_id
)
{
  auto node = new NodeImpl_Func{func_id, input_list};
  auto id = reg_node(node);
  mLogicList.push_back(id);
  return id;
}

// @brief セル型のノードの情報をセットする．
SizeType
ModelImpl::new_cell(
  const vector<SizeType>& input_list,
  ClibCell cell
)
{
  auto node = new NodeImpl_Cell{cell, input_list};
  auto id = reg_node(node);
  mLogicList.push_back(id);
  return id;
}
#endif

// @brief DFFを作る．
SizeType
ModelImpl::new_dff(
  char rs_val,
  const string& name
)
{
  auto seq = new SeqImpl_DFF{rs_val};
  return _reg_seq(seq, name);
}

// @brief ラッチを作る．
SizeType
ModelImpl::new_latch(
  char rs_val,
  const string& name
)
{
  auto seq = new SeqImpl_Latch{rs_val};
  return _reg_seq(seq, name);
}

// @brief cell タイプの DFF を作る．
SizeType
ModelImpl::new_seq_cell(
  ClibCell cell,
  const string& name
)
{
  set_library(cell.library());
  auto seq = new SeqImpl_Cell(cell);
  return _reg_seq(seq, name);
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

  // DFFの出力に印を作る．
  for ( auto& seq: mSeqArray ) {
    auto id = seq->data_output();
    mark.emplace(id);
  }

  // 出力ノードからファンインをたどり
  // post-order で番号をつける．
  // 結果としてノードは入力からのトポロジカル順
  // に整列される．
  for ( auto id: mOutputList ) {
    order_node(id, mark);
  }

  // DFFのファンインに番号をつける．
  for ( auto& seq: mSeqArray ) {
    order_node(seq->data_src(), mark);
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
  //auto& node = _node(id, "", "");
  auto node = mNodeArray[id].get();
  ASSERT_COND( node->is_logic() );
  for ( auto iid: node->fanin_list() ) {
    order_node(iid, mark);
  }
  mLogicList.push_back(id);
  mark.emplace(id);
}

// @brief カバーを登録する．
SizeType
ModelImpl::reg_cover(
  SizeType input_num,
  const vector<vector<Literal>>& cube_list,
  char opat
)
{
  auto id = func_num();
  auto func = FuncImpl::new_cover(input_num, cube_list, opat);
  mFuncArray.push_back(unique_ptr<FuncImpl>(func));
  return id;
}

// @brief 論理式を登録する．
SizeType
ModelImpl::reg_expr(
  const Expr& expr
)
{
  auto id = func_num();
  auto func = FuncImpl::new_expr(expr);
  mFuncArray.push_back(unique_ptr<FuncImpl>(func));
  return id;
}

// @brief 真理値表を登録する．
SizeType
ModelImpl::reg_tvfunc(
  const TvFunc& tvfunc
)
{
  auto id = func_num();
  auto func = FuncImpl::new_tvfunc(tvfunc);
  mFuncArray.push_back(unique_ptr<FuncImpl>(func));
  return id;
}

// @brief BDDを登録する．
SizeType
ModelImpl::reg_bdd(
  const Bdd& bdd
)
{
  auto id = func_num();
  // mBddMgr に属する BDD に変換しておく．
  auto my_bdd = mBddMgr.copy(bdd);
  auto func = FuncImpl::new_bdd(my_bdd);
  mFuncArray.push_back(unique_ptr<FuncImpl>(func));
  return id;
}

END_NAMESPACE_YM_BN

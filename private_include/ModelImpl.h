#ifndef MODELIMPL_H
#define MODELIMPL_H

/// @file ModelImpl.h
/// @brief ModelImpl のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/bn.h"
#include "ym/BnCover.h"
#include "ym/Expr.h"
#include "ym/TvFunc.h"
#include "ym/Bdd.h"
#include "ym/BddMgr.h"
#include "ym/ClibCellLibrary.h"
#include "ym/ClibCell.h"
#include "ym/JsonValue.h"
#include "NodeImpl.h"
#include "SeqImpl.h"


BEGIN_NAMESPACE_YM_BN

//////////////////////////////////////////////////////////////////////
/// @class ModelImpl ModelImpl.h "ModelImpl.h"
/// @brief BnModel の内部情報を表すクラス
//////////////////////////////////////////////////////////////////////
class ModelImpl
{
public:

  /// @brief コンストラクタ
  ModelImpl();

  /// @brief コピーコンストラクタ
  ModelImpl(
    const ModelImpl& src ///< [in] コピー元のオブジェクト
  );

  /// @brief デストラクタ
  ~ModelImpl() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 内容を取得する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief セルライブラリを返す．
  ClibCellLibrary
  library() const
  {
    return mLibrary;
  }

  /// @brief オプション情報を返す．
  JsonValue
  option() const;

  /// @brief 名前を返す．
  string
  name() const
  {
    return mName;
  }

  /// @brief コメントを返す．
  string
  comment() const
  {
    return mComment;
  }

  /// @brief 入力名を返す．
  string
  input_name(
    SizeType input_id ///< [in] 入力番号 ( 0 <= input_id < input_num() )
  ) const
  {
    _check_range(input_id, input_num(), "input_name(input_id)", "input_id");
    auto key = _input_key(input_id);
    if ( mSymbolDict.count(key) > 0 ) {
      return mSymbolDict.at(key);
    }
    return string{};
  }

  /// @brief 出力名を返す．
  string
  output_name(
    SizeType output_id ///< [in] 出力番号 ( 0 <= output_id < output_num() )
  ) const
  {
    _check_range(output_id, output_num(), "output_name(output_id)", "output_id");
    auto key = _output_key(output_id);
    if ( mSymbolDict.count(key) > 0 ) {
      return mSymbolDict.at(key);
    }
    return string{};
  }

  /// @brief ラッチ名を返す．
  string
  seq_name(
    SizeType seq_id ///< [in] ラッチ番号 ( 0 <= seq_id < seq_num() )
  ) const
  {
    _check_range(seq_id, seq_num(), "seq_name(seq_id)", "seq_id");
    auto key = _seq_key(seq_id);
    if ( mSymbolDict.count(key) > 0 ) {
      return mSymbolDict.at(key);
    }
    return string{};
  }

  /// @brief ノード数を返す．
  SizeType
  node_num() const
  {
    return mNodeArray.size();
  }

  /// @brief 入力数を返す．
  SizeType
  input_num() const
  {
    return mInputList.size();
  }

  /// @brief 入力のノード番号を返す．
  SizeType
  input(
    SizeType pos ///< [in] 位置 ( 0 <= pos < input_num() )
  ) const
  {
    _check_range(pos, input_num(), "input(pos)", "pos");
    return mInputList[pos];
  }

  /// @brief 入力のノード番号のリストを返す．
  const vector<SizeType>&
  input_list() const
  {
    return mInputList;
  }

  /// @brief 出力数を返す．
  SizeType
  output_num() const
  {
    return mOutputList.size();
  }

  /// @brief 出力のノード番号を返す．
  SizeType
  output(
    SizeType pos ///< [in] 位置 ( 0 <= pos < output_num() )
  ) const
  {
    _check_range(pos, output_num(), "output(pos)", "pos");
    return mOutputList[pos];
  }

  /// @brief 出力のノード番号のリストを返す．
  const vector<SizeType>&
  output_list() const
  {
    return mOutputList;
  }

  /// @brief 論理ノード数を返す．
  SizeType
  logic_num() const
  {
    return mLogicList.size();
  }

  /// @brief 論理ノードのノード番号を返す．
  SizeType
  logic(
    SizeType pos ///< [in] 位置 ( 0 <= pos < logic_num() )
  ) const
  {
    _check_range(pos, logic_num(), "logic(pos)", "pos");
    return mLogicList[pos];
  }

  /// @brief 論理ノード番号のリストを返す．
  const vector<SizeType>&
  logic_list() const
  {
    return mLogicList;
  }

  /// @brief ノードを取り出す．
  const NodeImpl&
  node(
    SizeType id ///< [in] ID番号
  ) const
  {
    _check_range(id, node_num(), "node(id)", "id");
    return mNodeArray[id];
  }

  /// @brief SEQノード数を返す．
  SizeType
  seq_num() const
  {
    return mSeqArray.size();
  }

  /// @brief SEQノードを取り出す．
  const SeqImpl&
  seq_node(
    SizeType id ///< [in] ID番号
  ) const
  {
    _check_range(id, seq_num(), "seq_node(id)", "id");
    return mSeqArray[id];
  }

  /// @brief カバーの種類の数を返す．
  SizeType
  cover_num() const
  {
    return mCoverArray.size();
  }

  /// @brief カバーを取り出す．
  const BnCover&
  cover(
    SizeType cover_id ///< [in] カバー番号
  ) const
  {
    _check_range(cover_id, cover_num(), "cover(cover_id)", "cover_id");
    return mCoverArray[cover_id];
  }

  /// @brief 論理式の数を返す．
  SizeType
  expr_num() const
  {
    return mExprArray.size();
  }

  /// @brief 論理式を取り出す．
  const Expr&
  expr(
    SizeType expr_id ///< [in] 論理式番号
  ) const
  {
    _check_range(expr_id, expr_num(), "expr(expr_id)", "expr_id");
    return mExprArray[expr_id];
  }

  /// @brief 真理値表型の関数の数を返す．
  SizeType
  func_num() const
  {
    return mFuncArray.size();
  }

  /// @brief 真理値表型の関数を返す．
  const TvFunc&
  func(
    SizeType func_id ///< [in] 関数番号 ( 0 <= func_id < func_num() )
  ) const
  {
    _check_range(func_id, func_num(), "func(func_id)", "func_id");
    return mFuncArray[func_id];
  }

  /// @brief BDDの数を返す．
  SizeType
  bdd_num() const
  {
    return mBddArray.size();
  }

  /// @brief BDDを返す．
  const Bdd&
  bdd(
    SizeType bdd_id ///< [in] BDD番号 ( 0 <= bdd_id < bdd_num() )
  ) const
  {
    _check_range(bdd_id, bdd_num(), "bdd(bdd_id)", "bdd_id");
    return mBddArray[bdd_id];
  }


public:
  //////////////////////////////////////////////////////////////////////
  // 設定用の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容をクリアする．
  void
  clear();

  /// @brief セルライブラリを設定する．
  ///
  /// 既に別のライブラリが設定されていたらエラーとなる．
  void
  set_library(
    ClibCellLibrary library ///< [in] ライブラリ
  );

  /// @brief オプション情報をセットする．
  void
  set_option(
    const JsonValue& option ///< [in] 設定するオプション
  );

  /// @brief 名前を設定する．
  void
  set_name(
    const string& name ///< [in] 名前
  )
  {
    mName = name;
  }

  /// @brief コメントを設定する．
  void
  set_comment(
    const string& comment ///< [in] コメント
  )
  {
    mComment = comment;
  }

  /// @brief 入力名をセットする．
  void
  set_input_name(
    SizeType input_id, ///< [in] 入力番号 ( 0 <= input_id < input_num() )
    const string& name ///< [in] 名前
  )
  {
    _check_range(input_id, input_num(), "set_input_name(input_id, name)", "input_id");
    auto key = _input_key(input_id);
    mSymbolDict.emplace(key, name);
  }

  /// @brief 出力名をセットする．
  void
  set_output_name(
    SizeType output_id, ///< [in] 出力番号 ( 0 <= output_id < output_num() )
    const string& name  ///< [in] 名前
  )
  {
    _check_range(output_id, output_num(), "set_output_name(output_id, name)", "output_id");
    auto key = _output_key(output_id);
    mSymbolDict.emplace(key, name);
  }

  /// @brief SEQ名をセットする．
  void
  set_seq_name(
    SizeType seq_id,   ///< [in] SEQ番号 ( 0 <= seq_id < seq_num() )
    const string& name ///< [in] 名前
  )
  {
    _check_range(seq_id, seq_num(), "set_seq_name(seq_id, name)", "seq_id");
    auto key = _seq_key(seq_id);
    mSymbolDict.emplace(key, name);
  }

  /// @brief 対応するID番号に入力用の印を付ける．
  void
  set_input(
    SizeType id            ///< [in] ID番号
  )
  {
    auto& node = _node(id, "set_input(id)", "id");
    auto iid = mInputList.size();
    mInputList.push_back(id);
    node.set_input(iid);
  }

  /// @brief 出力ノードをセットする．
  void
  set_output(
    SizeType pos,           ///< [in] 位置番号 ( 0 <= pos < output_num() )
    SizeType src_id         ///< [in] ソースのID番号
  )
  {
    _check_range(pos, output_num(), "set_output(pos, src_id)", "pos");
    _check_range(src_id, node_num(), "set_output(pos, src_id)", "src_id");
    mOutputList[pos] = src_id;
  }

  /// @brief 新しいノードを作る．
  ///
  /// @return ID番号を返す．
  SizeType
  new_node()
  {
    auto id = mNodeArray.size();
    mNodeArray.push_back(NodeImpl{});
    return id;
  }

  /// @brief 新しい入力ノードを作る．
  ///
  /// @return ID番号を返す．
  SizeType
  new_input()
  {
    auto id = new_node();
    set_input(id);
    return id;
  }

  /// @brief 新しい BnSeq の出力ノードを作る．
  ///
  /// @return ID番号を返す．
  SizeType
  new_seq_output(
    SizeType seq_id        ///< [in] BnSeq の ID 番号
  )
  {
    _check_range(seq_id, seq_num(), "new_seq_output(seq_id)", "seq_id");
    auto id = new_node();
    auto& node = mNodeArray[id];
    node.set_seq_output(seq_id);
    return id;
  }

  /// @brief 新しい出力ノードを作る．
  ///
  /// @return 出力番号を返す．
  SizeType
  new_output(
    SizeType src_id ///< [in] ソースのID番号
  )
  {
    _check_range(src_id, mNodeArray.size(), "new_output(src_id)", "src_id");
    auto oid = mOutputList.size();
    mOutputList.push_back(src_id);
    return oid;
  }

  /// @brief 新しいプリミティブ型の論理ノードを作る．
  ///
  /// @return ID番号を返す．
  SizeType
  new_primitive(
    const vector<SizeType>& input_list, ///< [in] 入力の識別子番号のリスト
    PrimType type                       ///< [in] プリミティブタイプ
  )
  {
    auto id = new_node();
    set_primitive(id, input_list, type);
    mLogicList.push_back(id);
    return id;
  }

  /// @brief 新しいAIG型の論理ノードを作る．
  ///
  /// @return ID番号を返す．
  SizeType
  new_aig(
    SizeType src0,          ///< [in] ソース0のID番号
    SizeType src1,          ///< [in] ソース1のID番号
    bool inv0,              ///< [in] ソース0の反転属性
    bool inv1               ///< [in] ソース1の反転属性
  )
  {
    auto id = new_node();
    set_aig(id, src0, src1, inv0, inv1);
    mLogicList.push_back(id);
    return id;
  }

  /// @brief 新しいカバー型の論理ノードを作る．
  ///
  /// @return ID番号を返す．
  SizeType
  new_cover(
    const vector<SizeType>& input_list, ///< [in] 入力の識別子番号のリスト
    SizeType cover_id                   ///< [in] カバー番号
  )
  {
    auto id = new_node();
    set_cover(id, input_list, cover_id);
    mLogicList.push_back(id);
    return id;
  }

  /// @brief 論理式型のノードの情報をセットする．
  ///
  /// @return ID番号を返す．
  SizeType
  new_expr(
    const vector<SizeType>& input_list, ///< [in] 入力の識別子番号のリスト
    SizeType expr_id                    ///< [in] 論理式番号
  )
  {
    auto id = new_node();
    set_expr(id, input_list, expr_id);
    mLogicList.push_back(id);
    return id;
  }

  /// @brief 真理値表型のノードの情報をセットする．
  ///
  /// @return ID番号を返す．
  SizeType
  new_func(
    const vector<SizeType>& input_list, ///< [in] 入力の識別子番号のリスト
    SizeType func_id                    ///< [in] 関数番号
  )
  {
    auto id = new_node();
    set_func(id, input_list, func_id);
    mLogicList.push_back(id);
    return id;
  }

  /// @brief BDD型のノードの情報をセットする．
  ///
  /// @return ID番号を返す．
  SizeType
  new_bdd(
    const vector<SizeType>& input_list, ///< [in] 入力の識別子番号のリスト
    SizeType bdd_id                     ///< [in] BDD番号
  )
  {
    auto id = new_node();
    set_bdd(id, input_list, bdd_id);
    mLogicList.push_back(id);
    return id;
  }

  /// @brief セル型のノードの情報をセットする．
  ///
  /// @return ID番号を返す．
  SizeType
  new_cell(
    const vector<SizeType>& input_list, ///< [in] 入力の識別子番号のリスト
    ClibCell cell                       ///< [in] セル
  )
  {
    auto id = new_node();
    set_cell(id, input_list, cell);
    mLogicList.push_back(id);
    return id;
  }

  /// @brief DFFを作る．
  ///
  /// @return ID番号を返す．
  SizeType
  new_dff(
    char rs_val = ' ',          ///< [in] リセットとプリセットが共にオンの時の値
    SizeType output_id = BAD_ID ///< [in] 出力のノード番号
  )
  {
    auto id = seq_num();
    auto& seq = _new_seq();
    if ( output_id == BAD_ID ) {
      output_id = new_seq_output(id);
    }
    seq.set_dff(rs_val, output_id);
    return id;
  }

  /// @brief ラッチを作る．
  ///
  /// @return ID番号を返す．
  SizeType
  new_latch(
    char rs_val = ' ',          ///< [in] リセットとプリセットが共にオンの時の値
    SizeType output_id = BAD_ID ///< [in] 出力のノード番号
  )
  {
    auto id = seq_num();
    auto& seq = _new_seq();
    if ( output_id == BAD_ID ) {
      output_id = new_seq_output(id);
    }
    seq.set_latch(rs_val, output_id);
    return id;
  }

  /// @brief cell タイプの DFF を作る．
  ///
  /// @return ID番号を返す．
  SizeType
  new_seq_cell(
    ClibCell cell             ///< [in] セル番号
  )
  {
  auto id = seq_num();
    set_library(cell.library());
    auto& seq = _new_seq();
    seq.set_cell(cell.id(), cell.pin_num());
    return id;
  }

  /// @brief プリミティブ型のノードの情報をセットする．
  void
  set_primitive(
    SizeType id,                        ///< [in] ID番号
    const vector<SizeType>& input_list, ///< [in] 入力の識別子番号のリスト
    PrimType type                       ///< [in] プリミティブタイプ
  )
  {
    auto& node = _node(id, "set_primitive(id, input_list, type)", "id");
    node.set_primitive(input_list, type);
  }

  /// @brief AIG型のノードの情報をセットする．
  void
  set_aig(
    SizeType id,   ///< [in] ID番号
    SizeType src0, ///< [in] ソース0のID番号
    SizeType src1, ///< [in] ソース1のID番号
    bool inv0,     ///< [in] ソース0の反転属性
    bool inv1      ///< [in] ソース1の反転属性
  )
  {
    auto& node = _node(id, "set_aig(id, src0, src1, inv0, inv1)", "id");
    node.set_aig(src0, src1, inv0, inv1);
  }

  /// @brief カバー型のノードの情報をセットする．
  void
  set_cover(
    SizeType id,                        ///< [in] ID番号
    const vector<SizeType>& input_list, ///< [in] 入力の識別子番号のリスト
    SizeType cover_id                   ///< [in] カバー番号
  )
  {
    auto& node = _node(id, "set_cover(id, input_list, cover_id)", "id");
    node.set_cover(input_list, cover_id);
  }

  /// @brief 論理式型のノードの情報をセットする．
  void
  set_expr(
    SizeType id,                        ///< [in] ID番号
    const vector<SizeType>& input_list, ///< [in] 入力の識別子番号のリスト
    SizeType expr_id                    ///< [in] 論理式番号
  )
  {
    auto& node = _node(id, "set_expr(id, input_list, expr_id)", "id");
    node.set_expr(input_list, expr_id);
  }

  /// @brief 真理値表型のノードの情報をセットする．
  void
  set_func(
    SizeType id,                        ///< [in] ID番号
    const vector<SizeType>& input_list, ///< [in] 入力の識別子番号のリスト
    SizeType func_id                    ///< [in] 関数番号
  )
  {
    auto& node = _node(id, "set_func(id, input_list, func_id)", "id");
    node.set_func(input_list, func_id);
  }

  /// @brief BDD型のノードの情報をセットする．
  void
  set_bdd(
    SizeType id,                        ///< [in] ID番号
    const vector<SizeType>& input_list, ///< [in] 入力の識別子番号のリスト
    SizeType bdd_id                     ///< [in] BDD番号
  )
  {
    auto& node = _node(id, "set_bdd(id, input_list, bdd_id)", "id");
    node.set_bdd(input_list, bdd_id);
  }

  /// @brief セル型のノードの情報をセットする．
  void
  set_cell(
    SizeType id,                        ///< [in] ID番号
    const vector<SizeType>& input_list, ///< [in] 入力の識別子番号のリスト
    ClibCell cell                       ///< [in] セル
  )
  {
    set_library(cell.library());
    auto& node = _node(id, "set_cell(id, input_list, cell)", "id");
    node.set_cell(input_list, cell.id());
  }

  /// @brief DFF/ラッチのソースをセットする．
  void
  set_data_src(
    SizeType id,         ///< [in] ID番号
    SizeType src_id      ///< [in] ソースのID番号
  )
  {
    auto& seq = _seq(id, "set_data_src(id, src_id)", "id");
    seq.set_data_src(src_id);
  }

  /// @brief DFFのクロック入力をセットする．
  void
  set_clock(
    SizeType id,         ///< [in] ID番号
    SizeType clock_id    ///< [in] クロックのID番号
  )
  {
    auto& seq= _seq(id, "set_clock(id, check_id)", "id");
    seq.set_clock(clock_id);
  }

  /// @brief ラッチのイネーブル入力をセットする．
  void
  set_enable(
    SizeType id,         ///< [in] ID番号
    SizeType enable_id   ///< [in] イネーブルのID番号
  )
  {
    auto& seq = _seq(id, "set_enable(id, enable_id)", "id");
    seq.set_enable(enable_id);
  }

  /// @brief DFF/ラッチのクリア入力をセットする．
  void
  set_clear(
    SizeType id,         ///< [in] ID番号
    SizeType clear_id    ///< [in] クリアのID番号
  )
  {
    auto& seq = _seq(id, "set_clear(id, clear_id", "id");
    seq.set_clear(clear_id);
  }

  /// @brief DFF/ラッチのプリセット入力をセットする．
  void
  set_preset(
    SizeType id,         ///< [in] ID番号
    SizeType preset_id   ///< [in] プリセットのID番号
  )
  {
    auto& seq = _seq(id, "set_preset(id, preset_id)", "id");
    seq.set_preset(preset_id);
  }

  /// @brief セル型のSEQノードのピンのノードをセットする．
  void
  set_seq_pin(
    SizeType id,         ///< [in] ID番号
    SizeType pos,        ///< [in] ピン番号
    SizeType node_id     ///< [in] ノード番号
  )
  {
    auto& seq = _seq(id, "set_dff_pin(id, pos, node_id)", "id");
    seq.set_cell_pin(pos, node_id);
  }

  /// @brief 論理ノードのリストを作る．
  void
  make_logic_list();

  /// @brief カバーを追加する．
  /// @return カバー番号を返す．
  SizeType
  add_cover(
    SizeType input_num,                       ///< [in] 入力数
    const vector<vector<Literal>>& cube_list, ///< [in] キューブのリスト
    char opat                                 ///< [in] 出力パタン ( '1' or '0' )
  );

  /// @brief 論理式を追加する．
  /// @return 論理式番号を返す．
  SizeType
  add_expr(
    const Expr& expr ///< [in] 論理式
  );

  /// @brief 真理値表を追加する．
  /// @return 関数番号を返す．
  SizeType
  add_func(
    const TvFunc& func ///< [in] 真理値表型の関数
  );

  /// @brief BDDを追加する．
  /// @return BDD番号を返す．
  SizeType
  add_bdd(
    const Bdd& bdd ///< [in] BDD
  );


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief トポロジカルソートを行い mLogicList にセットする．
  void
  order_node(
    SizeType id,                  ///< [in] ID番号
    unordered_set<SizeType>& mark ///< [in] マーク
  );

  /// @brief ノードを取り出す．
  NodeImpl&
  _node(
    SizeType id,              ///< [in] ID番号
    const string& func_name,  ///< [in] 関数名
    const string& index_name  ///< [in] ID番号の変数名
  )
  {
    _check_range(id, mNodeArray.size(), func_name, index_name);
    return mNodeArray[id];
  }

  /// @brief 新しい SeqImpl を割り当てる．
  SeqImpl&
  _new_seq()
  {
    mSeqArray.push_back(SeqImpl{});
    return mSeqArray.back();
  }

  /// @brief SeqImpl を取り出す．
  SeqImpl&
  _seq(
    SizeType id,              ///< [in] ID番号
    const string& func_name,  ///< [in] 関数名
    const string& index_name  ///< [in] ID番号の変数名
  )
  {
    _check_range(id, mSeqArray.size(), func_name, index_name);
    return mSeqArray[id];
  }

  /// @brief 添字の範囲をチェックする．
  void
  _check_range(
    SizeType index,
    SizeType num,
    const string& func_name,
    const string& index_name
  ) const
  {
    if ( index < 0 || index >= num ) {
      ostringstream buf;
      buf << "Error in ModelImpl::" << func_name << "."
	  << " " << index_name << " is out of range.";
      throw std::invalid_argument{buf.str()};
    }
  }

  /// @brief SymbolDict の入力用のキーを作る．
  string
  _input_key(
    SizeType input_id
  ) const
  {
    ostringstream buf;
    buf << "I" << input_id;
    return buf.str();
  }

  /// @brief SymbolDict の出力用のキーを作る．
  string
  _output_key(
    SizeType output_id
  ) const
  {
    ostringstream buf;
    buf << "O" << output_id;
    return buf.str();
  }

  /// @brief SymbolDict のSEQ用のキーを作る．
  string
  _seq_key(
    SizeType seq_id
  ) const
  {
    ostringstream buf;
    buf << "Q" << seq_id;
    return buf.str();
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // セルライブラリ
  ClibCellLibrary mLibrary;

  // 名前
  string mName;

  // コメント
  string mComment;

  // シンボルの辞書
  unordered_map<string, string> mSymbolDict;

  // 入力のノード番号のリスト
  vector<SizeType> mInputList;

  // 出力のノード番号のリスト
  vector<SizeType> mOutputList;

  // 論理ノード番号のリスト
  vector<SizeType> mLogicList;

  // ノードの配列
  vector<NodeImpl> mNodeArray;

  // SEQノードの配列
  vector<SeqImpl> mSeqArray;

  // カバー番号をキーにしてカバーを格納する配列
  vector<BnCover> mCoverArray;

  // 論理式番号をキーにして論理式を格納する配列
  vector<Expr> mExprArray;

  // 関数番号をキーにして真理値表型の関数を格納する配列
  vector<TvFunc> mFuncArray;

  // BDDマネージャ
  BddMgr mBddMgr;

  // BDD番号をキーにしてBDDを格納する配列
  vector<Bdd> mBddArray;

};

END_NAMESPACE_YM_BN

#endif // MODELIMPL_H

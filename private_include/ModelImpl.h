#ifndef MODELIMPL_H
#define MODELIMPL_H

/// @file ModelImpl.h
/// @brief ModelImpl のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/bn.h"
#include "ym/logic.h"
#include "ym/BnNode.h"
#include "ym/BnNodeList.h"
#include "ym/BnSeq.h"
#include "ym/BnFunc.h"
#include "ym/BddMgr.h"
#include "ym/ClibCellLibrary.h"
#include "ym/ClibCell.h"
#include "ym/JsonValue.h"
#include "NodeImpl.h"
#include "SeqImpl.h"
#include "FuncImpl.h"


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
  ~ModelImpl();

  /// @brief 複製を返す．
  ModelImpl*
  copy() const
  {
    return new ModelImpl{*this};
  }


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
  input_id(
    SizeType pos
  ) const
  {
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
  output_id(
    SizeType pos
  ) const
  {
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
  logic_id(
    SizeType pos ///< [in] 位置
  ) const
  {
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
  node_impl(
    SizeType id ///< [in] ID番号
  ) const
  {
    return *mNodeArray[id];
  }

  /// @brief SEQノード数を返す．
  SizeType
  seq_num() const
  {
    return mSeqArray.size();
  }

  /// @brief SEQノードを取り出す．
  const SeqImpl&
  seq_impl(
    SizeType id ///< [in] ID番号
  ) const
  {
    return *mSeqArray[id];
  }

  /// @brief 関数の数を返す．
  SizeType
  func_num() const
  {
    return mFuncArray.size();
  }

  /// @brief 関数情報を返す．
  const FuncImpl&
  func_impl(
    SizeType func_id ///< [in] 関数番号 ( 0 <= func_id < func_num() )
  ) const
  {
    return *mFuncArray[func_id];
  }

  /// @brief BnNode が自分のものか調べる．
  bool
  check(
    BnNode node
  ) const
  {
    return node.mModel.get() == this;
  }

  /// @brief BnSeq が自分のものか調べる．
  bool
  check(
    BnSeq seq
  ) const
  {
    return seq.mModel.get() == this;
  }

  /// @brief BnFunc が自分のものか調べる．
  bool
  check(
    BnFunc func
  ) const
  {
    return func.mModel.get() == this;
  }


public:
  //////////////////////////////////////////////////////////////////////
  // プロキシオブジェクトの生成
  //////////////////////////////////////////////////////////////////////

  /// @brief ノードを生成する．
  static
  BnNode
  new_node(
    const shared_ptr<const ModelImpl>& model,
    SizeType id
  )
  {
    return BnNode{model, id};
  }

  /// @brief ノードリストを生成する．
  static
  BnNodeList
  new_node_list(
    const shared_ptr<const ModelImpl>& model,
    const vector<SizeType>& id_list
  )
  {
    return BnNodeList{model, id_list};
  }

  /// @brief SEQを生成する．
  static
  BnSeq
  new_seq(
    const shared_ptr<const ModelImpl>& model,
    SizeType id
  )
  {
    return BnSeq{model, id};
  }

  /// @brief 論理関数を生成する．
  static
  BnFunc
  new_func(
    const shared_ptr<const ModelImpl>& model,
    SizeType id
  )
  {
    return BnFunc{model, id};
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
    auto key = _seq_key(seq_id);
    mSymbolDict.emplace(key, name);
  }

  /// @brief 対応するID番号に入力用の印を付ける．
  void
  set_input(
    SizeType id            ///< [in] ID番号
  );

  /// @brief 出力ノードをセットする．
  void
  set_output(
    SizeType pos,           ///< [in] 位置番号 ( 0 <= pos < output_num() )
    SizeType src_id         ///< [in] ソースのID番号
  )
  {
    mOutputList[pos] = src_id;
  }

  /// @brief プリミティブ型のノードの情報をセットする．
  void
  set_primitive(
    SizeType id,                        ///< [in] ID番号
    const vector<SizeType>& input_list, ///< [in] 入力の識別子番号のリスト
    PrimType type                       ///< [in] プリミティブタイプ
  );

  /// @brief AIG型のノードの情報をセットする．
  void
  set_aig(
    SizeType id,   ///< [in] ID番号
    SizeType src0, ///< [in] ソース0のID番号
    SizeType src1, ///< [in] ソース1のID番号
    bool inv0,     ///< [in] ソース0の反転属性
    bool inv1      ///< [in] ソース1の反転属性
  );

  /// @brief 関数型のノードの情報をセットする．
  void
  set_func(
    SizeType id,                        ///< [in] ID番号
    const vector<SizeType>& input_list, ///< [in] 入力の識別子番号のリスト
    SizeType func_id                    ///< [in] 関数番号
  );

  /// @brief セル型のノードの情報をセットする．
  void
  set_cell(
    SizeType id,                        ///< [in] ID番号
    const vector<SizeType>& input_list, ///< [in] 入力の識別子番号のリスト
    ClibCell cell                       ///< [in] セル
  );

  /// @brief 新しいノードを登録する．
  ///
  /// @return ID番号を返す．
  SizeType
  new_node()
  {
    auto id = mNodeArray.size();
    mNodeArray.push_back(unique_ptr<NodeImpl>{nullptr});
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
  );
#if 0
  {
    auto id = new_node();
    set_seq_output(id, seq_id);
    return id;
  }
#endif

  /// @brief 新しい出力ノードを作る．
  ///
  /// @return 出力番号を返す．
  SizeType
  new_output(
    SizeType src_id ///< [in] ソースのID番号
  );

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
    return id;
  }

  /// @brief 関数型のノードの情報をセットする．
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
    return id;
  }

  /// @brief DFFを作る．
  ///
  /// @return ID番号を返す．
  SizeType
  new_dff(
    const string& name = string{}, ///< [in] 名前
    SizeType output_id = BAD_ID,   ///< [in] 出力のノード番号
    SizeType clock_id = BAD_ID,    ///< [in] クロックのノード番号
    SizeType clear_id = BAD_ID,    ///< [in] クリアノード番号
    SizeType preset_id = BAD_ID,   ///< [in] プリセットノード番号
    char rs_val = ' '              ///< [in] リセットとプリセットが共にオンの時の値
  );

  /// @brief ラッチを作る．
  ///
  /// @return ID番号を返す．
  SizeType
  new_latch(
    const string& name = string{}, ///< [in] 名前
    SizeType output_id = BAD_ID,   ///< [in] 出力のノード番号
    SizeType enable_id = BAD_ID,   ///< [in] イネーブルのノード番号
    SizeType clear_id = BAD_ID,    ///< [in] クリアノード番号
    SizeType preset_id = BAD_ID,   ///< [in] プリセットノード番号
    char rs_val = ' '              ///< [in] リセットとプリセットが共にオンの時の値
  );

  /// @brief cell タイプの DFF を作る．
  ///
  /// @return ID番号を返す．
  SizeType
  new_seq_cell(
    ClibCell cell                  ///< [in] セル
  );

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

#if 0

  /// @brief DFFのクロック入力をセットする．
  void
  set_clock(
    SizeType id,         ///< [in] ID番号
    SizeType clock_id    ///< [in] クロックのID番号
  )
  {
    auto& seq = _seq(id, "set_clock(id, check_id)", "id");
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
#endif

  /// @brief 論理ノードのリストを作る．
  void
  make_logic_list();

  /// @brief カバーを登録する．
  /// @return 関数番号を返す．
  SizeType
  reg_cover(
    SizeType input_num,                       ///< [in] 入力数
    const vector<vector<Literal>>& cube_list, ///< [in] キューブのリスト
    char opat                                 ///< [in] 出力パタン ( '1' or '0' )
  );

  /// @brief 論理式を登録する．
  /// @return 関数番号を返す．
  SizeType
  reg_expr(
    const Expr& expr ///< [in] 論理式
  );

  /// @brief 真理値表を登録する．
  /// @return 関数番号を返す．
  SizeType
  reg_tvfunc(
    const TvFunc& func ///< [in] 真理値表型の関数
  );

  /// @brief BDDを登録する．
  /// @return 関数番号を返す．
  SizeType
  reg_bdd(
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

#if 0
  /// @brief ノードを取り出す．
  NodeImpl&
  _node(
    SizeType id,              ///< [in] ID番号
    const string& func_name,  ///< [in] 関数名
    const string& index_name  ///< [in] ID番号の変数名
  )
  {
    return mNodeArray[id];
  }

  /// @brief 新しい SeqImpl を割り当てる．
  SeqImpl&
  _new_seq()
  {
    mSeqArray.push_back(SeqImpl{});
    return mSeqArray.back();
  }
#endif

  /// @brief DFFタイプの SeqImpl を割り当てる．
  SeqImpl&
  _new_dff_seq(
    SizeType output_id,
    char rs_val
  );

  /// @brief Latchタイプの SeqImpl を割り当てる．
  SeqImpl&
  _new_latch_seq(
    SizeType output_id,
    char rs_val
  );

  /// @brief セルタイプの SeqImpl を割り当てる．
  SeqImpl&
  _new_cell_seq(
    ClibCell cell
  );

  /// @brief SeqImpl を取り出す．
  SeqImpl&
  _seq(
    SizeType id,              ///< [in] ID番号
    const string& func_name,  ///< [in] 関数名
    const string& index_name  ///< [in] ID番号の変数名
  )
  {
    return *mSeqArray[id];
  }

  /// @brief FuncImpl を取り出す．
  FuncImpl&
  _func(
    SizeType id
  )
  {
    return *mFuncArray[id];
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

  // BDDマネージャ
  BddMgr mBddMgr;

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
  vector<unique_ptr<NodeImpl>> mNodeArray;

  // SEQノードの配列
  vector<unique_ptr<SeqImpl>> mSeqArray;

  // 関数番号をキーにして FuncImpl を格納する配列
  vector<unique_ptr<FuncImpl>> mFuncArray;

};

END_NAMESPACE_YM_BN

#endif // MODELIMPL_H

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
  ModelImpl() = default;

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

  /// @brief 名前を返す．
  const string&
  name() const
  {
    return mName;
  }

  /// @brief コメントを返す．
  const string&
  comment() const
  {
    return mComment;
  }

  /// @brief セルライブラリを返す．
  ClibCellLibrary
  library() const
  {
    return mLibrary;
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

  /// @brief 入力名を返す．
  string
  input_name(
    SizeType pos ///< [in] 位置 ( 0 <= pos < input_num() )
  ) const
  {
    _check_range(pos, input_num(), "input_name(pos)", "pos");
    return mInputNameList[pos];
  }

  /// @brief 入力名のリストを返す．
  const vector<string>&
  input_name_list() const
  {
    return mInputNameList;
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

  /// @brief 出力名を返す．
  string
  output_name(
    SizeType pos ///< [in] 位置 ( 0 <= pos < output_num() )
  ) const
  {
    _check_range(pos, output_num(), "output_name(pos)", "pos");
    return mOutputNameList[pos];
  }

  /// @brief 出力名のリストを返す．
  const vector<string>&
  output_name_list() const
  {
    return mOutputNameList;
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

  /// @brief セルライブラリを設定する．
  ///
  /// 既に別のライブラリが設定されていたらエラーとなる．
  void
  set_library(
    ClibCellLibrary library ///< [in] ライブラリ
  );

  /// @brief 名前を設定する．
  void
  set_name(
    const string& name ///< [in] 名前
  );

  /// @brief コメントを設定する．
  void
  set_comment(
    const string& comment ///< [in] コメント
  );

  /// @brief 出力の名前を設定する．
  void
  set_output_name(
    SizeType pos,      ///< [in] 位置番号 ( 0 <= pos < output_num() )
    const string& name ///< [in] 名前
  );

  /// @brief 新しいノードを作る．
  ///
  /// @return ID番号を返す．
  SizeType
  new_node(
    const string& name = {} ///< [in] 名前
  );

  /// @brief 新しい入力ノードを作る．
  ///
  /// @return ID番号を返す．
  SizeType
  new_input(
    const string& name = {} ///< [in] 名前
  )
  {
    auto id = new_node(name);
    set_input(id);
    return id;
  }

  /// @brief 新しい出力ノードを作る．
  ///
  /// @return 新しい出力番号を返す．
  SizeType
  new_output(
    SizeType src_id,        ///< [in] ソースのID番号
    const string& name = {} ///< [in] 名前
  );

  /// @brief 新しいプリミティブ型の論理ノードを作る．
  ///
  /// @return ID番号を返す．
  SizeType
  new_primitive(
    const vector<SizeType>& input_list, ///< [in] 入力の識別子番号のリスト
    PrimType type,                      ///< [in] プリミティブタイプ
    const string& name = {}             ///< [in] 名前
  )
  {
    auto id = new_node(name);
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
    bool inv1,              ///< [in] ソース1の反転属性
    const string& name = {} ///< [in] 名前
  )
  {
    auto id = new_node(name);
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
    SizeType cover_id,                  ///< [in] カバー番号
    const string& name = {}             ///< [in] 名前
  )
  {
    auto id = new_node(name);
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
    SizeType expr_id,                   ///< [in] 論理式番号
    const string& name = {}             ///< [in] 名前
  )
  {
    auto id = new_node(name);
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
    SizeType func_id,                   ///< [in] 関数番号
    const string& name = {}             ///< [in] 名前
  )
  {
    auto id = new_node(name);
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
    SizeType bdd_id,                    ///< [in] BDD番号
    const string& name = {}             ///< [in] 名前
  )
  {
    auto id = new_node(name);
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
    ClibCell cell,                      ///< [in] セル
    const string& name = {}             ///< [in] 名前
  )
  {
    auto id = new_node(name);
    set_cell(id, input_list, cell);
    mLogicList.push_back(id);
    return id;
  }

  /// @brief DFFを作る．
  ///
  /// @return ID番号を返す．
  SizeType
  new_dff(
    char rs_val = ' ',           ///< [in] リセットとプリセットが共にオンの時の値
    SizeType output_id = BAD_ID, ///< [in] 出力のノード番号
    const string& name = {}      ///< [in] 名前
  )
  {
    auto id = seq_num();
    auto& seq = _new_seq(name);
    if ( output_id == BAD_ID ) {
      output_id = new_input();
    }
    seq.set_dff(rs_val, output_id);
    return id;
  }

  /// @brief ラッチを作る．
  ///
  /// @return ID番号を返す．
  SizeType
  new_latch(
    char rs_val = ' ',           ///< [in] リセットとプリセットが共にオンの時の値
    SizeType output_id = BAD_ID, ///< [in] 出力のノード番号
    const string& name = {}      ///< [in] 名前
  )
  {
    auto id = seq_num();
    auto& seq = _new_seq(name);
    if ( output_id == BAD_ID ) {
      output_id = new_input();
    }
    seq.set_latch(rs_val, output_id);
    return id;
  }

  /// @brief cell タイプの DFF を作る．
  ///
  /// @return ID番号を返す．
  SizeType
  new_seq_cell(
    ClibCell cell,             ///< [in] セル番号
    const string& name = {}    ///< [in] 名前
  )
  {
  auto id = seq_num();
    set_library(cell.library());
    auto& seq = _new_seq(name);
    seq.set_cell(cell.id(), cell.pin_num());
    return id;
  }

  /// @brief ノードに名前をつける．
  void
  set_node_name(
    SizeType id,       ///< [in] ID番号
    const string& name ///< [in] 名前
  );

  /// @brief 対応するID番号に入力用の印を付ける．
  void
  set_input(
    SizeType id ///< [in] ID番号
  );

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

  /// @brief カバー型のノードの情報をセットする．
  void
  set_cover(
    SizeType id,                        ///< [in] ID番号
    const vector<SizeType>& input_list, ///< [in] 入力の識別子番号のリスト
    SizeType cover_id                   ///< [in] カバー番号
  );

  /// @brief 論理式型のノードの情報をセットする．
  void
  set_expr(
    SizeType id,                        ///< [in] ID番号
    const vector<SizeType>& input_list, ///< [in] 入力の識別子番号のリスト
    SizeType expr_id                    ///< [in] 論理式番号
  );

  /// @brief 真理値表型のノードの情報をセットする．
  void
  set_func(
    SizeType id,                        ///< [in] ID番号
    const vector<SizeType>& input_list, ///< [in] 入力の識別子番号のリスト
    SizeType func_id                    ///< [in] 関数番号
  );

  /// @brief BDD型のノードの情報をセットする．
  void
  set_bdd(
    SizeType id,                        ///< [in] ID番号
    const vector<SizeType>& input_list, ///< [in] 入力の識別子番号のリスト
    SizeType bdd_id                     ///< [in] BDD番号
  );

  /// @brief セル型のノードの情報をセットする．
  void
  set_cell(
    SizeType id,                        ///< [in] ID番号
    const vector<SizeType>& input_list, ///< [in] 入力の識別子番号のリスト
    ClibCell cell                       ///< [in] セル
  );

  /// @brief SEQノードの名前をセットする．
  void
  set_seq_name(
    SizeType id,         ///< [in] ID番号
    const string& name   ///< [in] 名前
  );

  /// @brief DFF/ラッチのソースをセットする．
  void
  set_data_src(
    SizeType id,         ///< [in] ID番号
    SizeType src_id      ///< [in] ソースのID番号
  );

  /// @brief DFFのクロック入力をセットする．
  void
  set_clock(
    SizeType id,         ///< [in] ID番号
    SizeType clock_id    ///< [in] クロックのID番号
  );

  /// @brief ラッチのイネーブル入力をセットする．
  void
  set_enable(
    SizeType id,         ///< [in] ID番号
    SizeType enable_id   ///< [in] イネーブルのID番号
  );

  /// @brief DFF/ラッチのクリア入力をセットする．
  void
  set_clear(
    SizeType id,         ///< [in] ID番号
    SizeType clear_id    ///< [in] クリアのID番号
  );

  /// @brief DFF/ラッチのプリセット入力をセットする．
  void
  set_preset(
    SizeType id,         ///< [in] ID番号
    SizeType preset_id   ///< [in] プリセットのID番号
  );

  /// @brief セル型のSEQノードのピンのノードをセットする．
  void
  set_seq_pin(
    SizeType id,         ///< [in] ID番号
    SizeType pos,        ///< [in] ピン番号
    SizeType node_id     ///< [in] ノード番号
  );

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
  _new_seq(
    const string& name ///< [in] 名前
  )
  {
    mSeqArray.push_back(SeqImpl{name});
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


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 名前
  string mName;

  // コメント
  string mComment;

  // セルライブラリ
  ClibCellLibrary mLibrary;

  // 入力のノード番号のリスト
  vector<SizeType> mInputList;

  // 入力名のリスト
  vector<string> mInputNameList;

  // 出力のノード番号のリスト
  vector<SizeType> mOutputList;

  // 出力名のリスト
  vector<string> mOutputNameList;

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

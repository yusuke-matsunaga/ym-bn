#ifndef BNMODEL_H
#define BNMODEL_H

/// @file BnModel.h
/// @brief BnModel のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/bn.h"
#include "ym/clib.h"
#include "ym/logic.h"
#include "ym/json.h"


BEGIN_NAMESPACE_YM_BN

class ModelImpl;

//////////////////////////////////////////////////////////////////////
/// @class BnModel BnModel.h "BnModel.h"
/// @brief Boolean Network を表すクラス
///
/// 以下の情報を持つ．
/// - 入力ノードのリスト
/// - 出力ノードのリスト
/// - 論理ノードのリスト
/// - SEQノードのリスト
/// - カバー(BlifCover)のリスト
/// - 論理式(Expr)のリスト
/// - 関数(TvFunc)のリスト
/// - BDD(Bdd)のリスト
///
/// - 論理ノードは以下の情報を持つ．
///   * タイプ(BnNodeType)
///   * タイプごとの補助情報
///   * ファンインノードのリスト
///
/// - SEQノードは以下の情報を持つ．
///   * タイプ(BnSeqType)
///   * データ入力ノード
///   * クロック入力ノード
///   * クリア入力ノード
///   * プリセット入力ノード
///   * リセットとプリセットが共にオンの時の値('0', '1', 'X', or 'T')
///
/// - 実際には出力ノードという種類はなく，他のいずれかの
///   ノードとなっている．
/// - 論理ノードのリストは入力からのトポロジカル順
///   となっている．
///
/// - 扱えるファイルタイプは以下の通り
///   * blif(.blif)
///   * iscas89(.bench)
///   * aig(.aag, .aig)
///   * truth(IWLS2022)
///
/// - 回路名，コメント，入出力名をオプションとして持つ．
/// - 本来，入出力名はユニークであるべきだが，blif/iscas89
///   の場合は出力名は出力として参照されているノード名と等しい．
///   そのため入力ノードがそのまま出力となっている場合には
///   入力と出力に同じ名前を持つことになる．
/// - 複数の入出力をまとめてポートを定義することもできる．
///   ポート情報もオプションで保持される．
///
/// - 実装は本体の ModelImpl を指す ModelPtr のみを持つ．
///   ただし，同一の BnModel に属する BnNode, BnSeq 内でしか
///   共有しない．
//////////////////////////////////////////////////////////////////////
class BnModel
{
public:

  /// @brief 空のコンストラクタ
  BnModel();

  /// @brief コピーコンストラクタ
  ///
  /// '深い'コピーを行う．
  BnModel(
    const BnModel& src ///< [in] コピー元のオブジェクト
  );

  /// @brief ムーブコンストラクタ
  BnModel(
    BnModel&& src ///< [in] ムーブ元のオブジェクト
  );

  /// @brief デストラクタ
  ~BnModel();


public:
  //////////////////////////////////////////////////////////////////////
  /// @name 読み込みを行う関数
  /// @{
  //////////////////////////////////////////////////////////////////////

  /// @brief blif ファイルの読み込みを行う．
  /// @return 結果の BnModel を返す．
  ///
  /// 読み込みが失敗したら std::invalid_argument 例外を送出する．
  static
  BnModel
  read_blif(
    const string& filename,             ///< [in] ファイル名
    const string& clock_name = "clock", ///< [in] クロック信号の名前
    const string& reset_name = "reset"  ///< [in] リセット信号の名前
  );

  /// @brief blif ファイルの読み込みを行う(セルライブラリ付き)．
  /// @return 結果の BnModel を返す．
  ///
  /// 読み込みが失敗したら std::invalid_argument 例外を送出する．
  static
  BnModel
  read_blif(
    const string& filename,              ///< [in] ファイル名
    const ClibCellLibrary& cell_library, ///< [in] セルライブラリ
    const string& clock_name = "clock",  ///< [in] クロック信号の名前
    const string& reset_name = "reset"   ///< [in] リセット信号の名前
  );

  /// @brief iscas89(.bench) ファイルの読み込みを行う．
  /// @return 結果の BnModel を返す．
  ///
  /// 読み込みが失敗したら std::invalid_argument 例外を送出する．
  static
  BnModel
  read_iscas89(
    const string& filename,            ///< [in] ファイル名
    const string& clock_name = "clock" ///< [in] クロック信号の名前
  );

  /// @brief aag ファイルの読み込みを行う．
  /// @return 結果の BnModel を返す．
  ///
  /// 読み込みが失敗したら std::invalid_argument 例外を送出する．
  static
  BnModel
  read_aag(
    const string& filename,             ///< [in] ファイル名
    const string& clock_name = "clock", ///< [in] クロック信号の名前
    const string& reset_name = "reset"  ///< [in] リセット信号の名前
  );

  /// @brief aig ファイルの読み込みを行う．
  /// @return 結果の BnModel を返す．
  ///
  /// 読み込みが失敗したら std::invalid_argument 例外を送出する．
  static
  BnModel
  read_aig(
    const string& filename,             ///< [in] ファイル名
    const string& clock_name = "clock", ///< [in] クロック信号の名前
    const string& reset_name = "reset"  ///< [in] リセット信号の名前
  );

  /// @brief truth ファイルの読み込みを行う．
  /// @return 結果の BnModel を返す．
  ///
  /// 読み込みが失敗したら std::invalid_argument 例外を送出する．
  static
  BnModel
  read_truth(
    const string& filename ///< [in] ファイル名
  );

  /// @}
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  /// @name 内容を読み出す関数
  /// @{
  //////////////////////////////////////////////////////////////////////

  /// @brief セルライブラリを返す．
  ///
  /// - 場合によっては空のライブラリを返す．
  ClibCellLibrary
  library() const;

  /// @brief ノード数を返す．
  SizeType
  node_num() const;

  /// @brief 入力数を返す．
  SizeType
  input_num() const;

  /// @brief 入力のノードを返す．
  ///
  /// - 範囲外のアクセスは std::out_of_range 例外を送出する．
  BnNode
  input(
    SizeType pos ///< [in] 位置番号 ( 0 <= pos < input_num() )
  ) const;

  /// @brief 入力のノードのリストを返す．
  BnNodeList
  input_list() const;

  /// @brief 出力数を返す．
  SizeType
  output_num() const;

  /// @brief 入力のノードを返す．
  ///
  /// - 範囲外のアクセスは std::out_of_range 例外を送出する．
  BnNode
  output(
    SizeType pos ///< [in] 位置番号 ( 0 <= pos < output_num() )
  ) const;

  /// @brief 出力のノードのリストを返す．
  BnNodeList
  output_list() const;

  /// @brief 論理ノード数を返す．
  SizeType
  logic_num() const;

  /// @brief 論理ノードを返す．
  ///
  /// - 範囲外のアクセスは std::out_of_range 例外を送出する．
  BnNode
  logic(
    SizeType pos ///< [in] 位置番号 ( 0 <= pos < logic_num() )
  ) const;

  /// @brief 論理ノードのリストを返す．
  BnNodeList
  logic_list() const;

  /// @brief SEQノード数を返す．
  SizeType
  seq_num() const;

  /// @brief SEQノードを返す．
  ///
  /// - 範囲外のアクセスは std::out_of_range 例外を送出する．
  BnSeq
  seq(
    SizeType pos ///< [in] 位置番号 ( 0 <= pos < seq_num() )
  ) const;

  /// @brief 関数情報の数を返す．
  SizeType
  func_num() const;

  /// @brief 関数情報を取り出す．
  ///
  /// - 範囲外のアクセスは std::out_of_range 例外を送出する．
  BnFunc
  func(
    SizeType func_id ///< [in] カバー番号 ( 0 <= func_id < func_num() )
  ) const;

  /// @brief 内容を出力する．
  void
  print(
    ostream& s ///< [in] 出力先のストリーム
  ) const;

  /// @brief 等価比較関数
  ///
  /// 意味的ではなく形式的な等価を調べる．
  bool
  operator==(
    const BnModel& right ///< [in] 比較対象のオブジェクト
  ) const
  {
    return mImpl == right.mImpl;
  }

  /// @brief 非等価比較関数
  ///
  /// 意味的ではなく形式的な等価を調べる．
  bool
  operator!=(
    const BnModel& right ///< [in] 比較対象のオブジェクト
  ) const
  {
    return !operator==(right);
  }

  /// @}
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  /// @name オプション情報を取得する関数
  /// @{
  //////////////////////////////////////////////////////////////////////

  /// @brief オプション情報を表す JSON オブジェクトを返す．
  JsonValue
  option() const;

  /// @brief 名前を返す．
  string
  name() const;

  /// @brief コメントを返す．
  string
  comment() const;

  /// @brief 入力名を返す．
  ///
  /// - 範囲外のアクセスは std::out_of_range 例外を送出する．
  string
  input_name(
    SizeType input_id ///< [in] 入力番号 ( 0 <= input_id < input_num() )
  ) const;

  /// @brief 出力名を返す．
  ///
  /// - 範囲外のアクセスは std::out_of_range 例外を送出する．
  string
  output_name(
    SizeType output_id ///< [in] 出力番号 ( 0 <= output_id < output_num() )
  ) const;

  /// @brief ラッチ名を返す．
  ///
  /// - 範囲外のアクセスは std::out_of_range 例外を送出する．
  string
  seq_name(
    SizeType seq_id ///< [in] ラッチ番号 ( 0 <= seq_id < seq_num() )
  ) const;

  /// @}
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  /// @name 内容を設定する関数
  /// @{
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容をクリアする．
  void
  clear();

  /// @brief セルライブラリを設定する．
  ///
  /// 既に別のライブラリが設定されていたら std::invalid_argument 例外を送出する．
  void
  set_celllibrary(
    ClibCellLibrary lib ///< [in] ライブラリ
  );

  /// @brief オプション情報をセットする．
  void
  set_option(
    const JsonValue& option ///< [in] 追加するオプション
  );

  /// @}
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  /// @name 論理ノードの生成
  /// @{
  //////////////////////////////////////////////////////////////////////

  /// @brief 入力ノードを作る．
  /// @return 生成したノードを返す．
  BnNode
  new_input(
    const string& name = {} ///< [in] 名前
  );

  /// @brief 出力ノードを作る．
  /// @return 生成した出力ノードの出力番号を返す．
  ///
  /// - src は同じ BnModel に属するノードでなければならない．
  /// - 条件に合わない時は std::invalid_argument 例外を送出する．
  SizeType
  new_output(
    BnNode src,             ///< [in] ソースノード
    const string& name = {} ///< [in] 名前
  );

  /// @brief 新しいプリミティブ型の論理ノードを作る．
  /// @return 生成したノードを返す．
  ///
  /// - input_list の要素は同じ BnModel に属するノードでなければならない．
  /// - 条件に合わない時は std::invalid_argument 例外を送出する．
  BnNode
  new_primitive(
    PrimType prim_type,              ///< [in] プリミティブ型
    const vector<BnNode>& input_list ///< [in] 入力の識別子番号のリスト
  );

  /// @brief 新しいAIG型の論理ノードを作る．
  /// @return 生成したノードを返す．
  ///
  /// - src0, src1 は同じ BnModel に属するノードでなければならない．
  /// - 条件に合わない時は std::invalid_argument 例外を送出する．
  BnNode
  new_aig(
    BnNode src0,            ///< [in] ソース0のノード
    BnNode src1,            ///< [in] ソース1のノード
    bool inv0,              ///< [in] ソース0の反転属性
    bool inv1               ///< [in] ソース1の反転属性
  );

  /// @brief 新しい関数型の論理ノードを作る．
  /// @return 生成したノードを返す．
  ///
  /// - local_func は事前に reg_cover(), reg_expr(), reg_tvfunc(), reg_bdd()
  ///   で登録したものを使う．
  /// - input_list の要素は同じ BnModel に属するノードでなければならない．
  /// - input_list の要素数は local_func の条件に合致しなければならない．
  /// - 条件に合わない時は std::invalid_argument 例外を送出する．
  BnNode
  new_func(
    BnFunc local_func,               ///< [in] 関数
    const vector<BnNode>& input_list ///< [in] 入力の識別子番号のリスト
  );

  /// @brief 新しいセル型の論理ノードを作る．
  /// @return 生成したノードを返す．
  ///
  /// - input_list の要素は同じ BnModel に属するノードでなければならない．
  /// - cell はこのモデルに設定されているセルライブラリのセルでなければならない．
  /// - セルの入力数と input_list のサイズは一致しなければならない．
  /// - 条件に合わない時は std::invalid_argument 例外を送出する．
  BnNode
  new_cell(
    ClibCell cell,                   ///< [in] セル
    const vector<BnNode>& input_list ///< [in] 入力のノードのリスト
  );

  /// @}
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  /// @name 論理関数表現の登録
  /// @{
  //////////////////////////////////////////////////////////////////////

  /// @brief カバーを登録する．
  /// @return 登録結果(BnFunc)を返す．
  ///
  /// - cube_list 中に現れるリテラル番号は input_num 未満でなければならない．
  /// - opat は '0' か '1' でなければならない．
  /// - 上記の条件に合わない場合は std::invalid_argument 例外を送出する．
  BnFunc
  reg_cover(
    SizeType input_num,                       ///< [in] 入力数
    const vector<vector<Literal>>& cube_list, ///< [in] キューブのリスト
    char opat                                 ///< [in] 出力パタン ( '1' or '0' )
  );

  /// @brief 論理式を登録する．
  /// @return 登録結果(BnFunc)を返す．
  BnFunc
  reg_expr(
    const Expr& expr ///< [in] 論理式
  );

  /// @brief 真理値表を登録する．
  /// @return 登録結果(BnFunc)を返す．
  BnFunc
  reg_tvfunc(
    const TvFunc& func ///< [in] 真理値表型の関数
  );

  /// @brief BDDを登録する．
  /// @return 登録結果(BnFunc)を返す．
  BnFunc
  reg_bdd(
    const Bdd& bdd ///< [in] BDD
  );

  /// @}
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  /// @name SEQノードの生成/設定
  /// @{
  //////////////////////////////////////////////////////////////////////

  /// @brief DFFを作る．
  /// @return 生成したSEQノードを返す．
  ///
  /// - rs_val は ' ', '0', '1', 'X', 'T' のいずれか
  /// - そうでなければ std::invalid_argument 例外を送出する．
  BnSeq
  new_dff(
    char rs_val = ' ',      ///< [in] リセットとプリセットが共にオンの時の値
    const string& name = {} ///< [in] 名前
  );

  /// @brief ラッチを作る．
  /// @return 生成したSEQノードを返す．
  ///
  /// - rs_val は ' ', '0', '1', 'X', 'T' のいずれか
  /// - そうでなければ std::invalid_argument 例外を送出する．
  BnSeq
  new_latch(
    char rs_val = ' ',      ///< [in] リセットとプリセットが共にオンの時の値
    const string& name = {} ///< [in] 名前
  );

  /// @brief セルタイプのSEQノードを作る．
  /// @return 生成したSEQノードを返す．
  ///
  /// - cell はこのモデルに設定されているセルライブラリのセルでなければならない．
  /// - cell が順序回路でなければならない．
  /// - 条件に合わない時は std::invalid_argument 例外を送出する．
  BnSeq
  new_seq_cell(
    ClibCell cell,          ///< [in] セル
    const string& name = {} ///< [in] 名前
  );

  /// @brief DFF/ラッチのソースノードをセットする．
  ///
  /// - seq は同じ BnModel に属していなければならない．
  /// - src は同じ BnModel に属していなければならない．
  /// - 条件に合わない時は std::invalid_argument 例外を送出する．
  void
  set_data_src(
    BnSeq seq,    ///< [in] SEQノード
    BnNode src    ///< [in] 入力ノード
  );

  /// @brief DFFのクロック入力をセットする．
  ///
  /// - seq は同じ BnModel に属していなければならない．
  /// - clock は同じ BnModel に属していなければならない．
  /// - 条件に合わない時は std::invalid_argument 例外を送出する．
  void
  set_clock(
    BnSeq seq,    ///< [in] SEQノード
    BnNode clock  ///< [in] クロック入力ノード
  );

  /// @brief ラッチのイネーブル入力をセットする．
  ///
  /// - seq は同じ BnModel に属していなければならない．
  /// - enable は同じ BnModel に属していなければならない．
  /// - 条件に合わない時は std::invalid_argument 例外を送出する．
  void
  set_enable(
    BnSeq seq,     ///< [in] SEQノード
    BnNode enable  ///< [in] イネーブル入力
  );

  /// @brief DFF/ラッチのクリア入力をセットする．
  ///
  /// - seq は同じ BnModel に属していなければならない．
  /// - clear は同じ BnModel に属していなければならない．
  /// - 条件に合わない時は std::invalid_argument 例外を送出する．
  void
  set_clear(
    BnSeq seq,    ///< [in] SEQノード
    BnNode clear  ///< [in] クリア入力ノード
  );

  /// @brief DFF/ラッチのプリセット入力をセットする．
  ///
  /// - seq は同じ BnModel に属していなければならない．
  /// - preset は同じ BnModel に属していなければならない．
  /// - 条件に合わない時は std::invalid_argument 例外を送出する．
  void
  set_preset(
    BnSeq seq,    ///< [in] SEQノード
    BnNode preset ///< [in] プリセット入力ノード
  );

  /// @brief セルのピンに対応するノードをセットする．
  ///
  /// - seq は同じ BnModel に属していなければならない．
  /// - node は同じ BnModel に属していなければならない．
  /// - 条件に合わない時は std::invalid_argument 例外を送出する．
  /// - pos が範囲外の時は std::out_of_range 例外を送出する．
  void
  set_seq_pin(
    BnSeq seq,    ///< [in] SEQノード
    SizeType pos, ///< [in] ピン番号
    BnNode node   ///< [in] ノード
  );

  /// @}
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief vector<BnNode> から ID番号のリストを取り出す．
  ///
  /// 同じ Model に属していないとエラーとなる．
  vector<SizeType>
  to_id_list(
    const vector<BnNode>& node_list
  ) const;

  /// @brief BnNode のチェックを行う．
  ///
  /// 同じ Model に属していないとエラーとなる．
  void
  _check_node(
    const BnNode& node
  ) const;

  /// @brief BnSeq のチェックを行う．
  ///
  /// 同じ Model に属していないとエラーとなる．
  void
  _check_seq(
    const BnSeq& seq
  ) const;

  /// @brief BnFunc のチェックを行う．
  ///
  /// 同じ Model に属していないとエラーとなる．
  void
  _check_func(
    const BnFunc& func
  ) const;

  /// @brief 入力番号のチェックを行う．
  void
  _check_input_id(
    SizeType input_id
  ) const;

  /// @brief 出力番号のチェックを行う．
  void
  _check_output_id(
    SizeType output_id
  ) const;

  /// @brief 論理ノード番号のチェックを行う．
  void
  _check_logic_id(
    SizeType logic_id
  ) const;

  /// @brief SEQ番号のチェックを行う．
  void
  _check_seq_id(
    SizeType seq_id
  ) const;

  /// @brief 本体に登録されているライブラリとセルが属するライブラリが等しいか調べる．
  void
  _check_library_cell(
    ClibCell cell         ///< [in] セル
  ) const;

  /// @brief DFF/latch 用の rs_val が正しいかチェックする．
  void
  _check_rsval(
    char rsval
  ) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 実装本体
  shared_ptr<ModelImpl> mImpl;

};

END_NAMESPACE_YM_BN

#endif // BNMODEL_H

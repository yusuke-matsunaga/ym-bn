#ifndef BFMODEL_H
#define BFMODEL_H

/// @file BfModel.h
/// @brief BfModel のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/bnfe.h"
#include "ym/clib.h"
#include "ym/logic.h"


BEGIN_NAMESPACE_YM_BNFE

class ModelImpl;

//////////////////////////////////////////////////////////////////////
/// @class BfModel BfModel.h "BfModel.h"
/// @brief Boolean Network の読み込み結果を表すクラス
///
/// 扱えるファイルタイプは以下の通り
/// - blif(.blif)
/// - iscas89(.bench)
/// - aig(.aag, .aig)
///
/// 以下の情報を持つ．
/// - 入力ノードのリスト
/// - 出力ノードのリスト
/// - DFFノードのリスト
/// - 論理ノードのリスト
/// - 各ノードごとに以下の情報を持つ．
/// - カバー(BlifCover)のリスト
///
/// - Dffタイプは以下の情報を持つ．
///   * リセット値 ('0' or '1' or 'X')
///
/// 実際には出力ノードという種類はなく，他のいずれかの
/// ノードとなっている．
/// 論理ノードのリストは入力からのトポロジカル順
/// となっている．
///
/// - 読み込んだファイルによってはノードはユニークな名前を持つ．
/// - 入力名は入力ノード名と等しい
/// - blif/iscas89 の場合は出力名は出力として参照されている
///   ノード名と等しい．そのため入力ノードがそのまま出力となっている場合
///   には入力と出力に同じ名前を持つことになる．
//////////////////////////////////////////////////////////////////////
class BfModel
{
private:

  /// @brief 空のコンストラクタ
  BfModel();

  /// @brief ModelImpl* からのキャストコンストラクタ
  BfModel(
    ModelImpl* impl ///< [in] 実装オブジェクト
  );


public:

  /// @brief shared_ptr<ModelImpl> からのキャストコンストラクタ
  BfModel(
    const shared_ptr<ModelImpl>& impl ///< [in] 実装オブジェクト
  );

  /// @brief コピーコンストラクタ
  ///
  /// '浅い'コピーを行う．
  BfModel(
    const BfModel& src ///< [in] コピー元のオブジェクト
  );

  /// @brief ムーブコンストラクタ
  BfModel(
    BfModel&& src ///< [in] ムーブ元のオブジェクト
  );

  /// @brief コピー代入演算子
  ///
  /// '浅い'コピーを行う．
  BfModel&
  operator=(
    const BfModel& src ///< [in] コピー元のオブジェクト
  );

  /// @brief ムーブ代入演算子
  BfModel&
  operator=(
    BfModel&& src ///< [in] ムーブ元のオブジェクト
  );

  /// @brief デストラクタ
  ~BfModel();


public:
  //////////////////////////////////////////////////////////////////////
  /// @name 読み込みを行う関数
  /// @{
  //////////////////////////////////////////////////////////////////////

  /// @brief blif ファイルの読み込みを行う．
  /// @return 結果の BfModel を返す．
  ///
  /// 読み込みが失敗したら std::invalid_argument 例外を送出する．
  static
  BfModel
  read_blif(
    const string& filename ///< [in] ファイル名
  );

  /// @brief blif ファイルの読み込みを行う(セルライブラリ付き)．
  /// @return 結果の BfModel を返す．
  ///
  /// 読み込みが失敗したら std::invalid_argument 例外を送出する．
  static
  BfModel
  read_blif(
    const string& filename,             ///< [in] ファイル名
    const ClibCellLibrary& cell_library ///< [in] セルライブラリ
  );

  /// @brief iscas89(.bench) ファイルの読み込みを行う．
  /// @return 結果の BfModel を返す．
  ///
  /// 読み込みが失敗したら std::invalid_argument 例外を送出する．
  static
  BfModel
  read_iscas89(
    const string& filename ///< [in] ファイル名
  );

  /// @brief aag ファイルの読み込みを行う．
  /// @return 結果の BfModel を返す．
  ///
  /// 読み込みが失敗したら std::invalid_argument 例外を送出する．
  static
  BfModel
  read_aag(
    const string& filename ///< [in] ファイル名
  );

  /// @brief aig ファイルの読み込みを行う．
  /// @return 結果の BfModel を返す．
  ///
  /// 読み込みが失敗したら std::invalid_argument 例外を送出する．
  static
  BfModel
  read_aig(
    const string& filename ///< [in] ファイル名
  );

  /// @brief truth ファイルの読み込みを行う．
  /// @return 結果の BfModel を返す．
  ///
  /// 読み込みが失敗したら std::invalid_argument 例外を送出する．
  static
  BfModel
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

  /// @brief 深いコピーを行う．
  BfModel
  copy() const;

  /// @brief 名前を返す．
  ///
  /// 空の場合もある．
  string
  name() const;

  /// @brief コメントを返す．
  ///
  /// 空の場合もある．
  string
  comment() const;

  /// @brief 入力数を返す．
  SizeType
  input_num() const;

  /// @brief 入力のノードを返す．
  BfNode
  input(
    SizeType pos ///< [in] 位置番号 ( 0 <= pos < input_num() )
  ) const;

  /// @brief 入力のノードのリストを返す．
  vector<BfNode>
  input_list() const;

  /// @brief 入力名を返す．
  string
  input_name(
    SizeType pos ///< [in] 位置番号 ( 0 <= pos < input_num() )
  ) const;

  /// @brief 入力名のリストを返す．
  vector<string>
  input_name_list() const;

  /// @brief 出力数を返す．
  SizeType
  output_num() const;

  /// @brief 入力のノードを返す．
  BfNode
  output(
    SizeType pos ///< [in] 位置番号 ( 0 <= pos < output_num() )
  ) const;

  /// @brief 出力のノードのリストを返す．
  vector<BfNode>
  output_list() const;

  /// @brief 出力名を返す．
  string
  output_name(
    SizeType pos ///< [in] 位置番号 ( 0 <= pos < output_num() )
  ) const;

  /// @brief 出力名のリストを返す．
  vector<string>
  output_name_list() const;

  /// @brief DFF数を返す．
  SizeType
  dff_num() const;

  /// @brief DFFのノードを返す．
  BfNode
  dff(
    SizeType pos ///< [in] 位置番号 ( 0 <= pos < dff_num() )
  ) const;

  /// @brief DFFのノードのリストを返す．
  vector<BfNode>
  dff_list() const;

  /// @brief 論理ノード数を返す．
  SizeType
  logic_num() const;

  /// @brief 論理ノードを返す．
  BfNode
  logic(
    SizeType pos ///< [in] 位置番号 ( 0 <= pos < logic_num() )
  ) const;

  /// @brief 論理ノードのリストを返す．
  vector<BfNode>
  logic_list() const;

  /// @brief カバーの種類の数を返す．
  SizeType
  cover_num() const;

  /// @brief カバーを取り出す．
  const BfCover&
  cover(
    SizeType cover_id ///< [in] カバー番号
  ) const;

  /// @brief 論理式の数を返す．
  SizeType
  expr_num() const;

  /// @brief 論理式を返す．
  const Expr&
  expr(
    SizeType expr_id ///< [in] 論理式番号 ( 0 <= expr_id < expr_num() )
  ) const;

  /// @brief 真理値表型の関数の数を返す．
  SizeType
  func_num() const;

  /// @brief 真理値表型の関数を返す．
  const TvFunc&
  func(
    SizeType func_id ///< [in] 関数番号 ( 0 <= func_id < func_num() )
  ) const;

  /// @brief BDDの数を返す．
  SizeType
  bdd_num() const;

  /// @brief BDDを返す．
  Bdd
  bdd(
    SizeType bdd_id ///< [in] BDD番号 ( 0 <= bdd_id < bdd_num() )
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
    const BfModel& right ///< [in] 比較対象のオブジェクト
  ) const
  {
    return mImpl == right.mImpl;
  }

  /// @brief 非等価比較関数
  ///
  /// 意味的ではなく形式的な等価を調べる．
  bool
  operator!=(
    const BfModel& right ///< [in] 比較対象のオブジェクト
  ) const
  {
    return !operator==(right);
  }

  /// @}
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ID 番号から BfNode を作る．
  BfNode
  from_id(
    SizeType id
  ) const;

  /// @brief ID 番号のリストから vector<BfNode> を作る．
  vector<BfNode>
  from_id_list(
    const vector<SizeType>& id_list
  ) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 実装本体
  shared_ptr<ModelImpl> mImpl;

};

END_NAMESPACE_YM_BNFE

#endif // BFMODEL_H

# ===================================================================
# CMAKE のおまじない
# ===================================================================


# ===================================================================
# プロジェクト名，バージョンの設定
# ===================================================================


# ===================================================================
# オプション
# ===================================================================


# ===================================================================
# パッケージの検査
# ===================================================================


# ===================================================================
# ヘッダファイルの生成
# ===================================================================


# ===================================================================
# インクルードパスの設定
# ===================================================================
include_directories(
  include
  private_include
  )


# ===================================================================
#  マクロの定義
# ===================================================================

set ( TESTDATA_DIR "${CMAKE_CURRENT_SOURCE_DIR}/testdata/" )


# ===================================================================
# サブディレクトリの設定
# ===================================================================

add_subdirectory ( c++-srcs )
add_subdirectory ( test-programs )
if ( Python3_FOUND )
#  add_subdirectory ( py_capi )
#  add_subdirectory ( pytest )
endif ( Python3_FOUND )


# ===================================================================
#  ソースの設定
# ===================================================================


# ===================================================================
#  ターゲットの設定
# ===================================================================

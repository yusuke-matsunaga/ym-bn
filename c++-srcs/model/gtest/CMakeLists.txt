
# ===================================================================
# インクルードパスの設定
# ===================================================================


# ===================================================================
# サブディレクトリの設定
# ===================================================================


# ===================================================================
#  ソースファイルの設定
# ===================================================================



# ===================================================================
#  テスト用のターゲットの設定
# ===================================================================

ym_add_gtest( bn_ModelImpl_test
  ModelImpl_test.cc
  $<TARGET_OBJECTS:ym_bn_obj_d>
  $<TARGET_OBJECTS:ym_base_obj_d>
  $<TARGET_OBJECTS:ym_logic_obj_d>
  DEFINITIONS
  "-DDATAPATH=\"${TESTDATA_DIR}\""
  )

ym_add_gtest( bn_BnModel_test
  BnModel_test.cc
  $<TARGET_OBJECTS:ym_bn_obj_d>
  $<TARGET_OBJECTS:ym_base_obj_d>
  $<TARGET_OBJECTS:ym_logic_obj_d>
  DEFINITIONS
  "-DDATAPATH=\"${TESTDATA_DIR}\""
  )

ym_add_gtest( bn_BnModel_copymove_test
  BnModel_test.cc
  $<TARGET_OBJECTS:ym_bn_obj_d>
  $<TARGET_OBJECTS:ym_base_obj_d>
  $<TARGET_OBJECTS:ym_logic_obj_d>
  DEFINITIONS
  "-DDATAPATH=\"${TESTDATA_DIR}\""
  )


# ===================================================================
#  インストールターゲットの設定
# ===================================================================


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

ym_add_gtest( bn_NodeImpl_test
  NodeImpl_test.cc
  $<TARGET_OBJECTS:ym_bn_obj_d>
  $<TARGET_OBJECTS:ym_base_obj_d>
  $<TARGET_OBJECTS:ym_logic_obj_d>
  $<TARGET_OBJECTS:ym_cell_obj_d>
  DEFINITIONS
  "-DDATAPATH=\"${TESTDATA_DIR}\""
  )

target_include_directories( bn_NodeImpl_test
  PRIVATE ../
  )

ym_add_gtest( bn_SeqImpl_test
  SeqImpl_test.cc
  $<TARGET_OBJECTS:ym_bn_obj_d>
  $<TARGET_OBJECTS:ym_base_obj_d>
  $<TARGET_OBJECTS:ym_logic_obj_d>
  $<TARGET_OBJECTS:ym_cell_obj_d>
  DEFINITIONS
  "-DDATAPATH=\"${TESTDATA_DIR}\""
  )

target_include_directories( bn_SeqImpl_test
  PRIVATE ../
  )

ym_add_gtest( bn_ModelImpl_test
  ModelImpl_test.cc
  $<TARGET_OBJECTS:ym_bn_obj_d>
  $<TARGET_OBJECTS:ym_base_obj_d>
  $<TARGET_OBJECTS:ym_logic_obj_d>
  $<TARGET_OBJECTS:ym_cell_obj_d>
  DEFINITIONS
  "-DDATAPATH=\"${TESTDATA_DIR}\""
  )

ym_add_gtest( bn_BnNode_test
  BnNode_test.cc
  $<TARGET_OBJECTS:ym_bn_obj_d>
  $<TARGET_OBJECTS:ym_base_obj_d>
  $<TARGET_OBJECTS:ym_logic_obj_d>
  $<TARGET_OBJECTS:ym_cell_obj_d>
  DEFINITIONS
  "-DDATAPATH=\"${TESTDATA_DIR}\""
  )

ym_add_gtest( bn_BnSeq_test
  BnSeq_test.cc
  $<TARGET_OBJECTS:ym_bn_obj_d>
  $<TARGET_OBJECTS:ym_base_obj_d>
  $<TARGET_OBJECTS:ym_logic_obj_d>
  $<TARGET_OBJECTS:ym_cell_obj_d>
  DEFINITIONS
  "-DDATAPATH=\"${TESTDATA_DIR}\""
  )

ym_add_gtest( bn_BnModel_test
  BnModel_test.cc
  $<TARGET_OBJECTS:ym_bn_obj_d>
  $<TARGET_OBJECTS:ym_base_obj_d>
  $<TARGET_OBJECTS:ym_logic_obj_d>
  $<TARGET_OBJECTS:ym_cell_obj_d>
  DEFINITIONS
  "-DDATAPATH=\"${TESTDATA_DIR}\""
  )

ym_add_gtest( bn_BnModel_copymove_test
  BnModel_test.cc
  $<TARGET_OBJECTS:ym_bn_obj_d>
  $<TARGET_OBJECTS:ym_base_obj_d>
  $<TARGET_OBJECTS:ym_logic_obj_d>
  $<TARGET_OBJECTS:ym_cell_obj_d>
  DEFINITIONS
  "-DDATAPATH=\"${TESTDATA_DIR}\""
  )


# ===================================================================
#  インストールターゲットの設定
# ===================================================================

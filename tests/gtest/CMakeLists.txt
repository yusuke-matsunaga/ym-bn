
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
  )

ym_add_gtest( bn_DffImpl_test
  DffImpl_test.cc
  $<TARGET_OBJECTS:ym_bn_obj_d>
  $<TARGET_OBJECTS:ym_base_obj_d>
  $<TARGET_OBJECTS:ym_logic_obj_d>
  $<TARGET_OBJECTS:ym_cell_obj_d>
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

ym_add_gtest( bn_BnCover_test
  BnCover_test.cc
  $<TARGET_OBJECTS:ym_bn_obj_d>
  $<TARGET_OBJECTS:ym_base_obj_d>
  $<TARGET_OBJECTS:ym_logic_obj_d>
  $<TARGET_OBJECTS:ym_cell_obj_d>
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

ym_add_gtest( bn_BnDff_test
  BnDff_test.cc
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

ym_add_gtest( bn_read_blif_test
  read_blif_test.cc
  $<TARGET_OBJECTS:ym_bn_obj_d>
  $<TARGET_OBJECTS:ym_base_obj_d>
  $<TARGET_OBJECTS:ym_logic_obj_d>
  $<TARGET_OBJECTS:ym_cell_obj_d>
  DEFINITIONS
  "-DDATAPATH=\"${TESTDATA_DIR}\""
  )

ym_add_gtest( bn_read_iscas89_test
  read_iscas89_test.cc
  $<TARGET_OBJECTS:ym_bn_obj_d>
  $<TARGET_OBJECTS:ym_base_obj_d>
  $<TARGET_OBJECTS:ym_logic_obj_d>
  $<TARGET_OBJECTS:ym_cell_obj_d>
  DEFINITIONS
  "-DDATAPATH=\"${TESTDATA_DIR}\""
  )

ym_add_gtest( bn_read_aig_test
  read_aig_test.cc
  $<TARGET_OBJECTS:ym_bn_obj_d>
  $<TARGET_OBJECTS:ym_base_obj_d>
  $<TARGET_OBJECTS:ym_logic_obj_d>
  $<TARGET_OBJECTS:ym_cell_obj_d>
  DEFINITIONS
  "-DDATAPATH=\"${TESTDATA_DIR}\""
  )

ym_add_gtest( bn_read_truth_test
  read_truth_test.cc
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

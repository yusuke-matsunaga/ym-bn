
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

add_executable ( read_blif
  read_blif.cc
  $<TARGET_OBJECTS:ym_bnet_obj_d>
  $<TARGET_OBJECTS:ym_cell_obj_d>
  $<TARGET_OBJECTS:ym_logic_obj_d>
  $<TARGET_OBJECTS:ym_base_obj_d>
  )

target_compile_options ( read_blif
  PRIVATE "-g"
  )

target_link_libraries ( read_blif
  ${YM_LIB_DEPENDS}
  )

add_executable ( read_iscas89
  read_iscas89.cc
  $<TARGET_OBJECTS:ym_bnet_obj_d>
  $<TARGET_OBJECTS:ym_cell_obj_d>
  $<TARGET_OBJECTS:ym_logic_obj_d>
  $<TARGET_OBJECTS:ym_base_obj_d>
  )

target_compile_options ( read_iscas89
  PRIVATE "-g"
  )

target_link_libraries ( read_iscas89
  ${YM_LIB_DEPENDS}
  )

add_executable ( read_aag
  read_aag.cc
  $<TARGET_OBJECTS:ym_bnet_obj_d>
  $<TARGET_OBJECTS:ym_cell_obj_d>
  $<TARGET_OBJECTS:ym_logic_obj_d>
  $<TARGET_OBJECTS:ym_base_obj_d>
  )

target_compile_options ( read_aag
  PRIVATE "-g"
  )

target_link_libraries ( read_aag
  ${YM_LIB_DEPENDS}
  )


# ===================================================================
#  インストールターゲットの設定
# ===================================================================

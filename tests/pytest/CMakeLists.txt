
# ===================================================================
# パッケージの検査
# ===================================================================


# ===================================================================
# インクルードパスの設定
# ===================================================================
include_directories(
  ${Python3_INCLUDE_DIRS}
  )


# ===================================================================
# サブディレクトリの設定
# ===================================================================


# ===================================================================
#  ソースファイルの設定
# ===================================================================

set ( py_ymbn_SOURCES
  py_ymbn.cc
  )


# ===================================================================
#  executable ターゲットの設定
# ===================================================================

add_executable( py_ymbn
  ${py_ymbn_SOURCES}
  $<TARGET_OBJECTS:py_ymbn_obj>
  $<TARGET_OBJECTS:py_ymcell_obj>
  $<TARGET_OBJECTS:py_ymlogic_obj>
  $<TARGET_OBJECTS:ym_bn_obj>
  $<TARGET_OBJECTS:ym_cell_obj>
  $<TARGET_OBJECTS:ym_logic_obj>
  $<TARGET_OBJECTS:ym_base_obj>
  )

target_compile_options( py_ymbn
  PRIVATE "-O3"
  )

target_compile_definitions( py_ymbn
  PRIVATE "-DNODEBUG"
  )

target_link_libraries ( py_ymbn
  ${YM_LIB_DEPENDS}
  ${Python3_LIBRARIES}
  )

add_executable( py_ymbn_d
  ${py_ymbn_SOURCES}
  $<TARGET_OBJECTS:py_ymbn_obj_d>
  $<TARGET_OBJECTS:py_ymcell_obj_d>
  $<TARGET_OBJECTS:py_ymlogic_obj_d>
  $<TARGET_OBJECTS:ym_bn_obj_d>
  $<TARGET_OBJECTS:ym_cell_obj_d>
  $<TARGET_OBJECTS:ym_logic_obj_d>
  $<TARGET_OBJECTS:ym_base_obj_d>
  )

target_compile_options( py_ymbn_d
  PRIVATE "-g"
  )

target_link_libraries ( py_ymbn_d
  ${YM_LIB_DEPENDS}
  ${Python3_LIBRARIES}
  )


# ===================================================================
# テストターゲット
# ===================================================================

add_test( bn_py_ymbn_test
  py_ymbn -m pytest ${CMAKE_CURRENT_SOURCE_DIR}
  )

set_property( TEST bn_py_ymbn_test
  PROPERTY ENVIRONMENT TESTDATA_DIR=${TESTDATA_DIR}
  )

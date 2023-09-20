
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

set ( py_ymbnfe_SOURCES
  py_ymbnfe.cc
  )


# ===================================================================
#  executable ターゲットの設定
# ===================================================================

add_executable( py_ymbnfe
  ${py_ymbnfe_SOURCES}
  $<TARGET_OBJECTS:py_ymbnfe_obj>
  $<TARGET_OBJECTS:py_ymcell_obj>
  $<TARGET_OBJECTS:py_ymlogic_obj>
  $<TARGET_OBJECTS:ym_bnfe_obj>
  $<TARGET_OBJECTS:ym_cell_obj>
  $<TARGET_OBJECTS:ym_logic_obj>
  $<TARGET_OBJECTS:ym_base_obj>
  )

target_compile_options( py_ymbnfe
  PRIVATE "-O3"
  )

target_compile_definitions( py_ymbnfe
  PRIVATE "-DNODEBUG"
  )

target_link_libraries ( py_ymbnfe
  ${YM_LIB_DEPENDS}
  ${Python3_LIBRARIES}
  )

add_executable( py_ymbnfe_d
  ${py_ymbnfe_SOURCES}
  $<TARGET_OBJECTS:py_ymbnfe_obj_d>
  $<TARGET_OBJECTS:py_ymcell_obj_d>
  $<TARGET_OBJECTS:py_ymlogic_obj_d>
  $<TARGET_OBJECTS:ym_bnfe_obj_d>
  $<TARGET_OBJECTS:ym_cell_obj_d>
  $<TARGET_OBJECTS:ym_logic_obj_d>
  $<TARGET_OBJECTS:ym_base_obj_d>
  )

target_compile_options( py_ymbnfe_d
  PRIVATE "-g"
  )

target_link_libraries ( py_ymbnfe_d
  ${YM_LIB_DEPENDS}
  ${Python3_LIBRARIES}
  )


# ===================================================================
# テストターゲット
# ===================================================================

add_test( cell_py_ymbnfe_test
  py_ymbnfe -m pytest ${CMAKE_CURRENT_SOURCE_DIR}
  )

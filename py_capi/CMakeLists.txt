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


# ===================================================================
#  ターゲットの設定
# ===================================================================

ym_add_object_library ( py_ymbn
  ymbn_module.cc
  PyBnModel.cc
  PyBnNode.cc
  PyBnFunc.cc
  )

add_library ( py_ymbnlib
  SHARED
  $<TARGET_OBJECTS:py_ymbn_obj>
  $<TARGET_OBJECTS:py_ymlogic_obj>
  $<TARGET_OBJECTS:py_ymbase_obj>
  $<TARGET_OBJECTS:ym_bn_obj>
  $<TARGET_OBJECTS:ym_logic_obj>
  $<TARGET_OBJECTS:ym_base_obj>
  )

set_target_properties( py_ymbnlib
  PROPERTIES
  PREFIX ""
  OUTPUT_NAME "ymbn"
  )

target_link_libraries( py_ymbnlib
  ${YM_LIB_DEPENDS}
  ${Python3_LIBRARIES}
  )


# ===================================================================
#  インストールターゲットの設定
# ===================================================================

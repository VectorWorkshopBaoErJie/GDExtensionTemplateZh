# SPDX-License-Identifier: Unlicense  
# 该文件遵循无许可证（Unlicense）协议，即放弃版权，任何人可以自由使用、修改、分发该文件，无需遵守任何版权限制。  
  
# 定义一个自定义目标（target）名为 templates，该目标依赖于指定的源文件模板。  
# 这些模板用于生成Godot引擎的GDExtension（Godot动态扩展）的调试和发布版本。
add_custom_target( templates
    SOURCES
        template.debug.gdextension.in # 调试版本的GDExtension模板文件
        template.release.gdextension.in # 发布版本的GDExtension模板文件  
)

# 将自定义目标 templates 设置为当前项目（由 PROJECT_NAME 变量指定）的依赖项。  
# 这意味着在构建项目之前，会先构建 templates 目标。
add_dependencies( ${PROJECT_NAME} templates )

# 我们不应该依赖 CMAKE_BUILD_TYPE 变量（参考链接：https://github.com/asmaloney/GDExtensionTemplate/issues/25）  
# 但为了在当前项目和 godot-cpp 中修复此问题之前确保一切正常，我们需要检查 CMAKE_BUILD_TYPE 是否为我们期望的值。 
set ( ALLOWED_BUILDS "Debug;Release" ) # 允许的构建类型列表

# 检查 CMAKE_BUILD_TYPE 是否在允许的构建类型列表中。  
# 如果不在，则输出致命错误并停止构建过程。
if ( NOT "${CMAKE_BUILD_TYPE}" IN_LIST ALLOWED_BUILDS )
    message( FATAL_ERROR "CMAKE_BUILD_TYPE must be set to Debug or Release" )
endif()

# 根据构建类型获取GDExtension输入文件名  
string( TOLOWER ${CMAKE_BUILD_TYPE} BUILD_TYPE ) # 将 CMAKE_BUILD_TYPE 转换为小写，并存储在 BUILD_TYPE 变量中
set( GD_EXTENSION_FILE_INPUT template.${BUILD_TYPE}.gdextension.in ) # 根据构建类型设置GDExtension模板文件名  

# 如果使用MSYS2（通常与MinGW一起使用），则在模板文件中的库名前添加 "lib" 前缀的变通方法
if ( MINGW )
    set( LIB_PREFIX "lib")  # 设置 LIB_PREFIX 变量为 "lib"，用于在模板文件中可能需要的库名前缀  
endif()

# 从模板生成项目的.gdextension文件
set( GD_EXTENSION_FILE ${PROJECT_NAME}.gdextension ) # 设置生成的GDExtension文件名，基于项目名 
configure_file( ${GD_EXTENSION_FILE_INPUT} ${PROJECT_BINARY_DIR}/${PROJECT_NAME}/${GD_EXTENSION_FILE} )  # 使用 configure_file 命令从模板生成GDExtension文件，并放置在项目二进制目录下的项目文件夹中

# 从构建目录安装gdextension文件 
install(
    FILES ${BUILD_OUTPUT_DIR}/${GD_EXTENSION_FILE} # 指定要安装的文件，这里假设文件已经被放置到某个构建输出目录中  
    DESTINATION ${INSTALL_DIR} # 指定安装目标目录，${INSTALL_DIR} 应在之前被定义
)

# 清理之前设置的变量，避免潜在的污染 
unset( ALLOWED_BUILDS )  # 清除 ALLOWED_BUILDS 变量（尽管在此代码段之前未显示设置，但可能是在其他地方定义的）  
unset( BUILD_TYPE )  # 清除 BUILD_TYPE 变量  
unset( GD_EXTENSION_FILE )  # 清除 GD_EXTENSION_FILE 变量  
unset( GD_EXTENSION_FILE_INPUT )  # 清除 GD_EXTENSION_FILE_INPUT 变量  
unset( LIB_PREFIX )  # 清除 LIB_PREFIX 变量

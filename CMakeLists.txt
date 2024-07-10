# SPDX-License-Identifier: Unlicense  
# 声明本项目的许可证为无许可证（Unlicense），即放弃所有版权  

cmake_minimum_required( VERSION 3.22 )
# 设置CMake的最低版本要求为3.22  

message( STATUS "Using CMake ${CMAKE_VERSION}" )
# 打印当前使用的CMake版本信息  

# 强制要求外部构建（即源代码目录和构建目录分开）  
file( TO_CMAKE_PATH "${PROJECT_BINARY_DIR}/CMakeLists.txt" LOC_PATH )
# 将项目二进制目录下的CMakeLists.txt路径转换为CMake路径格式  

if ( EXISTS "${LOC_PATH}" )
    message( FATAL_ERROR "You cannot build in the source directory. Please use a build subdirectory." )
# 如果在源代码目录中检测到CMakeLists.txt，则报错并阻止构建  
endif()

# 添加模块路径  
list( APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/" )
# 将当前源目录下的cmake文件夹添加到CMake模块搜索路径中  

# 为所有构建类型开启链接时优化  
set( CMAKE_INTERPROCEDURAL_OPTIMIZATION_RELEASE ON )
# 仅在Release模式下开启跨过程优化  

# 输出编译命令到 compile_commands.json 文件（用于调试CMake问题）  
set( CMAKE_EXPORT_COMPILE_COMMANDS ON )

# 在macOS上构建通用库  
# 注意：CMAKE_OSX_ARCHITECTURES必须在project()之前设置  
if ( APPLE )
    set( CMAKE_OSX_ARCHITECTURES "x86_64;arm64" CACHE STRING "" )
# 如果是在macOS上构建，则设置目标架构为x86_64和arm64，以支持Apple Silicon和Intel Mac  
endif()


# 主项目信息  
# 定义项目名称为GDExtensionTemplate，使用的编程语言为C++，并设置版本号为0.1.0 
project( GDExtensionTemplate
    LANGUAGES
        CXX
    VERSION
        0.1.0
)

# 创建一个共享库，库名与项目名相同  
add_library( ${PROJECT_NAME} SHARED )

# 为目标（即我们的库）设置编译特性  
# 私有（PRIVATE）编译特性，指定使用C++17标准  
target_compile_features( ${PROJECT_NAME}
    PRIVATE
        cxx_std_17
)

# LIB_ARCH 变量用于存储正在构建的架构。它默认设置为构建系统的架构。  
# 对于 macOS，我们构建一个通用库（同时支持 arm64 和 x86_64 ）。 
set( LIB_ARCH ${CMAKE_SYSTEM_PROCESSOR} )
# 如果是在Apple平台上，将LIB_ARCH设置为"universal"，表示构建通用库  
if ( APPLE )
    set( LIB_ARCH "universal" )
endif()

# LIB_DIR 是实际库文件存放的目录。这个变量在构建目录和安装目录中都会用到，并且需要与 gdextension 文件中的路径保持一致。  
# 例如，对于 Linux 下的 release 版本和 x86_64 架构，路径为 "lib/Linux-x86_64/libGDExtensionTemplate.so" 
set( LIB_DIR "lib/${CMAKE_SYSTEM_NAME}-${LIB_ARCH}" )

message( STATUS "Building ${PROJECT_NAME} for ${LIB_ARCH} on ${CMAKE_SYSTEM_NAME}")

# 打印构建状态信息，显示正在为哪个架构和哪个系统构建项目名称 
set( BUILD_OUTPUT_DIR "${PROJECT_BINARY_DIR}/${PROJECT_NAME}/" )

# 设置目标属性  
# 这些属性包括：  
# CXX_VISIBILITY_PRESET hidden：设置C++符号的默认可见性为隐藏  
# VISIBILITY_INLINES_HIDDEN true：隐藏内联函数的可见性  
# RUNTIME_OUTPUT_DIRECTORY：运行时库文件的输出目录  
# LIBRARY_OUTPUT_DIRECTORY：库文件的输出目录 
set_target_properties( ${PROJECT_NAME}
    PROPERTIES
        CXX_VISIBILITY_PRESET hidden
        VISIBILITY_INLINES_HIDDEN true
        RUNTIME_OUTPUT_DIRECTORY "${BUILD_OUTPUT_DIR}/${LIB_DIR}"
        LIBRARY_OUTPUT_DIRECTORY "${BUILD_OUTPUT_DIR}/${LIB_DIR}"
)

# 如果未定义CMAKE_DEBUG_POSTFIX变量，则为项目目标设置DEBUG_POSTFIX属性，其值为"-d"  
# 这通常用于在构建调试版本时，在生成的可执行文件或库文件名中添加一个后缀，以便于区分  
if( NOT DEFINED CMAKE_DEBUG_POSTFIX )
    set_target_properties( ${PROJECT_NAME}
        PROPERTIES
            DEBUG_POSTFIX "-d"
    )
endif()

# 复制support_files目录下的额外文件到构建输出目录  
# 使用add_custom_command在目标构建完成后执行一个自定义命令  
# 该命令使用CMake的-E copy_directory选项来复制整个目录 
add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    # 指定该命令在目标构建完成后执行
    COMMAND ${CMAKE_COMMAND} -E copy_directory # 使用CMake的-E选项执行命令，这里是复制目录  
            "${CMAKE_SOURCE_DIR}/support_files" # 源目录，即项目源代码目录下的support_files目录
            ${BUILD_OUTPUT_DIR} # 目标目录，即之前设置的构建输出目录 
)

# 警告设置  
# 包含用于配置编译器警告的选项的脚本  
include( CompilerWarnings )

# 从Git创建并包含版本信息文件  
# 这个脚本可能负责从Git仓库中提取版本信息（如提交哈希、分支名等），并生成一个包含这些信息的文件  
include( GitVersionInfo )

# 添加子目录src到构建过程中  
# 这会处理src目录下的CMakeLists.txt文件，并构建其中的目标
add_subdirectory( src )

# 设置安装目录，用于安装库、扩展文件和支持文件  
# 安装位置为 ${CMAKE_INSTALL_PREFIX}/${PROJECT_NAME}/ 
set( INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/${PROJECT_NAME}/" )

# 打印安装目录信息
message( STATUS "Install directory: ${INSTALL_DIR}")

# 安装目标（库和可执行文件）  
# 将项目目标（${PROJECT_NAME}）安装到指定的安装目录  
# 库文件和运行时文件都安装到${INSTALL_DIR}/${LIB_DIR}目录下  
# 注意：这里假设LIB_DIR变量已在之前的代码中定义，用于指定库文件的子目录 
install( TARGETS ${PROJECT_NAME}
    LIBRARY
        DESTINATION ${INSTALL_DIR}/${LIB_DIR}
    RUNTIME
        DESTINATION ${INSTALL_DIR}/${LIB_DIR}
)

# 复制支持文件  
# 但这里实际上没有执行复制操作，因为使用了EXCLUDE模式匹配所有文件，导致没有文件被复制  
# 如果目的是复制除特定文件外的所有文件，需要调整PATTERN来排除这些特定文件
install( DIRECTORY "${CMAKE_SOURCE_DIR}/support_files/"
    DESTINATION ${INSTALL_DIR}
    PATTERN ".*" EXCLUDE # 这行排除了所有文件，可能是一个错误或者占位符  
)

# 添加templates子目录到构建过程中  
# 这会处理templates目录下的CMakeLists.txt文件，并构建其中的目标  
add_subdirectory( templates )

# ccache  
# 如果找到ccache，则启用它  
# ccache是一个编译器缓存工具，可以显著提高重复编译的速度  
include( ccache )

# 格式化  
# 添加一个自定义目标，用于一次性格式化所有代码  
# 这可能通过ClangFormat来实现，ClangFormat是一个基于LLVM的工具，用于自动格式化C/C++/Objective-C代码  
include( ClangFormat )

# godot-cpp  
# 从这里获取：https://github.com/godotengine/godot-cpp  
# 检查godot-cpp子模块是否已下载 
if ( NOT EXISTS "${CMAKE_CURRENT_LIST_DIR}/extern/godot-cpp/Makefile" )
    message(
        FATAL_ERROR
            "[${PROJECT_NAME}] The godot-cpp submodule was not downloaded. Please update submodules: git submodule update --init --recursive."
            # "[${PROJECT_NAME}] godot-cpp子模块未下载。请更新子模块：使用命令 git submodule update --init --recursive。"  
    )
endif()

# 设置GODOT_CPP_SYSTEM_HEADERS 为 ON，并将其作为缓存布尔值强制设置，虽然这里未提供具体描述，但通常这意味着将 godot-cpp 的头文件视为系统头文件处理
set( GODOT_CPP_SYSTEM_HEADERS ON CACHE BOOL "" FORCE )

# 将extern/godot-cpp目录作为子目录添加到构建过程中  
add_subdirectory( extern/godot-cpp )

# 设置godot-cpp目标的属性，将C++可见性预设为hidden  
# 这与主库的可见性需要保持一致，以确保二进制接口（ABI）的兼容性 
set_target_properties( godot-cpp
    PROPERTIES
        CXX_VISIBILITY_PRESET hidden    # visibility needs to be the same as the main library
)

# 将godot-cpp库链接到当前项目（${PROJECT_NAME}）的私有依赖中  
# 这意味着godot-cpp库将仅对当前项目可见，并且其接口不会暴露给链接到当前项目的其他库
target_link_libraries( ${PROJECT_NAME}
    PRIVATE
        godot-cpp
)

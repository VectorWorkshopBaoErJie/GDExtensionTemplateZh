// SPDX-License-Identifier: Unlicense

#include "godot_cpp/core/class_db.hpp"
#include "godot_cpp/core/version.hpp"

#include "GDExtensionTemplate.h"
#include "Version.h"

/// @file  
/// GDExtensionTemplate 示例实现文件。  
  
/*!  
@brief 获取此扩展的版本字符串。  
  
@details  
版本字符串是通过 cmake 和 src/Version.h.in 文件生成的。  
  
它采用“<项目名称> <最后的标签>-<#自最后标签以来的提交次数>-<简短的提交哈希>”的形式。  
如果没有自最后标签以来的提交，则仅显示标签。  
  
@return 版本字符串（例如 "Foo v1.2.3-gdedbd01"）。  
*/  
godot::String GDExtensionTemplate::version()  
{  
    return VersionInfo::VERSION_STR.data(); // 返回预定义在 VersionInfo 类中的 VERSION_STR 成员，该成员包含了版本信息字符串。  
}

/*!  
@brief 获取此扩展的 godot-cpp 版本字符串。  
  
@details  
版本字符串是通过 godot-cpp 的 core/version.hpp 生成的。它反映了 godot-cpp 库当前的版本信息。  
  
@return 版本字符串（例如 "godot-cpp v4.2.0-stable"）。  
*/  
godot::String GDExtensionTemplate::godotCPPVersion()  
{  
    // 拼接并返回 godot-cpp 的版本字符串，包括主版本号、次版本号、补丁号和状态（如 stable, beta 等）。  
    return "godot-cpp v" + godot::uitos( GODOT_VERSION_MAJOR ) + "." +  
           godot::uitos( GODOT_VERSION_MINOR ) + "." + godot::uitos( GODOT_VERSION_PATCH ) + "-" +  
           GODOT_VERSION_STATUS;  
}  
  
/// 将我们的方法绑定到 GDScript，以便 GDScript 可以访问它们。  
/// 这是在 Godot 引擎中注册 C++ 类的静态方法到 GDScript 的常规方式。  
void GDExtensionTemplate::_bind_methods()  
{  
    // 绑定 version 方法到 GDScript，使得在 GDScript 中可以调用 GDExtensionTemplate.version()。  
    godot::ClassDB::bind_static_method( "GDExtensionTemplate", godot::D_METHOD( "version" ),  
                                        &GDExtensionTemplate::version );  
    // 绑定 godotCPPVersion 方法到 GDScript，使得在 GDScript 中可以调用 GDExtensionTemplate.godot_cpp_version()。  
    // 注意：这里方法名在 GDScript 中被修改为 godot_cpp_version，以保持命名的一致性或避免与 GDScript 保留字冲突。  
    godot::ClassDB::bind_static_method( "GDExtensionTemplate",  
                                        godot::D_METHOD( "godot_cpp_version" ),  
                                        &GDExtensionTemplate::godotCPPVersion );  
}

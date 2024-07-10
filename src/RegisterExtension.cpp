// 从 godot-cpp/test/src 复制并修改。  
  
#include "gdextension_interface.h"  
  
#include "godot_cpp/core/class_db.hpp"  // 引入Godot C++绑定中的类数据库头文件，用于注册自定义类。  
#include "godot_cpp/core/defs.hpp"      // 引入Godot C++绑定中的定义头文件，包含一些宏定义和常量。  
#include "godot_cpp/godot.hpp"          // 引入Godot C++绑定的主头文件，提供了Godot API的C++封装。  
  
#include "Example.h"                    // 引入自定义的Example类头文件，该类需要在Godot中注册以便使用。  
#include "GDExtensionTemplate.h"        // 引入GDExtensionTemplate类头文件，可能是一个模板类用于注册多个Godot扩展。  
  
/// @file  
/// 此文件用于将我们的类注册到Godot引擎中，以便在Godot项目中可以使用这些类。  

namespace  
{  
    /// @brief 由Godot调用，以允许我们将自定义类注册到Godot中。  
    ///  
    /// @param p_level Godot正在初始化的级别  
    ///  
    /// @see GDExtensionInit 此函数是GDExtensionInit机制的一部分，用于在Godot引擎初始化时注册自定义模块或插件的类。  
    void initializeExtension( godot::ModuleInitializationLevel p_level )  
    {  
        // 仅当Godot正在初始化场景级别时才注册类  
        if ( p_level != godot::MODULE_INITIALIZATION_LEVEL_SCENE )  
        {  
            return;  
        }  
  
        // 注册一些自定义的类，包括普通类、单例类（如果ExampleRef设计为单例）、最小实例类（如果ExampleMin设计为最小实例类）  
        // 以及虚拟类和抽象类  
        godot::ClassDB::register_class<ExampleRef>(); // 注册一个可能作为单例的类  
        godot::ClassDB::register_class<ExampleMin>(); // 注册一个可能作为最小实例的类  
        godot::ClassDB::register_class<Example>();    // 注册一个普通类  
        godot::ClassDB::register_class<ExampleVirtual>(true); // 注册一个虚拟类，并指明需要实例化基类  
        godot::ClassDB::register_abstract_class<ExampleAbstract>(); // 注册一个抽象类  
  
        // 假设GDExtensionTemplate是一个用于演示或模板的类，也进行注册  
        godot::ClassDB::register_class<GDExtensionTemplate>();  
    }  
  
    /// @brief 由Godot调用，以允许我们进行任何清理工作。  
    ///  
    /// @see GDExtensionInit 与initializeExtension相对应，用于在Godot引擎卸载模块或插件时进行清理。  
    void uninitializeExtension( godot::ModuleInitializationLevel p_level )  
    {  
        // 通常，在模块卸载时不需要进行特定的清理，除非你有动态分配的资源需要释放  
        // 但如果仅针对场景级别进行注册，那么这里也仅在相应级别时返回  
        if ( p_level != godot::MODULE_INITIALIZATION_LEVEL_SCENE )  
        {  
            return;  
        }  
  
        // 注意：对于大多数用例，Godot的类注册是自动管理的，不需要手动取消注册。  
        // 如果你的扩展确实需要执行一些清理工作（如删除动态创建的资源），请在这里进行。  
    }  
}

extern "C"  
{  
    /// @brief 这是共享库的入口点。  
    ///  
    /// @note 此函数的名称必须与 templates/template.*.gdextension.in 文件中的 "entry_symbol" 相匹配，  
    ///       以确保Godot能够正确地加载和初始化此扩展库。  
    ///  
    /// @param p_get_proc_address 指向函数指针的接口，用于获取Godot API中的函数地址（需要更多信息）  
    /// @param p_library 指向库对象的指针，用于管理库中的类和资源（需要更多信息）  
    /// @param r_initialization 指向初始化结构的指针，用于存储初始化过程中的数据和状态（需要更多信息）  
    ///  
    /// @returns GDExtensionBool 返回GDExtensionBool类型，表示初始化是否成功。  
    ///  
    /// 此函数是 Godot 与 C++ 扩展交互的桥梁，它负责设置初始化函数、终止函数以及库所需的最小初始化级别。  
    GDExtensionBool GDE_EXPORT GDExtensionInit(  
        GDExtensionInterfaceGetProcAddress p_get_proc_address, GDExtensionClassLibraryPtr p_library,  
        GDExtensionInitialization *r_initialization )  
    {  
        // 使用RAII（Resource Acquisition Is Initialization）风格的对象来管理初始化过程  
        {  
            // 创建一个初始化对象，该对象封装了初始化过程  
            godot::GDExtensionBinding::InitObject init_obj(p_get_proc_address, p_library, r_initialization);  
  
            // 注册初始化函数，该函数在Godot加载扩展时被调用  
            init_obj.register_initializer(initializeExtension);  
  
            // 注册终止函数，该函数在Godot卸载扩展时被调用  
            init_obj.register_terminator(uninitializeExtension);  
  
            // 设置库所需的最小初始化级别为场景级别  
            // 这意味着库将在Godot的场景系统初始化时被加载  
            init_obj.set_minimum_library_initialization_level(godot::MODULE_INITIALIZATION_LEVEL_SCENE);  
  
            // 执行初始化过程，并返回结果  
            return init_obj.init();  
        }  
        // 注意：由于init_obj是在块作用域内创建的，一旦离开该作用域，它就会自动调用其析构函数进行清理（如果需要的话）  
        // 但在这个特定的例子中，由于我们直接返回了init_obj.init()的结果，并且没有在块外使用init_obj，  
        // 所以这里的RAII风格主要是为了清晰和封装初始化逻辑。  
    }  
}

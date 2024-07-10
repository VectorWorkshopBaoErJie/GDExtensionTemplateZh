[![GitHub](https://img.shields.io/github/license/asmaloney/GDExtensionTemplate)](协议) ![Build](https://github.com/asmaloney/GDExtensionTemplate/actions/workflows/main.yml/badge.svg)

# GDExtension 模版

本项目可作为创建新的基于 C++/CMake 的 Godot 4 GDExtension "扩展"的初始，可降低使用[CMake](https://cmake.org) 构建 GDExtension 的初始门槛。

目前对应 **[Godot 4.2](https://github.com/godotengine/godot/releases/tag/4.2-stable)** 这一发布版本。
 (这前版本可查看 [tags](https://github.com/asmaloney/GDExtensionTemplate/tags) ) 。

 CMake 常用于 C++ 开源项目，此项目为构建 Godot 扩展提供一个替代 _scons_ 系统的选择 (如果你使用 _scons_ ，可以查看 Nathan Franke 的 [gdextension](https://github.com/nathanfranke/gdextension) 模板或 Patrick 的 [GDExtensionSummator](https://github.com/paddy-exe/GDExtensionSummator)模板) 。

> **注意** 使用本时项目不要作为项目依赖，应当直接被复制使用，而不是分支的形式。通常 Git 并不擅长将一个项目的部分内容作为另一个项目的依赖项。 GitHub 提供了 **使用模板** 的按钮，在克隆仓库的旁边。这可以[创建一个没有所有历史记录的副本](https://docs.github.com/en/repositories/creating-and-managing-repositories/creating-a-repository-from-a-template)。

## 功能特性

此模板项目设置了大量构建细节，以便可以快速开始构建，专注于项目具体实现：

- 将 **[godot-cpp](https://github.com/godotengine/godot-cpp)作为子模块** 包含在内，并将其静态链接到您的共享库中
- 根据你的项目名称创建 `<project>.gdextension` 文件
- 将 `support_files` 目录中的任何支持文件复制到你的扩展目录中
- 包含将自定义图标添加到你的类/节点中的例子 ([见下文](#custom-node-icons))
- 自动生成一个 _**Version.h**_ 头文件，该文件：
  - 包含用于条件编译的预处理器宏
    ```cpp
    #if GDEXTENSIONTEMPLATE_VERSION < GDEXTENSIONTEMPLATE_VERSION_CHECK(2, 1, 0)
      // 执行操作
    #endif
    ```
  - 在版本字符串中包含 git 信息 (例如 `GDExtensionTemplate v1.0.1-gf6446f8` )
  - 包含一个示例，该示例将版本字符串暴露给 GDScript ，以便你可以像这样调用它
    ```py
    print(GDExtensionTemplate.version())
    ```
  - 当 git 分支/标签/HEAD 更改时，保持自身更新
- 使用 **[ccache](https://ccache.dev/)** (如果可用) 以加快重新构建速度
- 在macOS上构建**通用库** (x86_64和arm64) 
- 提供 **cmake目标** ：
  - _install_ ：将所有文件以正确的结构安装到 `CMAKE_INSTALL_PREFIX` 
  - _clang-format_ ：对所有源代码运行 `clang-format` 
- 包含用于以下操作的**GitHub工作流** (CI) ：
  - 在 **Linux x86_64** (gcc) 、**macOS通用** (clang) 和 **Windows x86_64** (MSVC) 上构建扩展
  - 在每次提交时生成调试和发布包
  - 在可用时使用 `ccache` 来缩短 CI 构建时间
  - 使用 `clang-format` 检查代码格式

## 先决条件

要在你的机器上本地使用此模板，需要以下工具：

- **[CMake](https://cmake.org/)** v3.22+
- 支持 **C++17** 以上的 C++ 编译器
- (可选) **[ccache](https://ccache.dev/)** ，用于加快重新构建速度
- (可选) **[clang-format](https://clang.llvm.org/docs/ClangFormat.html)** ，用于 linting 和自动代码格式化 (CI使用 clang-format 版本15)

GitHub Actions (CI) 已设置为包含所有这些工具。要查看如何在你的平台上下载它们，请查看[workflow](.github/workflows/main.yml)文件。

## 如何使用

### 安装

为了将此模板用于您自己的项目：

- _复制_ 此仓库，并将目录重命名为你的扩展名
  > GitHub 提供了一个 **使用此模板** 按钮 (在克隆仓库的旁边)。这将[创建一个没有所有历史记录的副本](https://docs.github.com/en/repositories/creating-and-managing-repositories/creating-a-repository-from-a-template)。
- 在 _CMakeLists.txt_ 中，将 `project` 宏中的 `GDExtensionTemplate` 更改为你的扩展名

  ```cmake
  project( <your_extension_name_here>
    LANGUAGES
        CXX
    VERSION
        0.1.0
  )
  ```
  如果您的项目中还包含纯 C 文件，请将 `C` 添加到语言列表中。
- 将 `src` 中的示例代码替换为你自己的代码 (建议保留 _RegisterExtension.cpp_ 并使用它来注册你的类)
  > **注意** 如果您更改了 _RegisterExtension.cpp_ 中的入口符号 (`GDExtensionInit`) ，则需要更新你的 `templates/*.gdextension.in` 文件。
- 将 `CHANGELOG.md` 替换为你自己的 (鼓励您遵守[语义化版本控制](https://semver.org/spec/v2.0.0.html)并使用[Keep a Changelog](https://keepachangelog.com/en/1.0.0/))
- 将此 `README.md` 替换为你自己的
- 替换自定义节点图标 ([见下文](#custom-node-icons))

可选：

- [为项目做贡献](#如何贡献) (不仅仅是💰!)
- 更改你想要支持的平台、架构：
  - 编辑 gdextension 模板 (`templates/*.gdextension.in`)
  - 更改 GitHub 工作流以构建正确的内容
- 更改 `.clang-format` 配置文件以符合你的 C++ 风格 ([选项文档](https://clang.llvm.org/docs/ClangFormatStyleOptions.html))
- 更改你想要强制执行的编译器警告 (请参阅[CompilerWarnings.cmake](cmake/CompilerWarnings.cmake))
- 更改 LICENSE

### 自定义节点图标

我已经为 `Example` 节点包含了一个自定义图标 (图标来自[SVGRepo](https://www.svgrepo.com/svg/207485/gardening-autumn)，并遵循[CC0](https://creativecommons.org/public-domain/cc0/)协议)，因此你可能需要删除或修改它以用于你自己的类或节点。

图标本身位于 `support_files/icons` 目录下，并在 `templates/*.gdextension.in` 文件中被引用。

要为您的自定义节点添加图标：

- 将图标文件添加到 `support_files/icons` 目录，并以你的节点命名(例如 `MyNode.svg` )
- 在每个 `templates/*.gdextension.in` 文件中，在 `[icons]` 部分为您的节点添加一个条目：
  ```
  MyNode = "icons/MyNode.svg"
  ```

`support_files` 目录中的所有内容都将复制到您的扩展中，因此如果您不想使用图标，请删除该目录并从 `templates/*.gdextension.in` 文件中删除 `[icons]` 部分。

### 构建与安装

以下是一个如何构建并安装发布版本的示例 (使用终端在此仓库的父目录中运行以下命令) :

#### 非 MSVC

```sh
$ cmake -B GDExtensionTemplate-build -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=GDExtensionTemplate-install GDExtensionTemplate
$ cmake --build GDExtensionTemplate-build --parallel
$ cmake --install GDExtensionTemplate-build
```

#### MSVC

```sh
$ cmake -B GDExtensionTemplate-build -G"Visual Studio 17 2022"  -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=GDExtensionTemplate-install GDExtensionTemplate
$ cmake --build GDExtensionTemplate-build --config Release
$ cmake --install GDExtensionTemplate-build
```

这告诉 CMake 使用 `Visual Studio 2022` 。在[CMake 网站](https://cmake.org/cmake/help/latest/manual/cmake-generators.7.html#visual-studio-generators) 上有一个 Visual Studio 生成器的列表——选择你正在使用的那个。

### Cmake 选项

此模板定义了以下额外的 CMake 选项：

| 选项                                                                       | 描述                                           | 默认值                                                                                                    |
| -------------------------------------------------------------------------- | ---------------------------------------------- | --------------------------------------------------------------------------------------------------------- |
| `CCACHE_PROGRAM`                                                           | `ccache` 的路径，用于加快重新构建速度          | 如果找到 `ccache` ，则此选项将自动设置为 **ON** 。如果您不想使用它，请将其设置为""。                      |
| `CLANG_FORMAT_PROGRAM`                                                     | `clang-format` 的路径，用于代码格式化。        | 如果 `clang-format` 已启用，则此选项将自动设置为 **ON** 。如果您不想使用它，请将其设置为""。              |
| `${PROJECT_NAME_UPPERCASE}_WARN_EVERYTHING` (例如`FOO_WARN_EVERYTHING`)    | 开启所有警告。(不适用于MSVC)                   | **OFF** (因为会过于嘈杂，但有时可能很有用)                                                                |
| `${PROJECT_NAME_UPPERCASE}_WARNING_AS_ERROR` (例如`FOO_WARNING_AS_ERROR`)  | 将警告视为错误。                               | **ON**                                                                                                    |


## 持续更新

一旦你的项目启动并运行，可能希望跟上 Godot 和 godot-cpp 的新版本。

关键是 _godot-cpp_ 的版本必须与你使用的 Godot 版本相匹配(请参阅 [godot-cpp 版本控制部分](https://github.com/godotengine/godot-cpp#versioning) )。因此，如果您想使用 _Godot 4.0 稳定版_ ，那么你需要将其与 [godot-cpp中的正确标签](https://github.com/godotengine/godot-cpp/tags) 相匹配。

一旦你知道了 godot-cpp 的正确版本，就更改你的扩展中的子模块(_extern/godot-cpp_)以指向该版本。

更新子模块并根据 API 更改对你的代码进行任何必要的更改，是你需要针对特定版本的 Godot 所做的唯一事情。

## 如何贡献

以下是一些您可以为项目做出贡献的方法：

### ✍ 撰写关于项目的文章

如果您觉得这个项目很有用，那就传播它吧！无论是文章、帖子、博客文章，只要是你喜欢的形式都可以。

### ⭐️ 点赞

如果您觉得这个项目很有用，请考虑给它点赞！这有助于我评估这个项目的有用程度。

### ☝ 提出问题

如果您遇到了不符合预期的问题，请提出[一个问题](https://github.com/asmaloney/GDExtensionTemplate/issues)，并提供所有相关信息以便复现问题，这将非常有帮助。

### 🐞 错误修复 & 🧪 新增功能

我很高兴审查任何[拉取请求](https://github.com/asmaloney/GDExtensionTemplate/pulls)。请尽量保持简短。每个拉取请求都应该是原子的，并且只解决一个问题。这有助于审查过程。

请注意，我不会接受所有内容，但我欢迎讨论。如果您提出重大更改，请首先将其作为[一个问题](https://github.com/asmaloney/GDExtensionTemplate/issues)提出以供讨论。

## 原始项目

本项目是 asmaloney [GDExtensionTemplate](https://github.com/asmaloney/GDExtensionTemplate.git) 的分支项目。

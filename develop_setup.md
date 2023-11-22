## 开发环境配置 ##
------------------------------------
### vscode (开发工具)    [opensource]

1. vscode 插件要求
	- ssh-remote     (远程连接)
	- clang-format  （代码格式化）
	- clangd         (代码补全及跳转)
	- cmake          (cmake 编译相关)
	- c/c++          (ms c/c++ 代码补全及跳转)
	- git            (git 相关)
	- python         (python 相关)

2. clangd 插件配置 (.vscode/settings.json)

```
{
	"editor.formatOnSave": true, // 保存文件自动格式化
	"C_Cpp.intelliSenseEngine": "disabled", // 禁用ms c/c++ 提示和补全功能
	"clangd.path": "clangd", // 直接使用环境变量
	"clangd.arguments": [
		"--all-scopes-completion", // 全局补全(补全建议会给出在当前作用域不可见的索引,插入后自动补充作用域标识符),例如在main()中直接写cout,即使没有`#include <iostream>`,也会给出`std::cout`的建议,配合"--header-insertion=iwyu",还可自动插入缺失的头文件
		"--background-index", // 后台分析并保存索引文件
		"--clang-tidy", // 启用 Clang-Tidy 以提供「静态检查」，下面设置 clang tidy 规则
		"--clang-tidy-checks=performance-*, bugprone-*, misc-*, google-*, modernize-*, readability-*, portability-*",
		"--compile-commands-dir=${workspaceFolder}/build/", // 编译数据库(例如 compile_commands.json 文件)的目录位置
		"--completion-parse=auto", // 当 clangd 准备就绪时，用它来分析建议
		"--completion-style=detailed", // 建议风格：打包(重载函数只会给出一个建议);还可以设置为 detailed
		"--enable-config",
		"--fallback-style=Webkit", // 默认格式化风格: 在没找到 .clang-format 文件时采用,可用的有 LLVM, Google, Chromium, Mozilla, Webkit, Microsoft, GNU
		"--function-arg-placeholders=true", // 补全函数时，将会给参数提供占位符，键入后按 Tab 可以切换到下一占位符，乃至函数末
		"--header-insertion-decorators", // 输入建议中，已包含头文件的项与还未包含头文件的项会以圆点加以区分
		"--header-insertion=iwyu", // 插入建议时自动引入头文件 iwyu
		"--include-cleaner-stdlib", // 为标准库头文件启用清理功能(不成熟!!!)
		"--log=verbose", // 让 Clangd 生成更详细的日志
		"--pch-storage=memory", // pch 优化的位置(Memory 或 Disk,前者会增加内存开销，但会提升性能)
		"--pretty", // 输出的 JSON 文件更美观
		"--ranking-model=decision_forest", // 建议的排序方案：hueristics (启发式), decision_forest (决策树)
		"-j=12" // 同时开启的任务数量
	],
	// Clangd 找不到编译数据库(例如 compile_flags.json 文件)时采用的设置,缺陷是不能直接索引同一项目的不同文件,只能分析系统头文件、当前文件和include的文件
	"clangd.fallbackFlags": [
		"-std=c++11",
		"-I${workspaceFolder}/src/include", //添加外部依赖 [不支持递归目录的搜索]
		// 配置内核头文件索引位置
		"-I/usr/src/linux-headers-6.2.0-33-generic/include",
		"-I/usr/src/linux-headers-6.2.0-33-generic/usr/include",
		"-I/usr/src/linux-headers-6.2.0-33-generic/arch/x86/include",
		"-I/usr/src/linux-headers-6.2.0-33-generic/arch/x86/include/generated",
		"-I/usr/src/linux-headers-6.2.0-33-generic/arch/ia64/include",
	]
}
```

3. vscode ssh-remote 免密登录

	- ssh-keygen -t rsa -C "email"
	- cat vscode-host-pc/.ssh/id_rsa.pub > remote-server/.ssh/authorized_keys
		    
### Virtualbox/ubuntu22.04 (虚拟机开发调试环境)   [opensource]

1. 网络连接方式的配置

	- 网络地址转换 NAT, 通过host指定的端口访问虚拟机
	- 桥接方式，虚拟机与host共享网络，需要手动配置虚拟机ipv4 地址(与host位于同一网段)，默认路由/DNS(与host相同)
	- 设置静态ip
	```
		network:
		version: 2
		renderer: NetworkManager
		ethernets:
		enp0s3:
			dhcp4: no
			dhcp6: no
			addresses: [10.0.2.15/24]
			gateway4: 10.0.2.2
			nameservers:
			addresses: [8.8.8.8, 114.114.114.114]
	```

2. virtualbox 虚拟机的显存必须设置到最大值


### git 相关配置 ###

- git config --global user.name "name"
- git config --global user.email "email"
- git config --global core.editor "vim"

### pip 相关配置 ###

- pip config set global.index-url "http://pypi.tuna.tsinghua.edu.cn/simple"
- pip config set install.trusted-host "pypi.tuna.tsinghua.edu.cn"


### 其他软件配置 ###                                 
- Windterm (SSH 远程工具)                        [opensource]
- Notepad++ (文本编辑工具)                       [opensource]
- 7-zip                                         [opensource]
- Visual studio community                       [opensource]
- WinMerge/Diffinity                            [opensource]


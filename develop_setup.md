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
	- Hex Editor     (二进制文件预览)

2. clangd 插件配置 (.vscode/settings.json)
- 安装 clang/clangd/clang-format/clang-tidy (14 或者更高版本)
- ubuntu18.04 安装最新版本 clang 参考: https://apt.llvm.org/
- clangd 全局配置
```
~/.config/clangd/config.yaml
CompileFlags:
    Add:
      - -std=c++11
      - -Wall
      - -Werror
      - -I./include

Diagnostics:
  ClangTidy:
    Add:
      - clang-analyzer-*
      - modernize-*
      - performance-*
      - readability-*

    Remove:
      - modernize-use-trailing-return-type
      - modernize-avoid-c-arrays
      - performance-no-int-to-ptr
      - readability-magic-numbers
      - readability-identifier-length
      - readability-function-cognitive-complexity
      - readability-implicit-bool-conversion
```

3. vscode ssh-remote 免密登录

	- ssh-keygen -t rsa -C "email"
	- cat vscode-host-pc/.ssh/id_rsa.pub > remote-server/.ssh/authorized_keys

4. 开启远程服务器 ssh 服务

	- apt install openssh-server openssh-client
	- service sshd start

5. 关闭防火墙

	- ufw disable(enable)

6. ubuntu-server 开启全屏
	- vim /etc/default/grub
	- set GRUB_CMDLINE_LINUX="vga=791" (设置tty默认分辨率为1024x768)
	- update-grub
	- reboot

7. clang-format 配置

```
${project}/.clang-format

---
# 语言: None, Cpp, Java, JavaScript, ObjC, Proto, TableGen, TextProto
Language:	Cpp
# BasedOnStyle:	LLVM
# 访问说明符(public、private等)的偏移
AccessModifierOffset:	-4
# 开括号(开圆括号、开尖括号、开方括号)后的对齐: Align, DontAlign, AlwaysBreak(总是在开括号后换行)
AlignAfterOpenBracket:	Align
# 连续赋值时，对齐所有等号
AlignConsecutiveAssignments:	true
# 连续声明时，对齐所有声明的变量名
AlignConsecutiveDeclarations:	true
# 左对齐逃脱换行(使用反斜杠换行)的反斜杠
AlignEscapedNewlinesLeft:	true
# 水平对齐二元和三元表达式的操作数
AlignOperands:	true
# 对齐连续的尾随的注释
AlignTrailingComments:	true
# 允许函数声明的所有参数在放在下一行
AllowAllParametersOfDeclarationOnNextLine:	true
# 允许短的块放在同一行
AllowShortBlocksOnASingleLine:	false
# 允许短的case标签放在同一行
AllowShortCaseLabelsOnASingleLine:	false
# 允许短的函数放在同一行: None, InlineOnly(定义在类中), Empty(空函数), Inline(定义在类中，空函数), All
AllowShortFunctionsOnASingleLine:	Empty
# 允许短的if语句保持在同一行
AllowShortIfStatementsOnASingleLine:	false
# 允许短的循环保持在同一行
AllowShortLoopsOnASingleLine:	false
# 总是在定义返回类型后换行(deprecated)
AlwaysBreakAfterDefinitionReturnType:	None
# 总是在返回类型后换行: None, All, TopLevel(顶级函数，不包括在类中的函数), 
#   AllDefinitions(所有的定义，不包括声明), TopLevelDefinitions(所有的顶级函数的定义)
AlwaysBreakAfterReturnType:	None
# 总是在多行string字面量前换行
AlwaysBreakBeforeMultilineStrings:	false
# 总是在template声明后换行
AlwaysBreakTemplateDeclarations:	false
# false表示函数实参要么都在同一行，要么都各自一行
BinPackArguments:	true
# false表示所有形参要么都在同一行，要么都各自一行
BinPackParameters:	true
# 大括号换行，只有当BreakBeforeBraces设置为Custom时才有效
BraceWrapping:   
  # class定义后面
  AfterClass:	true
  # 控制语句后面
  AfterControlStatement:	true
  # enum定义后面
  AfterEnum:	true
  # 函数定义后面
  AfterFunction:	true
  # 命名空间定义后面
  AfterNamespace:	true
  # ObjC定义后面
  AfterObjCDeclaration:	true
  # struct定义后面
  AfterStruct:	true
  # union定义后面
  AfterUnion:	true
  # catch之前
  BeforeCatch:	true
  # else之前
  BeforeElse:	true
  # 缩进大括号
  IndentBraces:	false
# 在二元运算符前换行: None(在操作符后换行), NonAssignment(在非赋值的操作符前换行), All(在操作符前换行)
BreakBeforeBinaryOperators:	NonAssignment
# 在大括号前换行: Attach(始终将大括号附加到周围的上下文), Linux(除函数、命名空间和类定义，与Attach类似), 
#   Mozilla(除枚举、函数、记录定义，与Attach类似), Stroustrup(除函数定义、catch、else，与Attach类似), 
#   Allman(总是在大括号前换行), GNU(总是在大括号前换行，并对于控制语句的大括号增加额外的缩进), WebKit(在函数前换行), Custom
#   注：这里认为语句块也属于函数
BreakBeforeBraces:	Custom
# 在三元运算符前换行
BreakBeforeTernaryOperators:	true
# 在构造函数的初始化列表的逗号前换行
BreakConstructorInitializersBeforeComma:	false
# 每行字符的限制，0表示没有限制
ColumnLimit:	120
# 描述具有特殊意义的注释的正则表达式，它不应该被分割为多行或以其它方式改变
CommentPragmas:	'^ IWYU pragma:'
# 构造函数的初始化列表要么都在同一行，要么都各自一行
ConstructorInitializerAllOnOneLineOrOnePerLine:	false
# 构造函数的初始化列表的缩进宽度
ConstructorInitializerIndentWidth:	4
# 延续的行的缩进宽度
ContinuationIndentWidth:	4
# 去除C++11的列表初始化的大括号{后和}前的空格
Cpp11BracedListStyle:	false
# 继承最常用的指针和引用的对齐方式
DerivePointerAlignment:	false
# 关闭格式化
DisableFormat:	false
# 自动检测函数的调用和定义是否被格式为每行一个参数(Experimental)
ExperimentalAutoDetectBinPacking:	false
# 需要被解读为foreach循环而不是函数调用的宏
ForEachMacros:	[ foreach, Q_FOREACH, BOOST_FOREACH ]
# 对#include进行排序，匹配了某正则表达式的#include拥有对应的优先级，匹配不到的则默认优先级为INT_MAX(优先级越小排序越靠前)，
#   可以定义负数优先级从而保证某些#include永远在最前面
IncludeCategories: 
  - Regex:	'^"(llvm|llvm-c|clang|clang-c)/'
    Priority:	2
  - Regex:	'^(<|"(gtest|isl|json)/)'
    Priority:	3
  - Regex:	'.*'
    Priority:	1
# 缩进case标签
IndentCaseLabels:	false
# 缩进宽度
IndentWidth:	4
# 函数返回类型换行时，缩进函数声明或函数定义的函数名
IndentWrappedFunctionNames:	false
# 保留在块开始处的空行
KeepEmptyLinesAtTheStartOfBlocks:	true
# 开始一个块的宏的正则表达式
MacroBlockBegin:	''
# 结束一个块的宏的正则表达式
MacroBlockEnd:	''
# 连续空行的最大数量
MaxEmptyLinesToKeep:	1
# 命名空间的缩进: None, Inner(缩进嵌套的命名空间中的内容), All
NamespaceIndentation:	Inner
# 使用ObjC块时缩进宽度
ObjCBlockIndentWidth:	4
# 在ObjC的@property后添加一个空格
ObjCSpaceAfterProperty:	false
# 在ObjC的protocol列表前添加一个空格
ObjCSpaceBeforeProtocolList:	true
# 在call(后对函数调用换行的penalty
PenaltyBreakBeforeFirstCallParameter:	19
# 在一个注释中引入换行的penalty
PenaltyBreakComment:	300
# 第一次在<<前换行的penalty
PenaltyBreakFirstLessLess:	120
# 在一个字符串字面量中引入换行的penalty
PenaltyBreakString:	1000
# 对于每个在行字符数限制之外的字符的penalty
PenaltyExcessCharacter:	1000000
# 将函数的返回类型放到它自己的行的penalty
PenaltyReturnTypeOnItsOwnLine:	60
# 指针和引用的对齐: Left, Right, Middle
PointerAlignment:	Left
# 允许重新排版注释
ReflowComments:	true
# 允许排序#include
SortIncludes:	false
# 在C风格类型转换后添加空格
SpaceAfterCStyleCast:	false
# 在赋值运算符之前添加空格
SpaceBeforeAssignmentOperators:	true
# 开圆括号之前添加一个空格: Never, ControlStatements, Always
SpaceBeforeParens:	ControlStatements
# 在空的圆括号中添加空格
SpaceInEmptyParentheses:	false
# 在尾随的评论前添加的空格数(只适用于//)
SpacesBeforeTrailingComments:	2
# 在尖括号的<后和>前添加空格
SpacesInAngles:	true
# 在容器(ObjC和JavaScript的数组和字典等)字面量中添加空格
SpacesInContainerLiterals:	true
# 在C风格类型转换的括号中添加空格
SpacesInCStyleCastParentheses:	true
# 在圆括号的(后和)前添加空格
SpacesInParentheses:	true
# 在方括号的[后和]前添加空格，lamda表达式和未指明大小的数组的声明不受影响
SpacesInSquareBrackets:	true
# 标准: Cpp03, Cpp11, Auto
Standard:	Cpp11
# tab宽度
TabWidth:	4
# 使用tab字符: Never, ForIndentation, ForContinuationAndIndentation, Always
UseTab:	Never
...

```
		    
### Virtualbox/ubuntu22.04 (虚拟机开发调试环境)   [opensource]

1. 网络连接方式的配置

	- 网络地址转换 NAT, 通过host指定的端口访问虚拟机
	- 桥接方式，虚拟机与host共享网络，需要手动配置虚拟机ipv4 地址(与host位于同一网段)，默认路由/DNS(与host相同)
	- 设置静态ip
	- apt install net-tools; ifconfig; route -n;(查看ip和网关)
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

- apt install python3-pip
- pip config set global.index-url "http://pypi.tuna.tsinghua.edu.cn/simple"
- pip config set install.trusted-host "pypi.tuna.tsinghua.edu.cn"
- pip install or python3 -m pip install target


### 其他软件配置 ###                                 
- Windterm (SSH 远程工具)                        [opensource]
- Notepad++ (文本编辑工具)                       [opensource]
- 7-zip                                         [opensource]
- Visual studio community                       [opensource]
- WinMerge/Diffinity                            [opensource]


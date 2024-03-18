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
      - readability-magic-numbers
      - readability-identifier-length
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


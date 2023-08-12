# 工程项目

## 编译
### 在vs_project文件夹下，包含x64和x86项目
* 支持库已经打包在**vs_project\thirdParty**文件夹下，应该可以做到开箱即用（maybe）
* 用Visual Studio 2022 (建议VS 2022) 打开 			
  *  vs_project\proj_msvc2022_x86_mt\Demo.sln  
    或者
  *  vs_project\proj_msvc2022_x64_mt\Demo.sln
  *  每个项目分别有两个项目配置:  
     * Console -> Debug 有终端输出
     * Window -> Release 无终端  

### 使用以下库静态库:  

**Qt库**  
* Qt5Widgets.lib  
* Qt5Core.lib  
* Qt5Gui.lib  
* qtmain.lib  (子系统设置为窗口时需要) 
* qtmultimedia.lib   

**Windows系统**  
* Winmm.lib  

**[Openssl]**
* libssl.lib  
* libcrypto.lib

[Openssl]:https://slproweb.com/products/Win32OpenSSL.html  

### 引入头文件[\<httplib.h\>](https://github.com/yhirose/cpp-httplib)

### 运行时需要的动态库:  

**Qt打包运行环境**
安装Qt后，使用Qt\bin下的windeployqt.exe 执行: 
```shell
windeployqt.exe "编译之后的程序.exe"
```
**Openssl (32位)**
* libssl-3.dll  
* libcrypto-3.dll  

**Openssl (64位)**
* libssl-3-x64.dll  
* libcrypto-3-x64.dll

**对于没有Visual C++的环境需要额外安装或者将以下dll放在exe文件同目录下**

* vcruntime140_1.dll 	**(x64)**
* vcruntime140.dll 		**(x86/x64)**
* msvcp140.dll 			**(x86/x64)**
* msvcp140_1.dll 		**(x86/x64)**

### 如果编译出现错误可以考虑将 编译中间文件输出目录 XXX.dir 删除
### 以上步骤可能无法解决所有问题，需要自行查找方案.
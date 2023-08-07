# Live2D Desktop Mascot   

## [更新内容](CHANGELOG.md)
### 2023-08-07
***changes***
* 聊天记录使用 sqlite 储存
* 重新启动应用会加载当天的所有聊天记录
* 聊天记录现在可以以天为单位在历史记录窗口查询
* 拆分 UI 资源和模型资源文件夹，现在 UI 资源固定放置在 assets 文件夹下
* 源代码由 GBK 更改为 UTF-8
* 自定义文本聊天和语音聊天统一用 json 格式进行传输，请求类型为 POST，参见[自定义聊天服务器模板](dev-server/dev-server.py)

***add***
* 添加随机播放内置文本，数据源一言：https://github.com/hitokoto-osc/sentences-bundle

***fix***
* 历史记录中播放音频无效
* 自定义语音聊天开启后再次启动程序开启无效

## 预览  

![预览1](sample_images/preview1.png)

![预览2](sample_images/preview2.png)

![预览3](sample_images/preview3.png)

Live2D 模型: [魔女之旅伊蕾娜：番剧画风live2d 活的！](https://www.bilibili.com/video/BV1KU4y1x7ep)  


## 功能 

* ### 自定义聊天服务器接口（可在设置页面更改）
  
	详细见[自定义聊天服务器模板](dev-server/dev-server.py)

	动作组 **Chat**，调用茉莉云或者自定义聊天接口时触发的动作。

	每次聊天都会在本地产生文本文件和音频缓存，注意及时清理。

* ### 自定义动作组与点击触发位置  


  参考 https://github.com/murcherful/Live2D_Displyer


## 注意事项

* 模型文件夹即Resources文件夹，其位置可更改，但至少有一个模型

* 音频和动作文件列表应放在对应模型的sounds和motions文件夹内，不能为其他位置  

* 音频文件只支持.wav格式，口型同步只支持单声道的.wav格式

* 程序错误无法启动时可删去config.json文件以初始化

* LPK模型需要解压为包含*.moc3, *.model3.json和纹理图片的文件夹形式

### 已知问题
- 适用 Unity 的 Live2D 模型可以加载但可能出现图层错误（穿模？）
- Cubism 2.0 及以前版本的模型（.moc，.mnt）无法加载
- 背景透明问题[#12](https://github.com/Arkueid/DesktopLive2D/issues/12) 
- <del>在历史记录中显示语音识别的结果 [#5](https://github.com/Arkueid/DesktopLive2D/issues/5) </del>
#### 关于背景透明的问题[#12](https://github.com/Arkueid/DesktopLive2D/issues/12) 

目前成功解决的一个场景如下：

在旧电脑上使用官方镜像安装 Windows 10 家庭版，未升级显卡驱动前，启动程序出现纯黑背景，更换模型出现#12的问题。升级显卡驱动后，使用 **集成显卡** 运行程序，无论是启动还是更换模型，#12的问题没有出现。使用 **独立显卡** 运行程序，启动时没有出现背景透明问题，但是更换模型后仍然出现背景透明问题。

背景透明问题可能和显卡驱动版本、显卡类型（核显、独显）有关。

## 开发环境
* Visual Studio 2022
* [Cubism Live2D SDK for Native]
* [Qt5](https://www.qt.io/download-qt-installer?hsCtaTracking=99d9dd4f-5681-48d2-b096-470725510d34%7C074ddad0-fdef-4e53-8aa8-5e8a876d6ab4)  
* [jsoncpp]  
* [cpp-httplib] + [openssl](https://slproweb.com/products/Win32OpenSSL.html)  
* 模型的语音借助[ACGTTS]生成  
* live2d模型来自:  
	* 碧蓝航线  
	* Cubism官方   
	* [再看一眼就会爆炸！波奇酱免费模型2.0](https://www.bilibili.com/video/BV1PY411k7Kj)  
	* [【虚拟主播模型】 宁宁vup化！](https://www.bilibili.com/video/BV1s7411d7y9)
* 
	以Cubism官方Native SDK为模板，结合以下两篇文章进行的修改：  
	https://zhuanlan.zhihu.com/p/126276925  
	https://zhuanlan.zhihu.com/p/511077879 

[cpp-httplib]:https://github.com/yhirose/cpp-httplib
[jsoncpp]:https://github.com/open-source-parsers/jsoncpp
[Cubism Live2D SDK for Native]:https://www.live2d.com/download/cubism-sdk/

[ACGTTS]:https://github.com/chinoll/ACGTTS  

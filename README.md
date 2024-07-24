# QRcode_scanner
基于qt的c++程序，通过快捷键来区域截图扫描二维码，使用Ctrl+Alt+Q进行截图。

## 使用说明
1. **快捷键截图**：当检测到Ctrl+Alt+Q时，自动弹出截图界面进行截图。
2. **托盘图标**：程序自动设置托盘图标，用户可以通过托盘图标退出程序或者显示主界面。
3. **复制二维码内容**：截图后，程序会自动检测二维码，识别内容并弹出窗口，用户可以选择复制内容或关闭窗口。

## 项目结构及文件说明
```
│  .gitignore               # 指定忽略的文件和目录
│  CMakeLists.txt           # CMake构建配置文件
│  CMakeLists.txt.user      # 用户特定的CMake配置文件
│  globalshortcut.cpp       # 实现全局快捷键功能的源文件
│  globalshortcut.h         # 声明全局快捷键功能的头文件
│  main.cpp                 # 程序的入口文件
│  mainwindow.cpp           # 实现主窗口功能的源文件
│  mainwindow.h             # 声明主窗口功能的头文件
│  mainwindow.ui            # 定义主窗口界面的UI文件
│  qrcodedialog.cpp         # 实现二维码识别对话框功能的源文件
│  qrcodedialog.h           # 声明二维码识别对话框功能的头文件
│  qrcodedialog.ui          # 定义二维码识别对话框界面的UI文件
│  README.md                # 项目简介及使用说明
│  resources.qrc            # 资源文件，包括图标等
│  screenshot.cpp           # 实现截图功能的源文件
│  screenshot.h             # 声明截图功能的头文件
│  trayicon.cpp             # 实现系统托盘图标功能的源文件
│  trayicon.h               # 声明系统托盘图标功能的头文件
│  vcpkg.json               # vcpkg包管理器配置文件
│
└─icons
    └── tray_icon.png       # 系统托盘图标文件
```

## 依赖项
- Qt5
- OpenCV

## 功能概述
- **截图功能**：使用快捷键Ctrl+Alt+Q进行截图，并弹出截图界面。
- **二维码识别**：在截图中自动识别二维码内容。
- **托盘图标**：程序在系统托盘中显示图标，用户可以通过图标进行程序的退出或显示主界面操作。

## 注意事项
本项目遵循MIT许可证，欢迎贡献代码或报告问题！
#include "globalshortcut.h"
#include <QApplication>
#include <QDebug>
#include <windows.h>


GlobalShortcut::GlobalShortcut(QObject *parent) : QObject(parent), timer(new QTimer(this))
{
    // 连接定时器的timeout信号到checkShortcut槽，以定期检查快捷键是否被按下
    connect(timer, &QTimer::timeout, this, &GlobalShortcut::checkShortcut);
    // 启动定时器，间隔100毫秒
    timer->start(100);
}


GlobalShortcut::~GlobalShortcut()
{
}

// 检查快捷键是否被按下，如果是，则发出shortcutActivated信号
void GlobalShortcut::checkShortcut()
{
    // 如果Ctrl+Alt+Q被按下，则发出信号
    if (isCtrlAltDPressed()) {
        emit shortcutActivated();
    }
}

// 检测Ctrl+Alt+Q是否被按下
bool GlobalShortcut::isCtrlAltDPressed()
{
    // 使用GetAsyncKeyState函数检查Ctrl, Alt和Q键是否同时被按下
    return (GetAsyncKeyState(VK_CONTROL) & 0x8000) && (GetAsyncKeyState(VK_MENU) & 0x8000) && (GetAsyncKeyState('Q') & 0x8000);
}
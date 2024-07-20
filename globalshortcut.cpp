#include "globalshortcut.h"
#include <QApplication>
#include <QDebug>
#include <windows.h>

GlobalShortcut::GlobalShortcut(QObject *parent) : QObject(parent), timer(new QTimer(this))
{
    connect(timer, &QTimer::timeout, this, &GlobalShortcut::checkShortcut);
    timer->start(100);
}

GlobalShortcut::~GlobalShortcut()
{
}

void GlobalShortcut::checkShortcut()
{
    if (isCtrlAltDPressed()) {
        emit shortcutActivated();
    }
}

bool GlobalShortcut::isCtrlAltDPressed()
{
    return (GetAsyncKeyState(VK_CONTROL) & 0x8000) && (GetAsyncKeyState(VK_MENU) & 0x8000) && (GetAsyncKeyState('Q') & 0x8000);
}

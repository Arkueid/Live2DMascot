/**
 * Copyright(c) Live2D Inc. All rights reserved.
 *
 * Use of this source code is governed by the Live2D Open Software license
 * that can be found at https://www.live2d.com/eula/live2d-open-software-license-agreement_en.html.
 */
#include <iostream>
#include <fstream>
#include "LAppDelegate.hpp"
#include "LAppDefine.hpp"
#include "glwidget.h"
#include <QtWidgets/qapplication.h>
#include <QtWidgets/qopenglwidget.h>
#include "json/json.h"
#include <unordered_map>
#include <vector>
#include <QtWidgets/qmessagebox.h>
#include <QtCore/qtextcodec.h>
#include "LApp.h"

using namespace std;
using namespace LAppDefine;


int main(int argc, char* argv[])
{
    LApp::GetInstance()->Initialize(argc, argv);
    LApp::GetInstance()->Run();
    LApp::GetInstance()->Release();
    return 0;
}
/**
 * Copyright(c) Live2D Inc. All rights reserved.
 *
 * Use of this source code is governed by the Live2D Open Software license
 * that can be found at https://www.live2d.com/eula/live2d-open-software-license-agreement_en.html.
 */
#include "LApp.h"


int main(int argc, char* argv[])
{
    LApp::GetInstance()->Initialize(argc, argv);
    LApp::GetInstance()->Run();
    LApp::GetInstance()->Release();
    return 0;
}
﻿/**
 * Copyright(c) Live2D Inc. All rights reserved.
 *
 * Use of this source code is governed by the Live2D Open Software license
 * that can be found at https://www.live2d.com/eula/live2d-open-software-license-agreement_en.html.
 */

#include "CubismMoc.hpp"
#include "CubismModel.hpp"
#include <fstream>

namespace Live2D { namespace Cubism { namespace Framework {

CubismMoc* CubismMoc::Create(const csmByte* mocBytes, csmSizeInt size)
{
    CubismMoc* cubismMoc = NULL;

    void* alignedBuffer = CSM_MALLOC_ALLIGNED(size, Core::csmAlignofMoc);
    memcpy(alignedBuffer, mocBytes, size);

    Core::csmMoc* moc = Core::csmReviveMocInPlace(alignedBuffer, size);
    const Core::csmMocVersion version = Core::csmGetMocVersion(alignedBuffer, size);

    if (moc)
    {
        cubismMoc = CSM_NEW CubismMoc(moc);
        cubismMoc->_mocVersion = version;
    }
    
   /* if (HasMocConsistency(moc, size)) CubismLogInfo("Moc file is valid");
    else {
        std::ofstream f("CubismMoc_fatal_error.log", std::ios::out);
        if (f.is_open()) {
            f << "Please check moc file of current model because it failed to pass the consistency test, which means it maybe insecure.\n";
        }
        f.close();
        exit(0);
    }*/
    return cubismMoc;
}

void CubismMoc::Delete(CubismMoc* moc)
{
    CSM_DELETE_SELF(CubismMoc, moc);
}

CubismMoc::CubismMoc(Core::csmMoc* moc)
                        : _moc(moc)
                        , _modelCount(0)
                        , _mocVersion(0)
{ }

CubismMoc::~CubismMoc()
{
    CSM_ASSERT(_modelCount == 0);

    CSM_FREE_ALLIGNED(_moc);
}

CubismModel* CubismMoc::CreateModel()
{
    CubismModel*     cubismModel = NULL;
    const csmUint32  modelSize = Core::csmGetSizeofModel(_moc);
    void*            modelMemory = CSM_MALLOC_ALLIGNED(modelSize, Core::csmAlignofModel);

    Core::csmModel* model = Core::csmInitializeModelInPlace(_moc, modelMemory, modelSize);

    if (model)
    {
        cubismModel = CSM_NEW CubismModel(model);
        cubismModel->Initialize();

        ++_modelCount;
    }

    return cubismModel;
}

void CubismMoc::DeleteModel(CubismModel* model)
{
    CSM_DELETE_SELF(CubismModel, model);
    --_modelCount;
}

Core::csmMocVersion CubismMoc::GetLatestMocVersion()
{
    return Core::csmGetLatestMocVersion();
}

Core::csmMocVersion CubismMoc::GetMocVersion()
{
    return _mocVersion;
}

csmBool CubismMoc::HasMocConsistency(void* address, const csmUint32 size)
{
    csmInt32 consistencyFlags = Core::csmHasMocConsistency(address, size);
    return consistencyFlags != 0 ? true : false;
}

}}}
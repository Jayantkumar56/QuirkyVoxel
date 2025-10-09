

// SPDX - License - Identifier: MIT
// Copyright(c) 2025 Jayantkumar56


#pragma once


#include "Layer.h"


class GameLayer : public Layer {
public:
    virtual void OnAttach() override;
    virtual void OnUpdate() override;
};

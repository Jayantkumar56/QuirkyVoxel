

// SPDX - License - Identifier: MIT
// Copyright(c) 2025 Jayantkumar56


#include "LayerManager.h"


namespace Mct {

	LayerManager::LayerManager(Application* app) noexcept :
            m_App         ( app ),
            m_IdGenerator ( 0   )
    {}

    void LayerManager::Pop() {
        m_ActionsQueue.emplace_back(LayerOp::Pop, InvalidId<size_t>, nullptr);
    }

    void LayerManager::Clear() {
        m_ActionsQueue.emplace_back(LayerOp::Clear, InvalidId<size_t>, nullptr);
    }

    void LayerManager::OnEvent(Event& e) {
        for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); ++it) {
            if (e.IsHandled())
                break;

            if ((*it)->IsDisabled())
                continue;

            (*it)->OnEvent(e);
        }
    }

    void LayerManager::UpdateLayers(float deltaTime) {
        ImguiContext::Begin();

        for (auto& layer : m_LayerStack) {
            if (!layer->IsDisabled()) {
                layer->OnUpdate(deltaTime);
            }
        }

        ImguiContext::End();
    }

    void LayerManager::Update() {
        if (m_ActionsQueue.empty())
            return;

        for (auto& action : m_ActionsQueue) {
            switch (action.Operation) {
                case LayerOp::Push            : PushImpl(std::move(action.Layer));     break;
                case LayerOp::Clear           : ClearImpl();                           break;
                case LayerOp::Pop             : PopImpl();                             break;
                case LayerOp::Remove          : RemoveImpl(action.LayerId);            break;
            }
        }

        m_ActionsQueue.clear();
    }

    void LayerManager::PushImpl(std::unique_ptr<Layer> layer) {
        std::unique_ptr<Layer>& pushedLayer = m_LayerStack.emplace_back(std::move(layer));
        pushedLayer->OnAttach();
    }

    void LayerManager::ClearImpl() {
        while (!m_LayerStack.empty()) {
            PopImpl();
        }
    }

    void LayerManager::PopImpl() {
        if (m_LayerStack.empty()) {
            MCT_WARN("No layer to pop.");
            return;
        }

        std::unique_ptr<Layer>& top = m_LayerStack.back();
        top->OnDetach();
        m_LayerStack.pop_back();
    }

    void LayerManager::RemoveImpl(size_t layerId) {
        auto it = std::find_if(m_LayerStack.begin(), m_LayerStack.end(),
            [layerId](const std::unique_ptr<Layer>& layer) noexcept -> bool {
                return layer->GetInstanceId() == layerId;
            }
        );

        if (it == m_LayerStack.end()) {
            MCT_WARN("Could not find layer with InstanceId: {}", layerId);
            return;
        }

        (*it)->OnDetach();
        m_LayerStack.erase(it);
    }

}

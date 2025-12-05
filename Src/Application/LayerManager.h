

// SPDX - License - Identifier: MIT
// Copyright(c) 2025 Jayantkumar56


#pragma once


#include "Layer.h"
#include "ImguiContext.h"
#include "Events/Event.h"

#include <vector>
#include <memory>


namespace Mct {

    class Application;

	class LayerManager {
    private:
        enum class LayerOp { Push, Clear, Pop, Remove };

        struct LayerAction {
            LayerOp Operation;
            size_t  LayerId;
            std::unique_ptr<Layer> Layer;
        };

    public:
        LayerManager(Application* app) noexcept;

		template<typename T, typename ...Args>
        requires (std::derived_from<T, Layer>&& std::constructible_from<T, Args...>)
        void PushLayer(Args&& ...args) {
            auto& action = m_ActionsQueue.emplace_back(
                LayerOp::Push,
                InvalidId<size_t>,
                std::make_unique<T>(std::forward<Args>(args)...)
            );

            action.Layer->m_Disabled   = false;
            action.Layer->m_TypeId     = TypeIdGenerator::Get<T>();
            action.Layer->m_InstanceId = m_IdGenerator.GetNext();
            action.Layer->m_App        = m_App;
        }

        template<typename T>
        requires std::derived_from<T, Layer>
        void Remove() {
            size_t typeId = TypeIdGenerator::Get<T>();

            for (const auto& layer : m_LayerStack) {
                if (typeId == layer->GetTypeId()) {
                    m_ActionsQueue.emplace_back(LayerOp::Remove, layer->GetInstanceId(), nullptr);
                }
            }
        }

        template<typename T>
        requires std::derived_from<T, Layer>
        void RemoveFirstFromTop() {
            size_t typeId = TypeIdGenerator::Get<T>();

            for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); ++it) {
                if (typeId == (*it)->GetTypeId()) {
                    m_ActionsQueue.emplace_back(LayerOp::Remove, (*it)->GetInstanceId(), nullptr);
                    break; // Found the top-most one, stop.
                }
            }
        }

        void Pop();
        void Clear();
        void OnEvent(Event& e);
        void UpdateLayers(float deltaTime);
        void Update();

    private:
        void PushImpl(std::unique_ptr<Layer> layer);
        void ClearImpl();
        void PopImpl();
        void RemoveImpl(size_t layerId);

	private:
        Application* m_App;
        LinearIdGenerator<size_t>           m_IdGenerator;
        std::vector<LayerAction>            m_ActionsQueue;
		std::vector<std::unique_ptr<Layer>> m_LayerStack;
	};

}

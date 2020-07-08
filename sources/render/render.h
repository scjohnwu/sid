#pragma once
#include "scene.h"

namespace sid {
    class Render {
        public:

        void Draw();

        private:

        ScenePtr m_CurrentScene;
    };
}
#pragma once

#include "node_base.h"

namespace CyclesShaderEditor {

	class MaterialOutputNode : public EditorNode {
	public:
		MaterialOutputNode(Point2 position);

		virtual bool can_be_deleted() override;
	};

}

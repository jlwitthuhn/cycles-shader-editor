#pragma once

#include "node_base.h"

namespace cse {

	class MaterialOutputNode : public EditableNode {
	public:
		MaterialOutputNode(FloatPos position);

		virtual bool can_be_deleted() override;
	};

}

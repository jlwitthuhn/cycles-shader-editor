#pragma once
// Header to contain typedefs for template abominations

#include <memory>
#include <set>

namespace cse {
	class EditableNode;

	typedef std::set<std::weak_ptr<EditableNode>, std::owner_less<std::weak_ptr<EditableNode>>> WeakNodeSet;
	typedef std::set<std::shared_ptr<EditableNode>, std::owner_less<std::shared_ptr<EditableNode>>> SharedNodeSet;
}

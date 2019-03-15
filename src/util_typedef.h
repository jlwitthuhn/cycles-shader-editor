#pragma once
// Header to contain typedefs for template abominations

#include <memory>
#include <set>

namespace cse {
	class EditorNode;

	typedef std::set<std::weak_ptr<EditorNode>, std::owner_less<std::weak_ptr<EditorNode>>> WeakNodeSet;
	typedef std::set<std::shared_ptr<EditorNode>, std::owner_less<std::shared_ptr<EditorNode>>> SharedNodeSet;
}

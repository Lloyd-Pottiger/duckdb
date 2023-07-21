#include "duckdb/optimizer/join_order/join_node.hpp"

#include "duckdb/common/limits.hpp"
#include "duckdb/planner/expression/list.hpp"
#include "duckdb/planner/operator/list.hpp"

namespace duckdb {

JoinNode::JoinNode(optional_ptr<JoinRelationSet> set) : set(set), info(nullptr), left(nullptr), right(nullptr) {
}

JoinNode::JoinNode(optional_ptr<JoinRelationSet> set, optional_ptr<NeighborInfo> info, optional_ptr<JoinNode> left,
                   optional_ptr<JoinNode> right, idx_t cost)
    : set(set), info(info), left(left), right(right), cost(cost) {
}

unique_ptr<EstimatedProperties> EstimatedProperties::Copy() {
	auto result = make_uniq<EstimatedProperties>(cardinality, cost);
	return result;
}

string JoinNode::ToString() {
	string result = "-------------------------------\n";
	result += set->ToString() + "\n";
	result += "cost = " + to_string(cost) + "\n";
	result += "left = \n";
	if (left) {
		result += left->ToString();
	}
	result += "right = \n";
	if (right) {
		result += right->ToString();
	}
	return result;
}

} // namespace duckdb

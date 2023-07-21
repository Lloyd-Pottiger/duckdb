//===----------------------------------------------------------------------===//
//                         DuckDB
//
// duckdb/optimizer/join_order/join_relation.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "duckdb/common/common.hpp"
#include "duckdb/common/unordered_map.hpp"
#include "duckdb/common/unordered_set.hpp"
#include "duckdb/common/optional_ptr.hpp"

namespace duckdb {
class LogicalOperator;

//! Set of relations, used in the join graph.
struct JoinRelationSet {
	JoinRelationSet(unsafe_unique_array<idx_t> relations, idx_t count) : relations(std::move(relations)), count(count) {
	}

	string ToString() const;

	unsafe_unique_array<idx_t> relations;
	idx_t count;

	static bool IsSubset(optional_ptr<JoinRelationSet> super, optional_ptr<JoinRelationSet> sub);
};

//! The JoinRelationTree is a structure holding all the created JoinRelationSet objects and allowing fast lookup on to
//! them
class JoinRelationSetManager {
public:
	//! Contains a node with a JoinRelationSet and child relations
	// FIXME: this structure is inefficient, could use a bitmap for lookup instead (todo: profile)
	struct JoinRelationTreeNode {
		unique_ptr<JoinRelationSet> relation;
		unordered_map<idx_t, unique_ptr<JoinRelationTreeNode>> children;
	};

public:
	//! Create or get a JoinRelationSet from a single node with the given index
	optional_ptr<JoinRelationSet> GetJoinRelation(idx_t index);
	//! Create or get a JoinRelationSet from a set of relation bindings
	optional_ptr<JoinRelationSet> GetJoinRelation(const unordered_set<idx_t> &bindings);
	//! Create or get a JoinRelationSet from a (sorted, duplicate-free!) list of relations
	optional_ptr<JoinRelationSet> GetJoinRelation(unsafe_unique_array<idx_t> relations, idx_t count);
	//! Union two sets of relations together and create a new relation set
	optional_ptr<JoinRelationSet> Union(optional_ptr<JoinRelationSet> left, optional_ptr<JoinRelationSet> right);
	// //! Create the set difference of left \ right (i.e. all elements in left that are not in right)
	// JoinRelationSet *Difference(JoinRelationSet *left, JoinRelationSet *right);

private:
	JoinRelationTreeNode root;
};

} // namespace duckdb

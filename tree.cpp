#include <iostream>
#include <vector>
#include <unordered_map>
#include <sstream>
#include <string>
#include <cctype>
#include <vector>
#include <cstdint>

using namespace std;

// 定义树的节点
struct TreeNode {
    int value;  // 节点值
    vector<TreeNode*> children;  // 存储子节点
    TreeNode() {}
    TreeNode(int val)
    {
        value = val;
    }
};

// 递归打印树的结构（前序遍历）
void printTree(TreeNode* root) {
    if (!root) return;

    // 打印当前节点的值
    cout << root->value;

    // 如果有子节点，则递归打印每个子节点
    if (!root->children.empty()) {
        cout << " { ";
        for (size_t i = 0; i < root->children.size(); ++i) {
            printTree(root->children[i]);
            if (i < root->children.size() - 1) {
                cout << ", ";
            }
        }
        cout << " }";
    }
}

// 解析输入的每一行，构建树
void newBuildTree(uint32_t root, vector<uint32_t> childs, unordered_map<int, TreeNode>& nodeMap) {
    // 获取或创建根节点
    auto& parentNode = nodeMap[root];

    for (auto child : childs) {
        // 如果子节点不存在，则创建它并初始化
        if (nodeMap.find(child) == nodeMap.end()) {
            nodeMap.emplace(child, TreeNode(child));  // 创建新节点
        }
        // 获取或创建子节点
        auto& childNode = nodeMap[child];
        parentNode.children.push_back(&childNode);  // 将子节点添加到根节点的子节点列表中
    }
}

struct Stmt {
    uint32_t id;
    vector<uint32_t> from;
    vector<uint32_t> to;
};

int main() {
    string line;
    unordered_map<int, TreeNode> nodeMap;  // 存储节点值到节点的映射
    vector<TreeNode*> trees;

    vector<Stmt> inputs = {
        {4, {3, 5}, {12}},
        {9, {6, 7, 8}, {12}},
        {12, {4, 9}, {13}},
        {13, {12}, {}},
        {16, {14, 15}, {25}},
        {17, {3, 5}, {25}},
        {20, {21}, {25}},
        {25, {16, 17, 20}, {}},
    };

    for (auto in : inputs) {
        // 保证根节点存在
        if (nodeMap.find(in.id) == nodeMap.end()) {
            nodeMap.emplace(in.id, TreeNode(in.id));
        }

        newBuildTree(in.id, in.from, nodeMap);  // 构建树

        cout << "Building tree with root " << in.id << endl;
        
        // 如果当前节点没有目标节点（即树到此为止），就输出树并清理
        if (in.to.empty()) {
            cout << "End of tree, root " << in.id << endl;
            trees.push_back(&nodeMap[in.id]);  // 记录这棵树的根节点
            printTree(&nodeMap[in.id]);
            cout << endl;
        }
    }
    cout << "total tree nums " << trees.size() << endl;

    return 0;
}

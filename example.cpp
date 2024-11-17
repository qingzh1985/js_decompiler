#include <iostream>
#include <vector>
#include <string>
#include <memory>

// 基类：AST 节点
struct Node {
    virtual ~Node() = default;
    virtual void print() const = 0; // 用于打印 AST 节点的函数
};

// Program 节点：表示整个程序
struct Program : public Node {
    std::vector<std::shared_ptr<Node>> body; // 代码的主体部分（如语句或表达式）

    void print() const override {
        std::cout << "Program:\n";
        for (const auto& stmt : body) {
            stmt->print();
        }
    }
};

// Literal 节点：表示一个字面量（数字、字符串等）
struct Literal : public Node {
    std::string value;  // 字面量的值
    std::string raw;    // 原始文本（可选）

    Literal(const std::string& val) : value(val), raw(val) {}

    void print() const override {
        std::cout << "Literal(" << raw << ")\n";
    }
};

// Identifier 节点：表示一个标识符（变量名、函数名等）
struct Identifier : public Node {
    std::string name; // 标识符的名称

    Identifier(const std::string& n) : name(n) {}

    void print() const override {
        std::cout << "Identifier(" << name << ")\n";
    }
};

// VariableDeclaration 节点：表示一个变量声明
struct VariableDeclaration : public Node {
    std::string kind; // 声明的类型，如 'let', 'const', 'var'
    std::vector<std::shared_ptr<Node>> declarations; // 声明列表

    VariableDeclaration(const std::string& k) : kind(k) {}

    void print() const override {
        std::cout << "VariableDeclaration(" << kind << "):\n";
        for (const auto& declarator : declarations) {
            declarator->print();
        }
    }
};

// VariableDeclarator 节点：表示具体的变量声明项（如 "a = 10"）
struct VariableDeclarator : public Node {
    std::shared_ptr<Identifier> id;   // 变量名
    std::shared_ptr<Node> init;       // 初始化值

    VariableDeclarator(std::shared_ptr<Identifier> id, std::shared_ptr<Node> init)
        : id(id), init(init) {}

    void print() const override {
        id->print();
        if (init) {
            std::cout << " = ";
            init->print();
        }
    }
};

// BinaryExpression 节点：表示二元表达式（如 a + b）
struct BinaryExpression : public Node {
    std::shared_ptr<Node> left;      // 左操作数
    std::string operatorType;        // 操作符（如 "+", "-", "*" 等）
    std::shared_ptr<Node> right;     // 右操作数

    BinaryExpression(std::shared_ptr<Node> l, const std::string& op, std::shared_ptr<Node> r)
        : left(l), operatorType(op), right(r) {}

    void print() const override {
        left->print();
        std::cout << " " << operatorType << " ";
        right->print();
    }
};

int main() {
    // 创建一个简单的 AST
    auto program = std::make_shared<Program>();

    // 变量声明
    auto varDecl = std::make_shared<VariableDeclaration>("const");
    auto varDeclarator = std::make_shared<VariableDeclarator>(
        std::make_shared<Identifier>("a"),
        std::make_shared<Literal>("10")
    );
    varDecl->declarations.push_back(varDeclarator);
    program->body.push_back(varDecl);

    // 二元表达式
    auto binaryExpr = std::make_shared<BinaryExpression>(
        std::make_shared<Identifier>("a"),
        "+",
        std::make_shared<Literal>("5")
    );
    program->body.push_back(binaryExpr);

    // 打印 AST
    program->print();

    return 0;
}

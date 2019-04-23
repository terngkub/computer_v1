#include "interpreter.hpp"

double power(double base, int power)
{
    if (power == 0)
        return 1;
    double ret = base;
    while (--power)
        ret *= base;
    return ret;
}

Interpreter::Interpreter(INode * ast) :
    ast(ast)
{}

INode * Interpreter::visit(INode * node)
{
    if (dynamic_cast<TermNode *>(node) != nullptr)
        return node;
    auto op_node = dynamic_cast<OperationNode *>(node);

    auto left = dynamic_cast<TermNode *>(visit(op_node->left));
    auto right = dynamic_cast<TermNode *>(visit(op_node->right));

    switch(op_node->op)
    {
        case TokenType::PLUS:
            update_term_map(left, true);
            update_term_map(right, true);
            break;

        case TokenType::MINUS:
            update_term_map(left, true);
            update_term_map(right, false);
            break;

        case TokenType::MULTIPLY:
        {
            auto coef = left->coef * right->coef;
            auto power = left->power + right->power;
            right->print();
            std::cout << "\nprint: " << coef << "^" << power << "\n";
            return (new TermNode(coef, power));
        }

        case TokenType::DIVIDE:
        {
            if (right->coef == 0)
                throw std::runtime_error("divide by zero");
            auto coef = left->coef / right->coef;
            auto power = left->power - right->power;
            return (new TermNode(coef, power));
        }

        case TokenType::POWER:
        {
            if (left->power == 0 and right->power == 0)
            {
                auto coef = power(left->coef, right->coef);
                return (new TermNode(coef, 0));

            }
            else if (left->power != 0 and right->power == 0)
            {
                return (new TermNode(left->coef, right->coef));
            }
            else
            {
                // error
            }
            break;
        }

        case TokenType::EQUAL:
            update_term_map(left, true);
            update_term_map(right, false);
            break;

        default:
            throw std::runtime_error("unsupported operation");
    }
    return nullptr;
}


void Interpreter::update_term_map(TermNode * node, bool is_plus)
{
    if (node == nullptr)
        return ;
    if (is_plus)
        std::cout << "plus: ";
    else
        std::cout << "minus: ";
    node->print();
    std::cout << "\n";
    if (term_map.find(node->power) == term_map.end())
        term_map[node->power] = node->coef;
    else if (is_plus)
        term_map[node->power] += node->coef;
    else
        term_map[node->power] -= node->coef;
    std::cout << "power: " << node->power << " coef: " << term_map[node->power] << "\n";
}

void Interpreter::print_map()
{
    for (auto it = term_map.begin(); it != term_map.end(); ++it)
        std::cout << it->first << ": " << it->second << '\n';
}

void Interpreter::interpret()
{
    auto node = dynamic_cast<TermNode *>(visit(ast));
    if (node != nullptr)
        update_term_map(node, true);
    print_map();
}
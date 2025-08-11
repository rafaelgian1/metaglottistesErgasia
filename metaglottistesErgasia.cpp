#include <iostream>
#include <vector>
#include <string>
#include <cctype>
using namespace std;

struct Node
{
    string sym;
    vector<Node *> kids;
    Node(string s) : sym(std::move(s)) {}
};

struct Parser
{
    string s;
    size_t i = 0;
    bool ok = true;

    Parser(string input) : s(std::move(input)) {}

    char peek()
    {
        while (i < s.size() && isspace((unsigned char)s[i]))
            i++;
        return (i < s.size()) ? s[i] : '\0';
    }
    bool match(char c)
    {
        if (peek() == c)
        {
            i++;
            return true;
        }
        ok = false;
        return false;
    }

    Node *G()
    {
        Node *n = new Node("G");
        if (!match('('))
            return fail(n, "αναμενόταν '('");
        n->kids.push_back(new Node("("));
        Node *m = M();
        n->kids.push_back(m);
        if (!ok)
            return n;
        if (!match(')'))
            return fail(n, "αναμενόταν ')'");
        n->kids.push_back(new Node(")"));
        return n;
    }

    Node *M()
    {
        Node *n = new Node("M");
        Node *y = Y();
        n->kids.push_back(y);
        if (!ok)
            return n;
        Node *z = Z();
        n->kids.push_back(z);
        return n;
    }

    Node *Y()
    {
        Node *n = new Node("Y");
        char p = peek();
        if (p == 'a')
        {
            match('a');
            n->kids.push_back(new Node("a"));
            return n;
        }
        if (p == 'b')
        {
            match('b');
            n->kids.push_back(new Node("b"));
            return n;
        }
        if (p == '(')
        {
            Node *g = G();
            n->kids.push_back(g);
            return n;
        }
        return fail(n, "αναμενόταν a, b ή '('");
    }

    Node *Z()
    {
        Node *n = new Node("Z");
        char p = peek();
        if (p == '*' || p == '+' || p == '-')
        {
            match(p);
            n->kids.push_back(new Node(string(1, p)));
            Node *m = M();
            n->kids.push_back(m);
            return n;
        }
        // ε (κενή συμβολοσειρά)
        n->kids.push_back(new Node("ε"));
        return n;
    }

    // helpers
    Node *fail(Node *n, const char * /*msg*/)
    {
        ok = false;
        return n;
    }

    void print(Node *node, int depth = 0)
    {
        for (int k = 0; k < depth; k++)
            cout << "  ";
        cout << node->sym << "\n";
        for (auto *c : node->kids)
            print(c, depth + 1);
    }
};

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    auto run = [&](const string &input)
    {
        Parser p(input);
        Node *tree = p.G();
        if (p.ok && p.peek() == '\0')
        {

            cout << "OK: Συντακτικά ορθή συμβολοσειρά.\n\n";
            cout << "Δέντρο ανάλυσης:\n";
            p.print(tree);
        }
        else
        {
            cout << "ΣΦΑΛΜΑ: Μη συντακτικά ορθή συμβολοσειρά.\n";
        }
    };

    string input = "((a-b)*(a+b))";
    cout << "Επίδειξη για την έκφραση " << input << "\n\n";
    run(input);

    cout << "Δώστε συμβολοσειρά για νέο έλεγχο: \n";
    string line;
    if (std::getline(cin, line))
    {
        run(line);
    }
    return 0;
}
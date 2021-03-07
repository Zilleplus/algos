#include<iostream>

struct List{
    int element;
    List* next;
};

void Pushback(List* l, int e)
{
    while(l->next!=nullptr)
    {
        l = l->next;
    }

    l->next = new List{e, nullptr};
}

List* PushFront(List* l, int e)
{
    return new List{e, l};
}

List* CreateList(int element)
{
    return new List{element, nullptr};
}

template<typename TUnOp>
void ForEach(List* l, TUnOp op)
{
    while(l->next != nullptr)
    {
        op(l->element);
        l = l->next;
    }
}

int main()
{
    List* l = CreateList(3);

    std::cout << "push";
    Pushback(l, 4);
    Pushback(l, 5);
    Pushback(l, 6);

    std::cout << "[ ";
    ForEach(l, [](int e) {std::cout << e << " ";});
    std::cout << "]";

    delete l;
}

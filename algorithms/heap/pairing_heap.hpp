#ifndef __PAIR_HEAP_HPP__
#define __PAIR_HEAP_HPP__

#include <functional>
#include <vector>
#include <queue>

namespace pg
{
	namespace heap
	{
		template <typename T, typename C = std::less<T>>
		class Pairing
		{
		private:
			C cmp;
			int size;

			struct node {
				T value;
				node* previous	{ nullptr };
				node* next 			{ nullptr };
				node* child 		{ nullptr };

				node(T&& v) : value(v) {}
			};

			node* root;

			node* meld(node *parent, node* leftmost)
			{
				if(!cmp(parent->value, leftmost->value))
					std::swap(parent, leftmost);

				if(parent->child != nullptr) {
					parent->child->previous = leftmost;
				}

				leftmost->next = parent->child;
				leftmost->previous = parent;
				parent->child = leftmost;

				return parent;
			}

			void meld(node* leftmost)
			{
				if(root == nullptr) {
					root = leftmost;
				}
				else {
					root = meld(root, leftmost);
				}
			}


		public:
			Pairing() : root(nullptr), size(0) {}
			Pairing(C cmp) : root(nullptr), size(0), cmp(cmp) {}

			bool Empty() const
			{
					return size == 0;
			}

			T& Peek()
			{
				return root.value;
			}

			void Push(T&& e)
			{
				meld(new node(std::move(e)));
				size++;
			}

			T Pop()
			{
				auto value = root->value;

				auto r = root;
				root = nullptr;
				size--;

				std::vector<node*> tree;

				for(auto c = r->child ; c != nullptr ; c = c->next)
					tree.push_back(c);

				for(int i = 0; i+1 < tree.size() ; i+=2)
				{
					tree[i] = meld(tree[i], tree[i+1]);
					tree[i+1] = nullptr;
				}

				if(tree.size()%2 == 1)
					meld(*tree.rbegin());

				auto end = (tree.size()%2 == 1) ? (tree.size() - 3): (tree.size() - 2);
				for(int i = end; i >= 0 ; i-=2) {
					meld(tree[i]);
				}

				r->child = nullptr;
				delete r;
				return value;
			}

			template <typename F>
			void Update(T&& e, F fct)
			{
				std::queue<node*> q;
				q.push(root);

				while(!q.empty())
				{
					auto n = q.front();
					q.pop();

					if(fct(n->value)) {
						bool direction = !cmp(e, n->value);
						bool leftmost = n->previous && (n->previous->child == n);
						n->value = e;

						if(n->next)
							n->next->previous = n->previous;

						if(leftmost && n->previous)
								n->previous->child = n->next;
						else if(n->previous)
								n->previous->next = n->next;

						if(direction) {
							std::vector<node*> tmp;
							for(auto c = n->child ; c != nullptr; c = c->next)
								tmp.push_back(c);

							n->child = nullptr;
							n->next = nullptr;
							n->previous = nullptr;

							for(auto c : tmp)
								meld(c);
							meld(n);
						}
						else {
							n->next = nullptr;
							n->previous = nullptr;
							meld(n);
						}

						return;
					}

					for(auto c = n->child ; c != nullptr ; c = c->next)
						q.push(c);
				}
			}

			void Update(T&& e, const T& current)
			{
				return Update(std::move(e), [&](const T& a) {
					return !(cmp(a, current) ^ cmp(current, a));
				});
			}
		};
	}
}

#endif

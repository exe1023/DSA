#include <utility>
#include <list>
#include <exception>
#include <iostream>
using namespace std;
struct EmptyHeap: public exception{};

template<class T>
class BinomialHeap {
    private:
        /* inner class: binomial tree */
        struct BinomialTree {
            int _size;
            T element;
            list<BinomialTree*> children;

            BinomialTree(T _ele): _size(1), element(_ele) {
                children.clear();
            }

            /* return the size of BinomialTree
             * #note that nullptr->size() == 0
             * (nullptr is a null pointer (like NULL in C))
             */
            int size() {
                return (this!=nullptr)?this->_size:0;
            }
            int position()
            {
            	int a = 1 , count = 0;
            	while(a != _size)
				{
					a *= 2;
					count ++;
				}
				return count;
            }
        };
        /* some definition for convinience
         */
        typedef BinomialTree BT;
        typedef BinomialHeap<T> BH;
        typedef pair<BT*, BT*> CarrySum;
        typedef pair<T, BH> MaxRemainder;

        /* Merge three binomial trees which have the same sizes
         *
         * INPUT:   a: operand a, b: operand b, c: carry in
         *          (each of them are either nullptr or a pointer of BinomialTree with size 2^n)
         *
         * OUTPUT:  a pair consist of two variables.
         *          first: carry out (nullptr or a pointer of BinomialTree with size 2^(n+1))
         *          second: sum (nullptr or a pointer of BinomialTree with size 2^n)
         *
         * (just as a full adder: http://en.wikipedia.org/wiki/Adder_%28electronics%29#Full_adder )
         */
        BT *binarymerge(BT *a , BT *b)
        {
        	if(a->element <= b->element)
        	{
        		b->children.push_back(a);
        		b->_size += a->size();
        		return b;
        	}
        	else
        	{
        		a->children.push_back(b);
        		a->_size += b->size();
        		return a;
        	}
        }
        CarrySum merge_tree(BT *a, BT *b, BT *c)
        {
            CarrySum result;
        	if(a == nullptr && b == nullptr && c == nullptr)
        		result =  make_pair(nullptr , nullptr);
        	else if(a != nullptr && b != nullptr && c == nullptr)  result = make_pair(binarymerge(a , b) , nullptr);
        	else if(a != nullptr && b == nullptr && c != nullptr)  result = make_pair(binarymerge(a , c) , nullptr);
        	else if(a == nullptr && b != nullptr && c != nullptr)  result = make_pair(binarymerge(b , c) , nullptr);
        	else if(a == nullptr && b == nullptr && c != nullptr)  result = make_pair(nullptr , c);
        	else if(a == nullptr && b != nullptr && c == nullptr)  result = make_pair(nullptr , b);
        	else if(a != nullptr && b == nullptr && c == nullptr)  result = make_pair(nullptr , a);
        	else result = make_pair(binarymerge(a , b) , c);
        	return result;
        }
        /* Pop the maximum element of a binomial tree and make other elements a binomial heap.
         *
         * INPUT:   a: a pointer of BinomialTree
         *
         * OUTPUT:  a pair consist of two variables.
         *          first: the maximum element
         *          second: a binomial heap consists of other elements
         */
        MaxRemainder pop_max(BT *a)
        {
        	T max = a->element;
        	BH newbheap;
            typename list<BT*>::iterator it = a->children.begin();
			for(it ; it != a->children.end() ; it++)
				newbheap.trees[ (*it)->position() ] = *it;
			delete a;
			MaxRemainder result(max , newbheap);
			return result;
        }

        int size;
        BT* trees[32]; //binomial trees of the binomial heap, where trees[i] is a tree with size 2^i.

    public:
        BinomialHeap(): size(0) {
            for(int i=0; i<32; ++i) trees[i] = nullptr;
        }
        BinomialHeap(T element): size(1) {
            for(int i=0; i<32; ++i) trees[i] = nullptr;
            trees[0] = new BT(element);
        }
        void reset()
        {
            for(int i = 0  ; i < 32 ; i++) trees[i] = nullptr;
            size = 0;
        }
        int heapsize(){return size;}
        T max()
        {
            int maxtree = -1;
            for(int i = 0 ; i < 32 ; i++)
                if(trees[i]->size() > 0 && (maxtree == -1 || trees[i]->element > trees[maxtree]->element))
                    maxtree = i;
            return trees[maxtree]->element;
        }
        /* merge all elements in the binomial heap b into *this, and clear the binomial heap b.
         *
         * INPUT:   b: a pointer of BinomialHeap
         */
        void merge(BH &b) {
        	BT * carry = nullptr;
        	for(int i = 0 ; i < 32 ; i++)
        	{
        		CarrySum carrysum( merge_tree(trees[i] , b.trees[i] , carry) );
        		trees[i] = carrysum.second;
        		carry = carrysum.first;
        	}
            size += b.size;
        }
        void insert(const T &element) {
            BH tmp = BH(element);
            merge(tmp);
        }
        T pop() {
            if(size==0) throw EmptyHeap();
            else {
                //find the tree contains maximum element
                int max_tree = -1;
                for(int i=0; i<32; ++i)
                    if(trees[i]->size() > 0 && (max_tree == -1 || trees[i]->element > trees[max_tree]->element))
                        max_tree = i;
                MaxRemainder m_r = pop_max(trees[max_tree]);
                T &max_element = m_r.first;
                BH &remainder = m_r.second;

                trees[max_tree] = nullptr;
                merge(remainder);
                size --;
                return max_element;
            }
        }
};

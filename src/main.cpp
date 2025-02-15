#include <iostream>
#include <cassert>
#include <cstdlib>
struct Block
{
	Block* next;
};

template<typename Type>
class PoolAllocator
{
public:
	PoolAllocator(std::size_t sizeInBytes) : freeList(nullptr), blockSize(sizeInBytes) {}
	Type* allocate(std::size_t n = 1)
	{
		if(freeList == nullptr)
		{
			std::size_t totalSize =  n * blockSize;
	//		void* memory = malloc(totalSize); // c++ way ::operator new(totalSize);
		//	Type* block = static_cast<Type*>(memory);

			 Type* block = static_cast<Type*>(::operator new(totalSize));

			//create a freeList for the new blcok
			for(std::size_t i  = 0; i < n; ++i)
			{
				Block* b = reinterpret_cast<Block*>(block + i);
				b->next = freeList;
				freeList = b;
			}
		}
		//retrun the next free block
		Block* block =  freeList;
		freeList = freeList->next;
		return reinterpret_cast<Type*>(block);
	}
	void deallocate(Type* p, std::size_t n = 1)
	{
		Block* block = reinterpret_cast<Block*>(p);
		for(std::size_t i = 0; i < n; ++i)
		{
			block->next = freeList;
			freeList = block;
			block = reinterpret_cast<Block*>(reinterpret_cast<char*>(block) + blockSize);
		}
	}
	~PoolAllocator()
	{
		while(freeList)
		{
			Block* temp = freeList;
			freeList = freeList->next;
			::operator delete(temp); // C++ way ::operator delete(temp);
		}
	}

private:
	Block* freeList;
	uint32_t blockSize;
};


int main()
{
	PoolAllocator<int> alloc(sizeof(int));
	std::size_t size = 20;
	int* ptr1 = alloc.allocate(size);
	for(std::size_t i = 0; i < size; ++i)
	{
		ptr1[i] = rand() % 100;
	}
	
	for(std::size_t i = 0; i < size; ++i)
	{
		std::cout << ptr1[i] << std::endl;
	}
	alloc.deallocate(ptr1);
	return 0;
}

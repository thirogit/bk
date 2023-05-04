#ifndef __NEWPTRFLUSHARRAY_H__
#define __NEWPTRFLUSHARRAY_H__

#include <vector>
#include <cstdint>
#include <cstddef>

template<class ARG>
class NewPtrFlushArray : public std::vector<ARG*>
{
private:
	bool m_bDeleteOnDestroy;

	void DeleteAll()
	{
		for(size_t i = 0,s = GetSize();i<s;i++)
			delete GetAt(i);
	}

public:

	NewPtrFlushArray(bool bDeleteOnDestroy = false) : m_bDeleteOnDestroy(bDeleteOnDestroy)
	{
	}

	~NewPtrFlushArray()
	{
		if(m_bDeleteOnDestroy)
		{
			DeleteAll();
		}
	}
	
	virtual void FreeAndFlush()
	{
		DeleteAll();
		RemoveAll();
	};

	size_t GetSize() const
	{
		return this->size();
	}

	ARG* GetAt(size_t iElementIndex)
	{
		return this->at(iElementIndex);
	}

	void RemoveAll()
	{
		this->clear();
	}

	void RemoveAt(size_t iIndex)
	{
		if(iIndex < this->size())
			this->erase(this->begin() + iIndex);
	}

	void Add(ARG* pElement)
	{
		this->push_back(pElement);
	}

	bool RemovePtr(ARG *Ptr)
	{
		size_t i, s;
		for(i = 0,s = GetSize();i<s;i++)
		{
			if(Ptr == GetAt(i))
			{
				RemoveAt(i);
				return true;
			}
		}
		return false;
	}

	void Append(const NewPtrFlushArray<ARG>& src)
	{
		typename NewPtrFlushArray<ARG>::const_iterator it = src.begin();
		while(it != src.end())
			this->push_back(*(it++));
	}

	unsigned int RemoveArray(NewPtrFlushArray<ARG> &array,bool bDelete = false)
	{
		size_t ii, ss, id, sd, removed = 0;
		ARG *aryPtr;

		NewPtrFlushArray<ARG> copy;
		copy.Append(array);

		for(ii=0,ss = GetSize(),sd = copy.GetSize();ii<ss;ii++)
		{
			if(sd <= 0) break;
			aryPtr = GetAt(ii);			
			for(id=0;id < sd;id++)
			{
				if(copy[id] == aryPtr)
				{
					copy.RemoveAt(id);
					RemoveAt(ii);
					removed++;
									
					if(bDelete) delete aryPtr;
					
					ii--;
					ss--;
					sd--;
					id--;
					break;
				}
			}
		}
		return removed;
	}

};

#endif
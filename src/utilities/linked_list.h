#pragma once

template<class TVal>
class linked_list
{
	linked_list();
	~linked_list();

	void SetValue(TVal val);
	void SetNext(linked_list* next);
	void SetPrev(linked_list* prev);

protected:
	TVal m_Value;
	linked_list* m_Next;
	linked_list* m_Prev;
};

template<class TVal>
linked_list<TVal>::linked_list()
	: m_Next(nullptr)
	, m_Prev(nullptr)
{
}

template<class TVal>
linked_list<TVal>::~linked_list()
{
	// #TODO: Deallocations
}

template<class TVal>
inline void linked_list<TVal>::SetValue(TVal val)
{
	m_Value = val
}

template<class TVal>
inline void linked_list<TVal>::SetNext(linked_list* next)
{
	m_Next = next;
}

template<class TVal>
inline void linked_list<TVal>::SetPrev(linked_list* prev)
{
	m_Prev = prev;
}



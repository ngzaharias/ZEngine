
template<typename Type, int32 Exponent>
auto PagedArray<Type, Exponent>::operator[](const int32 index)->Type&
{
	const int32 page = GetPage(index);
	const int32 offset = GetOffset(index);
	return m_Pages[page][offset];
}

template<typename Type, int32 Exponent>
auto PagedArray<Type, Exponent>::operator[](const int32 index) const -> const Type&
{
	const int32 page = GetPage(index);
	const int32 offset = GetOffset(index);
	return m_Pages[page][offset];
}

template<typename Type, int32 Exponent>
int32 PagedArray<Type, Exponent>::GetPage(const int32 index) const
{
	return index >> Exponent;
}

template<typename Type, int32 Exponent>
int32 PagedArray<Type, Exponent>::GetOffset(const int32 index) const
{
	return index & PageMask;
}

template<typename Type, int32 Exponent>
auto PagedArray<Type, Exponent>::Append(Type&& value)->Type&
{
	static_assert(std::is_move_constructible<Type>::value, "Implement Move Constructor.");
	m_Values.push_back(std::move(value));
	return GetLast();
}

template<typename Type, int32 Exponent>
auto PagedArray<Type, Exponent>::Append(const Type& value)->Type&
{
	m_Values.push_back(value);
	return GetLast();
}

template<typename Type, int32 Exponent>
void PagedArray<Type, Exponent>::Append(PagedArray&& range)
{
	std::move(std::begin(range.m_Values), std::end(range.m_Values), std::back_inserter(m_Values));
}

template<typename Type, int32 Exponent>
void PagedArray<Type, Exponent>::Append(const PagedArray& range)
{
	m_Values.insert(m_Values.end(), range.begin(), range.end());
}

template<typename Type, int32 Exponent>
void PagedArray<Type, Exponent>::Append(std::initializer_list<Type>&& range)
{
	m_Values.insert(m_Values.end(), range);
}

template<typename Type, int32 Exponent>
template<typename... Args>
auto PagedArray<Type, Exponent>::Emplace(Args&& ...args) -> Type&
{
	Page* page = m_Pages.GetLast();
	if (!page || page->GetCount() == PageSize)
		page = &m_Pages.Append();

	page->Emplace(std::forward(args...));
	return page->GetLast();
}

template<typename Type, int32 Exponent>
auto PagedArray<Type, Exponent>::Insert(const int32 index, Type&& value) -> Type&
{
	const int32 page = GetPage(index);
	const int32 offset = GetOffset(index);
	if (page >= m_Pages.GetCount())
		m_Pages.Resize(page + 1);

	Page& values = m_Pages[page];
	if (offset >= values.GetCount())
		values.Resize(offset + 1);

	return values[offset] = std::move(value);
}

template<typename Type, int32 Exponent>
auto PagedArray<Type, Exponent>::Insert(const int32 index, const Type& value) -> Type&
{
	const int32 page = GetPage(index);
	const int32 offset = GetOffset(index);
	if (page >= m_Pages.GetCount())
		m_Pages.Resize(page + 1);

	Page& values = m_Pages[page];
	if (offset >= values.GetCount())
		values.Resize(offset + 1);

	return values[offset] = value;
}

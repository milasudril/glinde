
ArrayDynamic<char> convert(Range<const uint32_t> src)
	{
	ArrayDynamic<char> dest;
	auto ptr_in=src.begin();
	while(ptr_in!=src.end())
		{
		auto code_point=*ptr_in;
		if (code_point < 0x80)
			{dest.append(code_point);}
		else
		if(code_point <= 0x7FF)
			{
			dest.append((code_point >> 6) + 0xC0);
			dest.append((code_point & 0x3F) + 0x80);
			}
		else
		if(code_point <= 0xFFFF)
			{
			dest.append((code_point >> 12) + 0xE0);
			dest.append(((code_point >> 6) & 0x3F) + 0x80);
			dest.append((code_point & 0x3F) + 0x80);
			}
		else
		if(code_point <= 0x10FFFF)
			{
			dest.append((code_point >> 18) + 0xF0);
			dest.append(((code_point >> 12) & 0x3F) + 0x80);
			dest.append(((code_point >> 6) & 0x3F) + 0x80);
			dest.append((code_point & 0x3F) + 0x80);
			}
		else
			{throw ExceptionEncode(dest.nameGet().begin(),factory.name,*ptr_in);}

		++ptr_in;
		}
	return std::move(dest);
	}
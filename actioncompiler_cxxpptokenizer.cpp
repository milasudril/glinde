#ifdef __WAND__
target[name[actioncompiler_cxxpptokenizer.o] type[object]]
#endif

#include "actioncompiler_cxxpptokenizer.h"
#include "logwriter.h"

using namespace Glinde;
using namespace ActionCompiler;

bool CxxPPTokenizer::read(Token& token)
	{
	if(m_reader.eof())
		{
		return 0;
		}
	auto state=m_state;
	token.value.clear();
	do
		{
		auto ch_in=m_reader.byteRead();
		switch(state)
			{
			case State::NEWLINE:
				if(!(ch_in<=' '))
					{
					switch(ch_in)
						{
						case '#':
							state=State::PREPROCESSOR_BEGIN;
							break;
						case '/':
							state=State::COMMENT_0;
							break;
						default:
							state=State::JUNK;
						}
					}
				break;

			case State::COMMENT_0:
				switch(ch_in)
					{
					case '/':
						state=State::COMMENT_1;
						break;
					case '*':
						state=State::COMMENT_2;
						break;
					default:
						state=State::JUNK;
					}
				break;

			case State::COMMENT_1:
				switch(ch_in)
					{
					case '\r':
					case '\n':
						state=State::NEWLINE;
						break;
					}
				break;

			case State::COMMENT_2:
				switch(ch_in)
					{
					case '*':
						state=State::COMMENT_3;
						break;
					}
				break;

			case State::COMMENT_3:
				switch(ch_in)
					{
					case '/':
						state=State::NEWLINE;
						break;
					default:
						state=State::COMMENT_2;
					}
				break;

			case State::PREPROCESSOR_BEGIN:
				switch(ch_in)
					{
					case '\r':
					case '\n':
						logWrite(Log::MessageType::WARNING,"%s: Newline after #",m_reader.nameGet());
						state=State::NEWLINE;
						break;

					default:
						if( !(ch_in<=' ') )
							{
							token.value.append(ch_in);
							token.type=Token::Type::DIRECTIVE;
							state=State::PREPROCESSOR;
							}
					}
				break;

			case State::PREPROCESSOR:
				switch(ch_in)
					{
					case '\r':
					case '\n':
						state=State::NEWLINE;
						goto done;

					default:
						if(ch_in<=' ')
							{
							state=State::PREPROCESSOR_NEXT;
							goto done;
							}
						token.value.append(ch_in);
					}
				break;

			case State::PREPROCESSOR_NEXT:
				switch(ch_in)
					{
					case '\r':
					case '\n':
						state=State::NEWLINE;
						break;
					case '"':
						state=State::STRING;
						token.type=Token::Type::STRING;
						break;

					case '<':
						state=State::SYSINCLUDE;
						token.type=Token::Type::SYSINCLUDE;
						break;

					default:
						if( !(ch_in<=' ') )
							{
							token.value.append(ch_in);
							token.type=Token::Type::OTHER;
							state=State::PREPROCESSOR;
							}
					}
				break;

			case State::STRING:
				switch(ch_in)
					{
					case '"':
						state=State::PREPROCESSOR_NEXT;
						goto done;

					case '\r':
					case '\n':
						logWrite(Log::MessageType::WARNING,"%s: Newline in string",m_reader.nameGet());
						state=State::NEWLINE;
						goto done;

					default:
						token.value.append(ch_in);
					}
				break;

			case State::SYSINCLUDE:
				switch(ch_in)
					{
					case '>':
						state=State::PREPROCESSOR_NEXT;
						goto done;

					case '\r':
					case '\n':
						logWrite(Log::MessageType::WARNING,"%s: Newline in string",m_reader.nameGet());
						state=State::NEWLINE;
						goto done;

					default:
						token.value.append(ch_in);
					}
				break;

			case State::JUNK:
				switch(ch_in)
					{
					case '\r':
					case '\n':
						state=State::NEWLINE;
						break;
					}
				break;
			}
		}
	while(!m_reader.eof());
done:
	m_state=state;
	return token.value.length()!=0;
	}

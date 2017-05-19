#ifndef __PG_PARSER_HPP__
#define __PG_PARSER_HPP__

#include <string>
#include <stdexcept>

namespace pg
{
	namespace parsing
	{
		struct ParsingSuccess 
		{
			std::string Content;
			std::string Remaining;
		};

		struct ParsingError {};

		template <typename T>
		class Optional 
		{
			T value;
			bool has_value;

			public:
			Optional() : has_value(false) {}
			Optional(T&& value) : value(value), has_value(true) {}

			T& Value()
			{
				if(!has_value)
					throw std::runtime_error("Optional has no value!");

				return value;
			}

			const T& Value() const 
			{
				return const_cast<Optional<T*>>(this)->Value;
			}

			bool HasValue() const noexcept { return has_value; }
		};

		struct ParsingResult 
		{
			Optional<ParsingSuccess> Success;
			Optional<ParsingError> Error;

			ParsingResult(ParsingSuccess&& s) : Success(std::move(s)) {}
			ParsingResult(ParsingError&& e) : Error(std::move(e)) {}
		};

		struct ErrorParser 
		{ 
			static ParsingResult Parse(const std::string&) 
			{ 
				return ParsingError{}; 
			} 
		}; 
		
		struct SuccessParser 
		{ 
			static ParsingResult Parse(const std::string& input) 
			{ 
				return ParsingSuccess{"", input}; 
			} 
		}; 

		template <typename...ARGS> 
		class Or
		{
			private:
				template <typename T = ErrorParser, typename...OTHERS>
				static ParsingResult InternalParse(const std::string& input)
				{
					auto result = T::Parse(input);
					if (result.Success.HasValue() || std::is_same<T, ErrorParser>::value)
						return result;

					return InternalParse<OTHERS...>(input);
				}

			public: 
				static ParsingResult Parse(const std::string& input) 
				{
					return InternalParse<ARGS...>(input);
				};
		};

		template <typename...ARGS>
		class And
		{
			
			private:
				template <typename T = SuccessParser, typename...OTHERS>
				static ParsingResult InternalParse(const std::string& input)
				{
					auto result = T::Parse(input);
					if (result.Error.HasValue())
						return ParsingError{};

					if constexpr(std::is_same<T, SuccessParser>::value)
						return result;

					auto remaining = InternalParse<OTHERS...>(result.Success.Value().Remaining);
					if (remaining.Error.HasValue())
						return ParsingError{};

					return ParsingSuccess { 
						result.Success.Value().Content + remaining.Success.Value().Content, 
						remaining.Success.Value().Remaining 
					};
				}

			public: 
				static ParsingResult Parse(const std::string& input) 
				{
					return InternalParse<ARGS...>(input);
				};
		};
	}
} 

#endif /* end of include guard: __PG_PARSER_HPP__ */

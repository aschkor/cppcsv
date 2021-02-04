#ifndef PARSER_HPP
#define PARSER_HPP


#include <tuple>
#include <fstream>
#include <string>
#include <queue>

namespace cppcsv{
	template <class ...Ts>
		class parser{
			public:
				parser(const std::string& path, const std::string& separator = ",");
				std::tuple<Ts...> read_line();
				bool eof();
			private:

				std::queue<std::string> split(const std::string& line);
				std::vector<size_t> list_sep(const std::string& line);
				std::ifstream file_;
				std::string separator_= ",";
		};

	template <class V>
		V convert(const std::string& cell);
}

#include "parser.ipp"

#endif

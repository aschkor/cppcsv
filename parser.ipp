#include <optional>

namespace cppcsv{

	void remove_double_double_quote(std::string& line){
		constexpr dd_quote {"\"\""};
		size_t pos = line.find_first_of(dd_quote);

		while (pos != std::string::npos){
			line.erase(pos,1);
			pos++;
			pos = line.find_first_of(dd_quote);
		}
	}
	template<class ...Ts>
		bool parser<Ts...>::eof(){
			if(this->file_.eof())
				return true;
			return this->file_.peek() == EOF;
		}

	template<class ...Ts>
		parser<Ts...>::parser(const std::string& path, const std::string& separator):file_{path}, separator_{separator}{
			if(!this->file_)
				throw "sonatehu";
		}

	template <class T>
		T convert(const std::string& cell){
			return static_cast<T>(cell);
		}

	template <>
		std::string convert<std::string>(const std::string& cell){
			remove_double_double_quote(cell);
			return cell;
		}

	template <>
		short convert<short> (const std::string& cell){
			return static_cast<short>(std::stoi(cell));
		}


	template <>
		unsigned short convert<unsigned short> (const std::string& cell){
			return static_cast<unsigned short>(std::stoi(cell));
		}


	template <>
		int convert<int> (const std::string& cell){
			return std::stoi(cell);
		}


	template <>
		unsigned int convert<unsigned int> (const std::string& cell){
			return static_cast<unsigned int>(std::stoul(cell));
		}

	template <>
		long convert<long> (const std::string& cell){
			return std::stol(cell);
		}

	template <>
		unsigned long convert<unsigned long> (const std::string& cell){
			return std::stoul(cell);
		}

	template <>
		long long convert<long long> (const std::string& cell){
			return std::stoll(cell);
		}

	template <>
		unsigned long long convert<unsigned long long> (const std::string& cell){
			return std::stoull(cell);
		}

	template <>
		float convert<float> (const std::string& cell){
			return std::stof(cell);
		}

	template <>
		double convert<double> (const std::string& cell){
			return std::stod(cell);
		}

	template <>
		long double convert<long double> (const std::string& cell){
			return std::stold(cell);
		}


	std::optional<size_t> find_first_double_quote(const std::string& line, const size_t pos){
		size_t p = line.find_first_of('"', pos);

		while(p != std::string::npos){

			if(p == line.size() - 1)
				return std::make_optional(p);

			const char c = line.at(p);

			if(p > 0 && c != line.at(p + 1) && c != line.at(p - 1))
				return std::make_optional(p);

			if(c != line.at(p + 1))
				return std::make_optional(p);

			p = line.find_first_of('"', p + 2);
		}
		return std::nullopt;
	}


	template<class ...Ts>
		std::vector<size_t> parser<Ts...>::list_sep(const std::string& line){
			std::vector<size_t> seps;
			size_t pos {line.find_first_of(this->separator_)};

			while(pos != std::string::npos){
				if(std::optional<size_t> double_quote = find_first_double_quote(line, pos)){
					if(double_quote.value() < pos){
						pos = find_first_double_quote(line, pos + 1);
						pos ++;
						continue;
					}
				}

				seps.push_back(pos);
				pos++;
				pos = line.find_first_of(this->separator_, pos);
			}
			return seps;
		}

	template <class ...Ts>
		std::queue<std::string> parser<Ts...>::split(const std::string& line){
			std::queue<std::string> res;
			size_t pos = 0;
			for(const size_t i : this->list_sep(line)){
				res.push(line.substr(pos, i - pos));
				pos += (i - pos) + 1;
			}
			if(pos + 1 < line.size())
				res.push(line.substr(pos));
			return res;
		}


	template <class U>
		std::tuple<U> parse(std::queue<std::string>& cell){
			return std::make_tuple<U>(convert<U>(cell.front()));
		}


	template <class U, class P, class ...Us>
		std::tuple<U, P,Us...> parse(std::queue<std::string>& line) {
			const std::string cell{line.front()};
			line.pop();
			return std::tuple_cat(std::make_tuple(convert<U>(cell)), parse<P,Us...>(line));
		}


	template<class ...Us>
		std::tuple<Us...> parser<Us...>::read_line(){
			std::string line;
			std::getline(this->file_,line);
			std::queue<std::string> t { this->split(line) };
			return parse<Us...>(t);
		}
}


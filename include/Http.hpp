#pragma once

#include "Base.hpp"

namespace test
{
	class HTTP
	{
	private:
		vector<string> line;
		map<string, string> fields;
		string body;

	protected:
		template<class T>
		void _split(const basic_string<T>& str1, const basic_string<T>& str2, vector<basic_string<T>>& str3) {
			size_t pos1 = 0;
			size_t pos2 = 0;
			while (true) {
				pos2 = str1.find(str2, pos1);
				if (pos2 != string::npos) {
					str3.push_back(str1.substr(pos1, pos2 - pos1));
					pos1 = pos2 + str2.length();
				}
				else {
					if (pos1 > 0) {
						str3.push_back(str1.substr(pos1, str1.length()));
					}
					break;
				}
			}
		}

		bool _parse(const s8* data) {
			basic_string<s8> d(data);
			size_t pos1 = d.find((s8*)"\r\n");
			size_t pos2 = d.find((s8*)"\r\n\r\n");

			if (pos2 > d.length() || pos1 > d.length()) {
				cout << "resolve faild" << endl;
				return false;
			}

			basic_string<s8> line((s8*)d.substr(0, pos1).data());
			basic_string<s8> header((s8*)(d.substr(pos1 + 2, pos2 - pos1 - 2).data()));

			vector<basic_string<s8>> lines;
			vector<basic_string<s8>> headers;

			_split<s8>(line, " ", lines);
			_split<s8>(header, "\r\n", headers);

			this->line = lines;

			for (auto i : headers) {
				vector<basic_string<s8>> temp;
				_split<s8>(i, ": ", temp);
				fields.insert(pair<string, string>(temp[0], temp[1]));
			}

			body = d.substr(pos2 + 4, d.size() - pos2 - 4).data();
		}

		vector<string> getLine() {
			return line;
		}

		bool setLine() {

		}

		string& getField(string field) {
			return fields[field];
		}

		bool setField(string name, string value) {

		}

		void getBody() {

		}

		void setBody() {

		}
	public:
		explicit HTTP() {}
	};
};


#ifndef CHAT_MESSAGE_HPP
#define CHAT_MESSAGE_HPP

#include <cstdio>
#include <cstdlib>
#include <cstring>


class Message
{
public:
	static constexpr  std::size_t header_length = 4;
	static constexpr std::size_t max_body_length = 512;
	Message()
		:_body_length(0)
	{

	}

	const char* data() const
	{
		return _data;
	}
	char* data() 
	{
		return _data;
	}

	std::size_t length() const // 외부에서 내부 변수 변경 불가능한 함수
	{
		return header_length + max_body_length;
	}

	const char* body() const
	{
		return _data + header_length;
	}
	 char* body() 
	{
		return _data + header_length;
	}

	 void setbody(char *str, int strNum)
	 {
		 int num = 0;
		 body_length(strNum);


		 for (int i = 4; i < 4 + strNum;i++)
		 {
			 _data[i] = str[num];
			 num++;
		 }

	 }
	 std::size_t body_length() const
	 {
		 return _body_length;
	 }
	 void body_length(std::size_t new_length)
	 {
		 _body_length= new_length;
		 if (_body_length > max_body_length)
			 _body_length = max_body_length;
	 }

	 bool decode_header()
	 {
		 char header[header_length + 1] = "";
		 strncat_s(header, _data, header_length); // 문자열 합쳐주기 
		 _body_length = std::atoi(header);

		 if (_body_length > max_body_length)
		 {
			 _body_length = 0;
			 return false;
		 }


		 return true;
	 }

	 void encode_header()
	 {
		 char header[header_length + 1] = "";
		 sprintf_s(header,"%4d", static_cast<int>(_body_length));
		 //-> 수를 문자열로 변환
		 std::memcpy(_data, header, header_length);
		 // 메모리 복사 
	 }
private:
	char _data[header_length + max_body_length];
	std::size_t _body_length;


};

#endif // CHAT_MESSAGE_HPP

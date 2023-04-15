
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
		:body_length_(0)
	{

	}

	const char* data() const
	{
		return data_;
	}
	char* data() 
	{
		return data_;
	}

	std::size_t length() const // 외부에서 내부 변수 변경 불가능한 함수
	{
		return header_length + max_body_length;
	}

	const char* body() const
	{
		return data_ + header_length;
	}
	 char* body() 
	{
		return data_ + header_length;
	}

	 void setbody(char *str, int strNum)
	 {
		 int num = 0;
		 body_length(strNum);


		 for (int i = 4; i < 4 + strNum;i++)
		 {
			 data_[i] = str[num];
			 num++;
		 }

	 }
	 std::size_t body_length() const
	 {
		 return body_length_;
	 }
	 void body_length(std::size_t new_length)
	 {
		 body_length_= new_length;
		 if (body_length_ > max_body_length)
			 body_length_ = max_body_length;
	 }

	 bool decode_header()
	 {
		 char header[header_length + 1] = "";
		 strncat_s(header, data_, header_length); // 문자열 합쳐주기 
		 body_length_ = std::atoi(header);

		 if (body_length_ > max_body_length)
		 {
			 body_length_ = 0;
			 return false;
		 }


		 return true;
	 }

	 void encode_header()
	 {
		 char header[header_length + 1] = "";
		 sprintf_s(header,"%4d", static_cast<int>(body_length_));
		 //-> 수를 문자열로 변환
		 std::memcpy(data_, header, header_length);
		 // 메모리 복사 
	 }
private:
	char data_[header_length + max_body_length];
	std::size_t body_length_;


};

#endif // CHAT_MESSAGE_HPP

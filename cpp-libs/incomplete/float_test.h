#pragma once

// uint32_t first value:
// 16,777,217

// uint64_t first value: 
// 9,007,199,254,740,991 (stack overflow)
// 9,007,199,254,740,993

template <typename float_type, typename int_type>
bool check_f_i_f(const int_type value)
{
	float_type float_value = value;
	int_type result = float_value;

	return result != value;
}

template <typename float_type, typename int_type>
void test_range()
{
	std::cout << "test_rage<" << typeid(float_type).name()
		<< ", " << typeid(int_type).name() << ">  ";

	int_type min = (std::numeric_limits<int_type>::min)();
	int_type max = (std::numeric_limits<int_type>::max)();

	std::cout << "digits: " << std::numeric_limits<float_type>::digits10
		<< ", " << std::numeric_limits<int_type>::digits10 << std::endl;

	for (int_type value = min; value < max; value += 1)
	{
		if (check_f_i_f<float_type, int_type>(value))
		{
			std::cout << "first value: " << value << std::endl;
			break;
		}
	}

	for (int_type value = max; value > min; value -= 1)
	{
		if (check_f_i_f<float_type, int_type>(value))
		{
			std::cout << "last value: " << value << std::endl;
			break;
		}
	}
}
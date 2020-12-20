#include <iostream>
#include <boost/asio.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <functional>

void Print(const boost::system::error_code&, boost::asio::deadline_timer& t, int& count)
{
		if (count < 5)
		{
				++count;
				t.expires_at(t.expires_at() + boost::posix_time::seconds(1));
				t.async_wait([&t, &count](const boost::system::error_code& code)
						{
							Print(code, t, count);
						});
		}
}

int main()
{
		int count = 0;

		// io_service对象
		boost::asio::io_service io;

		boost::asio::deadline_timer t(io, boost::posix_time::seconds(1));

		// 注册处理函数，在事件满足时自动调用，参数为以const boost::system::error_code&
		// 为唯一参数的函数对象，切记
		t.async_wait([&t, &count](const boost::system::error_code& code)
						{
							Print(code, t, count);
						});

		// io_service的run函数其实是一个循环,当所注册的所有事件都处理后返回，相当于在这里阻塞等待
		// 全部事件处理完成
		io.run();
		std::cout << "count = " << count << std::endl;

		return 0;
}

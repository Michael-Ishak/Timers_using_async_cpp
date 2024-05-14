#include <iostream>
#include <boost/asio.hpp>

void print(const boost::system::error_code& /*e*/)
{
    std::cout << "Hello, World!" << std::endl;
}

int main()
{
    boost::asio::io_context io;
    boost::asio::steady_timer t(io, boost::asio::chrono::seconds(5));
    t.async_wait(&print); // async_wait takes a callback argument of type boost::system::error_code to see whether the async timer expired successfully or not. We use a pointer since it is used in runtime
    io.run();
    return 0;   
}
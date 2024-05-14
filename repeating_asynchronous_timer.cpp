#include <iostream>
#include <boost/asio.hpp>
#include <functional>

void print(const boost::system::error_code& /*e*/, boost::asio::steady_timer* t, int* count) {
    if (*count < 5) {
        std::cout << *count << std::endl;
        ++(*count);
        t->expires_at(t->expiry() + boost::asio::chrono::seconds(1)); // This keeps updating the timer's expiry by 1 second, so that the timer doesn't drift from the 1 second mark because of delays in processing the handler
        t->async_wait([t, count](const boost::system::error_code& /*ec*/) { // we use an arrow as t is a pointer, if it wasn't a pointer we'd use a .
            print(/*ec*/ boost::system::error_code(), t, count); // bind the additional parameters for the print function to make the call to async_wait using lamba functions
        });
    }
}

int main() {
    boost::asio::io_context io;
    int count = 0;
    boost::asio::steady_timer t(io, boost::asio::chrono::seconds(1));
    t.async_wait([&t, &count](const boost::system::error_code& /*ec*/) {
        print(/*ec*/ boost::system::error_code(), &t, &count);
    });
    io.run();
    std::cout << "Final count is " << count << std::endl;
    return 0;
}
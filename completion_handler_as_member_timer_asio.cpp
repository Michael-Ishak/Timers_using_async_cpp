#include <iostream>
#include <boost/asio.hpp>
#include <functional>

class printer
{
    public:
        printer(boost::asio::io_context &io)
            : timer_(io, boost::asio::chrono::seconds(1)),
                count_(0) // This creates the function that will take io, and give a timer object to io and to go off every second, as well as giving a counter to count number of times function printer has been used
        {
            timer_.async_wait([this](const boost::system::error_code& /*ec*/) { // use a . instead of an -> since timer_ is not a pointer
                print(); // bind the additional parameters (this) for the print function to make the call to async_wait using lamba functions
        });
        }
        ~printer()
        {
            std::cout << "Final count is " << count_ << std::endl;
        }
        void print()
        {
            if (count_ < 5) // count isn't referenced in as a pointer because we're operating on the class members, rather than having a timer and counter passed in parameters
            {
            std::cout << count_ << std::endl;
            ++count_;

            timer_.expires_at(timer_.expiry() + boost::asio::chrono::seconds(1));
            timer_.async_wait([this](const boost::system::error_code& /*ec*/) { // use a . instead of an -> since timer_ is not a pointer
                print(); // bind the additional parameters (this) for the print function to make the call to async_wait using lamba functions
            });
        }
        }
    private:
        boost::asio::steady_timer timer_;
        int count_;

};

int main()
{
    boost::asio::io_context io;
    printer p(io);
    io.run();

    return 0;
}
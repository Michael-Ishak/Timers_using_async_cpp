#include <iostream>
#include <functional>
#include <boost/thread/thread.hpp>
#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>

class printer
{
    public:
        printer(boost::asio::io_context& io)
        : strand_(boost::asio::make_strand(io)),
            timer1_(io, boost::asio::chrono::seconds(1)),
            timer2_(io, boost::asio::chrono::seconds(1)),
            count_(0)
        {
            timer1_.async_wait(boost::asio::bind_executor(strand_,
                std::bind(&printer::print1, this)));

            timer2_.async_wait(boost::asio::bind_executor(strand_,
                std::bind(&printer::print2, this)));
        }
        ~printer()
        {
            std::cout << "Final count is " << count_ << std::endl;
        }

        void print1()
        {
            if (count_ < 10)
            {
                std::cout << "Timer 1: " << count_ << std::endl;
                ++count_;

                timer1_.expires_at(timer1_.expiry() + boost::asio::chrono::seconds(1));

                timer1_.async_wait(boost::asio::bind_executor(strand_,
                        std::bind(&printer::print1, this)));
            }
        }

        void print2()
        {
            if (count_ < 10)
            {
                std::cout << "Timer 2: " << count_ << std::endl;
                ++count_;

                timer2_.expires_at(timer2_.expiry() + boost::asio::chrono::seconds(1));

                timer2_.async_wait(boost::asio::bind_executor(strand_,
                        std::bind(&printer::print2, this)));
            }
        }
    private:
    boost::asio::strand<boost::asio::io_context::executor_type> strand_;
    boost::asio::steady_timer timer1_;
    boost::asio::steady_timer timer2_;
    int count_;
    };

int main()
{
    boost::asio::io_context io;
    printer p(io);
    std::thread t([&]{ io.run(); });
}
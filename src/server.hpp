#pragma once
#ifndef ARCHIPELAGO_SERVER_H
#define ARCHIPELAGO_SERVER_H

#include <iostream>
#include <boost/asio.hpp>
#include <mutex>

using boost::asio::ip::tcp;

class server
{
    public:
        server(boost::asio::io_context& io_context, short port):
        m_acceptor(io_context, tcp::endpoint(tcp::v4(), port))
        {
            instance = this;
            do_accept();
        }

        static server* GetInstance() { return instance; }

        static inline std::unique_ptr<std::vector<std::string>> message_queue = std::make_unique<std::vector<std::string>>();

    private:
        void inline do_accept();
    private:
        tcp::acceptor m_acceptor;
        static inline server* instance = nullptr;
};

class session : public std::enable_shared_from_this<session>
{
public:
    session(tcp::socket socket)  
    : m_socket(std::move(socket)) { }
    
    void run() {
        wait_for_request();
    }

private:
    void inline wait_for_request();

private:
    tcp::socket m_socket;
    boost::asio::streambuf m_buffer;
    std::mutex data_mutex;
};

void inline session::wait_for_request() {
    auto self(shared_from_this());

    boost::asio::async_read_until(m_socket, m_buffer, "\0", 
    [this, self](boost::system::error_code ec, std::size_t /*length*/)
    {

        if (!ec)  {
            std::string message{
                std::istreambuf_iterator<char>(&m_buffer), 
                std::istreambuf_iterator<char>() 
            };

            std::lock_guard<std::mutex> lock(data_mutex);
            server::GetInstance()->message_queue->push_back(message);

        } else {
            std::cout << "error: " << ec << std::endl;;
        }
    });
}

void inline server::do_accept()
        {
            m_acceptor.async_accept([this](boost::system::error_code ec, tcp::socket socket) {
                if (!ec) {
                    std::make_shared<session>(std::move(socket))->run();
                } else {
                    std::cout << "error: " << ec.message() << std::endl;
                }
                do_accept();
            });
        }

#endif
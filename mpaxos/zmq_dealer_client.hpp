#include <vector>
#include <thread>
#include <memory>
#include <functional>

#include <zmq.hpp>
//#include "zhelpers.hpp"
#include <chrono>

class client_task {
public:
    client_task(const char *id)
        : ctx_(1),
          client_socket_(ctx_, ZMQ_DEALER),
          my_id_(id)
    {
        printf("my_id_ %s\n\n", my_id_);
        client_socket_.setsockopt(ZMQ_IDENTITY, my_id_, strlen(my_id_));
        client_socket_.connect("tcp://192.168.5.20:5571");
    }

    void reply_ready() {

        try {
            char request_string[30] = {};
            sprintf(request_string, "Ready --%s", my_id_);
            client_socket_.send(request_string, strlen(request_string));
            printf("Sent Msg %s\n", request_string);

            zmq::message_t message;
            client_socket_.recv(&message);
       
            //  Dump the message as text or binary
            int size = message.size();
            std::string data(static_cast<char*>(message.data()), size);
            printf("Received size: %d data: %s\n", size, data.c_str());

        }
        catch (std::exception &e) {}
    }

    void listen_collect() {

        try {

            while (true) {

                zmq::message_t message;
                client_socket_.recv(&message);

                int size = message.size();
                std::string data(static_cast<char*>(message.data()), size);
                printf("Received size: %d data: %s\n", size, data.c_str());
                if (data.compare("Start Count!") == 0) 
                  break;
            }
        }
        catch (std::exception &e) {}
    }

    void listen() {

        try {

            while (true) {

                zmq::message_t message;
                client_socket_.recv(&message);

                int size = message.size();
                std::string data(static_cast<char*>(message.data()), size);
                printf("Received size: %d data: %s\n", size, data.c_str());
                if (data.compare("Terminate!") == 0) 
                  break;
            }
        }
        catch (std::exception &e) {}
    }

    void send_string(std::string str) {
      client_socket_.send(str.c_str(), str.size());
    }
    void start() {

        int request_nbr = 0;
        int total = 100000;
        try {

            auto start = std::chrono::high_resolution_clock::now();
            while (true) {
//                s_sleep(10);
                char request_string[30] = {};
                sprintf(request_string, "request #%d --%s", ++request_nbr, my_id_);
//                sprintf(request_string, "#%d", ++request_nbr);
                client_socket_.send(request_string, strlen(request_string));
                printf("Sent Msg %s\n", request_string);

                zmq::message_t message;
                client_socket_.recv(&message);
       
                //  Dump the message as text or binary
                int size = message.size();
                std::string data(static_cast<char*>(message.data()), size);
                printf("Received size: %d data: %s\n", size, data.c_str());
              if(request_nbr == total)
                break;
            }

            auto finish = std::chrono::high_resolution_clock::now();
            uint64_t period = std::chrono::duration_cast<std::chrono::milliseconds>(finish-start).count();
            int throughput = total * 1000 / period;
            printf("Over! Time: %llu ms Throughtput: %d\n", period, throughput);
        }
        catch (std::exception &e) {}
    }

private:
    zmq::context_t ctx_;
    zmq::socket_t client_socket_;
    const char *my_id_;
};


//int main (int argc, char *argv[])
//{
//  std::string id = "loli";
//  if (argc > 1)
//    id = argv[1];
//
//    client_task ct1(id.c_str());
//    ct1.start();
//
//    getchar();
//    return 0;
//}

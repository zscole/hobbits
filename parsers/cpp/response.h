#ifndef HOBBIT_RESPONSE_H
#define HOBBIT_RESPONSE_H

#include <cstdint>
#include <string>
#include <vector>
#include <stdexcept>

namespace hobbit{
    struct ewp_response{
        int response_status;
        std::string compression;
        std::string header;
        std::string body;
        bool has_body;
        ewp_response(){}
        ewp_response(const std::string& in)
        {
            this->parse(in);
        }

        void parse(const std::string& in)
        {
            size_t index = in.find('\n');
            const auto response_line = in.substr(0,index);
            const auto response_body = in.substr(index+1);

            auto request = explode(response_line,' ');
            if(request.size() < 3){
                throw std::invalid_argument("Not enough parameters");
                //Not enough parameters
            }
            this->response_status = std::stoi(request[0]);
            this->compression = request[1];

            this->header = response_body.substr(0,std::stoi(request[2]));
            if(request.size() == 4){
                this->body = response_body.substr(std::stoi(request[2]),std::stoi(request[3]));
                this->has_body = true;
            }else{
                this->has_body = false;
            }
            
        }

        std::string marshal() const noexcept 
        {
            std::string out = std::to_string(this->response_status) + " ";
            out += this->compression + " ";
            out += std::to_string(this->header.size());
            if(this->has_body){
                out += " "+std::to_string(this->body.size());
            }
            out += "\n";
            out += this->header+this->body;

            return out;
        }

    };

}


#endif
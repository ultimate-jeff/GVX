#pragma once
#include"includes.h"


enum status_map{
    unknown_error = -127,
    
    io_error = -6,
    bad_config = -5,
    already_exsists = -4,
    out_of_range = -3,
    invalid_arg = -2,
    error = -1 ,
    None = 0,
    good = 1,
    succsess = 2,
    ready = 3,
    busy = 4,
    complete = 5,
    initalizing = 6,
    pending = 7,
    end = 127
};

const string keywords[] = {
    "#", // function
    "@", // config 
    ".", // lable
    "%", // const,
};

namespace logger{
    struct Entry{
        string data;
        int8_t level;
    };

    vector<Entry> errors;

    inline
    void error(Entry entry){
        if(entry.level >= 10){
            cout << "fatal error" << endl;
            std::exit(-1);
        }
        errors.push_back(entry);
    }
    inline
    void error(string msg,int8_t level){
        Entry full_msg;
        full_msg.data = msg;
        full_msg.level = level;
        error(full_msg);
    }

}; // namespace logger

namespace fops{

struct Header{ // fill the rest in later
    char verifier[4] = {'g','v','x',0};
    uint32_t size;
    char isa[8] = {'g','3','2',0};
};

struct Gvx_file{ // fill this in
    Header header;
    vector<uint8_t> data;
    int8_t status;
};


bool open_file(string path,string* data){
    std::ifstream file(path);
    if(!file.is_open()){
        return false;
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    *data = buffer.str();
    return file.good();
}
bool write_file(string path,string* data){
    std::ofstream file(path);
    if(!file.is_open()){
        return false;
    }
    file << *data;
    return file.good();
}

template<typename T>
bool write_bin(string& path, vector<T>* data){
    std::ofstream file(path,std::ios::binary);
    if(!file.is_open()){
        return false;
    }
    file.write(reinterpret_cast<const char*>(data.data()), data.size());
    return file.good();
}
template<typename T>
bool read_bin(string& path,vector<T>* data){
    std::ifstream file(path);
    if(!file.is_open()){
        return false;
    }
    std::streamsize size = file.tellg();
    if(size < 0 || size % sizeof(T) != 0){
        return false;
    }
    file.seekg(0,std::ios::beg);
    data->resize(size / sizeof(T));
    if(size > 0){
        file.read(reinterpret_cast<char*>(data->data()),size);
    }
    return file.good();
}


}; // namespace fops

namespace strops{ // string ops

    bool remove_coments(string& raw,string& new_str){
        bool removing = false;
        for(size_t i = 0 ; i < raw.size() ; i++){
            char value = raw[i];
            if(!removing){
                new_str += value;
            }
            if(value == '~'){
                removing = true;
            }else if(removing && value == '\n'){
                removing = false;
            }
        }
        return !removing; // if it is removing at the end there was a coment that was not closed
    }

    int index(vector<string>* data,string desired_string){
        for(size_t i = 0 ; i < data->size() ; i++){
            if(data->at(i) == desired_string){
                return i;
            }
        }
        return -1;
    }

    bool contains(const vector<string>& data, const string& value){
        for(string elm : data){
            if(elm == value){
                return true;
            }
        }
        return false;
    }

    static inline void _split_flush(string& current,vector<string>& output){
        if(!current.empty()){
            output.push_back(current);
            current.clear();
        }
    }

    vector<string> split(const string& data, const string& delimiters,const string& keepers){
        vector<string> output;
        string current;
        for (char c : data) {
            if(delimiters.find(c) != string::npos){
                _split_flush(current,output);
            }
            else if(keepers.find(c) != string::npos){
                _split_flush(current,output);
                output.push_back(string(1,c));
            }
            else{
                current.push_back(c);
            }
        }
        if(!current.empty()){
            output.push_back(current);
        }
        return output;
    }

    void line_split(vector<string>* split_data,vector<string> delimiters,vector<vector<string>>* output){
        vector<string> current;
        for(string value : *split_data){
            bool found_delim = false;
            for(string c : delimiters){
                if(value.find(c) != string::npos){
                    output->push_back(current);
                    current.clear();
                    found_delim = true;
                    break;
                }
            }
            if(!found_delim){
                current.push_back(value);
            }
        }
    }

}; // namespace strops

namespace comp{

    struct Parse_pos{
        size_t pos[2];
        size_t parse1_pos[2];
    };

    struct Context{
        int8_t phase;
        Parse_pos pos;
        string raw;
        vector<string> parse0;
        vector<vector<string>> parse1;
    };

}; // namspace comp

/*


*/

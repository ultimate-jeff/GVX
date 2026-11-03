#pragma once
#include"includes.h"

namespace fops{

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


} // namespace file ops

namespace srtops{ // string ops

    int index(vector<string>* data,string desired_string){
        for(int i = 0 ; i < data->size() ; i++){
            if(data->at(i) == desired_string){
                return i;
            }
        }
        return -1;
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

}

const string keywords[] = {
    "#", // function
    "@", // config 
    ".", // lable
    "%", // const,
};
/*


*/

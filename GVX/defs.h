#pragma once
#include"includes.h"
#include"config.h"


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

struct Token {
    string data;
    size_t source_line = 0;

    Token& operator=(const std::string& str) {
        data = str;
        return *this;
    }
    Token& operator=(char c) {
        data = c;
        return *this;
    }

    char& operator[](size_t index) { return data[index]; }
    const char& operator[](size_t index) const { return data[index]; }

    char& at(size_t index) { return data.at(index); }
    const char& at(size_t index) const { return data.at(index); }

    size_t size() const { return data.size(); }
    size_t length() const { return data.length(); }

    bool empty() const { return data.empty(); }
    void clear() { data.clear(); }

    void push_back(char c) { data.push_back(c); }

    operator const std::string&() const { return data; }
    operator std::string&() { return data; }
};

namespace comp{

    struct Context{
        int8_t phase;
        Token raw;
        vector<Token> raw_parse;
        vector<Token> parse0;
        vector<vector<Token>> parse1;
        vector<uint8_t> output;
    };

}; // namspace comp

class Logger{
public:
    struct Entry{
        string data;
        float level;
        size_t line;

        size_t where(){
            return line;
        }
    };


    vector<Entry> errors;
    comp::Context* context;

    Logger(comp::Context* context_ptr){
        context = context_ptr;
    }

    size_t get_line(vector<Token>& token_snipit){
        return token_snipit[0].source_line;
    }

    void exit(){
        cout << "----------------" << endl;
        for(Entry e : errors){
            cout << "level " << e.level << "error at " << e.where() << " -> " << e.data << end;
        }
        cout << "----------------" << endl;
    }

    inline
    void error(Entry entry){
        if(entry.level >= 1.0){
            cout << "fatal error" << endl;
            exit();
        }
        errors.push_back(entry);
    }
    inline
    void error(string msg,float level,size_t line=0){
        Entry full_msg;
        full_msg.data = msg;
        full_msg.level = level;
        full_msg.line = line;
        error(full_msg);
    }

}; // class logger


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

    bool remove_coments(vector<Token>& raw,vector<Token>& new_token){
        bool removing = false;
        for(size_t i = 0 ; i < raw.size() ; i++){
            Token token = raw[i];
            if(token.data == COMENT_SYMBOLE){
                removing = !removing;
            }
            if(!removing && token.data != COMENT_SYMBOLE){
                new_token.push_back(token);
            }
        }
        return !removing; // if it is removing at the end there was a coment that was not closed
    }

    int index(vector<Token>* tokens,string desired){
        for(size_t i = 0 ; i < tokens->size() ; i++){
            if(tokens->at(i).data == desired){
                return i;
            }
        }
        return -1;
    }

    bool contains(const vector<Token>& tokens, const string& value){
        for(Token elm : tokens){
            if(elm.data == value){
                return true;
            }
        }
        return false;
    }

    static inline void _split_flush(string& current,vector<Token>& output,size_t line_num){
        if(!current.empty()){
            Token t;
            t.data = current;
            t.source_line = line_num;
            output.push_back(t);
            current.clear();
        }
    }

    vector<Token> split(Token& data, const string& delimiters,const string& keepers){
        vector<Token> output;
        Token current;
        size_t line = data.source_line;
        for (char c : data.data) {
            if(delimiters.find(c) != string::npos){
                _split_flush(current,output,line);
                if(c == '\n'){
                    line++;
                }
            }
            else if(keepers.find(c) != string::npos){
                _split_flush(current,output,line);
                Token t;
                t = string(1,c);
                t.source_line = line;
                output.push_back(t);
                if(c == '\n'){
                    line++;
                }
            }
            else{
                if(c == '\n'){
                    line++;
                }
                current.push_back(c);

            }
        }
        if(!current.empty()){
            output.push_back(current);
        }
        return output;
    }

    void line_split(vector<Token>& split_data,vector<string> delimiters,vector<vector<Token>>& output){
        vector<Token> current;
        for(Token value : split_data){
            bool found_delim = false;
            for(string c : delimiters){
                if(value.data.find(c) != string::npos){
                    output.push_back(current);
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


/*


*/

#include"includes.h"
#include"config.h"
#include"defs.h"

using std::string;
using std::cout;
using std::endl;
/*
# is compiler function
@ is compiler config 


*/
namespace logger{
    struct Entry{
        string data;
        int8_t level;
    };

    vector<Entry> errors;

    inline
    void error(string msg,int8_t level){
        Entry full_msg;
        full_msg.data = msg;
        full_msg.level = level;
        errors.push_back(full_msg);
    }
    inline
    void error(Entry entry){
        errors.push_back(entry);
    }
} // namespace logger


namespace comp{

    struct Confic{
        map<string,string> data;

        inline
        bool has(const string& key){
            return data.find(key) != data.end();
        }
        inline
        string get(const string& key,const string& defalt){
            auto value = data.find(key);
            if(value == data.end()){
                return defalt;
            }
            return value->second;
        }
        inline
        void set(const string& key,string value){
            data[key] = value;
        }
    };

    struct Context{
        Confic* config;
        map<string,int64_t>* consts;
        map<string,size_t>* labels;
        size_t location;
        size_t line_num;
        vector<string>* data;
        vector<vector<string>>* parse_data;
        string* raw;
    };

    void fill(Context context){

    }
    void _const(Context context){

    }

    const string comp_calls[] = {
        "fill",
        "const",
    };

    const array<std::function<void(Context)>,16> func_table = {
        fill,
        _const,
    };

    std::function<void(Context)> get_function(string token){
        for(int i = 0 ; i < (sizeof(comp_calls)/sizeof(string)) ; i++){
            if(comp_calls[i] == token){
                return func_table[i];
            }
        }
    }

} // namespace asmfuncs



void assemble(vector<string>& args){
    // gasm -a <input> <output>
    //      i0    i1     i2
    enum eml_map{
        input = 1,
        output = 2
    };
    cout << "assembling" << endl;
    try{
        string file;
        if(!fops::open_file(args.at(eml_map::input),&file)){
            cout << "could not open file -> " << args.at(eml_map::input) << endl;
            throw 75;
        }
        cout << "obtained file. comensing assembly " << endl;
        vector<string> data = srtops::split(file," ,\n","#.@;");
        vector<vector<string>> parse;
        srtops::line_split(&data,{";"},&parse);
        

    }
    catch(const std::exception& e){
        cout << "-----" << endl;
        cout << "gasm -a <input_file> <output_file>" << endl;
        cout << "gasm -help for help\n-----" << endl;
        std::cerr << e.what() << '\n';
    }
    

}


int main(int argc,char* argv[]){
    // commmand for mat ./assembler -a file output
    vector<string> args(argv + 1,argv + argc);
    if(args.empty()){
        std::cerr << "usage gasm -a <input> <output>" << endl;
        return 1;
    }

    string command = args[0];
    if(command == "-a"){
        // assemble
        assemble(args);
    }
    else if(command == "-d"){
        // disassemble
    }
    else if(command == "--version"){
        cout << "gasm version " << VERSION << endl;
        cout << "made by matthew R" << endl;
    }
    else if(command == "-help"){

    }
    

    
}

/*
```
#if 10 != 3 .true;
.false;

.true;
```
->
```
sub 10 3;
cmp 3 !=;
jmp .true;
.false;

.true;
```

*/
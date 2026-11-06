#include"includes.h"
#include"config.h"
#include"defs.h"

/* 
this dose the first pass and parses and removes coments
*/
inline 
void parse0(comp::Context& context,Logger& logger){
    context.raw_parse = strops::split(context.raw,"\n ",PARSE_KEEPERS);
    const bool comments_removed = strops::remove_coments(context.raw_parse,context.parse0);
    if(comments_removed){
        logger.error("unclosed comment",1,context.raw.size());
    }
}

inline
void parse1(comp::Context& context,Logger& logger){
    strops::line_split(context.parse0,{";"},context.parse1);

}

inline
int8_t assemble(string& raw,vector<uint8_t>& output){
    comp::Context context;
    context.raw = raw;
    Logger logger(&context);

    parse0(context,logger);

    parse1(context,logger);

    for(vector<Token> t2 : context.parse1){
        for(Token t : t2){
            cout << '{' << t.data << '}';
        }
        cout << endl;
    }


    return status_map::good;
}

int8_t assemble_from_args(vector<string>& args){
    // gasm -a <input> <output>
    //      i0    i1     i2
    cout << "assembling" << endl;
    try{
        string file;
        if(!fops::open_file(args[1],&file)){
           cout << "could not open file" << endl;
           std::exit(-1);
        }
        vector<uint8_t> output;
        int8_t status = assemble(file,output);
        if(status < 0){
            // error
        }

    }
    catch(const std::exception& e){
        cout << "-----" << endl;
        cout << "gasm -a <input_file> <output_file>" << endl;
        cout << "gasm -help for help\n-----" << endl;
        std::cerr << e.what() << '\n';
        return status_map::error;
    }
    
    return status_map::good;
}


int main(int argc,char* argv[]){
    // commmand for mat ./assembler -a file output
    vector<string> args(argv + 1,argv + argc);
    if(args.empty()){
        std::cerr << "usage gvx -a <input> <output>" << endl;
        return 1;
    }

    string command = args[0];
    if(command == "-a"){
        // assemble
        assemble_from_args(args);
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
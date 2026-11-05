#include"includes.h"
#include"config.h"
#include"defs.h"

/*
# is compiler function
@ is compiler config 


*/

inline
int8_t assemble(string& raw,vector<uint8_t>& output){
    comp::Context context;

    strops::remove_coments(raw,context.raw); // i was working on constructig the context and begening parsing 

    cout << context.raw << endl;


    return status_map::good;
}

int8_t assemble(vector<string>& args){
    // gasm -a <input> <output>
    //      i0    i1     i2
    cout << "assembling" << endl;
    try{
        string file;
        if(!fops::open_file(args[1],&file)){
            logger::error("could not find file",10);
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
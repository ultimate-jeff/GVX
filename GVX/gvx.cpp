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
namespace asmfuncs{
    void fill(int pos ,vector<string> args,vector<string>& data){

    }
    void _const(int pos ,vector<string> args,vector<string>& data){

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
        for(int i = 0 ; i < data.size();i++){
            cout << "{" << data.at(i) << "}" << endl;
        }

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
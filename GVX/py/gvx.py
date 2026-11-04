import re
import string
import json
import os
import sys

class Logger():
    def __init__(self):
        self.errors = []

    def error(self,msg:"str",level:"int"):
        self.errors.append(
            {
                "data":msg,
                "level":level
            }
        )

logger = Logger()


hash_func_table = {
    "const":None,
    "fill":None
}


def get_line(token,raw):
    # this is to figure out the exact line in the file due to the parsing removing the exact line
    return token["line"]

def parse(raw):
    return re.findall(r";|[@#$%.]|[^\s,;@#$%.]+", raw) #re.findall(r";|[^\s,;]+", raw)

def tokenize(raw:"str"):
    raw_lines = raw.split(";")
    lines = []
    for line_index in range(len(raw_lines)):
        parse_data = parse(raw_lines[line_index])
        if parse_data == []:
            continue
        lines.append(
            {
                "data":parse_data,
                "line":line_index
            }
        )
    return lines

def hash_pass(tokens:"list[dict[str]]"):
    pass

def get_config(tokens:"list[dict[str]]",raw:"str"):
    config = {}
    program = []
    for token in tokens:
        data = token["data"]
        if data[0] == "@":
            if(len(data) <= 2):
                logger.error(f"config enterd incorectly on line {get_line(token,raw)}",1)
                continue
            config[data[1]] = data[2:]
        else:
            program.append(token)
    return config,program
    

def compile(args:"list[str]"):
    print("\ncompialing\n")

    input_path = args[1]
    output_path = args[2]
    flags = args[2:]

    with open(input_path,"r") as f:
        raw = f.read()
    #data = re.split(r"[ ,]|(;)",raw)
    data = parse(raw)
    tokens = tokenize(raw)
    config,tokens = get_config(tokens,raw)
    print(tokens)



def main(args:"list[str]"):
    if args[0] == "-a":
        compile(args)


args = ["-a","test.gxs","out.gxe"]
main(args)
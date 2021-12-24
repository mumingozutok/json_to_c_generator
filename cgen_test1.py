import cgen as c
import json

def read_statement(dj):
    f = open(dj['body'], 'r')
    str = f.read()
    f.close()
    split_str = str.split('//@@@')
    return split_str[1]

def get_params(dj):
    param_list = []
    
    for i in dj['inputs']:
        param_list.append(c.Value(i['type'], i['name']))
    
    for i in dj['outputs']:
        param_list.append(c.Value(i['type'], i['name'])) 
    
    return param_list

def get_function_name(dj):
    return c.Value(dj['return_val'], dj['name'])

def json_to_c(dj):
    if(dj != None):
        #print(dj)               
        func =  c.FunctionBody(
        c.FunctionDeclaration(get_function_name(dj), get_params(dj)),
        c.Block([
            c.Statement(read_statement(dj))
            ])
        )
        print(func)
        return func 


def read_fbs_json(fb_type):
    try:
        f = open('fbs\\'+ fb_type.lower()+'.json')
        if f != None:
            return json.load(f)
    except:
        pass    

def create_functionblock(fb):
    try:
        fb_name = fb['fb']['name']
        fb_type = fb['fb']['type']
        return json_to_c(read_fbs_json(fb_type))
    except:
        pass

def main():
    c_code = ""
    f = open('program.json')
    code = json.load(f)
    f.close()

    for i in code['code']:
        try:
            if i['Action'] == 'CREATE' and i['fb']!= None:
                str = create_functionblock(i)
                if str != None:
                    c_code = code + str 
                    c_code = code + '\n'
                else:
                    print('function error at:', i)
        except:
            pass

    print(c_code)

    pass

if __name__ == '__main__':
    main()

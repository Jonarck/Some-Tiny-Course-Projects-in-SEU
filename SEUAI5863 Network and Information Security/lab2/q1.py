#this program calculates salted hashed password!

import crypt

possible_pw = ['abc1', 'abc2', 'abc3', 'abc4', 'abc5', 'abc6', 'abc7', 'abc8','abc9', 'abc10']

for pw in possible_pw:
    if crypt.crypt(pw, '$6$PZOyfkQz$') == '$6$PZOyfkQz$C8Rj2dcT2kjTHKYDJDzCMJ7GpJeklx7pafQjyONMdMqcBAeqcW/tDfEBOPczx7VdCF70E5CgXsG4krDZBjkmv0':
        print('The passward for cis4510suser is [' + pw + ']')
        exit()

print('Cracking Failed!')
